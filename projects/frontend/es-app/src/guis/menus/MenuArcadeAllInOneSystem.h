#pragma once

#include <guis/menus/base/Menu.h>

class MenuArcadeAllInOneSystem : public Menu
                                  , private ISwitchChanged
{
  public:
    //! Constructor
    explicit MenuArcadeAllInOneSystem(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      ArcadeOnOff,
      IncludeNeogeo,
      HideOriginals,
    };

    //! System manager reference
    SystemManager& mSystemManager;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};
