#include <guis/menus/MenuMain.h>
#include <guis/MenuMessages.h>
#include <recalbox/RecalboxSystem.h>
#include <animations/LambdaAnimation.h>

#include <guis/menus/MenuSystem.h>
#include <guis/menus/MenuUpdates.h>
#include <guis/menus/MenuGameSettings.h>
#include <guis/menus/MenuPads.h>
#include <guis/menus/MenuUserInterface.h>
#include <guis/menus/MenuSound.h>
#include <guis/menus/MenuNetwork.h>
#include <guis/menus/MenuAdvancedSettings.h>
#include <guis/menus/MenuCRT.h>
#include "guis/GuiBiosScan.h"
#include "guis/menus/MenuQuit.h"
#include <emulators/run/GameRunner.h>
#include "MenuScraper.h"
#include "MenuTate.h"
#include "MenuDownloadContents.h"
#include "MenuArcade.h"
#include "MenuDownloadGamePacks.h"
#include "guis/GuiMsgBoxScroll.h"
#include <guis/GuiScraperRun.h>

MenuMain::MenuMain(WindowManager& window, SystemManager& systemManager, const IGlobalVariableResolver& resolver)
  : Menu(window, _("MAIN MENU"))
  , mResolver(resolver)
  , mSystemManager(systemManager)
{
  // Bartop mode?
  bool bartop = RecalboxConf::Instance().GetMenuType() == RecalboxConf::Menu::Bartop;

  AddHeader("truc qui sert à rien");

  // Kodi
  if (RecalboxSystem::kodiExists() && RecalboxConf::Instance().GetKodiEnabled())
    AddSubMenu(_("KODI MEDIA CENTER"), MenuThemeData::MenuIcons::Type::Kodi, (int)Components::Kodi, this, _(MENUMESSAGE_START_KODI_HELP_MSG));

  AddHeader("trucs utiles");

  // System menu
  if (!bartop)
    AddSubMenu(_("SYSTEM SETTINGS"), MenuThemeData::MenuIcons::Type::System, (int)Components::System, this, _(MENUMESSAGE_SYSTEM_HELP_MSG));

  // Update menu
  if (!bartop)
    AddSubMenu(_("UPDATES"), MenuThemeData::MenuIcons::Type::Updates, (int)Components::Update, this, _(MENUMESSAGE_UPDATE_HELP_MSG));

  // Recalbox RGB Dual menu
  if(Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBDual)
    AddSubMenu(_("RECALBOX RGB DUAL"), MenuThemeData::MenuIcons::Type::RecalboxRGBDual, (int)Components::RecalboxRGBDual, this, _(MENUMESSAGE_RECALBOXRGBDUAL_HELP_MSG));
  if(Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma || Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJammaV2)
    AddSubMenu(_("RECALBOX RGB JAMMA"), MenuThemeData::MenuIcons::Type::RecalboxRGBDual, (int)Components::RecalboxRGBDual, this, _(MENUMESSAGE_RECALBOXRGBDUAL_HELP_MSG));

  // Games menu
  AddSubMenu(_("GAMES SETTINGS"), MenuThemeData::MenuIcons::Type::Games, (int)Components::Games, this, _(MENUMESSAGE_GAME_SETTINGS_HELP_MSG));

  // Download menu
  if (!bartop)
    AddSubMenu(_("DOWNLOAD CONTENTS"), MenuThemeData::MenuIcons::Type::Download, (int)Components::ContentDoanwloader, this, _(MENUMESSAGE_DOWNLOADERS_SETTINGS_HELP_MSG));

  // Controllers menu
  if (!bartop)
    AddSubMenu(_("CONTROLLERS SETTINGS"), MenuThemeData::MenuIcons::Type::Controllers, (int)Components::Controllers, this, _(MENUMESSAGE_CONTROLLER_HELP_MSG));

  // UI Settings menu
  if (!bartop)
    AddSubMenu(_("UI SETTINGS"), MenuThemeData::MenuIcons::Type::Ui, (int)Components::UISettings, this, _(MENUMESSAGE_UI_HELP_MSG));

  AddHeader("bidules bizarres");

  // Atcade menu
  if (!bartop)
    AddSubMenu(_("ARCADE SETTINGS"), MenuThemeData::MenuIcons::Type::Arcade, (int)Components::Arcade, this, _(MENUMESSAGE_ARCADE_HELP_MSG));

  // TATE menu
  if (!bartop)
    AddSubMenu(_("TATE SETTINGS"), MenuThemeData::MenuIcons::Type::Tate, (int)Components::Tate, this, _(MENUMESSAGE_TATE_HELP_MSG));

  // Sound menu
  AddSubMenu(_("SOUND SETTINGS"), MenuThemeData::MenuIcons::Type::Sound, (int)Components::Sound, this, _(MENUMESSAGE_SOUND_HELP_MSG));

  // Network
  if (!bartop)
    AddSubMenu(_("NETWORK SETTINGS"), MenuThemeData::MenuIcons::Type::Network, (int)Components::Network, this, _(MENUMESSAGE_NETWORK_HELP_MSG));

  // Scraper
  if (!bartop)
    AddSubMenu(_("SCRAPER"), MenuThemeData::MenuIcons::Type::Scraper, (int)Components::Scraper, this, _(MENUMESSAGE_SCRAPER_HELP_MSG));

  AddHeader("machins compliqués");

  // Advanced
  if (!bartop)
    AddSubMenu(_("ADVANCED SETTINGS"), MenuThemeData::MenuIcons::Type::Advanced, (int)Components::Advanced, this, _(MENUMESSAGE_ADVANCED_HELP_MSG));

  // Bios
  if (!bartop)
    AddSubMenu(_("BIOS CHECKING"), MenuThemeData::MenuIcons::Type::Games, (int)Components::Bios, this, _(MENUMESSAGE_BIOS_HELP_MSG));

  // License
  AddSubMenu(_("OPEN-SOURCE LICENSE"), MenuThemeData::MenuIcons::Type::License, (int)Components::License, this, _(MENUMESSAGE_LICENSE_HELP_MSG));

  // Quit
  AddSubMenu(_("QUIT"), MenuThemeData::MenuIcons::Type::Quit, (int)Components::Quit, this, _(MENUMESSAGE_QUIT_HELP_MSG));

  // Animation
  auto fadeFunc = [this](float t)
  {
    setOpacity((int)lerp<float>(0, 255, t));
    setPosition(getPosition().x(),
                lerp<float>(Renderer::Instance().DisplayHeightAsFloat(), (Renderer::Instance().DisplayHeightAsFloat() - mSize.y()) / 2, t));
  };

  setOpacity(0);
  setAnimation(new LambdaAnimation(fadeFunc, 200), 0);
}

void MenuMain::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Kodi: if (!GameRunner::Instance().RunKodi()) { LOG(LogWarning) << "[Kodi] Error running Kodi."; } break;
    case Components::System: mWindow.pushGui(new MenuSystem(mWindow, mSystemManager)); break;
    case Components::Update: mWindow.pushGui(new MenuUpdates(mWindow)); break;
    case Components::RecalboxRGBDual: mWindow.pushGui(new MenuCRT(mWindow, mSystemManager, Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma ? _("JAMMA SETTINGS") : _("CRT SETTINGS"))); break;
    case Components::Games: mWindow.pushGui(new MenuGameSettings(mWindow, mSystemManager)); break;
    case Components::ContentDoanwloader:
    {
      #if defined(BETA) || defined(DEBUG)
        mWindow.pushGui(new MenuDownloadContents(mWindow, mSystemManager)); break;
      #else
        mWindow.pushGui(new MenuDownloadGamePacks(mWindow, mSystemManager)); break;
      #endif
    }
    case Components::Controllers: mWindow.pushGui(new MenuPads(mWindow)); break;
    case Components::UISettings: mWindow.pushGui(new MenuUserInterface(mWindow, mSystemManager, mResolver)); break;
    case Components::Arcade: mWindow.pushGui(new MenuArcade(mWindow, mSystemManager, nullptr)); break;
    case Components::Tate: mWindow.pushGui(new MenuTate(mWindow, mSystemManager)); break;
    case Components::Sound: mWindow.pushGui(new MenuSound(mWindow)); break;
    case Components::Network: mWindow.pushGui(new MenuNetwork(mWindow)); break;
    case Components::Scraper:
    {
      if (GuiScraperRun::IsRunning())
        GuiScraperRun::CreateOrShow(mWindow, mSystemManager, SystemManager::List(), ScrapingMethod::All, &GameRunner::Instance(), Renderer::Instance().Is480pOrLower());
      else
        mWindow.pushGui(new MenuScraper(mWindow, mSystemManager));
      break;
    }
    case Components::Advanced: mWindow.pushGui(new MenuAdvancedSettings(mWindow, mSystemManager)); break;
    case Components::Bios: mWindow.pushGui(new GuiBiosScan(mWindow, mSystemManager)); break;
    case Components::License:
    {
      mWindow.pushGui(
        new GuiMsgBoxScroll(mWindow, "RECALBOX",
                            ScrambleSymetric2(String(MenuMessages::LICENCE_MSG, MenuMessages::LICENCE_MSG_SIZE), __MESSAGE_DECORATOR),
                            _("OK"), nullptr, "", nullptr, "", nullptr, TextAlignment::Left));
      break;
    }
    case Components::Quit: mWindow.pushGui(new MenuQuit(mWindow)); break;
  }
}

String MenuMain::ScrambleSymetric2(const String& input, const String& key)
{
  String buffer = input;
  int l = (int)key.size();

  for (size_t i = 0; i < input.size(); ++i)
    buffer[i] = (char)(input[i] ^ (key[i % l] + (i*17)));

  return buffer;
}
