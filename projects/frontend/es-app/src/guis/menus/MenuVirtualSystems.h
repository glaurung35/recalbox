//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISwitchChanged.h>

// Forward declaration
class SystemManager;

class MenuVirtualSystems : public Menu
                            , private ISubMenuSelected
                            , private ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     * @param systemManager System manager reference
     */
    explicit MenuVirtualSystems(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      AllGames,
      Multiplayers,
      LastPlayed,
      VirtualPerGenre,
      LightGun,
      Ports,
    };

    //! System manager reference
    SystemManager& mSystemManager;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) final;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) final;
};



