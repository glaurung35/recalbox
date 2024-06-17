//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/ISubMenuSelected.h>

//! Submenu item
class ItemSubMenu : public ItemBase
{
  public:
    ItemSubMenu(const String& label, const MenuThemeData& theme, ISubMenuSelected* interface, int identifier, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, help, unselectable)
      , mInterface(interface)
    {}
    ItemSubMenu(const String& label, const MenuThemeData& theme, ISubMenuSelected* interface, int identifier, MenuThemeData::MenuIcons::Type icon, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, icon, help, unselectable)
      , mInterface(interface)
    {}

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override
    {
      if (event.ValidReleased()) { Fire(); return true; }
      return false;
    }

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

      int hw = parent.FontHeight();
      int y = (int)(area.Top() + ((area.Height() - (float)hw) / 2.f));
      int x = (int)(area.Right() - (parent.Margin() + hw));
      std::shared_ptr<TextureResource>& texture = parent.Cache().GetElement(MenuThemeData::IconElement::Type::Arrow);
      Renderer::DrawTexture(*texture, x, y, hw, hw, true, color);
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
    float OverlayRightOffset(const Menu& parent, const ItemBase& data) override
    {
      (void)data;
      return parent.FontHeight() + parent.Margin();
    }

  private:
    //! Interface
    ISubMenuSelected* mInterface;

    //! Fire callback
    void Fire() override { mInterface->SubMenuSelected(Identifier()); }
};

