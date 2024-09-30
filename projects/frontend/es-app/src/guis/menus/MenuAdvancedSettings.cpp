//
// Created by bkg2k on 11/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuAdvancedSettings.h"
#include "MenuBootSettings.h"
#include "MenuVirtualSystems.h"
#include "MenuSystemList.h"
#include "MenuKodiSettings.h"
#include "MenuCRT.h"
#include "MenuResolutionSettings.h"
#include "GuiMenuPinballSettings.h"
#include "hardware/RPiEepromUpdater.h"
#include "views/MenuFilter.h"
#include "GuiMenuUserScripts.h"
#include <guis/MenuMessages.h>
#include <utils/Files.h>
#include <guis/GuiMsgBox.h>
#include <MainRunner.h>

MenuAdvancedSettings::MenuAdvancedSettings(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("ADVANCED SETTINGS"))
  , mSystemManager(systemManager)
  , mDefaultOverclock({ "none", _("NONE"), false, 0})
  , mLastHazardous(false)
  , mValidOverclock(false)
{
}

void MenuAdvancedSettings::BuildMenuItems()
{
  #if defined(BETA) || defined(DEBUG)
  AddSwitch(_("DEBUG LOGS"), RecalboxConf::Instance().GetDebugLogs(), (int)Components::DebugLogs, this, _(MENUMESSAGE_ADVANCED_DEBUGLOGS_HELP_MSG));
  #endif

  bool isCrt = Board::Instance().CrtBoard().IsCrtAdapterAttached();

  // Overclock choice
  mOverclock = AddList<Overclocking>(_("OVERCLOCK"), (int)Components::OverclockList, this, GetOverclockEntries(), GetCurrentOC(), mDefaultOverclock, _(MENUMESSAGE_ADVANCED_OVERCLOCK_HELP_MSG));

  // Boot
  AddSubMenu(_("BOOT SETTINGS"), (int)Components::BootSubMenu, this, _(MENUMESSAGE_ADVANCED_BOOT_HELP_MSG));

  // Virtual systems
  AddSubMenu(_("VIRTUAL SYSTEMS"), (int)Components::VirtualSubMenu, this, _(MENUMESSAGE_ADVANCED_VIRTUALSYSTEMS_HELP_MSG));
  // Visual Pinball
  if (Board::Instance().GetBoardType() == BoardType::Pi5 || Board::Instance().GetBoardType() == BoardType::PCx64)
    AddSubMenu(_("PINBALL SETTINGS"), (int)Components::PinballSubMenu, this, _(MENUMESSAGE_ADVANCED_PINBALL_HELP_MSG));

  // CRT
  if (Board::Instance().CanHaveCRTBoard())
    AddSubMenu(_("RECALBOX CRT"), (int)Components::CrtSubMenu, this, _(MENUMESSAGE_ADVANCED_CRT_HELP_MSG));

  // RESOLUTION
  if (!isCrt /*&&
      Board::Instance().GetBoardType() != BoardType::PCx64*/)
    AddSubMenu(_("RESOLUTIONS"), (int)Components::ResolutionSubMenu, this, _(MENUMESSAGE_ADVANCED_RESOLUTION_HELP_MSG));

  // Custom config for systems
  AddSubMenu(_("ADVANCED EMULATOR CONFIGURATION"), (int)Components::AdvancedSubMenu, this, _(MENUMESSAGE_ADVANCED_EMULATOR_ADVANCED_HELP_MSG));

  //Kodi
  #ifndef DEBUG
  if (RecalboxSystem::kodiExists())
    AddSubMenu(_("KODI SETTINGS"), (int)Components::KodiSubMenu, this, _(MENUMESSAGE_ADVANCED_KODI_HELP_MSG));
  #else
  AddSubMenu(_("KODI SETTINGS"), (int)Components::KodiSubMenu, this, _(MENUMESSAGE_ADVANCED_KODI_HELP_MSG));
  #endif

  // Cases
  if(!Case::SupportedManualCases().empty())
    AddList<String>(_("CASE MANAGEMENT"),  (int)Components::Cases, this, GetCasesEntries(), Case::CurrentCase().ShortName(), "", _(MENUMESSAGE_ADVANCED_CASES_HELP_MSG));

  // framerate
  AddSwitch(_("SHOW FRAMERATE"), RecalboxConf::Instance().GetGlobalShowFPS(), (int)Components::ShowFPS, this, _(MENUMESSAGE_ADVANCED_FRAMERATE_HELP_MSG));

  // Recalbox Manager
  AddSwitch(_("RECALBOX MANAGER"), RecalboxConf::Instance().GetSystemManagerEnabled(), (int)Components::Manager, this, _(MENUMESSAGE_ADVANCED_MANAGER_HELP_MSG));

  // User scripts
  if (NotificationManager::Instance().HasManualScript())
    AddSubMenu(_("USER SCRIPTS"), (int)Components::UserScripts, this, _(MENUMESSAGE_ADVANCED_USER_SCRIPTS));

  // Eeprom update
  if(MenuFilter::ShouldDisplayMenuEntry(MenuFilter::PiEeprom))
    AddSubMenu(_("BOOTLOADER UPDATE"), (int)Components::EepromUpdate, this, _(MENUMESSAGE_ADVANCED_EEPROM_UPDATE));

  // Reset Factory
  AddSubMenu(_("RESET TO FACTORY SETTINGS"), (int)Components::FactoryReset, this, _(MENUMESSAGE_ADVANCED_FACTORY_RESET));
}

MenuAdvancedSettings::~MenuAdvancedSettings()
{
  if (mValidOverclock)
    if (mOriginalOverclock != mOverclock->SelectedValue().File)
    {
      if (mLastHazardous)
        mWindow.pushGui(
          new GuiMsgBox(mWindow, _("TURBO AND EXTREM OVERCLOCK PRESETS MAY CAUSE SYSTEM UNSTABILITIES, SO USE THEM AT YOUR OWN RISK.\nIF YOU CONTINUE, THE SYSTEM WILL REBOOT NOW."),
                        _("YES"), [] { MainRunner::RequestQuit(MainRunner::ExitState::NormalReboot); }, _("NO"), [this] { ResetOverclock(); }));
      else
        RequestReboot();
    }
}

Overclocking MenuAdvancedSettings::GetCurrentOC()
{
  OverclockList oc = AvailableOverclocks();
  String currentOC = RecalboxConf::Instance().GetOverclocking();
  for(const Overclocking& overclock : oc)
    if (currentOC == overclock.File)
      return overclock;
  return mDefaultOverclock;
}

SelectorEntry<Overclocking>::List MenuAdvancedSettings::GetOverclockEntries()
{
  SelectorEntry<Overclocking>::List list;

  // Add entries
  OverclockList oc = AvailableOverclocks();
  mOriginalOverclock = RecalboxConf::Instance().GetOverclocking();
  bool ocFound = false;
  for(const Overclocking& overclock : oc)
  {
    bool found = mOriginalOverclock == overclock.File;
    ocFound |= found;
    String label = String(overclock.Description).Append(" (", 2).Append(overclock.Frequency).Append(" Mhz)", 5).Append(overclock.Hazardous ? " \u26a0" : "");
    list.push_back({ label, overclock, found });
  }
  std::sort(list.begin(), list.end(), [](const SelectorEntry<Overclocking>& a, const SelectorEntry<Overclocking>& b) { return a.mValue.Frequency < b.mValue.Frequency; });
  // Add none
  mValidOverclock = !list.empty();
  if (mValidOverclock)
    list.insert(list.begin(), { _("NONE"), mDefaultOverclock, !ocFound });
  if (!ocFound) mOriginalOverclock = "none";

  return list;
}


MenuAdvancedSettings::OverclockList MenuAdvancedSettings::AvailableOverclocks()
{
  OverclockList result;

  // Build final path to fetch overclocking configs from
  Path basePath(sOverclockBaseFolder);
  String boardFolder;
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::Pi0: boardFolder = "rpi0"; break;
    case BoardType::Pi02: boardFolder = "rpi02"; break;
    case BoardType::Pi1: boardFolder = "rpi1"; break;
    case BoardType::Pi2: boardFolder = "rpi2"; break;
    case BoardType::Pi3: boardFolder = "rpi3"; break;
    case BoardType::Pi3plus: boardFolder = "rpi3plus"; break;
    case BoardType::Pi4: boardFolder = "rpi4"; break;
    case BoardType::Pi400: boardFolder = "rpi400"; break;
    case BoardType::Pi5: boardFolder = "rpi5"; break;
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    case BoardType::UnknownPi:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG351P:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::RG351V:
    case BoardType::PCx86:
    case BoardType::PCx64: break;
  }
  { LOG(LogDebug) << "[Overclock] Board: " << (int)Board::Instance().GetBoardType(); }
  if (boardFolder.empty()) return result;
  Path finalPath = basePath / boardFolder;
  { LOG(LogDebug) << "[Overclock] Final source folder: " <<finalPath.ToString(); }

  // Get O/C config list
  Path::PathList list = finalPath.GetDirectoryContent();

  // Load and get description
  for(const Path& path : list)
  {
    String trash;
    String desc;
    bool hazard = false;
    int freq = 0;
    // Extract lines - doesn't matter if the file does not load, it returns an empty string
    for(const String& line : Files::LoadFile(path).Split('\n'))
    {
      if (line.StartsWith(LEGACY_STRING("# Description: ")))
        (void)line.Extract(':', trash, desc, true);
      else if (line.StartsWith(LEGACY_STRING("# Warning")))
        hazard = true;
      else if (line.StartsWith(LEGACY_STRING("# Frequency: ")))
      {
        String freqStr;
        (void)line.Extract(':', trash,freqStr, true);
        (void)freqStr.TryAsInt(freq);
      }
    }
    // Record?
    if (!desc.empty() && freq != 0)
    {
      result.push_back({ path.ToString(), desc, hazard, freq });
      { LOG(LogInfo) << "[Overclock] File " << path.ToString() << " validated."; }
    }
    else { LOG(LogError) << "[Overclock] Invalid file " << path.ToString(); }
  }

  return result;
}

void MenuAdvancedSettings::ResetOverclock()
{
  mOverclock->SetSelectedItemValue(mDefaultOverclock, false);
}

void MenuAdvancedSettings::MenuSingleChanged(int id, int index, const Overclocking& value)
{
  (void)index;
  if ((Components)id == Components::OverclockList)
  {
    RecalboxConf::Instance().SetOverclocking(value.File).Save();

    if (RecalboxSystem::MakeBootReadWrite())
    {
      Files::SaveFile(Path(sOverclockFile), Files::LoadFile(Path(value.File)));
      RecalboxSystem::MakeBootReadOnly();
      mLastHazardous = value.Hazardous;
      { LOG(LogInfo) << "[Overclock] Overclocking set to " << value.Description; }
    }
    else { LOG(LogError) << "[Overclock] Cannot make boot read write"; }
  }
}

void MenuAdvancedSettings::MenuSwitchChanged(int id, bool& status)
{
  switch ((Components)id)
  {
    case Components::DebugLogs:
    {
      MainRunner::SetDebugLogs(status, false);
      RecalboxConf::Instance().SetDebugLogs(status).Save();
      break;
    }
    case Components::ShowFPS: RecalboxConf::Instance().SetGlobalShowFPS(status).Save(); break;
    case Components::Manager: RecalboxConf::Instance().SetSystemManagerEnabled(status).Save(); break;
    case Components::OverclockList:
    case Components::BootSubMenu:
    case Components::VirtualSubMenu:
    case Components::AdvancedSubMenu:
    case Components::KodiSubMenu:
    case Components::PinballSubMenu:
    case Components::Cases:
    case Components::SecuritySubMenu:
    case Components::FactoryReset:
    case Components::CrtSubMenu:
    case Components::ResolutionSubMenu:
    case Components::EepromUpdate:
    case Components::UserScripts:
    default: break;
  }
}

void MenuAdvancedSettings::SubMenuSelected(int id)
{
  switch ((Components)id)
  {
    case Components::BootSubMenu: mWindow.pushGui(new MenuBootSettings(mWindow, mSystemManager)); break;
    case Components::CrtSubMenu: mWindow.pushGui(new MenuCRT(mWindow, mSystemManager, Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma ? _("JAMMA SETTINGS") : _("CRT SETTINGS"))); break;
    case Components::VirtualSubMenu: mWindow.pushGui(new MenuVirtualSystems(mWindow, mSystemManager)); break;
    case Components::AdvancedSubMenu: mWindow.pushGui(new MenuSystemList(mWindow, mSystemManager)); break;
    case Components::KodiSubMenu: mWindow.pushGui(new MenuKodiSettings(mWindow)); break;
    case Components::ResolutionSubMenu: mWindow.pushGui(new MenuResolutionSettings(mWindow, mSystemManager)); break;
    case Components::PinballSubMenu: mWindow.pushGui(new GuiMenuPinballSettings(mWindow)); break;
    case Components::UserScripts: mWindow.pushGui(new GuiMenuUserScripts(mWindow)); break;
    case Components::FactoryReset: ResetFactory(); break;
    case Components::EepromUpdate: EepromUpdate(); break;
    case Components::OverclockList:
    case Components::ShowFPS:
    case Components::SecuritySubMenu:
    case Components::Manager:
    case Components::DebugLogs:
    case Components::Cases:
    default: break;
  }
}

void MenuAdvancedSettings::EepromUpdate()
{
  RPiEepromUpdater updater;
  if(updater.Error())
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, _("Could not get bootloader status. Something went wrong"), _("OK"), nullptr));
    return;
  }
  if(updater.IsUpdateAvailable())
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, _("An update is available :\n").Append(_("Current version: \n")).Append(updater.CurrentVersion()).Append(_("\nLatest version: \n")).Append(updater.LastVersion()),
                                  _("UPDATE"), [this, updater]{
                                                { LOG(LogInfo) << "[EepromUpdate] Processing UPDATE to " << updater.LastVersion(); }
                                                if(updater.Update())
                                                {
                                                  mWindow.pushGui(
                                                    new GuiMsgBox(mWindow, _("Update success. The bootloader is up to date."), _("OK"), [&]{ RequestReboot(); }));
                                                  { LOG(LogInfo) << "[EepromUpdate] Update success to " << updater.LastVersion(); }
                                                }
                                                else
                                                {
                                                  mWindow.pushGui(new GuiMsgBox(mWindow, _("Could not update bootloader. Something went wrong"), _("OK"), nullptr));
                                                  { LOG(LogError) << "[EepromUpdate] Unable to update to " << updater.LastVersion(); }
                                                }
                                              },
                                  _("CANCEL"), nullptr));
  }
  else
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, _("Your bootloader is up to date.\nThe bootloader version is:\n").Append(updater.CurrentVersion()), _("OK"), nullptr));
  }
}

void MenuAdvancedSettings::ResetFactory()
{
  mWindow.pushGui(new GuiMsgBox(mWindow, _("RESET TO FACTORY SETTINGS\n\nYOU'RE ABOUT TO RESET RECALBOX AND EMULATOR SETTINGS TO DEFAULT VALUES.\nALL YOUR DATA LIKE GAMES, SAVES, MUSIC, SCREENSHOTS AND SO ON, WILL BE KEPT.\n\nTHIS OPERATION CANNOT BE UNDONE!\nARE YOU SURE YOU WANT TO RESET ALL YOUR SETTINGS?"),
                                _("NO"), nullptr,
                                _("YES"), std::bind(MenuAdvancedSettings::ResetFactoryReally, &mWindow)));
}

void MenuAdvancedSettings::ResetFactoryReally(WindowManager* window)
{
  String text("\u26a0 %TITLE% \u26a0\n\n%TEXT%");
  text.Replace("%TITLE%", _("WARNING!"))
      .Replace("%TEXT%", _("YOU'RE ONE CLICK AWAY FROM RESETTING YOUR RECALBOX TO FACTORY SETTINGS!\n\nARE YOU REALLY SURE YOU WANT TO DO THIS?"));
  window->pushGui(new GuiMsgBox(*window, text,
                              _("NO"), nullptr,
                              _("YES"), DoResetFactory));
}

void MenuAdvancedSettings::DoResetFactory()
{
  String::List deleteMe
  ({
    "/recalbox/share/system",                       // Recalbox & emulator configurations
    "/overlay/upper/*",                             // System overlay
    "/overlay/.configs/*",                          // System configurations
    "/overlay/upper.old",                           // System overlay backup
    "/overlay/.config",                             // Old system configurations
    "/boot/recalbox-backup.conf",                   // Recalbox configuration backup
    "/boot/crt/recalbox-crt-config.txt",            // CRT Configuration
    "/boot/crt/recalbox-crt-options.cfg",           // CRT Configuration
    "/boot/crt/recalbox-crt-options.cfg.backup",    // CRT Configuration
    "/boot/crt/rrgbd.hdmi",                         // CRT Configuration
    "/boot/crt/rrgbd.crt",                          // CRT Configuration
    "/boot/crt/.currentvideoconfig",                // CRT Configuration
    "/boot/crt/.stamprrgbdual",                     // CRT Configuration
    "/boot/crt/.stamprrgbdual31khz",                // CRT Configuration
    "/boot/crt/.stamphdmiforced",                   // CRT Configuration
  });

  // Make boot partition writable
  if (!RecalboxSystem::MakeBootReadWrite())
  { LOG(LogError) << "[ResetFactory] Error making boot r/w"; }

  // Delete all required folder/files
  for(const String& path : deleteMe)
    if (system(String("rm -rf ").Append(path).data()) != 0)
    { LOG(LogError) << "[ResetFactory] Error removing folder " << path; }

  IniFile& recalboxBoot(BootConf::Instance());
  // Reset rotation
  recalboxBoot.SetString("screen.rotation", "0");
  // Special case for rpizero plus GPiCase2W, that cannot be detected. Can be removed after the rpizero image is frozen
  if(Board::Instance().GetBoardType() == BoardType::Pi0 && recalboxBoot.AsString("case").starts_with("GPi2W"))
      recalboxBoot.SetString("case", "GPi2W");
  else
    recalboxBoot.SetString("case", "");

  recalboxBoot.Save();

  // Reset!
  if (system("shutdown -r now") != 0)
  { LOG(LogError) << "[ResetFactory] Error rebooting system"; }
}

SelectorEntry<String>::List MenuAdvancedSettings::GetCasesEntries()
{
  SelectorEntry<String>::List list;
  Case currentCase = Case::CurrentCase();
  if(currentCase.Automatic())
  {
    list.push_back({currentCase.DisplayName() + " (AUTO)", currentCase.ShortName(), true});
  }
  else {
    for(const Case& theCase : Case::SupportedManualCases())
      list.push_back({theCase.DisplayName(), theCase.ShortName(), theCase.Model() == currentCase.Model()});
  }

  return list;
}

void MenuAdvancedSettings::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  if ((Components)id == Components::Cases)
  {
    Case selectedCase = Case::FromShortName(value);
    Case currentCase = Case::CurrentCase();
    const auto install = [this, selectedCase] {
      bool installed = selectedCase.Install();
      { LOG(LogInfo) << "[Settings - Cases] Installed case " << selectedCase.DisplayName() << (installed ? " successfully" : " failed!"); }
      RequestReboot();
    };
    if(selectedCase.Model() != currentCase.Model())
    {
      const String installMessage = selectedCase.GetInstallMessage();
      if (installMessage != "")
        mWindow.pushGui(new GuiMsgBox(mWindow, installMessage, _("OK"), install));
      else
        install();
    }
  }
}
