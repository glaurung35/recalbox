//
// Created by bkg2k on 31/10/2020.
//

#include <MainRunner.h>
#include "GuiMenuThemeOptions.h"
#include "guis/MenuMessages.h"
#include "guis/GuiMsgBox.h"

GuiMenuThemeOptions::GuiMenuThemeOptions(WindowManager& window)
  : GuiMenuBase(window, _("THEME"), nullptr)
  , mTimer(0)
{
  // carousel transition option
  mCarousel = AddSwitch(_("CAROUSEL ANIMATION"), RecalboxConf::Instance().GetThemeCarousel(), (int)Components::Carousel, this, _(MENUMESSAGE_UI_CAROUSEL_HELP_MSG));

  // transition style
  mTransition = AddList(_("TRANSITION STYLE"), (int)Components::Transition, this, GetTransitionEntries(), _(MENUMESSAGE_UI_TRANSITION_HELP_MSG));

  // theme set
  mTheme = AddList(_("THEME SET"), (int)Components::Theme, this, GetThemeEntries(), _(MENUMESSAGE_UI_THEME_HELP_MSG));
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

  mOriginalTransition = RecalboxConf::Instance().GetThemeTransition();
  list.push_back({ _("FADE"), "fade", mOriginalTransition == "fade" });
  list.push_back({ _("SLIDE"), "slide", mOriginalTransition == "slide" });
  list.push_back({ _("INSTANT"), "instant", mOriginalTransition == "instant" });

  return list;
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuThemeOptions::GetThemeEntries()
{
  // Get theme list
  ThemeManager::ThemeList themelist = ThemeManager::AvailableThemes();
  mOriginalTheme = RecalboxConf::Instance().GetThemeFolder();
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = "recalbox-next";
  if (!themelist.contains(mOriginalTheme)) mOriginalTheme = themelist.begin()->first;

  // Sort names
  String::List sortedNames;
  for (const auto& kv : themelist) sortedNames.push_back(kv.first);
  std::sort(sortedNames.begin(), sortedNames.end(), [](const String& a, const String& b) { return a.ToLowerCase() < b.ToLowerCase(); });

  std::vector<ListEntry<String>> list;
  for (const String& name : sortedNames)
    list.push_back({ name, name, name == mOriginalTheme });

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
      WindowManager* window = &mWindow;
      Gui* gui = new GuiMsgBox(mWindow, _("Are you sure the selected theme is compatible with CRT screens?"),
                               _("YES"), [value, window] { RecalboxConf::Instance().SetThemeFolder(value).Save(); ThemeManager::Instance().DoThemeChange(window); },
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
