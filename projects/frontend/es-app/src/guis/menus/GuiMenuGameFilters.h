//
// Created by gugue_u on 29/03/2022.
//
//

#include "guis/menus/GuiMenuBase.h"
#include "guis/GuiMetaDataEd.h"

class ISimpleGameListView;

class GuiMenuGameFilters : public GuiMenuBase
                         , private ISwitchComponent
{
  public:
    /*!
     * @brief Constructor
     */
    GuiMenuGameFilters(WindowManager&window, SystemManager& systemManager);

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

    std::shared_ptr<TextComponent> mGame;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool& status) override;
};