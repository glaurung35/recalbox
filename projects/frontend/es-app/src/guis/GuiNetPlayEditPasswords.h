#pragma once

#include <netplay/DefaultPasswords.h>
#include "guis/menus/base/Menu.h"

class GuiNetPlayEditPasswords : public Menu
                              , IEditableChanged
                              , IActionTriggered
{
  public:
    explicit GuiNetPlayEditPasswords(WindowManager& window);

  private:
    /*
     * IEditableChanged implementation
     */

    void MenuEditableChanged(int id, const String& newText) final;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) final { (void)id; Close(); }
};
