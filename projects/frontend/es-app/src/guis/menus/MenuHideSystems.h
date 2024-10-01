//
// Created by bkg2k on 15/08/24.
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuHideSystems : public Menu
                         , public ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuHideSystems(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    //! System manager reference
    SystemManager& mSystemManager;

    //! List of activated genre virtual system
    HashMap<int, const SystemData*> mGenres;

    /*
     * ISwitchChanged implementation
     */

    //! Switch component changed
    void MenuSwitchChanged(int id, bool& status) override;
};
