//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/Gui.h>
#include <components/ITextListComponentOverlay.h>
#include "components/TextListComponent.h"
#include "ISubMenuSelected.h"
#include "ISwitchChanged.h"
#include "themes/MenuThemeDataCache.h"
#include "IHeaderChanged.h"
#include <guis/menus/base/ItemBase.h>

class Menu : public Gui
           , public ITextListComponentOverlay<ItemBase*>
           , public IHeaderChanged
{
  public:
    /*!
     * @brief Constructor
     * @param window Window manager
     * @param title Title (mandatory)
     * @param footer Title (mandatory)
     */
    Menu(WindowManager& window, const String& title, const String& footer);

    /*!
     * @brief Constructor
     * @param window Window manager
     * @param title Title (mandatory)
     */
    Menu(WindowManager& window, const String& title) : Menu(window, title, String::Empty) {}

    /*
     * API for Overlay
     */

    //! Total item height
    float ItemHeight() const { return mList->EntryHeight(); }

    //! Front Icon height
    float IconHeight() const { return mList->EntryHeight() * 0.7f; }

    //! Font height
    float FontHeight() const { return mList->FontHeight() * 0.8f; }

    //! Margin
    float Margin() const { return mTextMargin; }

    //! Texture cache
    MenuThemeDataCache& Cache() const { return mCache; };

    /*
     * Helpers
     */

    /*!
     * @brief Add simpple header
     * @param label Label
     * @return Item index
     */

    int AddHeader(const String& label);

    /*!
     * @brief Add Submenu
     * @param label Label
     * @param func Method to call back when selected
     * @param help Help string
     * @return Item index
     */
    int AddSubMenu(const String& label, int id, ISubMenuSelected* interface, const String& help, bool unselectable = false);

    /*!
     * @brief Add Submenu
     * @param label Label
     * @param func Method to call back when selected
     * @return Item index
     */
    int AddSubMenu(const String& label, int id, ISubMenuSelected* interface, bool unselectable = false) { return AddSubMenu(label, id, interface, String::Empty, unselectable); }

    /*!
     * @brief Add Submenu with Icon
     * @param label Label
     * @param icon Icon image path
     * @param func Method to call back when selected
     * @param help Help string
     * @return Item index
     */
    int AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface, const String& help, bool unselectable = false);

    /*!
     * @brief Add Submenu with Icon
     * @param label Label
     * @param icon Icon image path
     * @param func Method to call back when selected
     * @return Item index
     */
    int AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface, bool unselectable = false) { return AddSubMenu(label, icon, id, interface, String::Empty, unselectable); }

    /*!
     * @brief Add a switch menu entry
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @return Item index
     */
    int AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable = false);

    /*!
     * @brief Add a switch menu entry
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @return Item index
     */
    int AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, bool unselectable = false) { return AddSwitch(label, value, id, interface, String::Empty, unselectable); }

    /*!
     * @brief Add a switch menu entry
     * @param icon
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @return Item index
     */
    int AddSwitch(MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable = false);

    /*!
     * @brief Add a switch menu entry
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @return Item index
     */
    int AddSwitch(MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface, bool unselectable) { return AddSwitch(icon, label, value, id, interface, String::Empty, unselectable); }

    //    /*!
//     * @brief Add a switch menu entry
//     * @param text Menu text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @param help Help text
//     * @return Switch component
//     */
//    std::shared_ptr<RatingComponent> AddRating(const String& text, float value, int id, IRatingComponent* interface, const String& help);
//
//    /*!
//     * @brief Add a switch menu entry
//     * @param text Menu text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @return Switch component
//     */
//    std::shared_ptr<RatingComponent> AddRating(const String& text, float value, int id, IRatingComponent* interface);
//
//
//    /*!
//     * @brief Add non editable text
//     * @param text Caption text
//     * @param value Text value
//     * @param help Help text
//     * @return TextComponent
//     */
//    std::shared_ptr<TextComponent> AddText(const String& text, const String& value, const String& help);
//
//    /*!
//     * @brief Add non editable text
//     * @param text Caption text
//     * @param value Text value
//     * @return TextComponent
//     */
//    std::shared_ptr<TextComponent> AddText(const String& text, const String& value);
//
//    /*!
//     * @brief Add non editable text
//     * @param text Caption text
//     * @param value Text value
//     * @param color Text color
//     * @param help Help text
//     * @return TextComponent
//     */
//    std::shared_ptr<TextComponent> AddText(const String& text, const String& value, unsigned int color, const String& help);
//
//    /*!
//     * @brief Add non editable text
//     * @param text Caption text
//     * @param value Text value
//     * @param color Text color
//     * @return TextComponent
//     */
//    std::shared_ptr<TextComponent> AddText(const String& text, const String& value, unsigned int color);
//
//    /*!
//     * @brief Add editable text
//     * @param text Caption text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @param help Help text
//     * @return TextComponent
//     */
//    std::shared_ptr<EditableComponent> AddEditable(const String& edittitle, const String& text, const String& value, int id, IEditableComponent* interface, const String& help, bool masked);
//
//    /*!
//     * @brief Add editable text
//     * @param text Caption text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @return TextComponent
//     */
//    std::shared_ptr<EditableComponent> AddEditable(const String& edittitle, const String& text, const String& value, int id, IEditableComponent* interface, bool masked);
//
//    /*!
//     * @brief Add editable text
//     * @param text Caption text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @param help Help text
//     * @return TextComponent
//     */
//    std::shared_ptr<EditableComponent> AddEditable(const String& text, const String& value, int id, IEditableComponent* interface, const String& help, bool masked);
//
//    /*!
//     * @brief Add editable text
//     * @param text Caption text
//     * @param value Initial value
//     * @param callback Callback when entry changes
//     * @return TextComponent
//     */
//    std::shared_ptr<EditableComponent> AddEditable(const String& text, const String& value, int id, IEditableComponent* interface, bool masked);
//
//    /*!
//     * @brief Add a Slider menu entry
//     * @param text Menu text
//     * @param min Minimum value
//     * @param max Maximum value
//     * @param value Initial value
//     * @param id Identifier
//     * @param interface Callback interface
//     * @param help Help text
//     * @return Slider component
//     */
//    std::shared_ptr<SliderComponent> AddSlider(const String& text, float min, float max, float inc, float value, const String& suffix, int id, ISliderComponent* interface, const String& help);
//
//    /*!
//     * @brief Add a Slider menu entry
//     * @param text Menu text
//     * @param min Minimum value
//     * @param max Maximum value
//     * @param value Initial value
//     * @param id Identifier
//     * @param interface Callback interface
//     * @return Slider component
//     */
//    std::shared_ptr<SliderComponent> AddSlider(const String& text, float min, float max, float inc, float value, const String& suffix, int id, ISliderComponent* interface);
//
//    //! Normalized entry structure for List components
//    template<typename T> struct ListEntry
//    {
//      String mText;   //!< Displayed text
//      T mValue;       //!< Linked Values
//      bool mSelected; //!< Whether the entry is selected or not
//
//      //! Constructor
//      ListEntry(const String& t, const T& v, bool s)
//        : mText(t)
//        , mValue(v)
//        , mSelected(s)
//      {}
//    };
//
//    /*!
//     * @brief Add a choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param id Identifier
//     * @param interface callback interface
//     * @param entries Entries to add to the list
//     * @param help Help text
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddList(const String& text, int id, IOptionListComponent<T>* interface, const std::vector<ListEntry<T>>& entries, const String& help)
//    {
//      auto result = std::make_shared<OptionListComponent<T>>(mWindow, text);
//      if (!entries.empty())
//      {
//        for (const ListEntry<T>& entry : entries)
//          result->add(entry.mText, entry.mValue, entry.mSelected);
//        mMenu.addWithLabel(result, text, help);
//        result->SetInterface(id, interface);
//      }
//      return result;
//    }
//
//    /*!
//     * @brief Add a choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @param entries Entries to add to the list
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddList(const String& text, int id, IOptionListComponent<T>* interface, const std::vector<ListEntry<T>>& entries)
//    {
//      return AddList(text, id, interface, entries, String::Empty);
//    }
//
//    /*!
//     * @brief Add an empty choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @param help Help text
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddList(const String& text, int id, IOptionListComponent<T>* interface, const String& help)
//    {
//      auto result = std::make_shared<OptionListComponent<T>>(mWindow, text);
//      mMenu.addWithLabel(result, text, help);
//      result->SetInterface(id, interface);
//      return result;
//    }
//
//    /*!
//     * @brief Add an empty choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddList(const String& text, int id, IOptionListComponent<T>* interface)
//    {
//      return AddList(text, id, interface, String::Empty);
//    }
//
//    /*!
//     * @brief Add a multi-choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param id Identifier
//     * @param interface callback interface
//     * @param entries Entries to add to the list
//     * @param help Help text
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddMultiList(const String& text, int id, IOptionListMultiComponent<T>* interface, const std::vector<ListEntry<T>>& entries, const String& help)
//    {
//      auto result = std::make_shared<OptionListComponent<T>>(mWindow, text, true);
//      if (!entries.empty())
//      {
//        for (const ListEntry<T>& entry : entries)
//          result->add(entry.mText, entry.mValue, entry.mSelected);
//        mMenu.addWithLabel(result, text, help);
//        result->SetMultiInterface(id, interface);
//      }
//      return result;
//    }
//
//    /*!
//     * @brief Add a multi-choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @param entries Entries to add to the list
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddMultiList(const String& text, int id, IOptionListMultiComponent<T>* interface, const std::vector<ListEntry<T>>& entries)
//    {
//      return AddMultiList(text, id, interface, entries, String::Empty);
//    }
//
//    /*!
//     * @brief Add an empty multi-choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @param help Help text
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddMultiList(const String& text, int id, IOptionListMultiComponent<T>* interface, const String& help)
//    {
//      auto result = std::make_shared<OptionListComponent<T>>(mWindow, text, true);
//      mMenu.addWithLabel(result, text, help);
//      result->SetMultiInterface(id, interface);
//      return result;
//    }
//
//    /*!
//     * @brief Add an empty multi-choice list
//     * @tparam T Select object type
//     * @param text Menu text (and selection window title)
//     * @param callback Callback when a new value is selected
//     * @return OptionListComponent
//     */
//    template<typename T>
//    std::shared_ptr<OptionListComponent<T>> AddMultiList(const String& text, int id, IOptionListMultiComponent<T>* interface)
//    {
//      return AddMultiList(text, id, interface, String::Empty);
//    }

  protected:
    //! List's Color index: Selectable text
    static constexpr int sSelectableColor = 0;
    //! List's Color index: Unselectable text
    static constexpr int sUnselectableColor = 2;
    //! List's Color index: Background
    static constexpr int sBackgroundColor = 1;
    //! List's Color index: Header Background
    static constexpr int sHeaderBackgroundColor = 3;

    //! Menu theme
    const MenuThemeData& mTheme;

    /*!
     * @brief Proces input
     * @param event Inpuit event
     * @return True if the input has been processed
     */
    bool ProcessInput(const InputCompactEvent& event) override;

    /*!
     * @brief Called once per frame. Override to implement your own drawings.
     * Call your base::Update() to ensure animation and children are updated properly
     * @param deltaTime Elapsed time from the previous frame, in millisecond
     */
    void Update(int deltaTime) override
    {
      Gui::Update(deltaTime);
      if (!mMenuInitialized)
      {
        BuildMenu();
        SetMenuSize();
        mMenuInitialized = true;
      }
    }

  private:
    /// Menu cache
    MenuThemeDataCache& mCache;
    //! Background
    NinePatchComponent mBackground;
    //! Main component grid
    ComponentGrid mGrid;

    //! Item list
    Array<ItemBase*> mItems;
    //! Menu list
    std::shared_ptr<TextListComponent<ItemBase*>> mList;
    //! Title font
    std::shared_ptr<Font> mTitleFont;
    //! Title
    String mTitle;
    //! Footer
    String mFooter;
    //! Margin from border to text
    int mTextMargin;
    //! Margin from border to icon
    int mIconMargin;
    //! Menu initialized flag
    bool mMenuInitialized;

    /*!
     * @brief Build list using menu items
     */
    void BuildMenu();

    //! Return title height
    [[nodiscard]] float TitleHeight() const;

    //! Return footer height
    [[nodiscard]] float FooterHeight() const;

    //! Return Menu Width
    [[nodiscard]] float MenuWidth() const;

    //! Return Menu Height
    [[nodiscard]] float MenuHeight() const;

    //! Set menu size & position
    void SetMenuSize();

    //! Resize inner components when this component is resized
    void onSizeChanged() override;

    // Change the alpha component of the given color to 255% of its original value
    Colors::ColorRGBA Alpha25Percent(Colors::ColorRGBA color) { return (color & 0xFFFFFF00) | ((color & 0xFF) >> 2); }

    ////! Check th eitem at the given index and check if it's displayable or not
    bool IsDisplayable(int index) const;

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
    void OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size, ItemBase* const& data, unsigned int& color) override;

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @return left offset
     */
    float OverlayGetLeftOffset(ItemBase* const& data) override;

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    float OverlayGetRightOffset(ItemBase* const& data) override
    {
      return mList->getSelected()->OverlayRightOffset(*this, *data) + mTextMargin;
    }

    /*
     * IHeaderChanged
     */

    void HeaderFoldStateChanged() override
    {
      int index = mList->getCursorIndex();
      BuildMenu();
      mList->setCursorIndex(index);
    }
};
