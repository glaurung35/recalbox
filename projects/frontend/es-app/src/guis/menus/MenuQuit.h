//
// Created by thierry.imbert on 12/02/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>

class MenuQuit : public Menu
                  , private ISubMenuSelected
{
  public:
    explicit MenuQuit(WindowManager& window);

    static void PushQuitGui(WindowManager& window);

  private:
    enum class Components
    {
      Shutdown,
      FastShutdown,
      Reboot,
    };

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};
