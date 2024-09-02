//
// Created by bkg2k on 22/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include "IHeaderChanged.h"
#include "IItemDataProvider.h"

//! Submenu item
class ItemHeader : public ItemBase
{
  public:
    ItemHeader(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, IHeaderChanged* interface)
      : ItemBase(Type::Header, dataProvider, String::Empty, theme, 0, String::Empty, false, String::Empty)
      , mSectionTheme(theme.Section())
      , mInterface(interface)
      , mLabel(label)
      , mLabelWidth(0)
      , mGap(Renderer::Instance().DisplayWidthAsInt() / 160)
      , mIsOpened(true)
    {
      UpdateLabel();
    }

    //! Unfold status
    bool IsOpened() const { return mIsOpened; }

  private:
    //! Section menu theme reference
    const MenuThemeData::MenuElement& mSectionTheme;
    //! Interface
    IHeaderChanged* mInterface;
    //! Label (emptied in list)
    String mLabel;
    //! Displayable label
    String mDisplayableLabel;
    //! Label size
    int mLabelWidth;
    //! Gap between label & lines
    int mGap;
    //! Open status
    bool mIsOpened;

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
        UpdateLabel();
        if (mInterface != nullptr) Fire();
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
      color = selected ? mSectionTheme.selectedColor : mSectionTheme.color;
      Renderer::DrawRectangle(2, (int)area.CenterY() - 1, (int)area.Right() - (mLabelWidth + mGap * 2 + 2), 2, color);
      mSectionTheme.font->RenderDirect(mDisplayableLabel, area.Right() - (mLabelWidth + mGap + 2), 0.f, color);
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
    float OverlayRightOffset(int labelWidth) final { (void)labelWidth; return 0.f; }

    //! Fire callback
    void Fire() final { if (mInterface != nullptr) mInterface->HeaderFoldStateChanged(); }

    // Label changed
    void LabelChanged() final { }

    //! Update Displayable Label
    void UpdateLabel()
    {
      mDisplayableLabel = String(mIsOpened ? "▼ " : "◀ ").Append(mLabel);
      mLabelWidth = (int)mSectionTheme.font->sizeText(mDisplayableLabel).x();
    }
};

