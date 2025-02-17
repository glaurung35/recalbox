#pragma once

#include <components/base/ThemableComponent.h>
#include <resources/Font.h>

class ThemeData;

// Used to display text.
// TextComponent::setSize(x, y) works a little differently than most components:
//  * (0, 0)                     - will automatically calculate a size that fits the text on one line (expand horizontally)
//  * (x != 0, 0)                - wrap text so that it does not reach beyond x. Will automatically calculate a vertical size (expand vertically).
//  * (x != 0, y <= fontHeight)  - will truncate text so it fits within this box.
class TextComponent : public ThemableComponent
{
public:
	explicit TextComponent(WindowManager&window);
  TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color);
  TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color, TextAlignment align);
	TextComponent(WindowManager&window, const String& text, const std::shared_ptr<Font>& font, unsigned int color, TextAlignment align,
                Vector3f pos, Vector2f size, unsigned int bgcolor);

  ~TextComponent() override = default;

	void setFont(const std::shared_ptr<Font>& font);
	void setUppercase(bool uppercase);
	void onSizeChanged() override;
	void setText(const String& text);
	void setColor(unsigned int color) override;
	inline void setOriginColor(unsigned int color) { mOriginColor = color; }
	unsigned int getOriginColor() override { return mOriginColor; }

  void setHorizontalAlignment(TextAlignment align);
	void setVerticalAlignment(TextAlignment align) { mVerticalAlignment = align; }

	void setLineSpacing(float spacing);
	void setBackgroundColor(unsigned int color);

	void Render(const Transform4x4f& parentTrans) override;

	[[nodiscard]] String getValue() const override { return mOriginalText; }
	void setValue(const String& value) override { setText(value); }

	[[nodiscard]] unsigned char getOpacity() const override {	return (unsigned char)(mColor & 0xFF); }

  void setOpacity(unsigned char opacity) override;
	
	[[nodiscard]] inline std::shared_ptr<Font> getFont() const { return mFont; }

    /*!
     * @brief Apply theme element to this text component
     * @param element Theme element
     * @param properties Properties to set
     */
    void OnApplyThemeElement(const ThemeElement& theme, ThemePropertyCategory properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType() const override { return ThemeElementType::Text; }

private:
	void calculateExtent();

	void onTextChanged();

  std::shared_ptr<Font> mFont;
  int mTextWidth;
  int mTextHeight;
  String mOriginalText;
  String mDisplayableText;
	unsigned int mColor;
	unsigned int mOriginColor;
	unsigned int mBgColor;
  float mLineSpacing;
	unsigned char mColorOpacity;
	unsigned char mBgColorOpacity;
  TextAlignment mHorizontalAlignment;
  TextAlignment mVerticalAlignment;
	bool mUppercase;
	bool mAutoCalcExtentX;
  bool mAutoCalcExtentY;
  bool mClipped;
  bool mIsMultiline;
};
