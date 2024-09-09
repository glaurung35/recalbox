#pragma once

#include <netplay/DefaultPasswords.h>
#include "guis/menus/base/Menu.h"

class MenuNetPlayEditPasswords : public Menu
                              , IEditableChanged
                              , IActionTriggered
{
  public:
    explicit MenuNetPlayEditPasswords(WindowManager& window);

    //! Build menu items
    void BuildMenuItems() final;

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
