//
// Created by bkg2k on 31/01/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

class SystemManager;

class MenuResolutionByEmulator : public Menu
                                  , private ISingleSelectorChanged<String>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuResolutionByEmulator(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    //! System manager reference
    SystemManager& mSystemManager;

    static SelectorEntry<String>::List GetResolutionEntries();

    /*
     * ISingleSelectorChanged<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;
};



