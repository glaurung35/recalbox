//
// Created by gugue_u on 29/03/2022.
//
//

#include "guis/menus/base/Menu.h"

// Forward declaration
class SystemManager;

class MenuGameFilters : public Menu
                         , private ISwitchChanged
{
  public:
    /*!
     * @brief Constructor
     */
    MenuGameFilters(WindowManager&window, SystemManager& systemManager);

  private:
    enum class Components
    {
      Adult,
      FavoritesOnly,
      FavoritesFirst,
      ShowHidden,
      HidePreinstalled,
      NoGames,
      ShowOnlyLatestVersion,
      BoardGames,
      Players3AndPlus,
      ShowOnlyYoko,
      ShowOnlyTate
    };

    //! System manager reference
    SystemManager& mSystemManager;

    /*
     * ISwitchComponent implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};