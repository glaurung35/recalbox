//
// Created by bkg2k on 05/05/23.
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declarations
class SystemManager;

class MenuDownloadContents: public Menu
                          , private ISubMenuSelected
{
  public:
    //! Constructor
    MenuDownloadContents(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      Games,
      Themes,
      Overlay,
    };

    //! System manager reference
    SystemManager& mSystemManager;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};
