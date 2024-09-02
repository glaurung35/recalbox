//
// Created by bkg2k on 27/08/24.
//
#pragma once

#include "ItemBase.h"
#include "IActionTriggered.h"
#include "NinePatchImage.h"
#include "IItemDataProvider.h"
#include "MenuColors.h"
#include <themes/MenuThemeDataCache.h>

class ItemAction : public ItemBase
{
  public:
    ItemAction(IItemDataProvider& dataProvider, const String& label, const String& buttonLabel, const MenuThemeData& theme, IActionTriggered* interface, int identifier, bool positive, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Action, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mButtonText(positive ? String(buttonLabel).Append(' ').Append("▶") : String("◀").Append(' ').Append(buttonLabel))
      , mButtonFrame(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::ButtonFilled, 0, dataProvider.IconHeight()))
      , mInterface(interface)
      , mButtonWidth(0)
    {}

    ItemAction(IItemDataProvider& dataProvider, const String& label, const String& buttonLabel, const MenuThemeData& theme, IActionTriggered* interface, int identifier, bool positive, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Action, dataProvider, label, theme, identifier, String::Empty, unselectable, helpUnselectable)
      , mButtonText(positive ? String(buttonLabel).Append(' ').Append("▶") : String("◀").Append(' ').Append(buttonLabel))
      , mButtonFrame(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::ButtonFilled, 0, dataProvider.IconHeight()))
      , mInterface(interface)
      , mButtonWidth(0)
    {}

  private:
    //! Button text
    String mButtonText;
    //! Editor background resource
    NinePatchImage mButtonFrame;
    //! Interface
    IActionTriggered* mInterface;
    //! Button width
    int mButtonWidth;

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override
    {
      if (event.ValidReleased())
      {
        Fire();
        return true;
      }
      return false;
    }

    /*!
     * @brief Called once per frame
     * @param elapsed elaspsed time from last update call
     */
    void Update(int elapsed) final { (void)elapsed; }

    /*!
     * @brief Fill help list
     * @param help Help list to fill
     */
    void CollectHelpItems(Help& help) override { (void)help; }

    /*
     * IOverlay implementation
     */

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param menudata Parent menu
     * @param area item rectangle
     * @param text color
     */
    void OverlayDraw(int labelWidth, const Rectangle& area, Colors::ColorRGBA color, bool selected) final
    {
      (void)labelWidth;
      (void)selected;
      int xBack = area.Right() - mDataProvider.Margin() - mButtonFrame.ChunkWidth() - mButtonWidth;
      mButtonFrame.Render(Rectangle(xBack - mButtonFrame.ChunkWidth(), 2, mButtonWidth + mButtonFrame.ChunkWidth() * 2, mDataProvider.ItemHeight() - 4),
                          selected ? MenuColors::MergeColor(mTheme.Text().selectorColor, mTheme.Background().color) : mTheme.Background().color);
      mTheme.Text().font->RenderDirect(mButtonText, xBack, area.Top(), color, mButtonWidth, TextAlignment::Center);
    }

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param menudata Parent menu
     * @return left offset
     */
    float OverlayLeftOffset(int labelWidth) final { (void)labelWidth; return 0.f; }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param menudata Parent menu
     * @return right offset
     */
    float OverlayRightOffset(int labelWidth) final
    {
      (void)labelWidth;
      if (mButtonWidth == 0)
        mButtonWidth = Math::max(mDataProvider.ItemWidth() / 5, mTheme.Text().font->TextWidth(mButtonText));
      return mButtonWidth + mDataProvider.Margin() * 2 + mButtonFrame.ChunkWidth() * 2;
    }

    //! Fire callback
    void Fire() final { if (mInterface != nullptr) mInterface->MenuActionTriggered(Identifier()); }

    // Label changed
    void LabelChanged() final { }
};