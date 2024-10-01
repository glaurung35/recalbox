//
// Created by bkg2k on 24/08/24.
//
#pragma once

#include <utils/gl/Rectangle.h>

class MenuThemeDataCache;
class MenuThemeData;

class IItemDataProvider
{
  public:
    //! Default destructor
    virtual ~IItemDataProvider() = default;

    //! Total item height
    [[nodiscard]] virtual float ItemHeight() const = 0;

    //! Total item width, including overlays
    [[nodiscard]] virtual float ItemWidth() const = 0;
    //! Front Icon height
    [[nodiscard]] virtual float IconHeight() const = 0;

    //! Font height
    [[nodiscard]] virtual float FontHeight() const = 0;

    //! Margin
    [[nodiscard]] virtual float Margin() const = 0;

    //! Texture cache
    [[nodiscard]] virtual MenuThemeDataCache& Cache() const = 0;

    //! Get menu maximum area
    [[nodiscard]] virtual Rectangle GetMenuMaximumArea() const = 0;

    //! Get font spacing for drawing
    [[nodiscard]] virtual float Spacing() const = 0;

    //! Is 240p ?
    [[nodiscard]] virtual bool Is240p() const = 0;
};