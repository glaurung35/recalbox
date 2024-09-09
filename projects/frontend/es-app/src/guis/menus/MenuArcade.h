
//
// Created by bkg2k on 15/06/23.
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;
class IArcadeGamelistInterface;

class MenuArcade : public Menu
                    , private ISubMenuSelected
                    , private IMultiSelectorChanged<String>
                    , private ISwitchChanged
{
  public:
    /*!
     * @brief Constructor
     */
    MenuArcade(WindowManager& window, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      EnhancedView,
      FoldClones,
      HideBios,
      HideNonWorking,
      UseDatabasesNames,
      ManufacturersVirtual,
      ManufacturersFilter,
      GlobalArcadeSystem,
    };

    //! System manager reference
    SystemManager& mSystemManager;

    // IArcadeGamelistInterface for gamelist options
    IArcadeGamelistInterface* mArcade;

    // Manufacturer virtual system cached initial list
    String::List mManufacturersIdentifiers;

    //! Get virtual manufacturer/system entries
    SelectorEntry<String>::List GetManufacturersVirtualEntries();

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * IMultiSelectorChanged<String> implementation
     */

    void MenuMultiChanged(int id, int index, const String::List& value) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};
