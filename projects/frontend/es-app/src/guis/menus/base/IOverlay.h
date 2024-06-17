//
// Created by bkg2k on 22/08/24.
//
#pragma once

#include <utils/gl/Rectangle.h>
#include <utils/gl/Colors.h>

// Forward declarations
class Menu;
class ItemBase;

class IOverlay
{
  public:
    //! default destructor
    virtual ~IOverlay() = default;

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param parent Parent menu
     * @param area item rectangle
     * @param data Linked data
     * @param text color
     */
    virtual void OverlayDraw(const Menu& parent, const Rectangle& area, const ItemBase& data, Colors::ColorRGBA color) = 0;

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @return left offset
     */
    virtual float OverlayLeftOffset(const Menu& parent, const ItemBase& data) = 0;

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    virtual float OverlayRightOffset(const Menu& parent, const ItemBase& data) = 0;
};