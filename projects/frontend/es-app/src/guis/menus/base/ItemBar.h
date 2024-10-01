//
// Created by bkg2k on 01/09/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include "IItemDataProvider.h"

class ItemBar : public ItemBase
{
  public:
    ItemBar(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, int identifier, const String& text, float ratio, Colors::ColorRGBA forcedColor, const String& help, bool unselectable, const String& helpUnselectable)
    : ItemBase(Type::Bar, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
    , mOriginalText(text)
    , mTextWidth(0)
    , mRatio(ratio)
    , mForcedColor(forcedColor)
    {}

    //! Get text
    const String& Text() const { return mOriginalText; }

    /*!
     * @brief Set new text
     * @param newText new string
     */
    void SetText(const String& newText)
    {
      mOriginalText = newText;
      mTextWidth = 0; // Allow right part to self calibrate
    }

  private:
    //! Original Text
    String mOriginalText;
    //! Cut Text
    String mShortenedText;
    //! Text width
    int mTextWidth;
    //! Bar ratio
    float mRatio;
    //! Forced color
    Colors::ColorRGBA mForcedColor;

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override { (void)event; return false; }

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
      int x = (int)(area.Right() - mDataProvider.Margin() - mTextWidth);
      int ratio = (int)((float)mTextWidth * mRatio);
      Renderer::DrawRectangle(x + ratio, (int)(area.CenterY() - mDataProvider.IconHeight() / 2), mTextWidth - ratio, (int)mDataProvider.IconHeight(), 0xFFFFFF40);
      Renderer::DrawRectangle(x, (int)(area.CenterY() - mDataProvider.IconHeight() / 2), ratio, (int)mDataProvider.IconHeight(), 0x00000040);
      mTheme.Text().font->RenderDirect(mShortenedText, x, area.Top(), mForcedColor != 0 ? mForcedColor : color, (float)mTextWidth, TextAlignment::Center, mDataProvider.Spacing());
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
      if (mTextWidth == 0)
      {
        mTextWidth = (int)Math::max(mTheme.Text().font->TextWidth(mOriginalText), mDataProvider.ItemWidth() / 3);
        if (CalibrateLeftAndRightParts(labelWidth, mTextWidth, mDataProvider.Margin() * 2))
          mShortenedText = mTheme.Text().font->ShortenText(mOriginalText, mTextWidth);
        else
          mShortenedText = mOriginalText;
      }
      return mTextWidth + mDataProvider.Margin() * 2;
    }

    //! Fire callback
    void Fire() final {}

    // Label changed - Force this item to recalibrate
    void LabelChanged() final { mTextWidth = 0; }
};