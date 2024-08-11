//
// Created by bkg2k on 11/08/24.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <systems/SystemManager.h>

class MenuSystemCategories : public Menu
                              , private ISwitchChanged
                              , private ISingleSelectorChanged<SystemDescriptor::SystemCategory>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    MenuSystemCategories(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      Enabled,
      Category,
      QuickChange,
    };

    //! System Manager
    SystemManager& mSystemManager;

    //! Get Sorting List
    SelectorEntry<SystemDescriptor::SystemCategory>::List GetCategoryEntries();

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) final;

    /*
     * ISingleSelectorChanged<SystemDescriptor::SystemCategory> implementation
     */

    void MenuSingleChanged(int id, int index, const SystemDescriptor::SystemCategory& value) final;
};

