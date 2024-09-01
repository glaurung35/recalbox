#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISelectorChanged.h>
#include "components/ISwitchComponent.h"
#include <systems/SystemData.h>


class MenuTate : public Menu
                  , private ISingleSelectorChanged<RotationType>
                  , private ISwitchChanged
{
  public:
    //! Constructor
    MenuTate(WindowManager& window, SystemManager& systemManager);

    //! Destructor
    ~MenuTate() override;

  private:
    enum class Components
    {
      TateEnabled,
      TateGamesRotation,
      TateCompleteSystemRotation,
      TateOnly,
    };

    SystemManager& mSystemManager;

    //! Original enabled value
    bool mOriginalTateEnabled;
    //! Original Games Rotation value
    RotationType mOriginalGamesRotation;
    //! Original Games Rotation value
    RotationType mOriginalSystemRotation;
    //! Original tate only valeu
    bool mOriginalTateOnly;


    //! Get rotations entries
    static SelectorEntry<RotationType>::List GetRotationEntries();

    /*
     * ISingleSelectorChanged<String> implementation
     */
    void MenuSingleChanged(int id, int index, const RotationType& value) override;

    /*
     * ISwitchChanged implementation
     */
    void MenuSwitchChanged(int id, bool& status) override;
};
