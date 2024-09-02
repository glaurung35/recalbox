//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;
class SystemData;

class MenuSystemList : public Menu
                        , private ISubMenuSelected
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuSystemList(WindowManager& window, SystemManager& systemManager);

  private:
    //! System manager
    SystemManager& mSystemManager;

    // Submenu list
    HashMap<SystemData*, ItemSubMenu*> mMenus;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * Component Override
     */

    void onShow() override;
};



