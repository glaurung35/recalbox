//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuVirtualSystemPerGenre : public Menu
                                   , public ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuVirtualSystemPerGenre(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    //! System manager reference
    SystemManager& mSystemManager;

    //! List of activated genre virtual system
    HashMap<String, bool> mGenres;

    //! Switch component changed
    void MenuSwitchChanged(int id, bool& status) override;
};



