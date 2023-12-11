//
// Created by bkg2k on 11/12/23.
//
#pragma once

#include "utils/math/Transform4x4f.h"
#include "WindowManager.h"

class BoxComponent : public Component
{
  public:
    explicit BoxComponent(WindowManager& window)
      : Component(window)
    {
    }

    /*
     * Component override
     */

    /*!
     * @brief Render box on screen
     * @param trans Parent transformation
     */
    void Render(const Transform4x4f& trans) override;

    /*!
     * @brief Apply theme
     * @param theme Theme
     * @param view View name
     * @param element Element name
     * @param properties Properties to set
     */
    void applyTheme(const ThemeData& theme, const String& view, const String& element, ThemeProperties properties) override;

    /*!
     * @brief Set box color
     * @param color RGBA color
     */
    void setColor(Colors::ColorRGBA color) override { mColor = color; }

    /*!
     * @brief Get box color
     * @return Box color
     */
    Colors::ColorRGBA Color() const { return mColor; }

  private:
    //! Color
    Colors::ColorRGBA mColor;
};
