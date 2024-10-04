//
// Created by bkg2k on 15/06/23.
//

#include "MenuArcade.h"
#include "MenuArcadeAllInOneSystem.h"
#include <views/ViewController.h>
#include <guis/MenuMessages.h>
#include <systems/arcade/ArcadeVirtualSystems.h>

MenuArcade::MenuArcade(WindowManager& window, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface)
  :	Menu(window, _("ARCADE SETTINGS"))
  , mSystemManager(systemManager)
  , mArcade(arcadeInterface)
{
}

void MenuArcade::BuildMenuItems()
{
  AddSwitch(_("ENABLE ENHANCED VIEW"), RecalboxConf::Instance().GetArcadeViewEnhanced(), (int)Components::EnhancedView, this, _(MENUMESSAGE_UI_ARCADE_ENHANCED_MSG));

  AddSwitch(_("FOLD CLONES BY DEFAULT"), RecalboxConf::Instance().GetArcadeViewFoldClones(), (int)Components::FoldClones, this, _(MENUMESSAGE_UI_ARCADE_HIDE_CLONES_MSG));

  AddSwitch(_("HIDE BIOS"), RecalboxConf::Instance().GetArcadeViewHideBios(), (int)Components::HideBios, this, _(MENUMESSAGE_UI_ARCADE_HIDE_BIOS_MSG));

  AddSwitch(_("HIDE NON-WORKING GAMES"), RecalboxConf::Instance().GetArcadeViewHideNonWorking(), (int)Components::HideNonWorking, this, _(MENUMESSAGE_UI_ARCADE_HIDE_NONWORKING_MSG));

  AddSwitch(_("ALWAYS USE OFFICIAL NAMES"), RecalboxConf::Instance().GetArcadeUseDatabaseNames(), (int)Components::UseDatabasesNames, this, _(MENUMESSAGE_UI_ARCADE_OFFICIAL_NAMES_MSG));

  // Global menu if no arcade interface has been provided
  if (mArcade == nullptr)
  {
    // Per manufacturer systems
    AddMultiList<String>(_("MANUFACTURER VIRTUAL SYSTEMS"), (int)Components::ManufacturersVirtual, this, GetManufacturersVirtualEntries());

    // All-in-one arcade system options
    AddSubMenu(_("ARCADE ALL-IN-ONE SYSTEM"), (int)Components::GlobalArcadeSystem, this, _(MENUMESSAGE_ADVANCED_ARCADEVIRTUALSYSTEM_HELP_MSG));
  }
}

void MenuArcade::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::EnhancedView: RecalboxConf::Instance().SetArcadeViewEnhanced(status).Save(); break;
    case Components::FoldClones:
    {
      RecalboxConf::Instance().SetArcadeViewFoldClones(status).Save();
      ViewController::Instance().InvalidateAllGamelistsExcept(nullptr);
      break;
    }
    case Components::HideBios:
    {
      RecalboxConf::Instance().SetArcadeViewHideBios(status);
      if (mSystemManager.UpdatedTopLevelFilter())
      {
        ViewController::Instance().InvalidateAllGamelistsExcept(nullptr);
        RecalboxConf::Instance().Save();
      }
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetArcadeViewHideBios(!status);
        status = !status;
      }
      break;
    }
    case Components::HideNonWorking:
    {
      RecalboxConf::Instance().SetArcadeViewHideNonWorking(status);
      if (mSystemManager.UpdatedTopLevelFilter())
      {
        ViewController::Instance().InvalidateAllGamelistsExcept(nullptr);
        RecalboxConf::Instance().Save();
      }
      else
      {
        mWindow.displayMessage(_("There is no game to show after this filter is changed! No change recorded."));
        RecalboxConf::Instance().SetArcadeViewHideNonWorking(!status);
        status = !status;
      }
      break;
    }
    case Components::UseDatabasesNames:
    {
      RecalboxConf::Instance().SetArcadeUseDatabaseNames(status).Save();
      break;
    }
    case Components::ManufacturersVirtual:
    case Components::ManufacturersFilter:
    case Components::GlobalArcadeSystem:
    default: break;
  }
}

SelectorEntry<String>::List MenuArcade::GetManufacturersVirtualEntries()
{
  SelectorEntry<String>::List result;
  const RecalboxConf& conf = RecalboxConf::Instance();

  for(const std::pair<String, String>& rawIdentifier : ArcadeVirtualSystems::GetVirtualArcadeSystemListExtended())
  {
    String identifier(SystemManager::sArcadeManufacturerPrefix);
    identifier.Append(rawIdentifier.first).Replace('\\', '-');
    mManufacturersIdentifiers.push_back(identifier);
    result.push_back({ /*String(rawIdentifier).Replace('\\', " - ")*/rawIdentifier.second, identifier, conf.IsInCollectionArcadeManufacturers(identifier) });
  }
  return result;
}

void MenuArcade::MenuMultiChanged(int id, int index, const String::List& value)
{
  (void)index;
  if ((Components)id == Components::ManufacturersVirtual)
  {
    // Save
    RecalboxConf::Instance().SetCollectionArcadeManufacturers(value).Save();
    // Refresh all systems
    for(const String& manufacturer : mManufacturersIdentifiers)
      mSystemManager.UpdateVirtualArcadeManufacturerSystemsVisibility(manufacturer,
                                                                      RecalboxConf::Instance().IsInCollectionArcadeManufacturers(manufacturer) ?
                                                                      SystemManager::Visibility::ShowAndSelect :
                                                                      SystemManager::Visibility::Hide);
  }
}

void MenuArcade::SubMenuSelected(int id)
{
  if ((Components)id == Components::GlobalArcadeSystem)
    mWindow.pushGui(new MenuArcadeAllInOneSystem(mWindow, mSystemManager));
}
