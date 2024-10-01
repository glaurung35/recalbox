//
// Created by bkg2k on 15/08/24.
//

#include "MenuHideSystems.h"
#include <systems/SystemManager.h>

MenuHideSystems::MenuHideSystems(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("VIRTUAL SYSTEMS PER GENRE"))
  , mSystemManager(systemManager)
{
}

void MenuHideSystems::BuildMenuItems()
{
  // All games
  int i = 0;
  for(const SystemData* system : mSystemManager.AllSystems())
    if (!system->IsPorts() && !system->IsVirtual())
    {
      mGenres[i] = system;
      AddSwitch((_F(_("Hide {0}")) / system->FullName())(), RecalboxConf::Instance().GetSystemIgnore(*system), i++, this);
    }
}

void MenuHideSystems::MenuSwitchChanged(int id, bool& status)
{
  const SystemData* system = mGenres[id];
  RecalboxConf::Instance().SetSystemIgnore(*system, status).Save();
}
