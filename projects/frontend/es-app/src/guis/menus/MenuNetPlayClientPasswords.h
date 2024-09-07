#pragma once

#include "guis/Gui.h"
#include <guis/GuiNetPlay.h>
#include <guis/menus/base/Menu.h>

class MenuNetPlayClientPasswords : public Menu
                                , private ISingleSelectorChanged<int>
                                , private IActionTriggered
{
  public:
    explicit MenuNetPlayClientPasswords(WindowManager& window, LobbyGame& lobbygame);

  private:
    //! PAssword type
    enum class PasswordType
    {
      Player,
      Viewer,
    };

    //! Components
    enum class Components
    {
      JoinAs,
      Password,
      Start,
      Cancel,
      EditPasswords,
    };

    //! Lobby game
    LobbyGame mLobbyGame;

    //! Get password list
    SelectorEntry<int>::List GetPasswords();

    /*
     * ISingleSelectorChanged<int> implementation
     */

    void MenuSingleChanged(int id, int index, const int& value) final;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) final;
};
