//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuSystemList.h"
#include "MenuSystemConfiguration.h"
#include <systems/SystemManager.h>
#include <guis//menus/base/ItemSubMenu.h>

MenuSystemList::MenuSystemList(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("ADVANCED EMULATOR CONFIGURATION"))
  , mSystemManager(systemManager)
{
}

void MenuSystemList::BuildMenuItems()
{
  // For each activated systemmSystemManager
  const SystemManager::List& systems = mSystemManager.AllSystems();
  for(int i = 0; i < (int)systems.Count(); ++i)
    if (!systems[i]->IsVirtual())
      mMenus[systems[i]] = AddSubMenu("", i, this);
}

void MenuSystemList::SubMenuSelected(int id)
{
  SystemData& system = *mSystemManager.AllSystems()[id];
  MenuSystemConfiguration::AdvancedMenuOptions options = MenuSystemConfiguration::AdvancedMenuOptions::All;
  if (Board::Instance().CrtBoard().IsCrtAdapterAttached())
    options = MenuSystemConfiguration::AdvancedMenuOptions::Emulator |
              MenuSystemConfiguration::AdvancedMenuOptions::Rewind |
              MenuSystemConfiguration::AdvancedMenuOptions::AutoSave;
  mWindow.pushGui(new MenuSystemConfiguration(mWindow, system, mSystemManager, options));
}

void MenuSystemList::onShow()
{
  for(auto& kv : mMenus)
  {
    String emulator;
    String core;
    String name(kv.first->FullName());
    if (!Renderer::Instance().Is480pOrLower())
    {
      name.UpperCase();
      EmulatorManager::GetSystemEmulator(*kv.first, emulator, core);
      if (!emulator.empty())
      {
        name.Append(" - ").Append(emulator);
        if (emulator != core) name.Append(' ').Append(core);
      }
    }
    kv.second->ChangeLabel(name);
  }
}
