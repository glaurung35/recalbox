//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuSystem.h"
#include <guis/menus/MenuDiskUsage.h>
#include <guis/MenuMessages.h>
#include <Upgrade.h>
#include <MainRunner.h>

MenuSystem::MenuSystem(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("SYSTEM SETTINGS"))
  , mSystemManager(systemManager)
{
}

void MenuSystem::BuildMenuItems()
{
  // Version
  String version = Upgrade::Instance().CurrentVersion();
  String arch = "unknown";
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::UndetectedYet:        arch = "undetected"; break;
    case BoardType::Unknown:              arch = "unknown"; break;
    case BoardType::Pi0:                  arch = "RPi zero"; break;
    case BoardType::Pi02:                 arch = "RPi zero 2"; break;
    case BoardType::Pi1:                  arch = "RPi 1"; break;
    case BoardType::Pi2:                  arch = "RPi 2"; break;
    case BoardType::Pi3:                  arch = "RPi 3"; break;
    case BoardType::Pi3plus:              arch = "RPi 3B+"; break;
    case BoardType::Pi4:                  arch = "RPi 4"; break;
    case BoardType::Pi400:                arch = "RPi 400"; break;
    case BoardType::Pi5  :                arch = "RPi 5"; break;
    case BoardType::UnknownPi:            arch = "unknown RPi"; break;
    case BoardType::OdroidAdvanceGo:      arch = "Odroid GO Advance"; break;
    case BoardType::OdroidAdvanceGoSuper: arch = "Odroid GO Super"; break;
    case BoardType::PCx86:                arch = "x86"; break;
    case BoardType::PCx64:                arch = "x64"; break;
    case BoardType::RG351P:               arch = "RG351P/M"; break;
    case BoardType::RG353P:               arch = "RG353P"; break;
    case BoardType::RG353V:               arch = "RG353V"; break;
    case BoardType::RG353M:               arch = "RG353M"; break;
    case BoardType::RG503:                arch = "RG503"; break;
    case BoardType::RG351V:               arch = "RG351V"; break;
  }

  #if INTPTR_MAX == INT32_MAX
    const char* archSize = "32bits";
  #elif INTPTR_MAX == INT64_MAX
    const char* archSize = "64bits";
  #else
    #error "Environment not 32 or 64-bit."
  #endif
  arch.Append(' ').Append(archSize);
  AddText(_("VERSION"), version.Append(" (").Append(arch).Append(')'), _(MENUMESSAGE_VERSION_HELP_MSG));

  // Share space
  MountMonitor::DeviceMountReferences mounts = mSystemManager.GetMountMonitor().AllMountPoints();
  if (mounts.size() == 1)
  {
    mounts[0]->UpdateSize();
    AddText(_("DISK USAGE (FREE/TOTAL)"), mounts[0]->DisplayableFreeSpace(), RecalboxSystem::isFreeSpaceUnderLimit(mounts[0]->FreeSize()) ? 0xFF0000FF : 0, _(MENUMESSAGE_DISK_USAGE_HELP_MSG));
  }
  else
    AddSubMenu(_("DISK USAGE (FREE/TOTAL)"), (int)Components::DiskUsage, this, _(MENUMESSAGE_DISK_USAGE_HELP_MSG));

  // Storage device
  int currentIndex = 0;
  SelectorEntry<StorageDevices::Device>::List list = GetStorageEntries(currentIndex);
  if (!list.empty())
    mStorages = AddList<StorageDevices::Device>(_("STORAGE DEVICE"), (int)Components::Storage, this, list, list[currentIndex].mValue, list[currentIndex].mValue, _(MENUMESSAGE_STORAGE_DEVICE_HELP_MSG));

  // language choice
  mCulture = AddList<String>(_("LANGUAGE"), (int)Components::Culture, this, GetCultureEntries(), mOriginalCulture = RecalboxConf::Instance().GetSystemLanguage(), "en_US", _(MENUMESSAGE_LANGUAGE_HELP_MSG));

  // Keyboard
  mKeyboard = AddList<String>(_("KEYBOARD"), (int)Components::Keyboard, this, GetKeyboardEntries(), mOriginalKeyboard = RecalboxConf::Instance().GetSystemKbLayout(), "us", _(MENUMESSAGE_KEYBOARD_HELP_MSG));
}

MenuSystem::~MenuSystem()
{
  String newUUID = mStorages->SelectedValue().UUID;
  // Storage changed?
  if (newUUID != mOriginalStorage)
    Files::SaveFile(Path("/overlay/.configs/skipvideosplash"), newUUID);

  // Reboot?
  if ((newUUID != mOriginalStorage) ||
      (mCulture->SelectedValue() != mOriginalCulture) ||
      (mKeyboard->SelectedValue() != mOriginalKeyboard))
    RequestReboot();
}

SelectorEntry<StorageDevices::Device>::List MenuSystem::GetStorageEntries(int& currentIndex)
{
  SelectorEntry<StorageDevices::Device>::List list;

  mOriginalStorage = mStorageDevices.GetStorageDevice();
  int index = 0;
  for(const StorageDevices::Device& device : mStorageDevices.GetStorageDevices())
  {
    String display = device.DisplayName;
    if (device.Size != 0)
      display.Append(" (")
             .Append(_("Free"))
             .Append(" ")
             .Append(device.HumanFree())
             .Append('/')
             .Append(device.HumanSize())
             .Append(')');
    list.push_back({ display, device, device.Current });
    if (device.Current) currentIndex = index;
    index++;
  }

  return list;
}

SelectorEntry<String>::List MenuSystem::GetCultureEntries()
{
  return SelectorEntry<String>::List
  ({

    { "اللغة العربية" , "ar_YE" , mOriginalCulture == "ar_YE" },
    { "CATALÀ"        , "ca_ES" , mOriginalCulture == "ca_ES" },
    { "čeština"       , "cs_CZ" , mOriginalCulture == "cs_CZ" },
    { "DEUTSCH"       , "de_DE" , mOriginalCulture == "de_DE" },
    { "DEUTSCH (CH)"  , "de_CH" , mOriginalCulture == "de_CH" },
    { "ελληνικά"      , "el_GR" , mOriginalCulture == "el_GR" },
    { "ENGLISH"       , "en_US" , mOriginalCulture == "en_US" },
    { "ENGLISH (UK)"  , "en_GB" , mOriginalCulture == "en_GB" },
    { "ESPAÑOL"       , "es_ES" , mOriginalCulture == "es_ES" },
    { "EUSKARA"       , "eu_ES" , mOriginalCulture == "eu_ES" },
    { "PERSIAN"       , "fa_IR" , mOriginalCulture == "fa_IR" },
    { "FRANÇAIS"      , "fr_FR" , mOriginalCulture == "fr_FR" },
    { "GALICIAN"      , "gl_ES" , mOriginalCulture == "gl_ES" },
    { "MAGYAR"        , "hu_HU" , mOriginalCulture == "hu_HU" },
    { "ITALIANO"      , "it_IT" , mOriginalCulture == "it_IT" },
    { "日本語"         , "ja_JP" , mOriginalCulture == "ja_JP" },
    { "한국어"         , "ko_KR" , mOriginalCulture == "ko_KR" },
    { "Lëtzebuergesch", "lb_LU" , mOriginalCulture == "lb_LU" },
    { "latviešu"      , "lv_LV" , mOriginalCulture == "lv_LV" },
    { "BOKMAL"        , "nb_NO" , mOriginalCulture == "nb_NO" },
    { "NEDERLANDS"    , "nl_NL" , mOriginalCulture == "nl_NL" },
    { "NORSK"         , "nn_NO" , mOriginalCulture == "nn_NO" },
    { "POLSKI"        , "pl_PL" , mOriginalCulture == "pl_PL" },
    { "PORTUGUES"     , "pt_BR" , mOriginalCulture == "pt_BR" },
    { "ROMANIAN"      , "ro_RO" , mOriginalCulture == "ro_RO" },
    { "Русский"       , "ru_RU" , mOriginalCulture == "ru_RU" },
    { "SLOVAK"        , "sk_SK" , mOriginalCulture == "sk_SK" },
    { "SVENSKA"       , "sv_SE" , mOriginalCulture == "sv_SE" },
    { "TÜRKÇE"        , "tr_TR" , mOriginalCulture == "tr_TR" },
    { "UKRAINIAN"     , "uk_UA" , mOriginalCulture == "uk_UA" },
    { "简体中文"       , "zh_CN" , mOriginalCulture == "zh_CN" },
    { "正體中文"       , "zh_TW" , mOriginalCulture == "zh_TW" },
  });
}

SelectorEntry<String>::List MenuSystem::GetKeyboardEntries()
{
  return SelectorEntry<String>::List
  ({
    { "DE (German QWERTZ)"  , "de" , mOriginalKeyboard == "de" },
    { "DK (Denmark QWERTY)" , "dk" , mOriginalKeyboard == "dk" },
    { "ES (Spanish QWERTY)" , "es" , mOriginalKeyboard == "es" },
    { "FR (French AZERTY)"  , "fr" , mOriginalKeyboard == "fr" },
    { "IT (Italian QWERTY)" , "it" , mOriginalKeyboard == "it" },
    { "US (Standard QWERTY)", "us" , mOriginalKeyboard == "us" },
  });
}

void MenuSystem::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::DiskUsage:
    case Components::Storage: break;
    case Components::Culture: RecalboxConf::Instance().SetSystemLanguage(value).Save(); break;
    case Components::Keyboard: RecalboxConf::Instance().SetSystemKbLayout(value).Save(); break;
  }
}

void MenuSystem::MenuSingleChanged(int id, int index, const StorageDevices::Device& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::Storage: mStorageDevices.SetStorageDevice(value); break;
    case Components::DiskUsage:
    case Components::Culture:
    case Components::Keyboard: break;
  }
}

void MenuSystem::SubMenuSelected(int id)
{
  if ((Components)id == Components::DiskUsage)
    mWindow.pushGui(new MenuDiskUsage(mWindow, mSystemManager.GetMountMonitor()));
}
