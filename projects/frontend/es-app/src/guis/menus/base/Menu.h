//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/Gui.h>
#include <components/ITextListComponentOverlay.h>
#include "components/TextListComponent.h"
#include "IEditableChanged.h"
#include "ISubMenuSelected.h"
#include "ISwitchChanged.h"
#include "ISliderChanged.h"
#include "ISelectorChanged.h"
#include "IRatingChanged.h"
#include "themes/MenuThemeDataCache.h"
#include "IHeaderChanged.h"
#include "IItemDataProvider.h"
#include "utils/locale/LocaleHelper.h"
#include "SelectorEntry.h"
#include "IActionTriggered.h"
#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/ItemSelector.h>
#include <help/Help.h>

class Menu : public Gui
           , public ITextListComponentOverlay<ItemBase*>
           , public IHeaderChanged
           , public IItemDataProvider
{
  public:
    /*!
     * @brief Constructor
     * @param window Window manager
     * @param title Title (mandatory)
     * @param footer Title (mandatory)
     */
    Menu(WindowManager& window, const String& title, const String& footer = String::Empty, bool animated = false);

    //! Destructor
    ~Menu();

    /*!
     * @brief Inherited classes must add items in here
     */
    virtual void BuildMenuItems() = 0;

  protected:
    //! Menu theme
    const MenuThemeData& mTheme;
    //! Configuration reference
    RecalboxConf& mConfiguration;

    /*
     * Identification
     */

    int Count() const { return mItems.Count(); }

    bool IsHeader(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsHeader()  : false; }
    bool IsSubMenu(int index) const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsSubMenu() : false; }
    bool IsText(int index)    const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsText()    : false; }
    bool IsSwitch(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsSwitch()  : false; }
    bool IsSlider(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsSlider()  : false; }
    bool IsEditor(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsEditor()  : false; }
    bool IsList(int index)    const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsList()    : false; }
    bool IsMulti(int index)   const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsMulti()   : false; }
    bool IsRating(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsRating()  : false; }
    bool IsAction(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsAction()  : false; }
    bool IsBar(int index)     const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->IsBar()     : false; }

    /*
     * Get typed children
     */

    ItemHeader* AsHeader(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsHeader()   : nullptr; }
    ItemSubMenu* AsSubMenu(int index)                        const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsSubMenu()  : nullptr; }
    ItemText* AsInfoText(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsText()     : nullptr; }
    ItemSwitch* AsSwitch(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsSwitch()   : nullptr; }
    ItemSlider* AsSlider(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsSlider()   : nullptr; }
    template<typename T> ItemSelector<T>* AsList(int index)  const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsList<T>()  : nullptr; }
    template<typename T> ItemSelector<T>* AsMulti(int index) const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsMulti<T>() : nullptr; }
    ItemEditable* AsEditor(int index)                        const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsEditor()   : nullptr; }
    ItemRating* AsRating(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsRating()   : nullptr; }
    ItemAction* AsAction(int index)                          const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsAction()   : nullptr; }
    ItemBar* AsBar(int index)                                const { return ((unsigned int)index < (unsigned int)mItems.Count()) ? mItems[index]->AsBar()      : nullptr; }

    /*
     * Work with items
     */

    void SetSelectedItem(int index)
    {
      if (mList->IsEmpty()) return;
      mList->setCursorIndex(index % mList->size());
    }

    void SetSelectedItem(ItemBase* item)
    {
      mList->setCursor(item);
    }

    /*
     * API for Overlay
     */

    //! Total item height
    float ItemHeight() const override { return mList->EntryHeight(); }

    //! Total item height
    float ItemWidth() const override { return mList->getWidth(); }

    //! Front Icon height
    float IconHeight() const override { return mList->EntryHeight() * 0.7f; }

    //! Font height
    float FontHeight() const override { return mList->FontHeight() * 0.8f; }

    //! Margin
    float Margin() const override { return mTextMargin; }

    //! Texture cache
    MenuThemeDataCache& Cache() const override { return mCache; };

    //! Get maximum menu area
    Rectangle GetMenuMaximumArea() const override;

    /*
     * Helpers
     */

    /*!
     * @brief Rebuild the menu, for exemple if some option have changed and the menu needs to show the changes
     */
    void RebuildMenu() { mMenuInitialized = false; }

    /*!
     * @brief Add simpple header
     * @param label Label
     * @return Item index
     */

    ItemHeader* AddHeader(const String& label);

    /*!
     * @brief Add Submenu
     * @param label Label
     * @param func Method to call back when selected
     * @param help Help string
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSubMenu* AddSubMenu(const String& label, int id, ISubMenuSelected* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add Submenu
     * @param label Label
     * @param func Method to call back when selected
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSubMenu* AddSubMenu(const String& label, int id, ISubMenuSelected* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddSubMenu(label, id, interface, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add Submenu with Icon
     * @param label Label
     * @param icon Icon image path
     * @param func Method to call back when selected
     * @param help Help string
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSubMenu* AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add Submenu with Icon
     * @param label Label
     * @param icon Icon image path
     * @param func Method to call back when selected
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSubMenu* AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddSubMenu(label, icon, id, interface, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a switch menu entry
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add a switch menu entry
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddSwitch(label, value, id, interface, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a switch menu entry
     * @param icon icon type
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add a switch menu entry
     * @param icon icon type
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddSwitch(icon, label, value, id, interface, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a switch menu entry
     * @param icon Direct icon path
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(const Path& icon, const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add a switch menu entry
     * @param icon Direct icon path
     * @param text Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSwitch* AddSwitch(const Path& icon, const String& label, bool value, int id, ISwitchChanged* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddSwitch(icon, label, value, id, interface, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a rating menu entry
     * @param label Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @param help Help text
     * @return rating component
     */
    ItemRating* AddRating(const String& label, float value, int id, IRatingChanged* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add a rating menu entry
     * @param labal Menu text
     * @param value Initial value
     * @param callback Callback when entry changes
     * @return rating component
     */
    ItemRating* AddRating(const String& label, float value, int id, IRatingChanged* interface)
    {
      return AddRating(label, value, id, interface, String::Empty);
    }

    /*!
     * @brief Add non editable text
     * @param text Caption text
     * @param value Text value
     * @param help Help text
     * @return Item index
     */
    ItemText* AddText(const String& text, const String& value, const String& help);

    /*!
     * @brief Add non editable text
     * @param text Caption text
     * @param value Text value
     * @return Item index
     */
    ItemText* AddText(const String& text, const String& value)
    {
      return AddText(text, value, String::Empty);
    }

    /*!
     * @brief Add non editable text
     * @param text Caption text
     * @param value Text value
     * @param color Text color
     * @param help Help text
     * @return Item index
     */
    ItemText* AddText(const String& text, const String& value, unsigned int color, const String& help);

    /*!
     * @brief Add non editable text
     * @param text Caption text
     * @param value Text value
     * @param color Text color
     * @return Item index
     */
    ItemText* AddText(const String& text, const String& value, unsigned int color)
    {
      return AddText(text, value, color, String::Empty);
    }

    /*!
     * @brief Add editable text
     * @param label Caption text
     * @param text Text
     * @param ratio Bar ratio
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return ItemBar
     */
    ItemBar* AddBar(const String& label, const String& text, float ratio, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add editable text
     * @param label Caption text
     * @param text Text
     * @param ratio Bar ratio
     * @return ItemBar
     */
    ItemBar* AddBar(const String& label, const String& text, float ratio)
    {
      return AddBar(label, text, ratio, String::Empty);
    }

    /*!
     * @brief Add editable text
     * @param edittitle Edit title in vitytual keyboard
     * @param label Caption text
     * @param value Initial value
     * @param interface Callback when entry changes
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return TextComponent
     */
    ItemEditable* AddEditable(const String& edittitle, const String& label, const String& value, int id, IEditableChanged* interface, const String& help, bool masked, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add editable text
     * @param edittitle Edit title in vitytual keyboard
     * @param label Caption text
     * @param value Initial value
     * @param interface Callback when entry changes
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return TextComponent
     */
    ItemEditable* AddEditable(const String& edittitle, const String& label, const String& value, int id, IEditableChanged* interface, bool masked, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddEditable(edittitle, label, value, id, interface, String::Empty, masked, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add editable text
     * @param label Caption text
     * @param value Initial value
     * @param interface Callback when entry changes
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return TextComponent
     */
    ItemEditable* AddEditable(const String& label, const String& value, int id, IEditableChanged* interface, const String& help, bool masked, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add editable text
     * @param label Caption text
     * @param value Initial value
     * @param interface Callback when entry changes
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return TextComponent
     */
    ItemEditable* AddEditable(const String& label, const String& value, int id, IEditableChanged* interface, bool masked, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddEditable(label, value, id, interface, String::Empty, masked, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a Slider menu entry
     * @param label Menu text
     * @param min Minimum value
     * @param max Maximum value
     * @param value Initial value
     * @param id Identifier
     * @param interface Callback interface
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSlider* AddSlider(const String& label, float min, float max, float inc, float value, float defaultvalue, const String& suffix, int id, ISliderChanged* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add a Slider menu entry
     * @param text Menu text
     * @param min Minimum value
     * @param max Maximum value
     * @param value Initial value
     * @param id Identifier
     * @param interface Callback interface
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    ItemSlider* AddSlider(const String& label, float min, float max, float inc, float value, float defaultvalue, const String& suffix, int id, ISliderChanged* interface, bool unselectable = false, const String& helpUnselectable = String::Empty) { return AddSlider(label, min, max, inc, value, defaultvalue, suffix, id, interface, String::Empty, unselectable, helpUnselectable); }

    /*!
     * @brief Add a choice list
     * @tparam T Select object type
     * @param label Menu text (and selection window title)
     * @param id Identifier
     * @param interface callback interface
     * @param entries Entries to add to the list
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    template<typename T> ItemSelector<T>* AddList(const String& label, int id, ISingleSelectorChanged<T>* interface, const SelectorEntry<T>::List& entries, const T& initialvalue, const T& defaultvalue, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      if (entries.empty()) return nullptr;
      ItemSelector<T>* item = new ItemSelector<T>(*this, mWindow, label, mTheme, interface, id, entries, initialvalue, defaultvalue, help, unselectable, helpUnselectable);
      mItems.Add(item);
      return item;
    }

    /*!
     * @brief Add a choice list
     * @tparam T Select object type
     * @param label Menu text (and selection window title)
     * @param callback Callback when a new value is selected
     * @param entries Entries to add to the list
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item index
     */
    template<typename T> ItemSelector<T>* AddList(const String& label, int id, ISingleSelectorChanged<T>* interface, const SelectorEntry<T>::List& entries, const T& initialvalue, const T& defaultvalue, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddList(label, id, interface, entries, initialvalue, defaultvalue, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add a multi-choice list
     * @tparam T Select object type
     * @param label item label (and selection window title)
     * @param id Identifier
     * @param interface callback interface
     * @param entries Entries to add to the list
     * @param help Help text
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return OptionListComponent
     */
    template<typename T> ItemSelector<T>* AddMultiList(const String& label, int id, IMultiSelectorChanged<T>* interface, const SelectorEntry<T>::List& entries, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      if (entries.empty()) return nullptr;
      ItemSelector<T>* item = new ItemSelector<T>(*this, mWindow, label, mTheme, interface, id, entries, help, unselectable, helpUnselectable);
      mItems.Add(item);
      return item;
    }

    /*!
     * @brief Add a multi-choice list
     * @tparam T Select object type
     * @param label item label (and selection window title)
     * @param callback Callback when a new value is selected
     * @param entries Entries to add to the list
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item
     */
    template<typename T> ItemSelector<T>* AddMultiList(const String& label, int id, IMultiSelectorChanged<T>* interface, const SelectorEntry<T>::List& entries, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddMultiList(label, id, interface, entries, String::Empty, unselectable, helpUnselectable);
    }

    /*!
     * @brief Add an action button item
     * @param label Item label
     * @param buttonWord Button text - single word (verb) are better
     * @param id Identifier
     * @param interface Callback interface
     * @param help Optional help string
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item
     */
    ItemAction* AddAction(const String& label, const String& buttonWord, int id, bool positive, IActionTriggered* interface, const String& help, bool unselectable = false, const String& helpUnselectable = String::Empty);

    /*!
     * @brief Add an action button item
     * @param label Item label
     * @param buttonWord Button text - single word (verb) are better
     * @param id Identifier
     * @param interface Callback interface
     * @param unselectable Grayed ?
     * @param helpUnselectable Help string when the item is unselectable
     * @return Item
     */
    ItemAction* AddAction(const String& label, const String& buttonWord, int id, bool positive, IActionTriggered* interface, bool unselectable = false, const String& helpUnselectable = String::Empty)
    {
      return AddAction(label, buttonWord, id, positive, interface, String::Empty, unselectable, helpUnselectable);
    }

    //! Notify the user Recalbox is about to reboot
    void RequestReboot();

    //! Request a relaunch
    void RequestRelaunch();

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
      mList->getSelected()->Update(deltaTime);
    }

  private:
    //! Menu cache
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
    //! Animated menu
    bool mAnimated;

    /*!
     * @brief Fill help list
     * @param help Help list to fill
     * @return True if the method filled the list, false otherwise
     */
    bool CollectHelpItems(Help& help) override
    {
      if (mList->size() == 0) return false;
      help.Clear();
      mList->getSelected()->CollectHelpItems(help);
      help.Set(HelpType::AllDirections, _("CHOOSE"))
          .Set(HelpType::A, _("BACK"))
          .Set(HelpType::Y, _("HELP"))
          .Set(HelpType::Start, _("CLOSE"));
      return true;
    }

    //! Check if a reboot is pending. If not, send an infinite popup
    static void RebootPending(WindowManager* window);

    //! Relaunch
    static void Relaunch();

    //! Reboot
    static void Reboot();

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
    void OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size, int labelWidth, ItemBase* const& data, unsigned int& color) override;

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @return left offset
     */
    float OverlayGetLeftOffset(ItemBase* const& data, int labelWidth) override;

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @return right offset
     */
    float OverlayGetRightOffset(ItemBase* const& data, int labelWidth) override
    {
      return data->OverlayRightOffset(labelWidth) + mTextMargin;
    }

    /*
     * IHeaderChanged
     */

    void HeaderFoldStateChanged() override
    {
      /*int index = mList->getCursorIndex();
      BuildMenuItems();
      mList->setCursorIndex(index);*/
    }
};
