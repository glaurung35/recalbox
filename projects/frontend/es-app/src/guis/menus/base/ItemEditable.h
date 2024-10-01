//
// Created by bkg2k on 29/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include "guis/GuiArcadeVirtualKeyboard.h"
/*#include "IItemDataProvider.h"
#include "IEditableChanged.h"
#include "NinePatchImage.h"
#include <WindowManager.h>*/

//! Submenu item
class ItemEditable : public ItemBase
                   , private IGuiArcadeVirtualKeyboardInterface
{
  public:
    ItemEditable(WindowManager& window, IItemDataProvider& dataProvider, const String& editTitle, const String& label, const MenuThemeData& theme, IEditableChanged* interface, int identifier, const String& text, bool masked, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Editor, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mEditorBackground(TextureResource::get(Path(":/textinput_ninepatch.png"), false, true, true))
      , mWindow(window)
      , mEditTitle(editTitle.empty() ? label : editTitle)
      , mOriginalText(text)
      , mEditedText(text)
      , mInterface(interface)
      , mTextWidth(0)
      , mMasked(masked)
    {}

    ItemEditable(WindowManager& window, IItemDataProvider& dataProvider, const String& editTitle, const String& label, const MenuThemeData& theme, IEditableChanged* interface, int identifier, const String& text, bool masked, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Editor, dataProvider, label, theme, identifier, String::Empty, unselectable, helpUnselectable)
      , mEditorBackground(TextureResource::get(Path(":/textinput_ninepatch.png"), false, true, true))
      , mWindow(window)
      , mEditTitle(editTitle.empty() ? label : editTitle)
      , mOriginalText(text)
      , mEditedText(text)
      , mInterface(interface)
      , mTextWidth(0)
      , mMasked(masked)
    {}

    //! Get texte
    const String& Text() const { return mEditedText; }

    /*!
     * @brief Set new text
     * @param newText new string
     */
    void SetText(const String& newText)
    {
      mOriginalText = mEditedText = newText;
      mTextWidth = 0; // Allow right part to self calibrate
    }

  private:
    //! Editor background resource
    NinePatchImage mEditorBackground;
    //! Window manager reference
    WindowManager& mWindow;
    //! Edit title
    String mEditTitle;
    //! Original Text
    String mOriginalText;
    //! Edited text
    String mEditedText;
    //! Interface
    IEditableChanged* mInterface;
    //! Text width
    int mTextWidth;
    //! Masked text?
    bool mMasked;

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) override
    {
      if (event.ValidReleased())
      {
        mWindow.pushGui(new GuiArcadeVirtualKeyboard(mWindow, Label(), mEditedText, this));
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
      int xText = (int)(area.Right() - mDataProvider.Margin() - mEditorBackground.ChunkWidth() - mTextWidth);
      int xBack = xText - mEditorBackground.ChunkWidth();
      mEditorBackground.Render(Rectangle(xBack - mEditorBackground.ChunkWidth(), 2, mTextWidth + mEditorBackground.ChunkWidth() * 2, mDataProvider.ItemHeight() - 4),
                               selected ? MenuColors::MergeColor(mTheme.Text().selectorColor, mTheme.Background().color) : mTheme.Background().color);
      String displayable = mMasked ?
                           mTheme.Text().font->ShortenText(String('*', mEditedText.Count()), mTextWidth) :
                           mTheme.Text().font->ShortenText(mEditedText, mTextWidth);
      mTheme.Text().font->RenderDirect(displayable, xText, area.Top(), color, mDataProvider.Spacing());
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
        mTextWidth = mTheme.Text().font->TextWidth(mMasked ? String('*', mEditedText.Count()) : mEditedText);
        (void) CalibrateLeftAndRightParts(labelWidth, mTextWidth, mDataProvider.Margin() * 2);
        if (mTextWidth < mDataProvider.ItemWidth() / 4) mTextWidth = mDataProvider.ItemWidth() / 4;
      }
      return mTextWidth + mDataProvider.Margin() * 2 + mEditorBackground.ChunkWidth() * 2;
    }

    //! Fire callback
    void Fire() final { if (mInterface != nullptr) mInterface->MenuEditableChanged(Identifier(), mEditedText); }

    // Label changed - Force this item to recalibrate
    void LabelChanged() final { mTextWidth = 0; }

    /*
     * IGuiArcadeVirtualKeyboardInterface implementation
     */

    /*!
     * @brief Called when the edited text change.
     * Current text is available from the Text() method.
     */
    void ArcadeVirtualKeyboardTextChange(GuiArcadeVirtualKeyboard& vk, const String& text) final { (void)vk; mEditedText = text; }

    /*!
     * @brief Called when the edited text is validated (Enter or Start)
     * Current text is available from the Text() method.
     */
    void ArcadeVirtualKeyboardValidated(GuiArcadeVirtualKeyboard& vk, const String& text) final { (void)vk; mOriginalText = text; Fire(); }

    /*!
     * @brief Called when the edited text is cancelled.
     */
    void ArcadeVirtualKeyboardCanceled(GuiArcadeVirtualKeyboard& vk) final { (void)vk; mEditedText = mOriginalText; }
};
