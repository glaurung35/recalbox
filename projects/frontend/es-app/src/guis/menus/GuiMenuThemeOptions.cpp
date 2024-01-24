//
// Created by bkg2k on 31/10/2020.
//

#include <MainRunner.h>
#include "GuiMenuThemeOptions.h"
#include "guis/MenuMessages.h"
#include "guis/GuiMsgBox.h"
#include "EmulationStation.h"

GuiMenuThemeOptions::GuiMenuThemeOptions(WindowManager& window)
  : GuiMenuBase(window, _("THEME"), nullptr)
  , mTimer(0)
{
  // theme set
  mTheme = AddList(_("THEME SET"), (int)Components::Theme, this, GetThemeEntries(), _(MENUMESSAGE_UI_THEME_HELP_MSG));

  // carousel transition option
  AddSwitch(_("CAROUSEL ANIMATION"), RecalboxConf::Instance().GetThemeCarousel(), (int)Components::Carousel, this, _(MENUMESSAGE_UI_CAROUSEL_HELP_MSG));

  // transition style
  AddList(_("TRANSITION STYLE"), (int)Components::Transition, this, GetTransitionEntries(), _(MENUMESSAGE_UI_TRANSITION_HELP_MSG));

  // Region
  AddList(_("REGION"), (int)Components::Transition, this, GetRegionEntries(), _(MENUMESSAGE_UI_REGION_HELP_MSG));
}

GuiMenuThemeOptions::~GuiMenuThemeOptions()
{
  // If a timer is still active, apply theme right now !
  if (mTimer > 0)
    ThemeManager::Instance().DoThemeChange(&mWindow);
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuThemeOptions::GetTransitionEntries()
{
  std::vector<ListEntry<String>> list;

  String originalTransition = RecalboxConf::Instance().GetThemeTransition();
  list.push_back({ _("FADE"), "fade", originalTransition == "fade" });
  list.push_back({ _("SLIDE"), "slide", originalTransition == "slide" });
  list.push_back({ _("INSTANT"), "instant", originalTransition == "instant" });

  return list;
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuThemeOptions::GetRegionEntries()
{
  std::vector<ListEntry<String>> list;

  String region = RecalboxConf::Instance().GetThemeRegion();
  list.push_back({ _("Europe"), "eu", region == "eu" });
  list.push_back({ _("USA"), "us", region == "us" });
  list.push_back({ _("Japan"), "jp", region == "jp" });

  return list;
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuThemeOptions::GetThemeEntries()
{
  // Get theme list
  ThemeManager::ThemeList themelist = ThemeManager::AvailableThemes();
  mOriginalTheme = RecalboxConf::Instance().GetThemeFolder();
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = ThemeManager::sDefaultThemeFolder;
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = themelist.begin()->first;

  ThemeData::Compatibility currentMode = ThemeManager::Instance().GlobalResolver().HasJamma() ? ThemeData::Compatibility::Jamma :
                                         ThemeManager::Instance().GlobalResolver().HasCrt() ? ThemeData::Compatibility::Crt :
                                         ThemeData::Compatibility::Hdmi;

  String currentVersionString = PROGRAM_VERSION_STRING;
  int cut = (int)currentVersionString.find_first_not_of("0123456789.");
  if (cut >= 0) currentVersionString.Delete(cut, INT32_MAX);
  /*int currentVersion = (9 << 8) + 20; // Minimum version 9.2
  if (String major, minor; currentVersionString.Extract('.', major, minor, true))
  version = (major.AsInt() << 8) + minor.AsInt() * (minor.Count() < 2 ? 10 : 1);*/

  // Sort names
  String::List sortedNames;
  for (const auto& kv : themelist) sortedNames.push_back(kv.first);
  std::sort(sortedNames.begin(), sortedNames.end(), [](const String& a, const String& b) { return a.ToLowerCase() < b.ToLowerCase(); });

  std::vector<ListEntry<String>> list;
  for (const String& name : sortedNames)
  {
    String displayableName;

    int version = 0; // Unknown version
    ThemeData::Compatibility compatibility = ThemeData::Compatibility::Hdmi;
    ThemeData::Resolutions resolutions = ThemeData::Resolutions::HD | ThemeData::Resolutions::FHD;

    // Compatibility
    if (ThemeData::FetchCompatibility(themelist[name] / ThemeManager::sRootThemeFile, compatibility, resolutions, displayableName, version))
    {
      if ((compatibility & currentMode) != 0) displayableName.Insert(0, "\uF1C0 "); // (v) - compatibility match
      else displayableName.Insert(0, "\uF1CA "); // /!\ sign - compatibility mismatch
    }
    else continue; // Failed to inspect theme => no valid theme

    // Version
    if (version != 0) displayableName.Append(" (").Append(version >> 8).Append('.').Append(version & 0xFF).TrimRight('0').Append(')');
    //else displayableName.Append(" (").Append("\uF1C1").Append(')'); // (?) sign - no version info

    list.push_back({ displayableName, name, name == mOriginalTheme });
  }

  return list;
}

void GuiMenuThemeOptions::OptionListComponentChanged(int id, int index, const String& value, bool quickChange)
{
  (void)index;
  if ((Components)id == Components::Transition) RecalboxConf::Instance().SetThemeTransition(value).Save();
  else if ((Components)id == Components::Theme)
  {
    if (Board::Instance().CrtBoard().IsCrtAdapterAttached() && value != "recalbox-240p")
    {
      Gui* gui = new GuiMsgBox(mWindow, _("Are you sure the selected theme is compatible with CRT screens?"),
                               _("YES"), [this, value] { RecalboxConf::Instance().SetThemeFolder(value).Save(); ThemeManager::Instance().DoThemeChange(&mWindow); },
                               _("NO"), [this, index] { mTheme->setSelectedIndex(index); });
      mWindow.pushGui(gui);
    }
    else
    {
      RecalboxConf::Instance().SetThemeFolder(value).Save();
      if (quickChange) mTimer = sApplyChangeTimer;
      else
      {
        ThemeManager::Instance().DoThemeChange(&mWindow);
        mTimer = 0;
      }
    }
  }
  else if ((Components)id == Components::Region)
    RecalboxConf::Instance().SetThemeRegion(value).Save();
}

void GuiMenuThemeOptions::SwitchComponentChanged(int id, bool& status)
{
  if ((Components)id == Components::Carousel)
    RecalboxConf::Instance().SetThemeCarousel(status).Save();
}

void GuiMenuThemeOptions::Update(int elapsed)
{
  GuiMenuBase::Update(elapsed);

  if (mTimer > 0)
    if (mTimer -= elapsed; mTimer <= 0)
    {
      mTimer = 0;
      ThemeManager::Instance().DoThemeChange(&mWindow);
    }
}

bool GuiMenuThemeOptions::ProcessInput(const InputCompactEvent& event)
{
  GuiMenuBase::ProcessInput(event);

  // Reset chaneg timer (if any) when the user is still moving
  if (!event.Empty())
    if (mTimer > 0)
      mTimer = sApplyChangeTimer;
  return false;
}
