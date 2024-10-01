//
// Created by bkg2k on 19/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuSwitchKodiNetplay : public Menu
                            , private ISubMenuSelected
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuSwitchKodiNetplay(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      Kodi,
      Netplay,
    };

    //! System manager
    SystemManager& mSystemManager;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};



