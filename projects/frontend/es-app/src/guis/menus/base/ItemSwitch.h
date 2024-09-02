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
    /*!
     * @brief Constructor
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Callback interface
     * @param identifier Item identifier
     * @param initialState Initial state on/off
     * @param help Optional help text
     * @param unselectable Grayed ?
     */
    ItemSwitch(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, ISwitchChanged* interface, int identifier, bool initialState, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Switch, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mInterface(interface)
      , mState(initialState)
      , mWidth(0)
      , mHeight(0)
    {}

    /*!
     * @brief Constructor
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Callback interface
     * @param identifier Item identifier
     * @param icon Icon type
     * @param initialState Initial state on/off
     * @param help Optional help text
     * @param unselectable Grayed ?
     */
    ItemSwitch(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, ISwitchChanged* interface, int identifier, MenuThemeData::MenuIcons::Type icon, bool initialState, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Switch, dataProvider, label, theme, identifier, icon, help, unselectable, helpUnselectable)
      , mInterface(interface)
      , mState(initialState)
      , mWidth(0)
      , mHeight(0)
    {}

    /*!
     * @brief Constructor
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Callback interface
     * @param identifier Item identifier
     * @param icon Direct icon path
     * @param initialState Initial state on/off
     * @param help Optional help text
     * @param unselectable Grayed ?
     */
    ItemSwitch(IItemDataProvider& dataProvider, const String& label, const MenuThemeData& theme, ISwitchChanged* interface, int identifier, const Path& icon, bool initialState, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::Switch, dataProvider, label, theme, identifier, icon, help, unselectable, helpUnselectable)
      , mInterface(interface)
      , mState(initialState)
      , mWidth(0)
      , mHeight(0)
    {}

    //! Get current state
    bool State() const { return mState; }

    /*!
     * @brief Set state
     * @param state new state
     * @param callback True to fire the callback if new value is different from the previous
     */
    void SetState(bool state, bool callback)
    {
      bool previous = mState;
      mState = state;
      if (callback && previous != mState) Fire();
    }


  private:
    //! Interface
    ISwitchChanged* mInterface;
    //! Value
    bool mState;
    //! On resource
    std::shared_ptr<TextureResource> mOn;
    //! On resource
    std::shared_ptr<TextureResource> mOff;
    //! Resource width
    int mWidth;
    //! Resource height
    int mHeight;

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) final
    {
      if (event.ValidReleased()) { mState = !mState; Fire(); return true; }
      else if (event.AnyLeftReleased()) { if (mState) { mState = false; Fire(); } return true; }
      else if (event.AnyRightReleased()) { if (!mState) { mState = true; Fire(); } return true; }
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
    void CollectHelpItems(Help& help) override
    {
      help.Set(HelpType::B, _("TOGGLE"));
    }

    /*
     * Overlay
     */

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param parent Parent menu
     * @param area item rectangle
     * @param data Linked data
     * @param text color
     */
    void OverlayDraw(int labelWidth, const Rectangle& area, Colors::ColorRGBA color, bool selected) final
    {
      (void)labelWidth;
      (void)selected;
      LoadResources();
      Renderer::DrawTexture(mState ? *mOn : *mOff,
                            (int)(area.Right() - mDataProvider.Margin() - mWidth), (int)(area.Top() + (area.Height() - mHeight) / 2), mWidth, mHeight,
                            true, color);
    }

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @return left offset
     */
    float OverlayLeftOffset(int labelWidth) final { (void)labelWidth; return 0.f; }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    float OverlayRightOffset(int labelWidth) final
    {
      (void)labelWidth;
      LoadResources();
      return mWidth + mDataProvider.Margin() * 2;
    }

    //! Fire callback
    void Fire() final { if (mInterface != nullptr) mInterface->MenuSwitchChanged(Identifier(), mState); }

    // Label changed
    void LabelChanged() final { }

    //! Load resources
    void LoadResources()
    {
      if (mHeight != 0) return;
      mHeight = mDataProvider.IconHeight();
      if (!mOn)  mOn  = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::On, 0, mHeight);
      if (!mOff) mOff = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Off, 0, mHeight);
      mWidth = (mOn->realWidth() * mHeight) / mOn->realHeight();
    }
};

