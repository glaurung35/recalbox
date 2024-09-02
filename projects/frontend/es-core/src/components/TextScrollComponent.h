//
// Created by bkg2k on 04/12/23.
//
#pragma once

#include "components/base/ThemableComponent.h"
#include "resources/Font.h"

class ThemeData;

class TextScrollComponent : public ThemableComponent
{
  public:
    explicit TextScrollComponent(WindowManager& window);

    TextScrollComponent(WindowManager& window, const String& text, const std::shared_ptr<Font>& font,
                        unsigned int color);

    TextScrollComponent(WindowManager& window, const String& text, const std::shared_ptr<Font>& font,
                        unsigned int color, TextAlignment align);

    TextScrollComponent(WindowManager& window, const String& text, const std::shared_ptr<Font>& font,
                        unsigned int color, TextAlignment align, Vector3f pos, Vector2f size, unsigned int bgcolor);

    ~TextScrollComponent() override = default;

    void setFont(const std::shared_ptr<Font>& font);

    void setUppercase(bool uppercase);

    void setText(const String& text);

    void setColor(unsigned int color) override;

    inline void setOriginColor(unsigned int color)
    { mOriginColor = color; }

    unsigned int getOriginColor() override
    { return mOriginColor; }

    void setHorizontalAlignment(TextAlignment align);

    void setVerticalAlignment(TextAlignment align) { mVerticalAlignment = align; }

    void setBackgroundColor(unsigned int color);

    void setRenderBackground(bool render)
    { mRenderBackground = render; }

    void Render(const Transform4x4f& parentTrans) override;

    String getValue() const override
    { return mOriginalText; }

    void setValue(const String& value) override
    { setText(value); }

    unsigned char getOpacity() const override
    { return (unsigned char) (mColor & 0xFF); }

    void setOpacity(unsigned char opacity) override;

    std::shared_ptr<Font> getFont() const
    { return mFont; }

    void Update(int deltaTime) override;

    /*
     * Themable implementation
     */

    /*!
     * @brief Apply theme element to this nine-patch image
     * @param element Theme element
     * @param properties Properties to set
     */
    void OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType() const override { return ThemeElementType::ScrollText; }

    /*!
     * @brief Return compatible element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType2() const override { return ThemeElementType::Text; }

  private:
    static constexpr int sScrollSpeed1 = 80; // In pixel per seconds
    static constexpr int sScrollSpeed2 = 160; // In pixel per seconds
    static constexpr int sScrollPause = 2000; // In milliseconds

    enum class ScrollSteps
    {
      LeftPause,     //!< Start with a smart pause
      ScrollToRight, //!< Scroll until th elast char is visible
      RightPause,    //!< Smart pause again to let the user read the end
      RollOver,      //!< Scroll until text #2 left part is on the start position and then restart
    };

    void onTextChanged();

    std::shared_ptr<Font> mFont;

    String mOriginalText;
    String mDisplayableText;
    ScrollSteps mStep;
    int mMarqueeTime;
    int mOffset;
    int mTextWidth;
    int mTextHeight;
    unsigned int mColor;
    unsigned int mOriginColor;
    unsigned int mBgColor;
    unsigned char mColorOpacity;
    unsigned char mBgColorOpacity;
    TextAlignment mHorizontalAlignment;
    TextAlignment mVerticalAlignment;
    bool mRenderBackground;
    bool mUppercase;
};

