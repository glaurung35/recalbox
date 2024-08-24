//
// Created by bkg2k on 31/01/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuResolutionByEmulator.h"
#include "guis/MenuMessages.h"
#include "ResolutionAdapter.h"
#include <systems/SystemManager.h>

MenuResolutionByEmulator::MenuResolutionByEmulator(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("SYSTEM RESOLUTIONS"))
  , mSystemManager(systemManager)
{
  // For each activated system
  const SystemManager::List& systems = systemManager.AllSystems();
  for(int i = 0; i < (int)systems.Count(); ++i)
    if (!systems[i]->IsVirtual())
      AddList<String>(systems[i]->FullName(), i, this, GetResolutionEntries(), RecalboxConf::Instance().GetSystemVideoMode(*systems[i]), String::Empty, _(MENUMESSAGE_ADVANCED_RESOLUTION_SYSTEM_HELP_MSG));
}

SelectorEntry<String>::List MenuResolutionByEmulator::GetResolutionEntries()
{
  SelectorEntry<String>::List result;
  result.push_back({ _("USE GLOBAL"), "" });
  result.push_back({ _("NATIVE"), "default" });
  ResolutionAdapter resolutionAdapter;
  for(const ResolutionAdapter::Resolution& resolution : resolutionAdapter.Resolutions(false))
  {
    String reso = resolution.ToRawString();
    result.push_back({ resolution.ToString(), reso });
  }
  return result;
}

void MenuResolutionByEmulator::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  const SystemManager::List& systems = mSystemManager.AllSystems();
  if (value.empty()) RecalboxConf::Instance().DeleteSystemVideoMode(*systems[id]).Save();
  else RecalboxConf::Instance().SetSystemVideoMode(*systems[id], value).Save();
}
