//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "GuiMenuPinballSettings.h"
#include "ResolutionAdapter.h"
#include <components/SwitchComponent.h>
#include <WindowManager.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>

GuiMenuPinballSettings::GuiMenuPinballSettings(WindowManager& window)
  : GuiMenuBase(window, _("PINBALL SETTINGS"), nullptr)
{
  // Enable Pinball Cabinet
  mPinballCabinet = AddSwitch(_("ENABLE CABINET MODE"), RecalboxConf::Instance().GetPinballCabinet(), (int)Components::Cabinet, this, _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Enable Pinball Trail
  mPinballTrail = AddSwitch(_("ENABLE BALL TRAIL"), RecalboxConf::Instance().GetPinballTrail(), (int)Components::Trail, this, _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Select Backglass Screen
  AddList<String>(_("BACKGLASS SCREEN"), (int)Components::BackglassScreen, this, GetPinballBackglassScreenEntries(), _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
  
  // Select Backglass resolution
  AddList<String>(_("BACKGLASS RESOLUTION"), (int)Components::BackglassResolution, this, GetPinballBackglassResolutionEntries(), _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));

  // Select Profile
  AddList<String>(_("PROFILE"), (int)Components::Profile, this, GetPinballProfileEntries(), _(MENUMESSAGE_ADVANCED_PINBALL_CABINET_HELP_MSG));
}

void GuiMenuPinballSettings::SwitchComponentChanged(int id, bool& status)
{	
  switch((Components)id)
  {
    case Components::Cabinet: RecalboxConf::Instance().SetPinballCabinet(status).Save(); break;
    case Components::Trail: RecalboxConf::Instance().SetPinballTrail(status).Save(); break;
    case Components::BackglassScreen:
    case Components::BackglassResolution:
    default: break;
  }
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuPinballSettings::GetPinballBackglassResolutionEntries()
{
  std::vector<GuiMenuBase::ListEntry<String>> result;

  String currentOption = RecalboxConf::Instance().GetPinballBackglassResolution();
  result.push_back({ _("1920x1080"), "1920x1080", currentOption == "1920x1080" });
  result.push_back({ _("1280x1024"), "1280x1024", currentOption == "1280x1024" });

  return result;
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuPinballSettings::GetPinballBackglassScreenEntries()
{
  std::vector<GuiMenuBase::ListEntry<String>> result;

  String currentOption = RecalboxConf::Instance().GetPinballBackglassScreen();
  result.push_back({ _("1"), "1", currentOption == "1" });
  result.push_back({ _("2"), "2", currentOption == "2" });

  return result;
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuPinballSettings::GetPinballProfileEntries()
{
  std::vector<GuiMenuBase::ListEntry<String>> result;

  String currentOption = RecalboxConf::Instance().GetPinballProfile();
  result.push_back({ _("light"), "1", currentOption == "1" });
  result.push_back({ _("normal"), "2", currentOption == "2" });
  result.push_back({ _("max"), "3", currentOption == "3" });

  return result;
}

void GuiMenuPinballSettings::OptionListComponentChanged(int id, int index, const String& value, bool quickChange)
{
  (void)quickChange;
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
