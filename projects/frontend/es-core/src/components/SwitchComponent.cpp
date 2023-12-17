#include "SwitchComponent.h"
#include "Renderer.h"
#include "resources/Font.h"
#include "WindowManager.h"
#include "utils/locale/LocaleHelper.h"
#include "themes/MenuThemeData.h"
#include <themes/ThemeManager.h>

SwitchComponent::SwitchComponent(WindowManager&window)
  : Component(window)
  , mImage(window)
  , mInterface(nullptr)
  , mId(0)
  , mState(false)
  , mInitialState(false)
{
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
	mImage.setImage(mState ? menuTheme.Elements().on : menuTheme.Elements().off);
	mImage.setResize(0, Font::get(FONT_SIZE_MEDIUM)->getLetterHeight() * (Renderer::Instance().Is240p() ? 1.4f : 1.0f));
	mImage.setColorShift(menuTheme.Text().color);
	mOriginColor = menuTheme.Text().color;
	mSize = mImage.getSize();
}

void SwitchComponent::onSizeChanged()
{
	mImage.setSize(mSize);
}

void SwitchComponent::setColor(unsigned int color) {
	mImage.setColorShift(color);
}

bool SwitchComponent::ProcessInput(const InputCompactEvent& event)
{
	if(event.ValidPressed())
	{
		setState(!mState);
		return true;
	}

	return false;
}

void SwitchComponent::Render(const Transform4x4f& parentTrans)
{
	Transform4x4f trans = parentTrans * getTransform();

  mImage.Render(trans);

	renderChildren(trans);
}

void SwitchComponent::setImageState()
{
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  mImage.setImage(mState ? menuTheme.Elements().on : menuTheme.Elements().off);
}

void SwitchComponent::setState(bool state)
{
  if (state != mState)
  {
    mState = state;
	onStateChanged();
    setImageState();
  }
}

void SwitchComponent::onStateChanged()
{
	if (mChangedCallback)
	  mChangedCallback(mState);
	if (mInterface != nullptr)
	  mInterface->SwitchComponentChanged(mId, mState);
}

bool SwitchComponent::getHelpPrompts(Help& help)
{
	help.Set(Help::Valid(), _("CHANGE"));
	return true;
}

String SwitchComponent::getValue() const
{
  return mState ? "true" : "false";
}

bool SwitchComponent::changed() const
{
	return mInitialState != mState;
}

