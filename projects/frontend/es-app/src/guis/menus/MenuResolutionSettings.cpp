//
// Created by bkg2k on 31/01/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuResolutionSettings.h"
#include "MenuResolutionByEmulator.h"
#include "ResolutionAdapter.h"
#include <guis/MenuMessages.h>
#include <systems/SystemManager.h>

MenuResolutionSettings::MenuResolutionSettings(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("RESOLUTIONS"))
  , mSystemManager(systemManager)
{
}

void MenuResolutionSettings::BuildMenuItems()
{
  // Add global video mode
  AddList<String>(_("GLOBAL RESOLUTION"), (int)Components::GlobalResolution, this, GetGlobalResolutionEntries(), RecalboxConf::Instance().GetGlobalVideoMode(), "default", _(MENUMESSAGE_ADVANCED_RESOLUTION_GLOBAL_HELP_MSG));

  // Add frontend video mode
  AddList<String>(_("EMULATIONSTATION RESOLUTION"), (int)Components::FrontendResolution, this, GetFrontEndResolutionEntries(), RecalboxConf::Instance().GetEmulationstationVideoMode(), String::Empty, _(MENUMESSAGE_ADVANCED_RESOLUTION_FRONTEND_HELP_MSG));

  // Emulator resolutions
  AddSubMenu(_("EMULATORS SPECIFIC RESOLUTIONS"), (int)Components::Emulators, this, _(MENUMESSAGE_ADVANCED_RESOLUTION_EMULATORS_HELP_MSG));
}

SelectorEntry<String>::List MenuResolutionSettings::GetGlobalResolutionEntries()
{
  SelectorEntry<String>::List result;
  for(const ResolutionAdapter::Resolution& resolution : mResolutionAdapter.Resolutions(true))
  {
    String reso = resolution.ToRawString();
    result.push_back({ resolution.ToString(), reso });
  }
  return result;
}

SelectorEntry<String>::List MenuResolutionSettings::GetFrontEndResolutionEntries()
{
  SelectorEntry<String>::List result;
  result.push_back({ _("USE GLOBAL"), "" });
  result.push_back({ _("NATIVE"), "default" });
  for(const ResolutionAdapter::Resolution& resolution : mResolutionAdapter.Resolutions(true))
  {
    String reso = resolution.ToRawString();
    result.push_back({ resolution.ToString(), reso });
  }
  return result;
}

void MenuResolutionSettings::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::GlobalResolution:
    {
      RecalboxConf::Instance().SetGlobalVideoMode(value).Save();
      break;
    }
    case Components::FrontendResolution:
    {
      if (value.empty()) RecalboxConf::Instance().DeleteEmulationstationVideoMode().Save();
      else RecalboxConf::Instance().SetEmulationstationVideoMode(value).Save();
      break;
    }
    case Components::Emulators:
    default: break;
  }
}

void MenuResolutionSettings::SubMenuSelected(int id)
{
  if ((Components)id == Components::Emulators)
    mWindow.pushGui(new MenuResolutionByEmulator(mWindow, mSystemManager));
}
