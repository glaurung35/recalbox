//
// Created by bkg2k on 31/10/2020.
//

#include <MainRunner.h>
#include "GuiMenuThemeOptions.h"
#include "guis/MenuMessages.h"
#include "guis/GuiMsgBox.h"
#include "EmulationStation.h"

GuiMenuThemeOptions::GuiMenuThemeOptions(WindowManager& window, const IGlobalVariableResolver& resolver)
  : GuiMenuBase(window, _("THEME"), nullptr)
  , mResolver(resolver)
  , mTimer(0)
{
  // theme set
  mTheme = AddList<ThemeSpec>(_("THEME SET"), (int)Components::Theme, this, GetThemeEntries(), _(MENUMESSAGE_UI_THEME_HELP_MSG));

  // carousel transition option
  AddSwitch(_("CAROUSEL ANIMATION"), RecalboxConf::Instance().GetThemeCarousel(), (int)Components::Carousel, this, _(MENUMESSAGE_UI_CAROUSEL_HELP_MSG));

  // transition style
  AddList<String>(_("TRANSITION STYLE"), (int)Components::Transition, this, GetTransitionEntries(), _(MENUMESSAGE_UI_TRANSITION_HELP_MSG));

  // Region
  AddList<String>(_("REGION"), (int)Components::Region, this, GetRegionEntries(), _(MENUMESSAGE_UI_REGION_HELP_MSG));
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

std::vector<GuiMenuBase::ListEntry<ThemeSpec>> GuiMenuThemeOptions::GetThemeEntries()
{
  // Get theme list
  ThemeManager::ThemeList themelist = ThemeManager::AvailableThemes();
  mOriginalTheme = RecalboxConf::Instance().GetThemeFolder();
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = ThemeManager::sDefaultThemeFolder;
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = themelist.begin()->first;

  String currentVersionString = PROGRAM_VERSION_STRING;
  int cut = (int)currentVersionString.find_first_not_of("0123456789.");
  if (cut >= 0) currentVersionString.Delete(cut, INT32_MAX);

  // Sort names
  ThemeSpecList sortedNames;
  for (const auto& kv : themelist) sortedNames.push_back({ kv.first, kv.second });
  std::sort(sortedNames.begin(), sortedNames.end(), [](const ThemeSpec& a, const ThemeSpec& b) { return a.Name.ToLowerCase() < b.Name.ToLowerCase(); });

  std::vector<ListEntry<ThemeSpec>> list;
  for (const ThemeSpec& theme : sortedNames)
  {
    bool compatible = false;
    String displayableName = CheckCompatibility(theme.FolderPath, compatible, false);

    // (v) - compatibility match else /!\ sign - compatibility mismatch
    displayableName.Insert(0, compatible ? "\uF1C0 " : "\uF1CA ");

    list.push_back({ displayableName, theme, theme.Name == mOriginalTheme });
  }

  return list;
}

void GuiMenuThemeOptions::OptionListComponentChanged(int id, int index, const ThemeSpec& value, bool quickChange)
{
  (void)index;
  if ((Components)id == Components::Theme)
  {
    RecalboxConf::Instance().SetThemeFolder(value.Name).Save();
    if (quickChange)
    {
      mTimer = sApplyChangeTimer;
      mLastThemePath = value.FolderPath;
    }
    else
    {
      mTimer = 0;
      DoSwitchTheme(value.FolderPath);
    }
  }
}

void GuiMenuThemeOptions::OptionListComponentChanged(int id, int index, const String& value, bool quickChange)
{
  (void)index;
  (void)quickChange;
  if ((Components)id == Components::Transition) RecalboxConf::Instance().SetThemeTransition(value).Save();
  else if ((Components)id == Components::Region)
    RecalboxConf::Instance().SetThemeRegion(value).Save();
}

void GuiMenuThemeOptions::DoSwitchTheme(const Path& themePath)
{
  bool compatible = false;
  CheckCompatibility(Path(themePath), compatible, true);
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
      DoSwitchTheme(mLastThemePath);
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

String GuiMenuThemeOptions::CheckCompatibility(const Path& themePath, [[out]] bool& compatible, bool switchTheme)
{
  // Current state
  bool tate = mResolver.IsTate();
  ThemeData::Compatibility currentCompatibility = (mResolver.HasJamma() ? ThemeData::Compatibility::Jamma : ThemeData::Compatibility::None) |
                                                  (mResolver.HasCrt() ? ThemeData::Compatibility::Crt : ThemeData::Compatibility::None) |
                                                  (mResolver.HasHDMI() ? ThemeData::Compatibility::Hdmi : ThemeData::Compatibility::None);
  ThemeData::Resolutions currentResolution = mResolver.IsQVGA() ? ThemeData::Resolutions::QVGA :
                                             mResolver.IsVGA() ? ThemeData::Resolutions::VGA :
                                             mResolver.IsHD() ? ThemeData::Resolutions::HD :
                                             ThemeData::Resolutions::FHD;

  // Fetched data
  int recalboxVersion = 0; // Unknown version
  int themeVersion = 0; // Unknown version
  ThemeData::Compatibility compatibility = ThemeData::Compatibility::Hdmi;
  ThemeData::Resolutions resolutions = ThemeData::Resolutions::HD | ThemeData::Resolutions::FHD;
  String displayableName;

  if (ThemeData::FetchCompatibility(themePath / ThemeManager::sRootThemeFile, compatibility, resolutions, displayableName, themeVersion, recalboxVersion))
  {
    compatible = ((currentCompatibility & compatibility) != 0) &&
                 ((currentResolution & resolutions) != 0) &&
                 (!tate || hasFlag(compatibility, ThemeData::Compatibility::Tate));
    if (switchTheme)
    {
      if (!compatible)
      {
        String modeIssue = (currentCompatibility & compatibility) == 0 ?
          String("- ").Append(_("You display {0} is not in the list of this theme's supported displays:"))
          .Replace("{0}",GetDisplayList(currentCompatibility))
          .Append(' ').Append(GetDisplayList(compatibility)).Append(String::CRLF) : String::Empty;
        String resolutionIssue = (currentResolution & resolutions) == 0 ?
          String("- ").Append(_("You current resolution {0} is not in the list of this theme's supported resolutions:"))
          .Replace("{0}",GetResolutionList(currentResolution))
          .Append(' ').Append(GetResolutionList(resolutions)).Append(String::CRLF) : String::Empty;
        String tateIssue = tate && !hasFlag(compatibility, ThemeData::Compatibility::Tate) ?
          String("- ").Append(_("You're in TATE mode and this theme does not seem to support TATE.")).Append(String::CRLF) : String::Empty;
        String message = _("This theme may have one or more compatibility issues with your current display:\n").Append(modeIssue).Append(resolutionIssue).Append(tateIssue).Append(String::CRLF).Append("Are you sure to activate this theme?");
        mWindow.pushGui(new GuiMsgBox(mWindow, message, _("YES"), [this]{ ThemeManager::Instance().DoThemeChange(&mWindow); mTimer = 0; }, _("NO"), {}));
      }
      else
      {
        ThemeManager::Instance().DoThemeChange(&mWindow);
        mTimer = 0;
      }
    }
    if (themeVersion != 0) displayableName.Append(" (").Append(themeVersion >> 8).Append('.').Append(themeVersion & 0xFF).TrimRight('0').Append(')');
    return displayableName;
  }

  compatible = false;
  return String::Empty;
}

String GuiMenuThemeOptions::GetDisplayList(ThemeData::Compatibility display)
{
  String result;
  if ((display & ThemeData::Compatibility::Hdmi ) != 0) result = "HDMI";
  if ((display & ThemeData::Compatibility::Crt  ) != 0) { if (!result.empty()) result.Append(','); result.Append("CRT"); };
  if ((display & ThemeData::Compatibility::Jamma) != 0) { if (!result.empty()) result.Append(','); result.Append("JAMMA"); };
  return result;
}

String GuiMenuThemeOptions::GetResolutionList(ThemeData::Resolutions resolutions)
{
  String result;
  if ((resolutions & ThemeData::Resolutions::QVGA) != 0) result = "QVGA (240p)";
  if ((resolutions & ThemeData::Resolutions::VGA ) != 0) { if (!result.empty()) result.Append(','); result.Append("VGA (480p)"); };
  if ((resolutions & ThemeData::Resolutions::HD  ) != 0) { if (!result.empty()) result.Append(','); result.Append("HD (720p)"); };
  if ((resolutions & ThemeData::Resolutions::FHD ) != 0) { if (!result.empty()) result.Append(','); result.Append("FHD (1080p)"); };
  return result;
}

