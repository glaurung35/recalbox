//
// Created by bkg2k on 05/05/23.
//
#pragma once

#include <guis/menus/GuiMenuBase.h>
#include <components/SwitchComponent.h>

// Forward declarations
class SystemManager;

class GuiMenuDownloadGamePacks: public GuiMenuBase
                              , private IGuiMenuBase
{
  public:
    //! Constructor
    GuiMenuDownloadGamePacks(WindowManager& window, SystemManager& systemManager);

  private:
    SystemManager& mSystemManager;

    /*
     * IGuiMenuBase implementation
     */

    void SubMenuSelected(int id) override;
};
