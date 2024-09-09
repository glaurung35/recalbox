//
// Created by bkg2k on 24/08/24.
//
#pragma once

#include <themes/ThemeManager.h>
#include <utils/locale/LocaleHelper.h>
#include "SelectorEntry.h"
#include "ISelectorChanged.h"
#include "ItemBase.h"
#include "MenuColors.h"
#include "NinePatchImage.h"
#include "components/TextListComponent.h"
#include "components/TextScrollComponent.h"
#include <guis/menus/base/IItemDataProvider.h>

//! Callback interface from the GUI to this item
template<typename T>class ISelectorGUIInterface
{
  public:
    //! default destructor
    virtual ~ISelectorGUIInterface() = default;

    /*!
     * @brief Item in the GUI changed
     * @param value Item value
     * @param selected Select state
     */
    virtual void ItemChanged(const T& value, bool selected) = 0;

    /*!
     * @brief Select all/Unsellect all has been selected, select or unselect all
     * @param selected Tru to select all, false to unselec all
     */
    virtual void ItemChangedAll(bool selected) = 0;
};

template<typename T> class ItemSelector : public ItemBase
                                        , ISelectorGUIInterface<T>
{
  public:
    // Entry shortcut
    typedef SelectorEntry<T> Entry;
    // Entry list shortcut
    typedef SelectorEntry<T>::List List;

    /*!
     * @brief Single selector constructor
     * @param window Window manager
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Single selector callback
     * @param identifier Item identifier
     * @param values Selector values
     * @param initialValue Initial value (will be selected automatically)
     * @param defaultValue Default selected value
     * @param help Help string
     * @param unselectable Grayed ?
     */
    ItemSelector(IItemDataProvider& dataProvider, WindowManager& window, const String& label, const MenuThemeData& theme, ISingleSelectorChanged<T>* interface, int identifier, const List& values, const T& initialValue, const T& defaultValue, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::List, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mWindow(window)
      , mButton(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Button, 0, 0))
      , mSingleInterface(interface)
      , mMultiInterface(nullptr)
      , mList(UppercaseOnce(values))
      , mLastSelectedIndex(0)
      , mCount(0)
      , mMulti(false)
      , mArrowWidth(0)
      , mArrowHeight(0)
      , mItemMaxWidth(0)
      , mMargin(Renderer::Instance().DisplayWidthAsInt() / 160)
      , mQuickSelectionTimer(0)
      , mTruncated(false)
    {
      SelectSingleValue(initialValue, defaultValue);
    }

    /*!
     * @brief Single selector constructor
     * @param window Window manager
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Single selector callback
     * @param identifier Item identifier
     * @param icon Icon type
     * @param values Selector values
     * @param initialValue Initial value (will be selected automatically)
     * @param defaultValue Default selected value
     * @param help Help string
     * @param unselectable Grayed ?
     */
    ItemSelector(IItemDataProvider& dataProvider, WindowManager& window, const String& label, const MenuThemeData& theme, ISingleSelectorChanged<T>* interface, int identifier, MenuThemeData::MenuIcons::Type icon, const List& values, const T& initialValue, const T& defaultValue, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::List, dataProvider, label, theme, identifier, icon, help, unselectable, helpUnselectable)
      , mWindow(window)
      , mButton(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Button, 0, 0))
      , mSingleInterface(interface)
      , mMultiInterface(nullptr)
      , mList(UppercaseOnce(values))
      , mLastSelectedIndex(0)
      , mCount(0)
      , mMulti(false)
      , mArrowWidth(0)
      , mArrowHeight(0)
      , mItemMaxWidth(0)
      , mMargin(Renderer::Instance().DisplayWidthAsInt() / 320)
      , mQuickSelectionTimer(0)
      , mTruncated(false)
    {
      SelectSingleValue(initialValue, defaultValue);
    }

    /*!
     * @brief Multiple selector constructor
     * @param window Window manager
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Multi selector callback
     * @param identifier Item identifier
     * @param values Selector values
     * @param help Help string
     * @param unselectable Grayed ?
     */
    ItemSelector(IItemDataProvider& dataProvider, WindowManager& window, const String& label, const MenuThemeData& theme, IMultiSelectorChanged<T>* interface, int identifier, const List& values, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::List, dataProvider, label, theme, identifier, help, unselectable, helpUnselectable)
      , mWindow(window)
      , mButton(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Button, 0, 0))
      , mSingleInterface(nullptr)
      , mMultiInterface(interface)
      , mList(UppercaseOnce(values))
      , mLastSelectedIndex(0)
      , mCount(0)
      , mMulti(true)
      , mArrowWidth(0)
      , mArrowHeight(0)
      , mItemMaxWidth(0)
      , mMargin(Renderer::Instance().DisplayWidthAsInt() / 320)
      , mQuickSelectionTimer(0)
      , mTruncated(false)
    {
      UpdateCount();
    }

    /*!
     * @brief Multiple selector constructor
     * @param window Window manager
     * @param label Item label
     * @param theme Menu theme reference
     * @param interface Multi selector callback
     * @param identifier Item identifier
     * @param icon Icon type
     * @param values Selector values
     * @param help Help string
     * @param unselectable Grayed ?
     */
    ItemSelector(IItemDataProvider& dataProvider, WindowManager& window, const String& label, const MenuThemeData& theme, IMultiSelectorChanged<T>* interface, int identifier, MenuThemeData::MenuIcons::Type icon, const List& values, const String& help, bool unselectable, const String& helpUnselectable)
      : ItemBase(Type::List, dataProvider, label, theme, identifier, icon, help, unselectable, helpUnselectable)
      , mWindow(window)
      , mButton(dataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Button, 0, 0))
      , mSingleInterface(nullptr)
      , mMultiInterface(interface)
      , mList(UppercaseOnce(values))
      , mLastSelectedIndex(0)
      , mCount(0)
      , mMulti(true)
      , mArrowWidth(0)
      , mArrowHeight(0)
      , mItemMaxWidth(0)
      , mMargin(Renderer::Instance().DisplayWidthAsInt() / 320)
      , mQuickSelectionTimer(0)
      , mTruncated(false)
    {
      UpdateCount();
    }

    //! Destructor
    ~ItemSelector()
    {
      // Fire immediately if a delayed timer is still active
      if (mQuickSelectionTimer > 0) Fire();
    }

    /*
     * Accessors
     */

    //! Count for multi-lists. Always 1 for single lists
    [[nodiscard]] int SelectedCount() const { return mMulti ? mCount : 1; }
    //! Last selected index (or modified in multi-lists)
    [[nodiscard]] int SelectedIndex() const { return mLastSelectedIndex; }
    //! Last selected value (or modified in multi-lists)
    [[nodiscard]] T SelectedValue() const { return mList[mLastSelectedIndex].mValue; }
    //! Selected value list. Contains always one item in single lists
    [[nodiscard]] std::vector<T> AllSelectedValues() const
    {
      std::vector<T> list;
      for (const Entry& entry: mList) if (entry.mSelected) list.push_back(entry.mValue);
      return list;
    }

    // TODO: Check T is a cardinal
    [[nodiscard]] Array<T> AllSelectedValuesAsArray() const
    {
      Array<T> list(SelectedCount());
      for (const Entry& entry: mList) if (entry.mSelected) list.Add(entry.mValue);
      return list;
    }

    /*!
     * @brief Rebuild the item using a new array of data
     * @param values New values
     * @param initialValue Initial value
     * @param defaultValue Default value
     */
    void ChangeSelectionItems(const List& values, const T& initialValue, const T& defaultValue)
    {
      mList = values;
      SelectSingleValue(initialValue, defaultValue);
    }

    /*!
     * @brief Rebuild the item using a new array of data. Previous selection is the new initial data
     * @param values New values
     * @param defaultValue Default value
     */
    void ChangeSelectionItems(const List& values, const T& defaultValue)
    {
      T previousValue = mList[mLastSelectedIndex].mValue;
      mList = values;
      SelectSingleValue(previousValue, defaultValue);
    }

    /*!
     * @brief Rebuild the item using a new array of data. Previous selection is the new initial data
     * and the first available data is the default
     * @param values New values
     * @param defaultValue Default value
     */
    void ChangeSelectionItems(const List& values)
    {
      T previousValue = mList[mLastSelectedIndex].mValue;
      mList = values;
      T defaultValue = mList.front().mValue;
      SelectSingleValue(previousValue, defaultValue);
    }

    /*!
     * @brief Set selected item by index
     * @param index Index to set
     * @param callback True to call the interface callback
     */
    void SetSelectedItemIndex(int index, bool callback)
    {
      SelectSingleValue(mList[index % (int)mList.size()], mList[index % (int)mList.size()]);
      if (callback) Fire();
    }

    /*!
     * @briefSet selected item by value
     * @param value Value to set
     * @param callback True to call the interface callback
     */
    void SetSelectedItemValue(const T& value, bool callback)
    {
      SelectSingleValue(value, value);
      if (callback) Fire();
    }

  private:
    /*
     * Item base implementation
     */

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    bool ProcessInput(const InputCompactEvent& event) final
    {
      if (event.ValidReleased()) ShowGUI();
      else if (!mMulti)
      {
        if (event.AnyLeftReleased()) MoveSelection(false);
        else if (event.AnyRightReleased()) MoveSelection(true);
      }
      return false;
    }

    /*!
     * @brief Called once per frame
     * @param elapsed elaspsed time from last update call
     */
    void Update(int elapsed) final
    {
      if (mQuickSelectionTimer > 0)
        if (mQuickSelectionTimer -= elapsed; mQuickSelectionTimer <= 0)
        {
          mQuickSelectionTimer = 0;
          Fire();
        }
    }

    /*!
     * @brief Fill help list
     * @param help Help list to fill
     */
    void CollectHelpItems(Help& help) override
    {
      help.Set(HelpType::B, _("SELECT"));
      if (!mMulti) help.Set(HelpType::LeftRight, _("CHANGE"));
    }

    //! Fire callback
    void Fire() final
    {
      if (mMulti)
      {
        if (mMultiInterface != nullptr)
        {
          std::vector<T> list;
          for (const Entry& entry: mList) if (entry.mSelected) list.push_back(entry.mValue);
          if (mMultiInterface != nullptr)
            mMultiInterface->MenuMultiChanged(mIdentifier, mLastSelectedIndex, list);
        }
      }
      else if (mSingleInterface != nullptr)
        for (const Entry& entry: mList)
          if (entry.mSelected)
            if (mSingleInterface != nullptr)
              mSingleInterface->MenuSingleChanged(mIdentifier, mLastSelectedIndex, entry.mValue);
    }

    // Label changed - Force this item to recalibrate
    void LabelChanged() final { mArrowHeight = 0; }

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param menudata Parent menu
     * @param area item rectangle
     * @param text color
     */
    void OverlayDraw(int labelWidth, const Rectangle& area, Colors::ColorRGBA color, bool selected) final
    {
      (void)area;
      (void)color;
      (void)selected;
      LoadArrow(labelWidth);
      int xRightArrow = area.Right() - mDataProvider.Margin() - mArrowWidth;
      int xText = xRightArrow - mMargin - mItemMaxWidth;
      int xLeftArrow = xText - mMargin - mArrowWidth;
      int yArrows = area.CenterY() - mArrowHeight / 2;
      Renderer::DrawTexture(*mArrow, xRightArrow, yArrows, mArrowWidth, mArrowHeight, true, color);
      Renderer::DrawTexture(*mArrow, xLeftArrow, yArrows, mArrowWidth, mArrowHeight, true, false, true, color);
      mButton.Render(Rectangle(xText - 2, 2, mItemMaxWidth + 4, area.Height() - 4), color);
      if (mMulti)
      {
        Font& font = *mTheme.Text().font;
        if (mTruncated) font.RenderDirect(font.ShortenText(mSummary, mItemMaxWidth), xText, 0, color, mItemMaxWidth, TextAlignment::Center);
        else            font.RenderDirect(mSummary, xText, 0, color, mItemMaxWidth, TextAlignment::Center);
      }
      else
      {
        Font& font = *mTheme.Text().font;
        if (mTruncated) font.RenderDirect(font.ShortenText(mList[mLastSelectedIndex].mText, mItemMaxWidth), xText, 0, color, mItemMaxWidth, TextAlignment::Center);
        else            font.RenderDirect(mList[mLastSelectedIndex].mText, xText, 0, color, mItemMaxWidth, TextAlignment::Center);
      }
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
      LoadArrow(labelWidth);
      return (float)(mItemMaxWidth + mArrowWidth * 2 - mMargin * 2 + mDataProvider.Margin());
    }

    // Uppercase text once
    static ItemSelector::List UppercaseOnce(const ItemSelector::List& items)
    {
      ItemSelector::List result(items);
      // Uppercase once
      for(SelectorEntry<T>& entry : result)
        entry.mText.UpperCaseUTF8();
      return result;
    }

    //! Selector GUI
    class Selector : public Gui
                   , public ITextListComponentOverlay<SelectorEntry<T>*>
    {
      public:
        Selector(WindowManager& window, IItemDataProvider& dataprovider, ISelectorGUIInterface<T>& interface, ItemBase& item, List& items, bool multi, int selectedIndex);

      private:
        //! Main component grid
        ComponentGrid mGrid;
        //! List component
        std::shared_ptr<TextListComponent<SelectorEntry<T>*>> mItemList;

        //! Unselected checkbox resource
        std::shared_ptr<TextureResource> mUnselected;
        //! Selected checkbox resource
        std::shared_ptr<TextureResource> mSelected;

        //! Data provider
        IItemDataProvider& mDataProvider;
        //! Callback interface
        ISelectorGUIInterface<T>& mCallback;
        //! Theme menu reference
        const MenuThemeData& mTheme;
        //! Background image
        NinePatchImage mBackground;
        //! Button image
        NinePatchImage mButton;
        //! List reference from the selector
        List& mList;
        //! Checkbox width
        int mCheckBoxWidth;
        //! Checkbox height
        int mCheckBoxHeight;
        //! Multi mode?
        bool mMulti;
        //! Initialized ?
        bool mInitialized;

        /*!
         * @brief This little window displays over the menu option that called it
         * @return True if the GUI is an overlay
         */
        [[nodiscard]] virtual bool IsOverlay() const final { return true; }

        /*!
         * @brief Notification of an input event
         * @param event Compact event
         * @return Implementation must return true if it consumed the event.
         */
        bool ProcessInput(const InputCompactEvent& event) final
        {
          if (event.StartReleased()) { mWindow.CloseAll(); return true;}
          if (event.CancelReleased()) { Close(); }
          if (event.ValidReleased())
          {
            SelectorEntry<T>* entry = mItemList->getSelected();
            // Select all / Unselect all
            if (entry == nullptr)
            {
              int index = mItemList->getCursorIndex();
              mCallback.ItemChangedAll(index == 0);
              return true;
            }
            // Normal item
            if (mMulti) entry->mSelected = !entry->mSelected;
            else Close();
            mCallback.ItemChanged(entry->mValue, entry->mSelected);
            return true;
          }
          return Gui::ProcessInput(event);
        }

        /*!
         * @brief Lazy selector initialization
         * @param deltaTime Elapsed time from the previous frame, in millisecond
         */
        void Update(int deltaTime) final
        {
          Gui::Update(deltaTime);
          if (!mInitialized)
          {
            SetMenuSize();
            mInitialized = true;
          }
        }

        void Render(const Transform4x4f& parentTrans) final
        {

          Transform4x4f trans = parentTrans * getTransform();
          Renderer::SetMatrix(trans);

          Vector2i screen(Renderer::Instance().DisplayWidthAsInt(), Renderer::Instance().DisplayHeightAsInt());
          Renderer::Instance().PushClippingRect(Vector2i(0, (int)trans.translation().y() + mGrid.getRowHeight(0)), screen);
          // Render background
          mBackground.Render(Rectangle(-2, -2, mSize.x() + 4, mSize.y() + 4), mTheme.Background().color);
          // Pop clipping
          Renderer::Instance().PopClippingRect();

          // Get clipping area
          Renderer::Instance().PushClippingRect(Vector2i(0, 0), Vector2i(screen.x(), (int)trans.translation().y() + mGrid.getRowHeight(0)));
          // Redraw part of the background
          mBackground.Render(Rectangle(-2, -2, mSize.x() + 4, mSize.y() + 4), (mTheme.Text().selectorColor & 0xFFFFFF00) | (mTheme.Background().color & 0xFF));
          // Pop clipping
          Renderer::Instance().PopClippingRect();

          // Render grid
          Gui::Render(parentTrans);
        }

        /*!
         * @brief Fill help list
         * @param help Help list to fill
         */
        bool CollectHelpItems(Help& help) override
        {
          if (mMulti)
            help.Set(HelpType::B, _("TOGGLE"))
                .Set(HelpType::A, _("BACK"));
          else
            help.Set(HelpType::B, _("SELECT"))
                .Set(HelpType::A, _("BACK"));
          return true;
        }

        //! Set menu size
        void SetMenuSize()
        {
          Rectangle areaMax(mDataProvider.GetMenuMaximumArea());
          Rectangle menu(Renderer::Instance().DisplayWidthAsFloat(),
                         (Renderer::Instance().DisplayHeightAsFloat() - MenuHeight()) / 2.f,
                         MenuWidth(), MenuHeight());
          // Constraint
          if (menu.Right() > areaMax.Right()) menu.Move(areaMax.Right() - menu.Right(), 0);
          if (menu.Left() < areaMax.Left()) menu.MoveLeft(areaMax.Left() - menu.Left());
          if (menu.Bottom() > areaMax.Bottom()) menu.Move(0, areaMax.Bottom() - menu.Bottom());
          if (menu.Top() < areaMax.Top()) menu.MoveTop(areaMax.Top() - menu.Top());
          menu.Round();
          // Set
          setSize(menu.Width(), menu.Height());
          setPosition(menu.Left(), menu.Top());
          mWindow.UpdateHelpSystem();
        }

        //! Get title height
        float TitleHeight() const
        {
          float padding = (Renderer::Instance().DisplayHeightAsFloat() * 0.03f);
          return mTheme.Text().font->getLetterHeight() + padding;
        }

        /// Calculate menu height
        float MenuHeight() const
        {
          const float maxHeight = Renderer::Instance().DisplayHeightAsFloat() * 0.9f;
          float baseHeight = TitleHeight() + 8;
          int maxrows = (int)(maxHeight - baseHeight) / (int)mItemList->EntryHeight();
          return baseHeight + (float)(Math::min((int)mItemList->size(), maxrows)) * mItemList->EntryHeight();
        }

        //! Get fixed selector menu width
        float MenuWidth() const
        {
          int maxWidth = 0;
          for(const SelectorEntry<T>& entry : mList)
            if (int w = mTheme.SmallText().font->TextWidth(entry.mText);
                w > maxWidth)
              maxWidth = w;
          maxWidth += mDataProvider.Margin() * 2;
          if (mMulti) maxWidth += mDataProvider.Margin() + mCheckBoxWidth;
          return (float)maxWidth;
        }

        //! Adjust sizes
        void onSizeChanged() override;

        /*
         * Overlay interface implementation
         */

        /*!
         * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
         * so that the text won't draw over the overlay if required
         * @param parentTrans Parent transform matrice
         * @param position Top/Left of the item rectangle
         * @param size  Width/Height of the item rectangle
         * @param data Linked data
         */
        void OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size, int labelWidth, SelectorEntry<T>* const& data, unsigned int& color) override
        {
          (void)parentTrans;
          (void)position;
          (void)labelWidth;
          if (mMulti)
          {
            if (data != nullptr)
              Renderer::DrawTexture(data->mSelected ? *mSelected : *mUnselected,
                                    mDataProvider.Margin(), (size.y() - mCheckBoxHeight) / 2,
                                    mCheckBoxWidth, mCheckBoxHeight, true, color);
            else
              mButton.Render(Rectangle(Vector2f(0, 0), size).Contract(mDataProvider.Margin() * 2, 1),
                             mItemList->getSelected() == data ? mTheme.Background().color : mTheme.Text().color);
          }
        }

        /*!
         * @brief Get the left offset (margin to the text) if any
         * @param data Linked data
         * @return left offset
         */
        float OverlayGetLeftOffset(SelectorEntry<T>* const& data, int labelWidth) override
        {
          (void)data;
          (void)labelWidth;
          if (mMulti)
          {
            // Lazy resources load
            if (mCheckBoxHeight == 0)
            {
              mCheckBoxHeight = mTheme.SmallText().font->getMaxHeight();
              if (!mUnselected) mUnselected = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::CheckBox, 0, mCheckBoxHeight);
              if (!mSelected) mSelected = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::CheckBoxFilled, 0, mCheckBoxHeight);
              mCheckBoxWidth = (mSelected->realWidth() * mCheckBoxHeight) / mSelected->realHeight();
            }
            return mDataProvider.Margin() * 2 + mCheckBoxWidth;
          }
          return mDataProvider.Margin();
        }

        /*!
         * @brief Get the right offset (margin from text to right limit) if any
         * @param data Linked data
         * @return right offset
         */
        float OverlayGetRightOffset(SelectorEntry<T>* const& data, int labelWidth) override
        {
          (void)data;
          (void)labelWidth;
          return mDataProvider.Margin();
        }
    };

    //! Apply change timer
    static constexpr int sQuickChangeTimer = 750;

    //! Windows manager reference
    WindowManager& mWindow;

    //! Left arrow resource
    std::shared_ptr<TextureResource> mArrow;
    //! Niepatch empty button
    NinePatchImage mButton;

    //! Single interface
    ISingleSelectorChanged<T>* mSingleInterface;
    //! Multiple interface
    IMultiSelectorChanged<T>* mMultiInterface;

    //! Value list
    List mList;
    //! Multi-selector summary cache
    String mSummary;
    //! Last changed/selected index
    int mLastSelectedIndex;
    //! Selected value count in multi mode
    int mCount;
    //! List mode
    bool mMulti;
    //! Arrow width
    int mArrowWidth;
    //! Arrow height
    int mArrowHeight;
    //! Maximum item's text length
    int mItemMaxWidth;
    //! Margin between text and arrows
    int mMargin;
    //! Quick selection timer
    int mQuickSelectionTimer;
    //! Pössible truncated text
    bool mTruncated;

    //! Load Arrow
    void LoadArrow(int labelWidth)
    {
      if (mArrowHeight != 0) return;

      // Load arrow image
      mArrowHeight = mDataProvider.FontHeight();
      if (!mArrow) mArrow = mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::OptionArrow, 0, mArrowHeight);
      mArrowWidth = (mArrow->realWidth() * mArrowHeight) / mArrow->realHeight();

      // Calculate items' length
      int width = 0;
      if (!mMulti)
      {
        Font& font = *mTheme.Text().font;
        for (const SelectorEntry<T>& entry: mList)
          if (int newWidth = (int)font.TextWidth(entry.mText); newWidth > width)
            width = newWidth;
      }
      else
      {
        int maxChar = (int)log10((float)mList.size()) + 1;
        width = mTheme.Text().font->TextWidth(String('0', maxChar).Append('/').Append('0', maxChar).Append(' ').Append(_("SELECTED")));
      }

      // Add spacers & arrows width
      int decoWidth = mArrowWidth * 2 + mMargin * 3;

      // Constraint ?
      mTruncated = CalibrateLeftAndRightParts(labelWidth, mItemMaxWidth = width, decoWidth);
    }

    /*!
     * @brief Ensure the given value is the only one selected in the list
     * @param initial Initial value to initialise to selected value
     * @param defaultValue Default value if the initial value is not found ihn the list.
     * if the default value is neither found, LOG an error
     */
    void SelectSingleValue(const T& initial, const T& defaultValue)
    {
      for(Entry& entry : mList) entry.mSelected = false;
      bool found = false;
      for(int i = (int)mList.size(); --i >= 0; ) if (Entry& entry = mList[i]; entry.mValue == initial) { entry.mSelected = found = true; mLastSelectedIndex = i; break; }
      if (!found)
        for(int i = (int)mList.size(); --i >= 0; ) if (Entry& entry = mList[i]; entry.mValue == defaultValue) { entry.mSelected = found = true; mLastSelectedIndex = i; break; }
      if (!found)
      {
        LOG(LogError) << "[Menu:ItemSelector] Neither initial nor default value were found in item " << Label();
        if (!mList.empty()) mList.front().mSelected = true;
        mLastSelectedIndex = 0;
      }
    }

    void MoveSelection(bool forward)
    {
      if (mMulti) return;
      for(int i = (int)mList.size(); --i >= 0; )
        if (Entry& entry = mList[i]; entry.mSelected)
        {
          int index = ((i + (forward ? 1 : -1)) + (int)mList.size()) % (int)mList.size();
          entry.mSelected = false;
          mList[mLastSelectedIndex = index].mSelected = true;
          ItemChanged(mList[index].mValue, true);
          mQuickSelectionTimer = sQuickChangeTimer;
          break;
        }
    }

    /*!
     * @brief Show GUI
     * @param value
     * @param selected
     */
    void ShowGUI()
    {
      Selector* selector = new Selector(mWindow, mDataProvider, *this, *this, mList, mMulti, mLastSelectedIndex);
      mWindow.pushGui(selector);
    }

    /*!
     * @brief Update selected item counter
     */
    void UpdateCount()
    {
      mCount = 0;
      for(const Entry& entry : mList) if (entry.mSelected) mCount++;
      mSummary = String(mCount).Append('/').Append((int)mList.size()).Append(' ').Append(_("SELECTED"));
    }

    /*
     * ISelectorGUIChangeCallback
     */

    /*!
     * @brief  Callback from the GUI when a change occured
     * @param value Value selected
     * @param selected Selected state in multu mode, otherwise always true
     */
    void ItemChanged(const T& value, bool selected) final
    {
      // Select new item
      for(int i = (int)mList.size(); --i >= 0;)
      {
        Entry& entry = mList[i];
        if (!mMulti) entry.mSelected = false;
        if (entry.mValue == value)
        {
          entry.mSelected = mMulti ? selected : true;
          mLastSelectedIndex = i;
        }
      }
      // Count
      if (mMulti) UpdateCount();
      // Callback
      Fire();
    }

    /*!
     * @brief Select all/Unsellect all has been selected, select or unselect all
     * @param selected Tru to select all, false to unselec all
     */
    void ItemChangedAll(bool selected) final
    {
      if (!mMulti) return;
      // Select/Unsellect all
      for(Entry& entry : mList)
        entry.mSelected = selected;
      // Count
      if (mMulti) UpdateCount();
      // Callback
      Fire();
    }
};

template<typename T> ItemSelector<T>::Selector::Selector(WindowManager& window, IItemDataProvider& dataprovider, ISelectorGUIInterface<T>& interface, ItemBase& item, ItemSelector::List& items, bool multi, int selectedIndex)
  : Gui(window)
  , mGrid(window, Vector2i(1, 3))
  , mDataProvider(dataprovider)
  , mCallback(interface)
  , mTheme(ThemeManager::Instance().Menu())
  , mBackground(TextureResource::get(mTheme.Background().path, false, true, true))
  , mButton(mDataProvider.Cache().GetElement(MenuThemeData::IconElement::Type::Button, 0, 0))
  , mList(items)
  , mCheckBoxWidth(0)
  , mCheckBoxHeight(0)
  , mMulti(multi)
  , mInitialized(false)
{
  addChild(&mGrid);

  // set up title
  std::shared_ptr<TextScrollComponent> scrolltitle = std::make_shared<TextScrollComponent>(mWindow);
  scrolltitle->setHorizontalAlignment(TextAlignment::Center);
  scrolltitle->setText(String(' ').Append(item.Label()).Append(' ').ToUpperCaseUTF8());
  scrolltitle->setFont(mTheme.Text().font);
  scrolltitle->setColor(mTheme.Background().color);
  mGrid.setEntry(scrolltitle, Vector2i(0, 0), false, true);

  // set up list
  mItemList = std::make_shared<TextListComponent<SelectorEntry<T>*>>(mWindow);
  mItemList->SetOverlayInterface(this);
  mItemList->setUppercase(false);
  mItemList->setFont(mTheme.SmallText().font);
  mItemList->setSelectedColor(mTheme.Text().selectedColor);
  mItemList->setSelectorColor(mTheme.Text().selectorColor);
  mItemList->setColorAt(MenuColors::sSelectableColor, mTheme.Text().color);                   // Text color
  mItemList->setColorAt(MenuColors::sBackgroundColor, mTheme.Background().color);             // Unselected Background color
  mItemList->setColorAt(MenuColors::sUnselectableColor, MenuColors::Alpha25Percent(mTheme.Text().color)); // Grayed item
  mItemList->setColorAt(MenuColors::sHeaderBackgroundColor, 0x00000040);                      // Grayed item - TODO: make this color themable
  mItemList->setSelectorHeight(mItemList->EntryHeight());
  mItemList->setShiftSelectedTextColor(true);
  mItemList->setAutoAlternate(true);
  mItemList->setAlignment(mMulti ? HorizontalAlignment::Left : HorizontalAlignment::Center);
  mItemList->setCursorChangedCallback([this](CursorState state) { if (state == CursorState::Stopped) mWindow.UpdateHelpSystem(); });
  mGrid.setEntry(mItemList, Vector2i(0, 1), true);

  // Fill selector
  if (mMulti)
  {
    mItemList->add(_("SELECT ALL"), nullptr, 0);
    mItemList->add(_("SELECT NONE"), nullptr, 0);
  }
  for(SelectorEntry<T>& entry : mList)
    mItemList->add(entry.mText, &entry, 0);
  mItemList->setCursorIndex(mMulti ? 2 : selectedIndex);

  mGrid.resetCursor();
}

template<typename T> void ItemSelector<T>::Selector::onSizeChanged()
{
  // update grid row/col sizes
  mGrid.setRowHeightPerc(0, TitleHeight() / mSize.y());
  mGrid.setRowHeightPerc(1, 0);
  mGrid.setRowHeightPerc(2, 0.02f);
  mGrid.setColWidthPerc(0, 0);

  mGrid.setSize(mSize);
}



