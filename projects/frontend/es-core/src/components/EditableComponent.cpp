//
// Created by bkg2k on 21/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <components/EditableComponent.h>
#include <guis/GuiArcadeVirtualKeyboard.h>
#include <themes/ThemeData.h>
#include <WindowManager.h>

EditableComponent::EditableComponent(WindowManager& window)
  : Component(window)
  , mBackground(window, Path(":/textinput_ninepatch.png"))
  , mFont(Font::get(FONT_SIZE_MEDIUM))
  , mTextWidth(0)
  , mTextHeight(0)
  , mInterface(nullptr)
  , mIndentifier(0)
  , mColor(0x000000FF)
  , mOriginColor(0x000000FF)
  , mLineSpacing(1.5f)
  , mHorizontalAlignment(TextAlignment::Left)
  , mVerticalAlignment(TextAlignment::Center)
  , mColorOpacity(0xFF)
  , mUppercase(false)
  , mAutoCalcExtentX(true)
  , mAutoCalcExtentY(true)
  , mMasked(false)
{
  mPosition = Vector3f::Zero();
  mSize = Vector2f::Zero();
  mBackground.setCenterColor(0xFFFFFF80/*0x00000020*/);
  mBackground.setEdgeColor(0xFFFFFF80/*0x00000020*/);
}

EditableComponent::EditableComponent(WindowManager&window, const String& editTitle, const String& text, const std::shared_ptr<Font>& font, unsigned int color, int id, IEditableComponent* interface, bool masked)
  : EditableComponent(window)
{
  mMasked = masked;
  mIndentifier = id;
  mInterface = interface;
  mFont = font;
  mOriginalText = text;
  mEditTitle = editTitle;
  mColor = color;
  mColorOpacity = (unsigned char)(color & 0xFF);
  mOriginColor = color;
  onTextChanged();
}

EditableComponent::EditableComponent(WindowManager&window, const String& editTitle, const String& text, const std::shared_ptr<Font>& font, unsigned int color, const std::function<void(const String&)>& callback)
  : EditableComponent(window)
{
  mTextChanged = callback;
  mFont = font;
  mOriginalText = text;
  mEditTitle = editTitle;
  mColor = color;
  mColorOpacity = (unsigned char)(color & 0xFF);
  mOriginColor = color;
  onTextChanged();
}

EditableComponent::EditableComponent(WindowManager&window, const String& editTitle, const String& text, const std::shared_ptr<Font>& font, unsigned int color, TextAlignment align, const std::function<void(const String&)>& callback)
  : EditableComponent(window)
{
  mTextChanged = callback;
  mFont = font;
  mOriginalText = text;
  mEditTitle = editTitle;
  mColor = color;
  mColorOpacity = (unsigned char)(color & 0xFF);
  mOriginColor = color;
  mHorizontalAlignment = align;
  onTextChanged();
}

void EditableComponent::onSizeChanged()
{
  mAutoCalcExtentX = (getSize().x() == 0);
  mAutoCalcExtentY = (getSize().y() == 0);
  onTextChanged();
}

void EditableComponent::setFont(const std::shared_ptr<Font>& font)
{
  mFont = font;
  onTextChanged();
}

void EditableComponent::setColor(unsigned int color)
{
  mColor = color;
  unsigned char opacity = mColor & 0x000000FF;
  Component::setOpacity(opacity);

  mColorOpacity = mColor & 0x000000FF;
}

//  Scale the opacity
void EditableComponent::setOpacity(unsigned char opacity)
{
  // This method is mostly called to do fading in-out of the Text component element.
  // Therefore, we assume here that opacity is a fractional value (expressed as an int 0-255),
  // of the opacity originally set with setColor() or setBackgroundColor().

  unsigned char o = (unsigned char)((float)opacity / 255.f * (float) mColorOpacity);
  mColor = (mColor & 0xFFFFFF00) | (unsigned char) o;

  Component::setOpacity(opacity);
}

void EditableComponent::setText(const String& text)
{
  mOriginalText = text;
  onTextChanged();
}

void EditableComponent::setUppercase(bool uppercase)
{
  mUppercase = uppercase;
  onTextChanged();
}

void EditableComponent::Render(const Transform4x4f& parentTrans)
{
  if(mThemeDisabled) return;
  Transform4x4f trans = parentTrans * getTransform();

  Renderer::SetMatrix(trans);
  if (mBackground.getSize().x() != mSize.x() &&
      mBackground.getSize().y() != mSize.y())
    mBackground.setSize(mSize);
  mBackground.Render(trans);

  float yOff = 0;
  switch(mVerticalAlignment)
  {
    case TextAlignment::Top: yOff = 0; break;
    case TextAlignment::Bottom: yOff = (getSize().y() - mTextHeight); break;
    case TextAlignment::Center: yOff = (getSize().y() - mTextHeight) / 2.0f; break;
    case TextAlignment::Left:
    case TextAlignment::Right: break;
  }
  Vector3f off(mBackground.MargingX(), yOff, 0);

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
    float usableSize = mSize.x() - mBackground.MargingX() * 2;
    switch(mHorizontalAlignment)
    {
      case TextAlignment::Left:
        Renderer::DrawRectangle(mBackground.MargingX(), 0.0f, mTextCache->metrics.size.x(), mTextCache->metrics.size.y(), 0x00000033);
        break;
      case TextAlignment::Center:
        Renderer::DrawRectangle(mBackground.MargingX() +(usableSize - mTextCache->metrics.size.x()) / 2.0f, 0.0f, mTextCache->metrics.size.x(), mTextCache->metrics.size.y(), 0x00000033);
        break;
      case TextAlignment::Right:
        Renderer::DrawRectangle(mBackground.MargingX() + usableSize - mTextCache->metrics.size.x(), 0.0f, mTextCache->metrics.size.x(), mTextCache->metrics.size.y(), 0x00000033);
        break;
      case TextAlignment::Top:
      case TextAlignment::Bottom:break;
    }
  }*/

  mFont->RenderDirect(mDisplayableText, 0, 0, (mColor >> 8 << 8) | mOpacity, mTextWidth, mHorizontalAlignment, mLineSpacing);
}

void EditableComponent::calculateExtent()
{
  if(mAutoCalcExtentX)
  {
    mSize = mFont->sizeText(mDisplayableText, mLineSpacing) + mBackground.MargingX() * 2;
  }else{
    if(mAutoCalcExtentY)
    {
      mSize[1] = mFont->sizeWrappedText(mDisplayableText, getSize().x(), mLineSpacing).y();
    }
  }
}

void EditableComponent::onTextChanged()
{
  mDisplayableText = mMasked ? String('*', mOriginalText.size()) : (mUppercase ? mOriginalText.ToUpperCaseUTF8() : mOriginalText);
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
  float usableSize = mSize.x() - mBackground.MargingX() * 2;
  if(!isMultiline && (mSize.x() != 0) && (size.x() > usableSize || addAbbrev))
  {
    // abbreviate text
    const String abbrev = "\u2026";
    Vector2f abbrevSize = mFont->sizeText(abbrev);

    while(!mDisplayableText.empty() && size.x() + abbrevSize.x() > usableSize)
    {
      size_t newSize = Font::getPrevCursor(mDisplayableText, mDisplayableText.size());
      mDisplayableText.erase(newSize, mDisplayableText.size() - newSize);
      size = mFont->sizeText(mDisplayableText);
    }

    mDisplayableText.Append(abbrev);

    mTextWidth = size.x() - mBackground.MargingX() * 2;
    mTextHeight = size.y();
  }
  else
  {
    mDisplayableText = mFont->wrapText(mDisplayableText, usableSize);
    size = mFont->sizeWrappedText(mDisplayableText, usableSize, mLineSpacing);
    mTextWidth = size.x();
    mTextHeight = size.y();
  }
}

void EditableComponent::setHorizontalAlignment(TextAlignment align)
{
  mHorizontalAlignment = align;
  onTextChanged();
}

void EditableComponent::setLineSpacing(float spacing)
{
  mLineSpacing = spacing;
  onTextChanged();
}

/*void EditableComponent::applyTheme(const ThemeData& theme, const String& view, const String& element, ThemeProperties properties)
{
  Component::applyTheme(theme, view, element, properties);

  const ThemeElement* elem = theme.getElement(view, element, "text");
  if(elem == nullptr) return;

  if (hasFlag(properties, ThemeProperties::Color) && elem->HasProperty("color"))
    setColor((unsigned int)elem->AsInt("color"));

  if(hasFlag(properties, ThemeProperties::Alignment) && elem->HasProperty("alignment"))
  {
    String str = elem->AsString("alignment");
    if(str == "left")
      setHorizontalAlignment(TextAlignment::Left);
    else if(str == "center")
      setHorizontalAlignment(TextAlignment::Center);
    else if(str == "right")
      setHorizontalAlignment(TextAlignment::Right);
    else
    { LOG(LogError) << "[EditableComponent] Unknown text alignment string: " << str; }
  }

  if (hasFlag(properties, ThemeProperties::Text) && elem->HasProperty("text"))
    setText(elem->AsString("text"));

  if (hasFlag(properties, ThemeProperties::ForceUppercase) && elem->HasProperty("forceUppercase"))
    setUppercase(elem->AsBool("forceUppercase"));

  if (hasFlag(properties, ThemeProperties::LineSpacing) && elem->HasProperty("lineSpacing"))
    setLineSpacing(elem->AsFloat("lineSpacing"));

  setFont(Font::getFromTheme(elem, properties, mFont));
}*/

void EditableComponent::StartEditing()
{
  mTextBackup = mOriginalText;
  mWindow.pushGui(new GuiArcadeVirtualKeyboard(mWindow, mEditTitle, mTextBackup, this));
}

void EditableComponent::ArcadeVirtualKeyboardTextChange(GuiArcadeVirtualKeyboard& vk, const String& text)
{
  (void)vk;
  setText(text);
}

void EditableComponent::ArcadeVirtualKeyboardValidated(GuiArcadeVirtualKeyboard& vk, const String& text)
{
  (void)vk;
  (void)text;
  if (mTextChanged)
    mTextChanged(text);
  if (mInterface != nullptr)
  {
    mInterface->EditableComponentTextChanged(mIndentifier, text);
    mParent->onSizeChanged();
  }
}

void EditableComponent::ArcadeVirtualKeyboardCanceled(GuiArcadeVirtualKeyboard& vk)
{
  (void)vk;
  setText(mTextBackup);
}