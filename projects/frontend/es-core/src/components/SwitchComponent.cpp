#include "SwitchComponent.h"
#include "Renderer.h"
#include "resources/Font.h"
#include "Window.h"
#include "Locale.h"
#include "MenuThemeData.h"

SwitchComponent::SwitchComponent(Window* window)
  : GuiComponent(window),
    mImage(window),
    mState(false)
{
	auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
	mImage.setImage(mState ? menuTheme->iconSet.on : menuTheme->iconSet.off);
	mImage.setResize(0, Font::get(FONT_SIZE_MEDIUM)->getLetterHeight());
	mImage.setColorShift(menuTheme->menuText.color);
	mOriginColor = menuTheme->menuText.color;
	mSize = mImage.getSize();
}

void SwitchComponent::onSizeChanged()
{
	mImage.setSize(mSize);
}

void SwitchComponent::setColor(unsigned int color) {
	mImage.setColorShift(color);
}

bool SwitchComponent::input(InputConfig* config, Input input)
{
	if(config->isMappedTo("b", input) && input.value)
	{
		mState = !mState;
		onStateChanged();
		return true;
	}

	return false;
}

void SwitchComponent::render(const Transform4x4f& parentTrans)
{
	Transform4x4f trans = parentTrans * getTransform();
	
	mImage.render(trans);

	renderChildren(trans);
}

void SwitchComponent::setState(bool state)
{
	mState = state;
	mInitialState = mState;
	onStateChanged();
}

void SwitchComponent::onStateChanged()
{
	mImage.setImage(mState ? ":/on.svg" : ":/off.svg");
}

std::vector<HelpPrompt> SwitchComponent::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("b", _("CHANGE")));
	return prompts;
}

std::string SwitchComponent::getValue() const {
  return mState ? "true" : "false";
}

bool SwitchComponent::changed() {
	return mInitialState != mState;
}
