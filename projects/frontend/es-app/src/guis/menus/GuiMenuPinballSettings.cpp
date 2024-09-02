//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "GuiMenuPinballSettings.h"
#include <guis/MenuMessages.h>

GuiMenuPinballSettings::GuiMenuPinballSettings(WindowManager& window)
  : Menu(window, _("PINBALL SETTINGS"))
{
  // Enable Pinball Cabinet
  AddSwitch(_("ENABLE CABINET MODE"), RecalboxConf::Instance().GetPinballCabinet(), (int)Components::Cabinet, this, _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Enable Pinball Trail
  AddSwitch(_("ENABLE BALL TRAIL"), RecalboxConf::Instance().GetPinballTrail(), (int)Components::Trail, this, _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Select Backglass Screen
  AddList<String>(_("BACKGLASS SCREEN"), (int)Components::BackglassScreen, this, GetPinballBackglassScreenEntries(), RecalboxConf::Instance().GetPinballBackglassScreen(), "1", _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Select Backglass resolution
  AddList<String>(_("BACKGLASS RESOLUTION"), (int)Components::BackglassResolution, this, GetPinballBackglassResolutionEntries(), RecalboxConf::Instance().GetPinballBackglassResolution(), "1920x1080", _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));

  // Select Profile
  AddList<String>(_("PROFILE"), (int)Components::Profile, this, GetPinballProfileEntries(), RecalboxConf::Instance().GetPinballProfile(), "1", _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
}

void GuiMenuPinballSettings::MenuSwitchChanged(int id, bool& status)
{	
  switch((Components)id)
  {
    case Components::Cabinet: RecalboxConf::Instance().SetPinballCabinet(status).Save(); break;
    case Components::Trail: RecalboxConf::Instance().SetPinballTrail(status).Save(); break;
    case Components::BackglassScreen:
    case Components::BackglassResolution:
    case Components::Profile:
    default: break;
  }
}

SelectorEntry<String>::List GuiMenuPinballSettings::GetPinballBackglassResolutionEntries()
{
  return SelectorEntry<String>::List
  {
    {_("1920x1080"), "1920x1080" },
    {_("1280x1024"), "1280x1024" },
  };
}

SelectorEntry<String>::List GuiMenuPinballSettings::GetPinballBackglassScreenEntries()
{
  return SelectorEntry<String>::List
  {
    { _("1"), "1"},
    { _("2"), "2"},
  };
}

SelectorEntry<String>::List GuiMenuPinballSettings::GetPinballProfileEntries()
{
  return SelectorEntry<String>::List
  {
    { _("light"), "1" },
    { _("normal"), "2" },
    { _("max"), "3" },
  };
}

void GuiMenuPinballSettings::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::BackglassResolution:
    {
      if (value.empty()) RecalboxConf::Instance().DeletePinballBackglassResolution().Save();
      else RecalboxConf::Instance().SetPinballBackglassResolution(value).Save();
      break;
    }
    case Components::BackglassScreen:
    {
      if (value.empty()) RecalboxConf::Instance().DeletePinballBackglassScreen().Save();
      else RecalboxConf::Instance().SetPinballBackglassScreen(value).Save();
      break;
    }
    case Components::Profile:
    {
      if (value.empty()) RecalboxConf::Instance().DeletePinballProfile().Save();
      else RecalboxConf::Instance().SetPinballProfile(value).Save();
      break;
    }
    case Components::Cabinet:
    case Components::Trail:
    default: break;
  }
}
