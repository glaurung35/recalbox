#include "Window.h"
#include "Renderer.h"
#include "utils/Log.h"
#include "Settings.h"
#include <algorithm>
#include <iomanip>
#include <guis/GuiMsgBoxScroll.h>
#include <guis/GuiInfoPopup.h>
#include "components/HelpComponent.h"
#include "components/ImageComponent.h"
#include "guis/GuiMsgBox.h"
#include "RecalboxConf.h"
#include "Locale.h"
#include "MenuThemeData.h"
#include "views/ViewController.h"

Window::Window()
  : mInfoPopup(nullptr),
    mFrameTimeElapsed(0),
    mFrameCountElapsed(0),
    mAverageDeltaTime(10),
		mNormalizeNextUpdate(false),
	  mAllowSleep(true),
	  mSleeping(false),
	  mTimeSinceLastInput(0),
	  mRenderedHelpPrompts(false)
{
	mHelp = new HelpComponent(*this);
	mBackgroundOverlay = new ImageComponent(*this);
	auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
	mBackgroundOverlay->setImage(menuTheme->menuBackground.fadePath);
}

Window::~Window() {
	delete mBackgroundOverlay;
	deleteAllGui();
	delete mHelp;
}

void Window::pushGui(GuiComponent* gui)
{
	mGuiStack.push_back(gui);
	gui->updateHelpPrompts();
}

void Window::displayMessage(const std::string& message)
{
	mMessages.push_back(message);
}

void Window::displayScrollMessage(const std::string& title, const std::string& message)
{
	mScrollTitle.push_back(title);
	mScrollMessages.push_back(message);
}

void Window::removeGui(GuiComponent* gui)
{
	for (auto i = mGuiStack.begin(); i != mGuiStack.end(); i++)
	{
		if(*i == gui)
		{
			i = mGuiStack.erase(i);

			if(i == mGuiStack.end() && !mGuiStack.empty()) // we just popped the stack and the stack is not empty
				mGuiStack.back()->updateHelpPrompts();

			return;
		}
	}
}

GuiComponent* Window::peekGui()
{
	if(mGuiStack.empty())
		return nullptr;

	return mGuiStack.back();
}

void Window::deleteClosePendingGui()
{
  // Delete top GUI if pendingf for deletion
  if (!mGuiStack.empty())
    if (mGuiStack.back()->IsPendingForDeletion())
    {
      GuiComponent* topGui = mGuiStack.back();
      mGuiStack.pop_back();
      delete topGui;
    }
}

void Window::deleteAllGui()
{
	while (peekGui() != &ViewController::Instance())
		delete peekGui();
}

bool Window::Initialize(unsigned int width, unsigned int height, bool initRenderer)
{
    if (initRenderer) {
        if(!Renderer::initialize((int)width, (int)height))
        {
            LOG(LogError) << "Renderer failed to initialize!";
            return false;
        }
    }

  std::string glExts = (const char*) glGetString(GL_EXTENSIONS);
  LOG(LogInfo) << "Checking available OpenGL extensions...";
  LOG(LogInfo) << " ARB_texture_non_power_of_two: "
               << (glExts.find("ARB_texture_non_power_of_two") != std::string::npos ? "OK" : "MISSING");

  InputManager::Instance().Initialize();
	ResourceManager::getInstance()->reloadAll();

	//keep a reference to the default fonts, so they don't keep getting destroyed/recreated
	if(mDefaultFonts.empty())
	{
		mDefaultFonts.push_back(Font::get(FONT_SIZE_SMALL));
		mDefaultFonts.push_back(Font::get(FONT_SIZE_MEDIUM));
		mDefaultFonts.push_back(Font::get(FONT_SIZE_LARGE));
	}

	mBackgroundOverlay->setResize(Renderer::getDisplayWidthAsFloat(), Renderer::getDisplayHeightAsFloat());

	// update our help because font sizes probably changed
	if(peekGui() != nullptr)
		peekGui()->updateHelpPrompts();

	return true;
}

void Window::Finalize()
{
  InputManager::Instance().Finalize();
	ResourceManager::getInstance()->unloadAll();
	Renderer::finalize();
}

void Window::textInput(const char* text)
{
	if(peekGui() != nullptr)
		peekGui()->textInput(text);
}

void Window::ProcessInput(const InputCompactEvent& event)
{
	if(mSleeping)
	{
		// wake up
		mTimeSinceLastInput = 0;
		mSleeping = false;
		onWake();
		return;
	}

	mTimeSinceLastInput = 0;
	if(peekGui() != nullptr)
	{
		this->peekGui()->ProcessInput(event);
	}

	//else
	//{

		/*if(config->isMappedTo("PageDown", input) && input.value
					&& ViewController::get()->isViewing(ViewController::SYSTEM_SELECT)
					&& mGuiStack.size() == 1 )
		{
			auto s = std::make_shared<GuiInfoPopup>(this, "Controller:\n" + config->getDeviceName(), 2);
			this->setInfoPopup(s);
		}*/
	/*	if(peekGui() && !KonamiCode(config, input, this))
		{
			this->peekGui()->input(config, input);
		}
	}*/
}

void Window::update(int deltaTime)
{
	if(!mMessages.empty()){
		std::string message = mMessages.back();
		mMessages.pop_back();
		pushGui(new GuiMsgBox(*this, message));
	}

	if(!mScrollMessages.empty()){
		std::string message = mScrollMessages.back();
		std::string title = mScrollTitle.back();
		mScrollMessages.pop_back();
		mScrollTitle.pop_back();
		pushGui(new GuiMsgBoxScroll(*this, title, message, _("OK"), [] {}, "", nullptr, "", nullptr, TextAlignment::Left));
	}

	if(mNormalizeNextUpdate)
	{
		mNormalizeNextUpdate = false;
		if(deltaTime > mAverageDeltaTime)
			deltaTime = mAverageDeltaTime;
	}

	mFrameTimeElapsed += deltaTime;
	mFrameCountElapsed++;
	if(mFrameTimeElapsed > 500)
	{
		mAverageDeltaTime = mFrameTimeElapsed / mFrameCountElapsed;
		
		if(Settings::Instance().DrawFramerate())
		{
			std::string ss = Strings::ToString(1000.0f * (float)mFrameCountElapsed / (float)mFrameTimeElapsed, 1) + "fps, "
			               + Strings::ToString((float)mFrameTimeElapsed / (float)mFrameCountElapsed, 2) + "ms";

			// vram
			float textureVramUsageMb = TextureResource::getTotalMemUsage() / (1024.0f * 1024.0f);
			float textureTotalUsageMb = TextureResource::getTotalTextureSize() / (1024.0f * 1024.0f);
			float fontVramUsageMb = Font::getTotalMemUsage() / (1024.0f * 1024.0f);
			ss += "\nFont VRAM: " + Strings::ToString(fontVramUsageMb, 2)
			    + " Tex VRAM: " + Strings::ToString(textureVramUsageMb, 2)
			    + " Tex Max: " + Strings::ToString(textureTotalUsageMb, 2);

			mFrameDataText = std::unique_ptr<TextCache>(mDefaultFonts.at(1)->buildTextCache(ss, 50.f, 50.f, 0xFF00FFFF));
		}

		mFrameTimeElapsed = 0;
		mFrameCountElapsed = 0;
	}

	mTimeSinceLastInput += deltaTime;

	// Process GUI pending for deletion
  deleteClosePendingGui();

	// Process highest GUI
	if(peekGui() != nullptr)
		peekGui()->update(deltaTime);
}

void Window::render()
{
	Transform4x4f transform = Transform4x4f::Identity();

	mRenderedHelpPrompts = false;

	// draw only bottom and top of GuiStack (if they are different)
	if (!mGuiStack.empty())
	{
		auto& bottom = mGuiStack.front();
		auto& top = mGuiStack.back();

		bottom->render(transform);
		if(bottom != top)
		{
			mBackgroundOverlay->render(transform);
			top->render(transform);
		}
	}

	if(!mRenderedHelpPrompts)
		mHelp->render(transform);

	if(Settings::Instance().DrawFramerate() && mFrameDataText)
	{
		Renderer::setMatrix(Transform4x4f::Identity());
		mDefaultFonts.at(1)->renderTextCache(mFrameDataText.get());
	}

	unsigned int screensaverTime = (unsigned int)Settings::Instance().ScreenSaverTime();
	if(mTimeSinceLastInput >= screensaverTime && screensaverTime != 0)
	{

		if (!isProcessing() && mAllowSleep)
		{
			renderScreenSaver();
			// go to sleep
			mSleeping = true;
			onSleep();
		}
	}
	if(mInfoPopup)
		{
		mInfoPopup->render(transform);
		}
}

void Window::renderWaitingScreen(const std::string& text)
{
	Transform4x4f trans = Transform4x4f::Identity();
	Renderer::setMatrix(trans);
	Renderer::drawRect(0, 0, Renderer::getDisplayWidthAsInt(), Renderer::getDisplayHeightAsInt(), 0xFFFFFFFF);

	ImageComponent splash(*this, true);
	splash.setResize(Renderer::getDisplayWidthAsFloat() * 0.6f, 0.0f);
	splash.setImage(Path(":/splash.svg"));
	splash.setPosition((Renderer::getDisplayWidthAsFloat() - splash.getSize().x()) / 2, (Renderer::getDisplayHeightAsFloat() - splash.getSize().y()) / 2 * 0.6f);
	splash.render(trans);

	auto& font = mDefaultFonts.at(1);
	TextCache* cache = font->buildTextCache(text, 0, 0, 0x656565FF);
	trans = trans.translate(Vector3f(Math::round((Renderer::getDisplayWidthAsFloat() - cache->metrics.size.x()) / 2.0f),
											Math::round(Renderer::getDisplayHeightAsFloat() * 0.835f), 0.0f));
	Renderer::setMatrix(trans);
	font->renderTextCache(cache);
	delete cache;

	Renderer::swapBuffers();
}
void Window::renderLoadingScreen()
{
  renderWaitingScreen(_("LOADING..."));
}

void Window::renderHelpPromptsEarly()
{
	mHelp->render(Transform4x4f::Identity());
	mRenderedHelpPrompts = true;
}

void Window::setHelpPrompts(const std::vector<HelpPrompt>& prompts, const HelpStyle& style)
{
	mHelp->clearPrompts();
	mHelp->setStyle(style);

	std::vector<HelpPrompt> addPrompts;

	std::map<std::string, bool> inputSeenMap;
	std::map<std::string, int> mappedToSeenMap;
	for (const auto& prompt : prompts)
	{
		// only add it if the same icon hasn't already been added
	  if (inputSeenMap.insert(std::make_pair<std::string, bool>(std::string(prompt.first), true)).second)
		{
			// this symbol hasn't been seen yet, what about the action name?
			auto mappedTo = mappedToSeenMap.find(prompt.second);
			if(mappedTo != mappedToSeenMap.end())
			{
				// yes, it has!

				// can we combine? (dpad only)
			  if((strcmp(prompt.first.c_str(), "up/down") == 0 && strcmp(addPrompts.at(mappedTo->second).first.c_str(), "left/right") == 0) ||
			     (strcmp(prompt.first.c_str(), "left/right") == 0 && strcmp(addPrompts.at(mappedTo->second).first.c_str(), "up/down") == 0))
				{
					// yes!
				  addPrompts.at(mappedTo->second).first = "up/down/left/right";
					// don't need to add this to addPrompts since we just merged
				}else{
					// no, we can't combine!
					addPrompts.push_back(prompt);
				}
			}else{
				// no, it hasn't!
				mappedToSeenMap.insert(std::pair<std::string, int>(prompt.second, addPrompts.size()));
				addPrompts.push_back(prompt);
			}
		}
	}

	// sort prompts so it goes [dpad_all] [dpad_u/d] [dpad_l/r] [a/b/x/y/l/r] [start/select]
	std::sort(addPrompts.begin(), addPrompts.end(), [](const HelpPrompt& a, const HelpPrompt& b) -> bool {
		
		static const char* map[] = {
			"up/down/left/right",
			"up/down",
			"left/right",
			"a", "b", "x", "y", "l", "r", 
			"start", "select", 
			nullptr
		};
		
		int i = 0;
		int aVal = 0;
		int bVal = 0;
		while(map[i] != nullptr)
		{
			if(a.first == map[i])
				aVal = i;
			if(b.first == map[i])
				bVal = i;
			i++;
		}

		return aVal > bVal;
	});

	mHelp->setPrompts(addPrompts);
}


void Window::onSleep()
{
}


void Window::onWake()
{
}

void Window::renderShutdownScreen()
{
  renderWaitingScreen(_("PLEASE WAIT..."));
}

bool Window::isProcessing()
{
	return count_if(mGuiStack.begin(), mGuiStack.end(), [](GuiComponent* c) { return c->isProcessing(); }) > 0;
}

void Window::renderScreenSaver()
{
	Renderer::setMatrix(Transform4x4f::Identity());
	unsigned char opacity = Settings::Instance().ScreenSaverBehavior() == "dim" ? 0xA0 : 0xFF;
	Renderer::drawRect(0, 0, Renderer::getDisplayWidthAsInt(), Renderer::getDisplayHeightAsInt(), 0x00000000 | opacity);
}

void Window::doWake()
{
	mTimeSinceLastInput = 0;
	mSleeping = false;
	onWake();
}

bool Window::KonamiCode(InputDevice* config, InputEvent input, Window&window)
{
  (void)window;

	if (input.Value() == 0)	return false;

	bool codeOk = false;

	for (auto valstring : mInputVals)
	{
		if (config->IsMatching(valstring, input) && (this->mKonami[this->mKonamiCount] == valstring))
		{
			this->mKonamiCount ++;
			codeOk = true;
		}
	}

	if (!codeOk)
	{
		this->mKonamiCount = 0; // current input is incorrect, reset counter
	}

	if (this->mKonamiCount == sKonamiLength)
	{
		auto s = std::make_shared<GuiInfoPopup>(*this, "I entered Konami Code and all I get is this lame popup", 4, 50);
		this->setInfoPopup(s);
		return true;
	}
	return false;
}
