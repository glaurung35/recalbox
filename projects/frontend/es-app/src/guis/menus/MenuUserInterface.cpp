//
// Created by bkg2k on 31/10/2020.
//

#include "MenuUserInterface.h"
#include "MenuThemeOptions.h"
#include "MenuPopupSettings.h"
#include "MenuScreensavers.h"
#include "MenuThemeConfiguration.h"
#include "MenuGameFilters.h"
#include "MenuHideSystems.h"
#include <guis/MenuMessages.h>
#include <guis/GuiMsgBox.h>
#include <MainRunner.h>
#include <guis/menus/base/ItemSlider.h>

MenuUserInterface::MenuUserInterface(WindowManager& window, SystemManager& systemManager, const IGlobalVariableResolver& resolver)
  : Menu(window, _("UI SETTINGS"))
  , mResolver(resolver)
  , mSystemManager(systemManager)
  , mBrightness(nullptr)
  , mOriginalSort(RecalboxConf::Instance().GetSystemSorting())
{
  // Screensavers
  AddSubMenu(_("SCREENSAVER"), (int)Components::ScreenSaver, this, _(MENUMESSAGE_UI_SCREENSAVER_HELP_MSG));

  // Brightness
  if (Board::Instance().HasBrightnessSupport())
      mBrightness = AddSlider(_("BRIGHTNESS"), 0.f, 8.f, 1.f, 6.f, (float)RecalboxConf::Instance().GetBrightness(), "", (int)Components::Brightness, this);

  // Theme
  AddSubMenu(_("THEME"), (int)Components::Theme, this, _(MENUMESSAGE_UI_THEME_HELP_MSG));

  // Theme Options
  AddSubMenu(_("THEME CONFIGURATION"), (int)Components::ThemeConfig, this, _(MENUMESSAGE_UI_THEME_CONFIGURATION_MSG));

  // System categories
  AddSubMenu(_("SYSTEM CATEGORIES"), (int)Components::SystemCategories, _(MENUMESSAGE_UI_CATEGORIES_FILTERS_MSG));

  // System sort
  AddList<SystemSorting>(_("SYSTEM SORTING"), (int)Components::SystemSort, this, GetSortingEntries(), RecalboxConf::Instance().GetSystemSorting(), SystemSorting::Default, _(MENUMESSAGE_ADVANCED_SORTINGOPTION_HELP_MSG));

  // quick system select (left/right in game list view)
  AddSwitch(_("QUICK SYSTEM SELECT"), RecalboxConf::Instance().GetQuickSystemSelect(), (int)Components::QuickSelect, this, _(MENUMESSAGE_UI_QUICK_HELP_MSG));

  // show help
  AddSwitch(_("ON-SCREEN HELP"), RecalboxConf::Instance().GetShowHelp(), (int)Components::Help, this, _(MENUMESSAGE_UI_ONSCREENHELP_HELP_MSG));

  // Swap validate and cancel buttons
  AddSwitch(_("SWAP VALIDATE/CANCEL BUTTONS"), RecalboxConf::Instance().GetSwapValidateAndCancel(), (int)Components::SwapValidateAndCancel, this, _(MENUMESSAGE_UI_SWAP_VALIDATE_CANCEL_BUTTONS_HELP_MSG));

  // display clock
  AddSwitch(_("CLOCK IN MENU"), RecalboxConf::Instance().GetClock(), (int)Components::Clock, this, _(MENUMESSAGE_UI_CLOCK_HELP_MSG));

  // Popup settings
  AddSubMenu(_("POPUP SETTINGS"), (int)Components::Popups, this, _(MENUMESSAGE_UI_POPUP_HELP_MSG));

  // filters
  AddSubMenu(_("GAME FILTERS"),  (int)Components::Filters, this, _(MENUMESSAGE_UI_GAME_FILTERS_MSG));

  // Display filename
  AddSwitch(_("DISPLAY BY FILENAME"), RecalboxConf::Instance().GetDisplayByFileName(), (int)Components::DisplayByFileName, this, _(MENUMESSAGE_UI_FILE_NAME_MSG));

  // Game List Update
  AddSubMenu(_("UPDATE GAMES LISTS"), (int)Components::UpdateGamelist, this, _(MENUMESSAGE_UI_UPDATE_GAMELIST_HELP_MSG));

  // System hiding
  AddSubMenu(_("HIDE SYSTEMS INDIVIDUALLY"), (int)Components::HideSystems, this,
             _("Hide or un-hide regular systems individually"));
}

void MenuUserInterface::ReloadGamelists()
{
  mWindow.pushGui(new GuiMsgBox(mWindow, _("REALLY UPDATE GAMES LISTS ?"), _("YES"), [] {
    MainRunner::RequestQuit(MainRunner::ExitState::Relaunch, true);
  }, _("NO"), nullptr));
}

void MenuUserInterface::Update(int deltaTime)
{
  Menu::Update(deltaTime);

  // Synchronize volume bar if the volume is modified elsewhere
  if (mBrightness != nullptr)
  {
    int realBrightness = RecalboxConf::Instance().GetBrightness();
    if (realBrightness != (int)mBrightness->Value())
      mBrightness->SetValue((float)realBrightness, false);
  }
}

void MenuUserInterface::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::ScreenSaver: mWindow.pushGui(new MenuScreensavers(mWindow, mSystemManager)); break;
    case Components::Popups: mWindow.pushGui(new MenuPopupSettings(mWindow)); break;
    case Components::Theme: mWindow.pushGui(new MenuThemeOptions(mWindow, mResolver)); break;
    case Components::ThemeConfig: mWindow.pushGui(new MenuThemeConfiguration(mWindow, RecalboxConf::Instance().GetThemeFolder())); break;
    case Components::UpdateGamelist: ReloadGamelists(); break;
    case Components::Filters: mWindow.pushGui(new MenuGameFilters(mWindow, mSystemManager)); break;
    case Components::HideSystems: mWindow.pushGui(new MenuHideSystems(mWindow, mSystemManager)); break;
    case Components::SystemCategories: mWindow.pushGui(new GuiMenuSystemCategories(mWindow, mSystemManager)); break;
    case Components::Brightness:
    case Components::Clock:
    case Components::SwapValidateAndCancel:
    case Components::Help:
    case Components::SystemSort:
    case Components::DisplayByFileName:
    case Components::QuickSelect: break;
  }
}

void MenuUserInterface::MenuSliderMoved(int id, float value)
{
  if ((Components)id == Components::Brightness)
    if (RecalboxConf::Instance().GetBrightness() != (int)value)
    {
      Board::Instance().SetBrightness((int) value);
      RecalboxConf::Instance().SetBrightness((int) value).Save();
    }
}

void MenuUserInterface::MenuSwitchChanged(int id, bool& status)
{
  SystemData* systemData = ViewController::Instance().CurrentSystem();

  switch((Components)id)
  {
    case Components::Clock: RecalboxConf::Instance().SetClock(status).Save(); break;
    case Components::Help: RecalboxConf::Instance().SetShowHelp(status).Save(); break;
    case Components::QuickSelect: RecalboxConf::Instance().SetQuickSystemSelect(status).Save(); break;
    case Components::SwapValidateAndCancel:
    {
      RecalboxConf::Instance().SetSwapValidateAndCancel(status).Save();
      UpdateHelpBar();
      break;
    }
    case Components::DisplayByFileName:
      RecalboxConf::Instance().SetDisplayByFileName(status).Save();
      ViewController::Instance().GetOrCreateGamelistView(systemData)->refreshList();
      ViewController::Instance().InvalidateAllGamelistsExcept(nullptr);
      break;
    case Components::Popups:
    case Components::Theme:
    case Components::ThemeConfig:
    case Components::UpdateGamelist:
    case Components::ScreenSaver:
    case Components::SystemSort:
    case Components::Filters:
    case Components::Brightness:
    case Components::HideSystems:
    default: break;
  }
}


void MenuUserInterface::MenuSingleChanged(int id, int index, const SystemSorting& value)
{
  (void)index;
  (void)id;
  RecalboxConf::Instance().SetSystemSorting(value).Save();

  mSystemManager.SystemSorting();
  ViewController::Instance().getSystemListView().Sort();
}

std::vector<SelectorEntry<SystemSorting>> MenuUserInterface::GetSortingEntries()
{
  mOriginalSort = RecalboxConf::Instance().GetSystemSorting();
  return std::vector<SelectorEntry<SystemSorting>>
  ({
    { _("DEFAULT")                                    , SystemSorting::Default                                    },
    { _("Name")                                       , SystemSorting::Name                                       },
    { _("RELEASE DATE")                               , SystemSorting::ReleaseDate                                },
    { _("TYPE, THEN NAME")                            , SystemSorting::SystemTypeThenName                         },
    { _("TYPE, THEN RELEASE DATE")                    , SystemSorting::SystemTypeThenReleaseDate                  },
    { _("MANUFACTURER, THEN NAME")                    , SystemSorting::ManufacturerThenName                       },
    { _("MANUFACTURER, THEN RELEASE DATE")            , SystemSorting::ManufacturerThenReleaseData                },
    { _("TYPE, THEN MANUFACTURER, THEN NAME")         , SystemSorting::SystemTypeThenManufacturerThenName         },
    { _("TYPE, THEN MANUFACTURER, THEN RELEASE DATE") , SystemSorting::SystemTypeThenManufacturerThenReleasdeDate },
  });
}

