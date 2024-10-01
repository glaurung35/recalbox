//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuKodiSettings.h"
//#include "ResolutionAdapter.h"
//#include <WindowManager.h>
//#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>

MenuKodiSettings::MenuKodiSettings(WindowManager& window)
  : Menu(window, _("KODI SETTINGS"))
{
}

void MenuKodiSettings::BuildMenuItems()
{
  // Enable Kodi
  AddSwitch(_("ENABLE KODI"), RecalboxConf::Instance().GetKodiEnabled(), (int)Components::Enabled, this, _(MENUMESSAGE_ADVANCED_KODI_ENABLE_HELP_MSG));

  // Select kodi video mode
  AddList<String>(_("KODI RESOLUTION"), (int)Components::KodiResolution, this, GetKodiResolutionsEntries(),
                  !RecalboxConf::Instance().IsDefinedKodiVideoMode() ? String::Empty : RecalboxConf::Instance().GetKodiVideoMode(), String::Empty,
                  _(MENUMESSAGE_ADVANCED_RESOLUTION_KODI_HELP_MSG));

  // Run Kodi at startup
  AddSwitch(_("KODI AT START"), RecalboxConf::Instance().GetKodiAtStartup(), (int)Components::RunAtStartup, this, _(MENUMESSAGE_ADVANCED_KODI_AT_START_HELP_MSG));

  // Run kodi using X
  AddSwitch(_("START KODI WITH X"), RecalboxConf::Instance().GetKodiXButton(), (int)Components::ButtonX, this, _(MENUMESSAGE_ADVANCED_KODI_X_HELP_MSG));
}

void MenuKodiSettings::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::Enabled: RecalboxConf::Instance().SetKodiEnabled(status).Save(); break;
    case Components::RunAtStartup: RecalboxConf::Instance().SetKodiAtStartup(status).Save(); break;
    case Components::ButtonX: RecalboxConf::Instance().SetKodiXButton(status).Save(); break;
    case Components::KodiResolution:
    default: break;
  }
}

SelectorEntry<String>::List MenuKodiSettings::GetKodiResolutionsEntries()
{
  SelectorEntry<String>::List result;

  result.push_back({ _("NATIVE"), "default" });
  result.push_back({ _("USE GLOBAL"), "" });
  for(const ResolutionAdapter::Resolution& resolution : mResolutionAdapter.Resolutions(false))
  {
    String reso = resolution.ToRawString();
    result.push_back({ resolution.ToString(), reso });
  }
  return result;
}

void MenuKodiSettings::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::KodiResolution:
    {
      if (value.empty()) RecalboxConf::Instance().DeleteKodiVideoMode().Save();
      else RecalboxConf::Instance().SetKodiVideoMode(value).Save();
      break;
    }
    case Components::Enabled:
    case Components::RunAtStartup:
    case Components::ButtonX:
    default: break;
  }
}
