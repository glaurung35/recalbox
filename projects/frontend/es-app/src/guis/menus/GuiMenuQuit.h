//
// Created by thierry.imbert on 12/02/2020.
//
#pragma once

#include <guis/menus/GuiMenuBase.h>

class GuiMenuQuit : public GuiMenuBase
                  , private ISubMenuSelected
{
  public:
    explicit GuiMenuQuit(WindowManager& window);

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
