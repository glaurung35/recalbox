//
// Created by gugue_u on 29/03/2022.
//
//

#include "MenuGameFilters.h"
#include "views/ViewController.h"
#include "guis/MenuMessages.h"

MenuGameFilters::MenuGameFilters(WindowManager& window, SystemManager& systemManager)
  :	Menu(window, _("GAME FILTERS"))
  , mSystemManager(systemManager)
{
}

void MenuGameFilters::BuildMenuItems()
{
  AddSwitch(_("SHOW ONLY LATEST VERSION") + " (BETA)", RecalboxConf::Instance().GetShowOnlyLatestVersion(), (int)Components::ShowOnlyLatestVersion, this, _(MENUMESSAGE_UI_ONLY_LAST_VERSION_MSG));

  AddSwitch(_("SHOW ONLY FAVORITES"), RecalboxConf::Instance().GetFavoritesOnly(), (int)Components::FavoritesOnly, this, _(MENUMESSAGE_UI_FAVORITES_ONLY_MSG));

  AddSwitch(_("SHOW FAVORITES FIRST"), RecalboxConf::Instance().GetFavoritesFirst(), (int)Components::FavoritesFirst, this, _(MENUMESSAGE_UI_FAVORITES_FIRST_MSG));

  AddSwitch(_("SHOW HIDDEN GAMES"), RecalboxConf::Instance().GetShowHidden(), (int)Components::ShowHidden, this, _(MENUMESSAGE_UI_SHOW_HIDDEN_MSG));

  AddSwitch(_("HIDE MAHJONG AND CASINO GAMES"), RecalboxConf::Instance().GetHideBoardGames(), (int)Components::BoardGames, this, _(MENUMESSAGE_UI_HIDE_BOARD_GAMES_MSG));

  AddSwitch(_("HIDE ADULT GAMES"), RecalboxConf::Instance().GetFilterAdultGames(), (int)Components::Adult, this, _(MENUMESSAGE_UI_HIDE_ADULT_MSG));

  AddSwitch(_("HIDE PREINSTALLED GAMES"), RecalboxConf::Instance().GetGlobalHidePreinstalled(), (int)Components::HidePreinstalled, this, _(MENUMESSAGE_UI_HIDE_PREINSTALLED_MSG));

  AddSwitch(_("SHOW ONLY 3+ PLAYERS GAMES"), RecalboxConf::Instance().GetShowOnly3PlusPlayers(), (int)Components::Players3AndPlus, this, _(MENUMESSAGE_UI_SHOW_ONLY_3_PLUS_PLAYERS_GAMES));

  AddSwitch(_("SHOW ONLY YOKO (HORIZONTAL) GAMES"), RecalboxConf::Instance().GetShowOnlyYokoGames(), (int)Components::ShowOnlyYoko, this, _(MENUMESSAGE_UI_SHOW_ONLY_YOKO_GAMES));

  AddSwitch(_("SHOW ONLY TATE (VERTICAL) GAMES"), RecalboxConf::Instance().GetShowOnlyTateGames(), (int)Components::ShowOnlyTate, this, _(MENUMESSAGE_UI_SHOW_ONLY_TATE_GAMES));

  AddSwitch(_("HIDE NO GAMES"), RecalboxConf::Instance().GetHideNoGames(), (int)Components::NoGames, this, _(MENUMESSAGE_UI_HIDE_NO_GAMES_MSG));
}

void MenuGameFilters::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::ShowOnlyLatestVersion:
    {
      RecalboxConf::Instance().SetShowOnlyLatestVersion(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetShowOnlyLatestVersion(!status);
        status = !status;
      }
      break;
    }
    case Components::FavoritesOnly:
    {
      RecalboxConf::Instance().SetFavoritesOnly(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no favorite games in any system!"));
        RecalboxConf::Instance().SetFavoritesOnly(!status);
        status = !status;
      }
      break;
    }
    case Components::FavoritesFirst:
    {
      RecalboxConf::Instance().SetFavoritesFirst(status).Save();
      break;
    }
    case Components::ShowHidden:
    {
      RecalboxConf::Instance().SetShowHidden(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetShowHidden(!status);
        status = !status;
      }
      break;
    }
    case Components::Adult:
    {
      RecalboxConf::Instance().SetFilterAdultGames(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetFilterAdultGames(!status);
        status = !status;
      }
      break;
    }
    case Components::BoardGames:
    {
      RecalboxConf::Instance().SetHideBoardGames(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetHideBoardGames(!status);
        status = !status;
      }
      break;
    }
    case Components::HidePreinstalled:
    {
      RecalboxConf::Instance().SetGlobalHidePreinstalled(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetGlobalHidePreinstalled(!status);
        status = !status;
      }
      break;
    }
    case Components::NoGames:
    {
      RecalboxConf::Instance().SetHideNoGames(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetHideNoGames(!status);
        status = !status;
      }
      break;
    }
    case Components::Players3AndPlus:
    {
      RecalboxConf::Instance().SetShowOnly3PlusPlayers(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetShowOnly3PlusPlayers(!status);
        status = !status;
      }
      break;
    }
    case Components::ShowOnlyYoko:
    {
      RecalboxConf::Instance().SetShowOnlyYokoGames(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetShowOnlyYokoGames(!status);
        status = !status;
      }
      break;
    }
    case Components::ShowOnlyTate:
    {
      RecalboxConf::Instance().SetShowOnlyTateGames(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetShowOnlyTateGames(!status);
        status = !status;
      }
      break;
    }
    default: break;
  }
}
