#include "components/TextComponent.h"
#include "WindowManager.h"
#include "themes/ThemeData.h"
#include "utils/Files.h"

TextComponent::TextComponent(WindowManager&window)
  : ThemableComponent(window)
  , mFont(Font::get(FONT_SIZE_MEDIUM))
  , mTextWidth(0)
  , mTextHeight(0)
  , mColor(0x000000FF)
  , mOriginColor(0x000000FF)
  , mBgColor(0)
  , mLineSpacing(1.5f)
  , mColorOpacity(0xFF)
  , mBgColorOpacity(0)
  , mHorizontalAlignment(TextAlignment::Left)
  , mVerticalAlignment(TextAlignment::Center)
  , mUppercase(false)
  , mAutoCalcExtentX(true)
  , mAutoCalcExtentY(true)
  , mClipped(false)
  , mIsMultiline(false)
{
  mPosition = Vector3f::Zero();
  mSize = Vector2f::Zero();
}
TextComponent::TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color)
  : TextComponent(window)
{
  mFont = font;
  mOriginalText = text;
  mColor = color;
  mColorOpacity = (unsigned char)(color & 0xFF);
  mOriginColor = color;
  onTextChanged();
}

TextComponent::TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color, TextAlignment align)
  : TextComponent(window)
{
  mFont = font;
  mOriginalText = text;
  mColor = color;
  mColorOpacity = (unsigned char)(color & 0xFF);
  mOriginColor = color;
  mHorizontalAlignment = align;
  onTextChanged();
}

TextComponent::TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color, TextAlignment align,
                             Vector3f pos, Vector2f size, unsigned int bgcolor)
  : TextComponent(window, text, font, color, align)
{
  mBgColor = bgcolor;
  mBgColorOpacity = (unsigned char)(bgcolor & 0xFF);
  mPosition = pos;
  mSize = size;
}

void TextComponent::onSizeChanged()
{
  mAutoCalcExtentX = (getSize().x() == 0);
  mAutoCalcExtentY = (getSize().y() == 0);
  onTextChanged();
}

void TextComponent::setFont(const std::shared_ptr<Font>& font)
{
  if (mFont != font)
  {
    mFont = font;
    onTextChanged();
  }
}

void TextComponent::setColor(unsigned int color)
{
  if (color != mColor)
  {
    mColor = color;
    unsigned char opacity = mColor & 0x000000FF;
    Component::setOpacity(opacity);

    mColorOpacity = mColor & 0x000000FF;
  }
}

//  Set the color of the background box
void TextComponent::setBackgroundColor(unsigned int color)
{
  mBgColor = color;
  mBgColorOpacity = mBgColor & 0x000000FF;
}


//  Scale the opacity
void TextComponent::setOpacity(unsigned char opacity)
{
  if (opacity != mOpacity)
  {
    // This method is mostly called to do fading in-out of the Text component element.
    // Therefore, we assume here that opacity is a fractional value (expressed as an int 0-255),
    // of the opacity originally set with setColor() or setBackgroundColor().

    unsigned char o = (unsigned char) ((float) opacity / 255.f * (float) mColorOpacity);
    mColor = (mColor & 0xFFFFFF00) | (unsigned char) o;

    unsigned char bgo = (unsigned char) ((float) opacity / 255.f * (float) mBgColorOpacity);
    mBgColor = (mBgColor & 0xFFFFFF00) | (unsigned char) bgo;

    Component::setOpacity(opacity);
  }
}

void TextComponent::setText(const String& text)
{
  if (mOriginalText != text)
  {
    mOriginalText = text;
    onTextChanged();
  }
}

void TextComponent::setUppercase(bool uppercase)
{
  if (uppercase != mUppercase)
  {
    mUppercase = uppercase;
    onTextChanged();
  }
}

void TextComponent::Render(const Transform4x4f& parentTrans)
{
  if(mThemeDisabled) return;
  
  Transform4x4f trans = parentTrans * getTransform();

  if ((mBgColor & 0xFF) != 0)
  {
    Renderer::SetMatrix(trans);
    Renderer::DrawRectangle(0.f, 0.f, mSize.x(), mSize.y(), mBgColor);
  }

  float yOff = 0;
  switch(mVerticalAlignment)
    {
    case TextAlignment::Top:
      yOff = 0;
      break;
    case TextAlignment::Bottom:
      yOff = (getSize().y() - mTextHeight);
      break;
    case TextAlignment::Center:
      yOff = (getSize().y() - mTextHeight) / 2.0f;
      break;
      case TextAlignment::Left:
      case TextAlignment::Right: break;
    }
  Vector3f off(0, yOff, 0);

  /*if (Settings::Instance().DebugText())
  {
    // draw the "textbox" area, what we are aligned within
    Renderer::SetMatrix(trans);
    Renderer::DrawRectangle(0.f, 0.f, mSize.x(), mSize.y(), 0xFF000033);
  }*/

  trans.translate(off);
  trans.round();
  Renderer::SetMatrix(trans);

  // draw the text area, where the text actually is going
  /*if (Settings::Instance().DebugText())
  {
    switch(mHorizontalAlignment)
    {
    case TextAlignment::Left:
      Renderer::DrawRectangle(0.0f, 0.0f, (float)mTextWidth, (float)mTextWidth, 0x00000033);
      break;
    case TextAlignment::Center:
      Renderer::DrawRectangle((mSize.x() - mTextWidth) / 2.0f, 0.0f, (float)mTextWidth, (float)mTextHeight, 0x00000033);
      break;
    case TextAlignment::Right:
      Renderer::DrawRectangle(mSize.x() - mTextWidth, 0.0f, (float)mTextWidth, (float)mTextHeight, 0x00000033);
      break;
      case TextAlignment::Top:
      case TextAlignment::Bottom:break;
    }
  }*/

  mFont->RenderDirect(mDisplayableText, 0, 0, (mColor >> 8 << 8) | mOpacity, mSize.x(), mHorizontalAlignment, mLineSpacing);
}

void TextComponent::calculateExtent()
{
  if(mAutoCalcExtentX)
  {
    mSize = mFont->sizeText(mDisplayableText, mLineSpacing);
  }else{
    if(mAutoCalcExtentY)
    {
      mSize[1] = mFont->sizeWrappedText(mDisplayableText, getSize().x(), mLineSpacing).y();
    }
  }
}

void TextComponent::onTextChanged()
{
  mDisplayableText = mUppercase ? mOriginalText.ToUpperCaseUTF8() : mOriginalText;
  calculateExtent();
  if (!mFont || mDisplayableText.empty()) return;

  const bool isMultiline = (mSize.y() == 0 || mSize.y() > mFont->getHeight()*1.2f);
  bool addAbbrev = false;
  if (!isMultiline)
  {
    int newline = mDisplayableText.Find('\n');
    if (newline >= 0) mDisplayableText = mDisplayableText.Delete(newline, INT32_MAX); // single line of text - stop at the first newline since it'll mess everything up
    addAbbrev = newline >= 0;
  }

  Vector2f size = mFont->sizeText(mDisplayableText);
  if(!isMultiline && (mSize.x() != 0) && (size.x() > mSize.x() || addAbbrev))
  {
    // abbreviate text
    const String abbrev = "\u2026";
    Vector2f abbrevSize = mFont->sizeText(abbrev);

    while(!mDisplayableText.empty() && size.x() + abbrevSize.x() > mSize.x())
    {
      size_t newSize = Font::getPrevCursor(mDisplayableText, mDisplayableText.size());
      mDisplayableText.erase(newSize, mDisplayableText.size() - newSize);
      size = mFont->sizeText(mDisplayableText);
    }

    mDisplayableText.Append(abbrev);

    mTextWidth = size.x();
    mTextHeight = size.y();
  }
  else
  {
    mDisplayableText = mFont->wrapText(mDisplayableText, mSize.x());
    size = mFont->sizeWrappedText(mDisplayableText, getSize().x(), mLineSpacing);
    mTextWidth = size.x();
    mTextHeight = size.y();
  }
}

void TextComponent::setHorizontalAlignment(TextAlignment align)
{
  mHorizontalAlignment = align;
  onTextChanged();
}

void TextComponent::setLineSpacing(float spacing)
{
  mLineSpacing = spacing;
  onTextChanged();
}

void TextComponent::OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties)
{
  if (hasFlag(properties, ThemePropertyCategory::Color))
  {
    setColor(element.HasProperty(ThemePropertyName::Color) ? (unsigned int)element.AsInt(ThemePropertyName::Color) : 0);
    setBackgroundColor(element.HasProperty(ThemePropertyName::BackgroundColor) ? (unsigned int)element.AsInt(ThemePropertyName::BackgroundColor) : 0);
  }

  if(hasFlag(properties, ThemePropertyCategory::Alignment))
  {
    if (element.HasProperty(ThemePropertyName::Alignment))
    {
      String str = element.AsString(ThemePropertyName::Alignment);
      if (str == "left") setHorizontalAlignment(TextAlignment::Left);
      else if (str == "center") setHorizontalAlignment(TextAlignment::Center);
      else if (str == "right") setHorizontalAlignment(TextAlignment::Right);
      else { LOG(LogError) << "[TextComponent] Unknown text alignment string: " << str; }
    }
    else setHorizontalAlignment(TextAlignment::Left);
  }

  if (hasFlag(properties, ThemePropertyCategory::Text | ThemePropertyCategory::Path))
  {
    if      (element.HasProperty(ThemePropertyName::Text)) setText(element.AsString(ThemePropertyName::Text));
    else if (element.HasProperty(ThemePropertyName::Path)) setText(Files::LoadFile(element.AsPath(ThemePropertyName::Path)));
    else                                                   setText(String::Empty);
  }

  if (hasFlag(properties, ThemePropertyCategory::ForceUppercase))
    setUppercase(element.HasProperty(ThemePropertyName::ForceUppercase) && element.AsBool(ThemePropertyName::ForceUppercase));

  if (hasFlag(properties, ThemePropertyCategory::LineSpacing))
    setLineSpacing(element.HasProperty(ThemePropertyName::LineSpacing) ? element.AsFloat(ThemePropertyName::LineSpacing) : 1.5f);

  setFont(Font::getFromTheme(element, properties, mFont));
}
