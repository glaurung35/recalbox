//
// Created by bkg2k on 05/05/23.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "systems/SystemDescriptor.h"

// Forward declarations
class SystemManager;

class MenuDownloadGamePacks: public Menu
                              , private IActionTriggered
{
  public:
    //! Constructor
    MenuDownloadGamePacks(WindowManager& window, SystemManager& systemManager);

  private:
    //! Descriptor local array
    std::vector<SystemDescriptor> mDescriptors;

    //! System manager reference
    SystemManager& mSystemManager;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) override;
};
