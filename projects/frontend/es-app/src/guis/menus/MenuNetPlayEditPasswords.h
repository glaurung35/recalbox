#pragma once

#include <netplay/DefaultPasswords.h>
#include "guis/menus/base/Menu.h"

class MenuNetPlayEditPasswords : public Menu
                              , IEditableChanged
                              , IActionTriggered
{
  public:
    explicit MenuNetPlayEditPasswords(WindowManager& window);

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
