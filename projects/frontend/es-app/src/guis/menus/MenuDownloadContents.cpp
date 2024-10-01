//
// Created by bkg2k on 05/05/23.
//

#include "MenuDownloadContents.h"
#include "MenuDownloadGamePacks.h"
#include <systems/SystemManager.h>

MenuDownloadContents::MenuDownloadContents(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("DOWNLOAD CONTENTS"))
  , mSystemManager(systemManager)
{
}

void MenuDownloadContents::BuildMenuItems()
{
  AddSubMenu("Free game packs", (int)Components::Games, this);
  AddSubMenu("Overlays", (int)Components::Overlay, this);
  AddSubMenu("Themes", (int)Components::Themes, this);
}

void MenuDownloadContents::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Games: mWindow.pushGui(new MenuDownloadGamePacks(mWindow, mSystemManager)); break;
    case Components::Themes:
    case Components::Overlay:
    {
      mWindow.displayMessage("Not yet available.");
      break;
    }
    default: break;
  }
}
