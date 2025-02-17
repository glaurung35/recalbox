#include "components/DateTimeComponent.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "utils/Log.h"
#include "utils/locale/LocaleHelper.h"
#include "themes/MenuThemeData.h"
#include <themes/ThemeManager.h>

DateTimeComponent::DateTimeComponent(WindowManager&window, Display dispMode)
  : ThemableComponent(window)
  , mEditing(false)
  , mEditIndex(0)
  , mDisplayMode(dispMode)
  , mRelativeUpdateAccumulator(0)
  , mColor(0x777777FF)
  , mOriginColor(0)
  , mFont(Font::get(FONT_SIZE_SMALL, Path(FONT_PATH_LIGHT)))
  , mUppercase(false)
  , mAutoSize(true)
{
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
	setFont(menuTheme.SmallText().font);
	setColor(menuTheme.Text().color);
	setOriginColor(mColor);
	mFlag = true;
	updateTextCache();
}

void DateTimeComponent::setDisplayMode(Display mode)
{
	mDisplayMode = mode;
	updateTextCache();
}

bool DateTimeComponent::ProcessInput(const InputCompactEvent& event)
{
	if (event.ValidReleased())
	{
		if(mDisplayMode != Display::RelativeToNow) //don't allow editing for relative times
			mEditing = !mEditing;

		if(mEditing)
		{
			//started editing
			mTimeBeforeEdit = mTime;
		}

		return true;
	}

	if(mEditing)
	{
		if (event.CancelReleased())
		{
			mEditing = false;
			mTime = mTimeBeforeEdit;
			updateTextCache();
			return true;
		}

		int incDir = 0;
		if (event.AnyUpPressed() || event.L1Pressed())
			incDir = 1;
		else if (event.AnyDownPressed() || event.R1Pressed())
			incDir = -1;

		if(incDir != 0)
		{
			if(mEditIndex == 0)
			{
        mTime.AddYears(incDir);
			}
			else if(mEditIndex == 1)
			{
        mTime.AddMonth(incDir);
			}
			else if(mEditIndex == 2)
			{
        mTime.AddDays(incDir);
			}

			updateTextCache();
			return true;
		}

		if (event.AnyRightPressed())
		{
			mEditIndex++;
			if(mEditIndex >= (int)mCursorBoxes.size())
				mEditIndex--;
			return true;
		}
		
		if (event.AnyLeftPressed())
		{
			mEditIndex--;
			if(mEditIndex < 0)
				mEditIndex++;
			return true;
		}
	}

	return Component::ProcessInput(event);
}

void DateTimeComponent::Update(int deltaTime)
{
	if(mDisplayMode == Display::RelativeToNow || mDisplayMode == Display::RealTime)
	{
		mRelativeUpdateAccumulator += deltaTime;
		if(mRelativeUpdateAccumulator > 1000)
		{
			mRelativeUpdateAccumulator = 0;
			updateTextCache();
		}
	}

  Component::Update(deltaTime);
}

void DateTimeComponent::Render(const Transform4x4f& parentTrans)
{
  if(mThemeDisabled) return;

	Transform4x4f trans = parentTrans * getTransform();

  // vertically center
  Vector3f off(0, (mSize.y() - mFont->getMaxHeight()) / 2, 0);
  trans.translate(off);
  trans.round();

  Renderer::SetMatrix(trans);

  std::shared_ptr<Font> font = getFont();

  Display mode = getCurrentDisplayMode();
  const String dispString = mUppercase ? getDisplayString(mode).UpperCase() : getDisplayString(mode);
  font->RenderDirect(dispString, 0, 0, (mColor & 0xFFFFFF00) | getOpacity(), mSize.x(), mHorizontalAlignment);

  if(mEditing)
  {
    if(mEditIndex >= 0 && (unsigned int)mEditIndex < mCursorBoxes.size())
    {
      Renderer::DrawRectangle((int)mCursorBoxes[mEditIndex][0], (int)mCursorBoxes[mEditIndex][1],
        (int)mCursorBoxes[mEditIndex][2], (int)mCursorBoxes[mEditIndex][3], 0x00000022);
    }
  }
}

void DateTimeComponent::setValue(const String& val)
{
  DateTime dt(false);
  if (DateTime::FromCompactISO6801(val, dt))
  {
    mTime = dt;
    updateTextCache();
  }
}

void DateTimeComponent::setValue(const DateTime& dt)
{
  mTime = dt;
  updateTextCache();
}

String DateTimeComponent::getValue() const
{
	return mTime.ToCompactISO8601();
}

DateTimeComponent::Display DateTimeComponent::getCurrentDisplayMode() const
{
	/*if(mEditing)
	{
		if(mDisplayMode == DISP_RELATIVE_TO_NOW)
		{
			//TODO: if time component == 00:00:00, return DISP_DATE, else return DISP_DATE_TIME
			return DISP_DATE;
		}
	}*/

	return mDisplayMode;
}

String DateTimeComponent::getDisplayString(Display mode) const
{
	char strbuf[256];

	switch(mode)
	{
	  case Display::Date: return mTime.ToStringFormat("%YYYY/%MM/%dd");
    case Display::DateTime: return mTime.ToStringFormat("%YYYY/%MM/%dd %HH:%mm:%ss");
    case Display::Time: return mTime.ToStringFormat("%HH:%mm:%ss");
    case Display::Year: return mTime.ToStringFormat("%YYYY");
    case Display::RealTime: return DateTime().ToStringFormat("%HH:%mm:%ss");
    case Display::RelativeToNow:
		{
            TimeSpan diff = DateTime() - mTime;
            if (diff.IsNegative() || diff.TotalDays() > 3560)
                return _("never");

			if (diff.TotalSeconds() < 2) return _("just now");
			if (diff.TotalSeconds() < 60) { snprintf(strbuf, sizeof(strbuf), _N("%i sec ago", "%i secs ago", diff.TotalSeconds()).c_str(), (int) diff.TotalSeconds()); return strbuf; }
			if (diff.TotalMinutes() < 60) { snprintf(strbuf, sizeof(strbuf), _N("%i min ago", "%i mins ago", diff.TotalMinutes()).c_str(), (int) diff.TotalMinutes()); return strbuf; }
			if (diff.TotalHours()   < 24) { snprintf(strbuf, sizeof(strbuf), _N("%i hour ago", "%i hours ago", diff.TotalHours()).c_str(), (int) diff.TotalHours()); return strbuf; }
			snprintf(strbuf, sizeof(strbuf), _N("%i day ago", "%i days ago", diff.TotalDays()).c_str(), (int) diff.TotalDays());
			return strbuf;
		}
	  default: break;
	}

	return "???";
}

std::shared_ptr<Font> DateTimeComponent::getFont() const
{
	if(mFont)
		return mFont;

	return Font::get(FONT_SIZE_MEDIUM);
}

void DateTimeComponent::updateTextCache()
{
	mFlag = !mFlag;

  Display mode = getCurrentDisplayMode();
  const String dispString = mUppercase ? getDisplayString(mode).UpperCase() : getDisplayString(mode);
	if(mAutoSize)
	{
		mSize = mFont->sizeText(dispString);

		//mAutoSize = false;
		if(getParent() != nullptr)
			getParent()->onSizeChanged();
	}

	//set up cursor positions
	mCursorBoxes.clear();

	if(dispString.empty() || mode == Display::RelativeToNow)
		return;

	//month
	Vector2f start(0, 0);
	Vector2f end = mFont->sizeText(dispString.SubString(0, 2));
	Vector2f diff = end - start;
	mCursorBoxes.push_back(Vector4f(start[0], start[1], diff[0], diff[1]));

	//day
	start[0] = mFont->sizeText(dispString.SubString(0, 3)).x();
	end = mFont->sizeText(dispString.SubString(0, 5));
	diff = end - start;
	mCursorBoxes.push_back(Vector4f(start[0], start[1], diff[0], diff[1]));

	//year
	start[0] = mFont->sizeText(dispString.SubString(0, 6)).x();
	end = mFont->sizeText(dispString.SubString(0, 10));
	diff = end - start;
	mCursorBoxes.push_back(Vector4f(start[0], start[1], diff[0], diff[1]));

	//if mode == DISP_DATE_TIME do times too but I don't wanna do the logic for editing times because no one will ever use it so screw it
}

void DateTimeComponent::setColor(unsigned int color)
{
	mColor = color;
}

void DateTimeComponent::setFont(const std::shared_ptr<Font>& font)
{
	mFont = font;
	updateTextCache();
}

void DateTimeComponent::setHorizontalAlignment(TextAlignment align)
{
	mHorizontalAlignment = align;
	updateTextCache();
}

void DateTimeComponent::onSizeChanged()
{
	mAutoSize = false;
	updateTextCache();
}

void DateTimeComponent::setUppercase(bool uppercase)
{
	mUppercase = uppercase;
	updateTextCache();
}

void DateTimeComponent::OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties)
{
	// We set mAutoSize BEFORE calling GuiComponent::applyTheme because it calls
	// setSize(), which will call updateTextCache(), which will reset mSize if 
	// mAutoSize == true, ignoring the theme's value.
  // #TODO: Hu? This doesn't make sense, mAutoSize is true if there is no size property
	if (hasFlag(properties, ThemePropertyCategory::Size))
		mAutoSize = !element.HasProperty(ThemePropertyName::Size);

	if (hasFlag(properties, ThemePropertyCategory::Color))
		setColor(element.HasProperty(ThemePropertyName::Color) ? (unsigned int)element.AsInt(ThemePropertyName::Color) : 0);

	if (hasFlag(properties, ThemePropertyCategory::Display))
  {
    if (element.HasProperty(ThemePropertyName::Display))
    {
      String str = element.AsString(ThemePropertyName::Display);
      if (str == "date") setDisplayMode(Display::Date);
      else if (str == "dateTime") setDisplayMode(Display::DateTime);
      else if (str == "year") setDisplayMode(Display::Year);
      else if (str == "realTime") setDisplayMode(Display::RealTime);
      else if (str == "time") setDisplayMode(Display::Time);
      else if (str == "RelativeToNow") setDisplayMode(Display::RelativeToNow);
      else { LOG(LogError) << "[DateTimeComponent] Unknown date time display mode string: " << str; }
    }
    //else setDisplayMode(Display::Date);
  }

	if (hasFlag(properties, ThemePropertyCategory::Alignment))
  {
    if (element.HasProperty(ThemePropertyName::Alignment))
    {
      String str = element.AsString(ThemePropertyName::Alignment);
      if (str == "left") setHorizontalAlignment(TextAlignment::Left);
      else if (str == "center") setHorizontalAlignment(TextAlignment::Center);
      else if (str == "right") setHorizontalAlignment(TextAlignment::Right);
      else
      { LOG(LogError) << "[DateTimeComponent] Unknown text alignment string: " << str; }
    }
    else setHorizontalAlignment(TextAlignment::Left);
  }
	if (hasFlag(properties, ThemePropertyCategory::ForceUppercase))
		setUppercase(element.HasProperty(ThemePropertyName::ForceUppercase) && element.AsBool(ThemePropertyName::ForceUppercase));

	setFont(Font::getFromTheme(element, properties, mFont));
}
