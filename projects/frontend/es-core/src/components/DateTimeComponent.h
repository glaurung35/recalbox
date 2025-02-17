#pragma once

#include "components/base/ThemableComponent.h"
#include "resources/Font.h"
#include <utils/datetime/DateTime.h>

// Used to enter or display a specific point in time.
class DateTimeComponent : public ThemableComponent
{
public:
	enum class Display
	{
		Date,
		DateTime,
		Time,
		RealTime,
		RelativeToNow,
		Year,
	};

           DateTimeComponent(WindowManager&window, Display dispMode);
  explicit DateTimeComponent(WindowManager&window)
    : DateTimeComponent(window, Display::Date)
  {
  }

	void setValue(const String& val) override;
	void setValue(const DateTime& dt);
	String getValue() const override;

	bool ProcessInput(const InputCompactEvent& event) override;
	void Update(int deltaTime) override;
	void Render(const Transform4x4f& parentTrans) override;
	void onSizeChanged() override;

	// Set how the point in time will be displayed:
	//  * DISP_DATE - only display the date.
	//  * DISP_DATE_TIME - display both the date and the time on that date.
	//  * DISP_RELATIVE_TO_NOW - intelligently display the point in time relative to right now (e.g. "5 secs ago", "3 minutes ago", "1 day ago".  Automatically updates as time marches on.
    //  * DISP_YEAR - only display the year.
    // The initial value is DISP_DATE.
	void setDisplayMode(Display mode);

	void setColor(unsigned int color) final; // Text color.
	inline void setOriginColor(unsigned int color){mOriginColor = color;};
	inline unsigned int getOriginColor() override{return mOriginColor;};
	void setFont(const std::shared_ptr<Font>& font); // Font to display with. Default is Font::get(FONT_SIZE_MEDIUM).
	void setUppercase(bool uppercase); // Force text to be uppercase when in DISP_RELATIVE_TO_NOW mode.
	void setHorizontalAlignment(TextAlignment align);

    /*!
     * @brief Apply theme element to this date component
     * @param element Theme element
     * @param properties Properties to set
     */
    void OnApplyThemeElement(const ThemeElement& theme, ThemePropertyCategory properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType() const override { return ThemeElementType::DateTime; }

  private:
	[[nodiscard]] std::shared_ptr<Font> getFont() const;

	[[nodiscard]] String getDisplayString(Display mode) const;
	[[nodiscard]] Display getCurrentDisplayMode() const;
	
	void updateTextCache();

	DateTime mTime;
	DateTime mTimeBeforeEdit;

	bool mEditing;
	int mEditIndex;
	Display mDisplayMode;

	int mRelativeUpdateAccumulator;

	std::vector<Vector4f> mCursorBoxes;

	unsigned int mColor;
	unsigned int mOriginColor;
	std::shared_ptr<Font> mFont;
    TextAlignment mHorizontalAlignment = TextAlignment::Right;
	bool mUppercase;

	bool mAutoSize;

	bool mFlag;
};
