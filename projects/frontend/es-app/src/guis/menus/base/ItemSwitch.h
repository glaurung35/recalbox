//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/ISwitchChanged.h>

//! Submenu item
class ItemSwitch : public ItemBase
{
  public:
    ItemSwitch(const String& label, const MenuThemeData& theme, ISwitchChanged* interface, int identifier, bool initialState, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, help, unselectable)
      , mInterface(interface)
      , mState(initialState)
    {}
    ItemSwitch(const String& label, const MenuThemeData& theme, ISwitchChanged* interface, int identifier, MenuThemeData::MenuIcons::Type icon, bool initialState, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, icon, help, unselectable)
      , mInterface(interface)
      , mState(initialState)
    {}

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override { (void)event; return false; }

    /*!
     * @brief Called once per frame, after Update.
     * @param area Area to draw within
     */
    void Render(const Rectangle& area) override { (void)area; }

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
    float OverlayLeftOffset(const Menu& parent, const ItemBase& data) override
    {
      (void)parent;
      (void)data;
      return 0.f;
    }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    float OverlayRightOffset(const Menu& parent, const ItemBase& data) override
    {
      (void)parent;
      (void)data;
      return 0.f;
    }

  private:
    //! Interface
    ISwitchChanged* mInterface;
    //! Value
    bool mState;

    //! Fire callback
    void Fire() override { mInterface->MenuSwitchChanged(Identifier(), mState); }
};

