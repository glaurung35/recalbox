#include "components/ButtonComponent.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "themes/MenuThemeData.h"
#include <themes/ThemeManager.h>

ButtonComponent::ButtonComponent(WindowManager&window, const String& text, const String& helpText, const std::function<void()>& func, bool upperCase)
  : Component(window)
  , mFont(Font::get(FONT_SIZE_MEDIUM))
  , mFocused(false)
  , mEnabled(true)
  , mTextColorFocused(0xFFFFFFFF), mTextColorUnfocused(0x777777FF)
  , mModdedColor(0)
  , mTextWidth(0)
  , mTextHeight(0)
  , mButton(":/button.png")
  , mButton_filled(":/button_filled.png")
  , mBox(window, mButton)
{
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
	mFont = menuTheme.Text().font;
	mTextColorUnfocused = menuTheme.Text().color;
	mTextColorFocused = menuTheme.Text().selectedColor;
	mColor = menuTheme.Text().color;
	mButton = menuTheme.Elements().FromType(MenuThemeData::IconElement::Type::Button);
	mButton_filled = menuTheme.Elements().FromType(MenuThemeData::IconElement::Type::ButtonFilled);

	setPressedFunc(func);
	setText(text, helpText, upperCase);
	updateImage();
}

void ButtonComponent::onSizeChanged()
{
	mBox.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));
}

void ButtonComponent::setPressedFunc(std::function<void()> f)
{
	mPressedFunc = std::move(f);
}

bool ButtonComponent::ProcessInput(const InputCompactEvent& event)
{
	if(event.ValidReleased())
	{
		if(mPressedFunc && mEnabled)
			mPressedFunc();
		return true;
	}

	return Component::ProcessInput(event);
}

void ButtonComponent::setText(const String& text, const String& helpText, bool upperCase, bool resize, bool doUpdateHelpPrompts)
{
	mText = upperCase ? text.ToUpperCaseUTF8() : text;
	mHelpText = helpText;

  Vector2f size = mFont->sizeText(mText);
  mTextWidth = size.x();
  mTextHeight = size.y();

	if (resize)
	{
		float minWidth = mFont->sizeText("DELETE").x() + 12;
		setSize(Math::max((float)mTextWidth + 12, minWidth), mTextHeight + 8);
	}

	if (doUpdateHelpPrompts)
    UpdateHelpBar();
}

void ButtonComponent::autoSizeFont()
{
	if (mFont->sizeText(mText).x() + 6 > getSize().x()) {
		mFont = Font::get(FONT_SIZE_SMALL);
		onSizeChanged();
	}
	if (mFont->sizeText(mText).x() + 6 > getSize().x()) {
		mFont = Font::get(FONT_SIZE_EXTRASMALL);
		onSizeChanged();
	}
}

bool ButtonComponent::isTextOverlapping()
{
	// + 2 to keep a small space between text and border
	return mFont->sizeText(mText).x() + 2  > getSize().x();
}

void ButtonComponent::onFocusGained()
{
	mFocused = true;
	updateImage();
}

void ButtonComponent::onFocusLost()
{
	mFocused = false;
	updateImage();
}

void ButtonComponent::setEnabled(bool enabled)
{
	mEnabled = enabled;
	updateImage();
}

void ButtonComponent::updateImage()
{
	if(!mEnabled || !mPressedFunc)
	{
		mBox.setImagePath(mButton_filled);
		mBox.setCenterColor(0x770000FF);
		mBox.setEdgeColor(0x770000FF);
		return;
	}

	// If a new color has been set.
	if (mNewColor) {
		mBox.setImagePath(mButton_filled);
		mBox.setCenterColor(mModdedColor);
		mBox.setEdgeColor(mModdedColor);
		return;
	}

	mBox.setCenterColor(mColor);
	mBox.setEdgeColor(mColor);
	mBox.setImagePath(mFocused ? mButton_filled : mButton);
}

void ButtonComponent::Render(const Transform4x4f& parentTrans)
{
  Transform4x4f trans = (parentTrans * getTransform()).round();

  mBox.Render(trans);

  Vector3f centerOffset((mSize.x() - mTextWidth) / 2, (mSize.y() - mTextHeight) / 2, 0);
  centerOffset.round();
  trans = trans.translate(centerOffset);

  Renderer::SetMatrix(trans);
  mFont->RenderDirect(mText, 0, 0, getCurTextColor());
  trans = trans.translate(-centerOffset);

	renderChildren(trans);
}

unsigned int ButtonComponent::getCurTextColor() const
{
	if(!mFocused)
		return mTextColorUnfocused;
	else
		return mTextColorFocused;
}

bool ButtonComponent::CollectHelpItems(Help& help)
{
	help.Set(Help::Valid(), mHelpText.empty() ? mText : mHelpText);
	return true;
}
