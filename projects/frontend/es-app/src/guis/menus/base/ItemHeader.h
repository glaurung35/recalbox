//
// Created by bkg2k on 22/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include "IHeaderChanged.h"

//! Submenu item
class ItemHeader : public ItemBase
{
  public:
    ItemHeader(const String& label, const MenuThemeData& theme, IHeaderChanged* interface)
      : ItemBase(Type::Header, label, theme, 0, String::Empty, false)
      , mInterface(interface)
      , mIsOpened(true)
    {}

    //! Unfold status
    bool IsOpened() const { return mIsOpened; }

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override
    {
      if (event.ValidReleased())
      {
        mIsOpened = !mIsOpened;
        if (mInterface != nullptr) mInterface->HeaderFoldStateChanged();
        return true;
      }
      return false;
    }

    /*!
     * @brief Called once per frame, after Update.
     * @param area Area to draw within
     */
    void Render(const Rectangle& area) override { (void)area; }

    /*
     * IOverlay implementation
     */

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param parent Parent menu
     * @param area item rectangle
     * @param data Linked data
     * @param text color
     */
    void OverlayDraw(const Menu& parent, const Rectangle& area, const ItemBase& data, Colors::ColorRGBA color) override
    {
      (void)parent;
      (void)area;
      (void)data;
      (void)color;
    }

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @return left offset
     */
    float OverlayLeftOffset(const Menu& parent, const ItemBase& data) override { (void)parent; (void)data; return 0.f; }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    float OverlayRightOffset(const Menu& parent, const ItemBase& data) override { (void)parent; (void)data; return 0.f; }

  private:
    //! Interface
    IHeaderChanged* mInterface;
    //! Open status
    bool mIsOpened;

    //! Fire callback
    void Fire() override {}
};

