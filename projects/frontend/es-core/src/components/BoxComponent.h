//
// Created by bkg2k on 11/12/23.
//
#pragma once

#include "utils/math/Transform4x4f.h"
#include "WindowManager.h"
#include <components/base/ThemableComponent.h>

class BoxComponent : public ThemableComponent
{
  public:
    explicit BoxComponent(WindowManager& window)
      : ThemableComponent(window)
      , mColor(0)
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
     * @brief Set box color
     * @param color RGBA color
     */
    void setColor(Colors::ColorRGBA color) override { mColor = color; }

    /*!
     * @brief Get box color
     * @return Box color
     */
    [[nodiscard]] Colors::ColorRGBA Color() const { return mColor; }

    /*
     * Themable implementation
     */

    /*!
     * @brief Called when a theme element is applyed on the current component.
     * @param element Element instance
     * @param properties Properties to update
     */
    void OnApplyThemeElement(const ThemeElement& element, ThemePropertiesType properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] String ThemeElementType() const override { return "box"; }

  private:
    //! Color
    Colors::ColorRGBA mColor;
};
