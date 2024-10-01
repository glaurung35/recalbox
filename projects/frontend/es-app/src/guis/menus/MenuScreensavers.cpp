//
// Created by bkg2k on 31/10/2020.
//

#include <systems/SystemManager.h>
#include "MenuScreensavers.h"
#include "guis/MenuMessages.h"

MenuScreensavers::MenuScreensavers(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("SCREENSAVER"))
  , mSystemManager(systemManager)
{
}

void MenuScreensavers::BuildMenuItems()
{
  // screensaver time
  AddSlider(_("SCREENSAVER AFTER"), 0.f, 30.f, 1.f, 5.f,  (float)RecalboxConf::Instance().GetScreenSaverTime(), "m", (int)Components::Time, this, _(MENUMESSAGE_UI_SCREENSAVER_AFTER_HELP_MSG));

  // screensaver behavior
  AddList<RecalboxConf::Screensaver>(_("SCREENSAVER BEHAVIOR"), (int)Components::Type, this, GetTypeEntries(), RecalboxConf::Instance().GetScreenSaverType(), RecalboxConf::Screensaver::Dim, _(MENUMESSAGE_UI_SCREENSAVER_BEHAVIOR_HELP_MSG));

  // add systems
  AddMultiList(_("SYSTEMS TO SHOW IN DEMO"), (int)Components::SystemList, this, GetSystemEntries(), _(MENUMESSAGE_UI_SCREENSAVER_SYSTEMSDEMO_HELP_MSG));
}

SelectorEntry<RecalboxConf::Screensaver>::List MenuScreensavers::GetTypeEntries()
{
  SelectorEntry<RecalboxConf::Screensaver>::List list;

  RecalboxConf::Screensaver type = RecalboxConf::Instance().GetScreenSaverType();
  if (Board::Instance().HasSuspendResume())
    list.push_back({ _("suspend"), RecalboxConf::Screensaver::Suspend, type == RecalboxConf::Screensaver::Suspend });
  list.push_back({ _("dim"), RecalboxConf::Screensaver::Dim, type == RecalboxConf::Screensaver::Dim });
  list.push_back({ _("black"), RecalboxConf::Screensaver::Black, type == RecalboxConf::Screensaver::Black });
  list.push_back({ _("demo"), RecalboxConf::Screensaver::Demo, type == RecalboxConf::Screensaver::Demo });
  list.push_back({ _("gameclip"), RecalboxConf::Screensaver::Gameclip, type == RecalboxConf::Screensaver::Gameclip });

  return list;
}

SelectorEntry<String>::List MenuScreensavers::GetSystemEntries()
{
  SelectorEntry<String>::List list;

  for (const SystemData* system : mSystemManager.AllSystems())
    if (system->HasGame() && !system->Descriptor().IsPort())
      list.push_back({ system->FullName(), system->Name(), RecalboxConf::Instance().IsInScreenSaverSystemList(system->Name()) });

  return list;
}

void MenuScreensavers::MenuSliderMoved(int id, float value)
{
  if ((Components)id == Components::Time)
    RecalboxConf::Instance().SetScreenSaverTime((int)value).Save();
}

void MenuScreensavers::MenuSingleChanged(int id, int index, const RecalboxConf::Screensaver& value)
{
  (void)index;
  if ((Components)id == Components::Type) RecalboxConf::Instance().SetScreenSaverType(value).Save();
}

void MenuScreensavers::MenuMultiChanged(int id, int index, const String::List& value)
{
  (void)index;
  if ((Components)id == Components::SystemList)
  {
    RecalboxConf::Instance().SetScreenSaverSystemList(value).Save();
  }
}
