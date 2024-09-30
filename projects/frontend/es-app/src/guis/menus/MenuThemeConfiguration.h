//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISelectorChanged.h>

class MenuThemeConfiguration : public Menu
                                , private ISingleSelectorChanged<String>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    MenuThemeConfiguration(WindowManager& window, const String& theme);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    //! Apply change timer when using quick change
    static constexpr int sApplyChangeTimer = 750;

    //! Components
    enum class Components
    {
      ColorSet,
      IconSet,
      MenuSet,
      SystemView,
      GamelistView,
      GameClipView,
    };

    //! Theme name
    String mThemeName;

    /*!
     * @brief Build an option menu
     * @param label Menu label
     * @param help Help msg
     * @param selected Currently selected item
     * @param items All items
     * @param id Menu id
     * @return Item
     */
    ItemSelector<String>* BuildSelector(const String& label, const String& help, const String& selected, const String::List& items, Components id);

    static bool TrySortNumerically(SelectorEntry<String>::List& list);

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;
};
