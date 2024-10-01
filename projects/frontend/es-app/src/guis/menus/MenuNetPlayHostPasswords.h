#pragma once

#include "guis/Gui.h"
#include <guis/menus/base/Menu.h>
#include <netplay/DefaultPasswords.h>
#include <games/FileData.h>

template<typename T>
class OptionListComponent;
class SwitchComponent;

class MenuNetPlayHostPasswords : public Menu
                              , private ISwitchChanged
                              , private ISingleSelectorChanged<int>
                              , private IActionTriggered
{
  public:
    //! Constructor
    MenuNetPlayHostPasswords(WindowManager& window, FileData& game);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    // Component indexes
    enum class Components
    {
      UseForPlayers,
      UseForViewers,
      LastForPlayers,
      LastForViewers,
      Start,
      Cancel,
      EditPasswords,
    };

    //! Target game
    FileData& mGame;

    SelectorEntry<int>::List GetPasswords();

    /*
     * ISwitchChanged impolementation
     */

    void MenuSwitchChanged(int id, bool& state) final;

    /*
     * ISingleSelectorChanged<int> implementation
     */

    void MenuSingleChanged(int id, int index, const int& value) final;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) final;
};
