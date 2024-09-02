//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/ISubMenuSelected.h>
#include "IItemDataProvider.h"

//! Submenu item
class ItemSubMenu : public ItemBase
{
  public:
    ItemSubMenu(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, ISubMenuSelected* interface, int identifier, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::SubMenu, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mInterface(interface)
      , mWidth(0)
      , mHeight(0)
    {}
    ItemSubMenu(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, ISubMenuSelected* interface, int identifier, MenuThemeData::MenuIcons::Type icon, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::SubMenu, dataProvider, label, theme, identifier, icon, help, unselectable, helpUnselectable)
      , mInterface(interface)
      , mWidth(0)
      , mHeight(0)
    {}

    /*!
     * @brief Allow to change the label of a submenu, since there is no underlying calculation depending
     * of the label width
     * @param newLabel New label
     */
    void ChangeLabel(const String& newLabel) { mLabel = newLabel; }

  private:
    //! Interface
    ISubMenuSelected* mInterface;
    //! On resource
    std::shared_ptr<TextureResource> mArrow;
    //! Arrow width
    int mWidth;
    //! Arrow height
    int mHeight;

    //! Fire callback
    void Fire() final { if (mInterface != nullptr) mInterface->SubMenuSelected(Identifier()); }

    // Label changed
    void LabelChanged() final { }

    //! Load Arrow
    void LoadArrow()
    {
      if (mHeight != 0) return;
      mHeight = mDataProvider.FontHeight();
      if (!mArrow) mArrow = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Arrow, 0, mHeight);
      mWidth = (mArrow->realWidth() * mHeight) / mArrow->realHeight();
    }


    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) final
    {
      if (event.ValidReleased()) { Fire(); return true; }
      return false;
    }

    /*!
     * @brief Called once per frame
     * @param elapsed elaspsed time from last update call
     */
    void Update(int elapsed) final { (void)elapsed; };

    /*!
     * @brief Fill help list
     * @param help Help list to fill
     */
    void CollectHelpItems(Help& help) override
    {
      help.Set(HelpType::B, _("OPEN"));
    }

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param menudata Parent menu data
     * @param area item rectangle
     * @param text color
     */
    void OverlayDraw(int labelWidth, const Rectangle& area, Colors::ColorRGBA color, bool selected) final
    {
      (void)labelWidth;
      (void)selected;
      LoadArrow();
      int y = (int)(area.Top() + ((area.Height() - (float)mHeight) / 2.f));
      int x = (int)(area.Right() - (mDataProvider.Margin() + mWidth));
      Renderer::DrawTexture(*mArrow, x, y, mWidth, mHeight, true, color);
    }

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param menudata Parent menu data
     * @return left offset
     */
    float OverlayLeftOffset(int labelWidth) final { (void)labelWidth; return 0.f; }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param menudata Parent menu data
     * @return right offset
     */
    float OverlayRightOffset(int labelWidth) final
    {
      (void)labelWidth;
      LoadArrow();
      return mDataProvider.FontHeight() + mDataProvider.Margin();
    }
};

