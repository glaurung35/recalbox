#include <systems/SystemManager.h>
#include "guis/GuiScraperMulti.h"
#include "Renderer.h"
#include "utils/Log.h"
#include "views/ViewController.h"

#include "components/TextComponent.h"
#include "components/ButtonComponent.h"
#include "components/ScraperSearchComponent.h"
#include "components/MenuComponent.h" // for makeButtonGrid
#include "guis/GuiMsgBox.h"
#include "Locale.h"
#include "MenuThemeData.h"

using namespace boost::locale;

GuiScraperMulti::GuiScraperMulti(Window* window, const std::queue<ScraperSearchParams>& searches, bool approveResults)
  :	GuiComponent(window),
    mSearchQueue(searches),
    mBackground(window, ":/frame.png"),
    mGrid(window, Vector2i(1, 5))
{
	auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
	
	mBackground.setImagePath(menuTheme->menuBackground.path);
	mBackground.setCenterColor(menuTheme->menuBackground.color);
	mBackground.setEdgeColor(menuTheme->menuBackground.color);
	
	assert(!mSearchQueue.empty());

	addChild(&mBackground);
	addChild(&mGrid);

	mIsProcessing = true;

	mTotalGames = mSearchQueue.size();
	mCurrentGame = 0;
	mTotalSuccessful = 0;
	mTotalSkipped = 0;

	// set up grid
	mTitle = std::make_shared<TextComponent>(mWindow, _("SCRAPING IN PROGRESS"), menuTheme->menuTitle.font, menuTheme->menuTitle.color, TextAlignment::Center);
	mGrid.setEntry(mTitle, Vector2i(0, 0), false, true);

	mSystem = std::make_shared<TextComponent>(mWindow, _("SYSTEM"), menuTheme->menuText.font, menuTheme->menuText.color, TextAlignment::Center);
	mGrid.setEntry(mSystem, Vector2i(0, 1), false, true);

	mSubtitle = std::make_shared<TextComponent>(mWindow, _("subtitle text"), menuTheme->menuFooter.font, menuTheme->menuFooter.color, TextAlignment::Center);
	mGrid.setEntry(mSubtitle, Vector2i(0, 2), false, true);

	mSearchComp = std::make_shared<ScraperSearchComponent>(mWindow, 
		approveResults ? ScraperSearchComponent::SearchType::AutoAcceptMatchingCRC : ScraperSearchComponent::SearchType::AutoAcceptFirst);
	mSearchComp->setAcceptCallback(std::bind(&GuiScraperMulti::acceptResult, this, std::placeholders::_1));
	mSearchComp->setSkipCallback(std::bind(&GuiScraperMulti::skip, this));
	mSearchComp->setCancelCallback(std::bind(&GuiScraperMulti::finish, this));
	mGrid.setEntry(mSearchComp, Vector2i(0, 3), mSearchComp->getSearchType() != ScraperSearchComponent::SearchType::AutoAcceptFirst, true);

	std::vector< std::shared_ptr<ButtonComponent> > buttons;

	if(approveResults)
	{
	  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("INPUT"), _("search"), [&] { 
			mSearchComp->openInputScreen(mSearchQueue.front()); 
			mGrid.resetCursor(); 
		}));

	  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("SKIP"), _("SKIP"), [&] {
			skip();
			mGrid.resetCursor();
		}));
	}

	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("STOP"), _("stop (progress saved)"), std::bind(&GuiScraperMulti::finish, this)));

	mButtonGrid = makeButtonGrid(mWindow, buttons);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 4), true, false);

	setSize(Renderer::getDisplayWidthAsFloat() * 0.95f, Renderer::getDisplayHeightAsFloat() * 0.849f);
	setPosition((Renderer::getDisplayWidthAsFloat() - mSize.x()) / 2, (Renderer::getDisplayHeightAsFloat() - mSize.y()) / 2);

	doNextSearch();
}

GuiScraperMulti::~GuiScraperMulti()
{
	// view type probably changed (basic -> detailed)
	for (auto& system : SystemManager::Instance().GetVisibleSystemList())
		ViewController::get()->reloadGameListView(system, false);
}

void GuiScraperMulti::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	mGrid.setRowHeightPerc(0, mTitle->getFont()->getLetterHeight() * 1.9725f / mSize.y(), false);
	mGrid.setRowHeightPerc(1, (mSystem->getFont()->getLetterHeight() + 2) / mSize.y(), false);
	mGrid.setRowHeightPerc(2, mSubtitle->getFont()->getHeight() * 1.75f / mSize.y(), false);
	mGrid.setRowHeightPerc(4, mButtonGrid->getSize().y() / mSize.y(), false);
	mGrid.setSize(mSize);
}

void GuiScraperMulti::doNextSearch()
{
	if(mSearchQueue.empty())
	{
		finish();
		return;
	}

	// update title
	std::stringstream ss;
	mSystem->setText(StringUtil::toUpper(mSearchQueue.front().system->getFullName()));

	// update subtitle
	ss.str(""); // clear
	char strbuf[256];
	snprintf(strbuf, 256, _("GAME %i OF %i").c_str(), mCurrentGame + 1, mTotalGames);

	ss << strbuf << " - " << StringUtil::toUpper(mSearchQueue.front().game->getPath().filename().string());
	mSubtitle->setText(ss.str());

	mSearchComp->search(mSearchQueue.front());
}

void GuiScraperMulti::acceptResult(const ScraperSearchResult& result)
{
	ScraperSearchParams& search = mSearchQueue.front();

	search.game->Metadata().Merge(result.mdl);
	search.system->UpdateGamelistXml();

	mSearchQueue.pop();
	mCurrentGame++;
	mTotalSuccessful++;
	doNextSearch();
}

void GuiScraperMulti::skip()
{
	mSearchQueue.pop();
	mCurrentGame++;
	mTotalSkipped++;
	doNextSearch();
}

void GuiScraperMulti::finish()
{
	std::stringstream ss;
	if(mTotalSuccessful == 0)
	{
		ss << _("WE CAN'T FIND ANY SYSTEMS!\n"
			"CHECK THAT YOUR PATHS ARE CORRECT IN THE SYSTEMS CONFIGURATION FILE, AND "
			"YOUR GAME DIRECTORY HAS AT LEAST ONE GAME WITH THE CORRECT EXTENSION.\n"
			"\n"
			"VISIT RECALBOX.FR FOR MORE INFORMATION."
			);
	} else {
	  char strbuf[256];
	  snprintf(strbuf, 256, ngettext("%i GAME SUCCESSFULLY SCRAPED!", "%i GAMES SUCCESSFULLY SCRAPED!", mTotalSuccessful).c_str(), mTotalSuccessful);
	  ss << strbuf;

	  if(mTotalSkipped > 0) {
	    snprintf(strbuf, 256, ngettext("%i GAME SKIPPED.", "%i GAMES SKIPPED.", mTotalSkipped).c_str(), mTotalSkipped);
	    ss << "\n" << strbuf;
	  }
	}

	mWindow->pushGui(new GuiMsgBox(mWindow, ss.str(), 
				       _("OK"), [&] { delete this; }));

	mIsProcessing = false;
}

