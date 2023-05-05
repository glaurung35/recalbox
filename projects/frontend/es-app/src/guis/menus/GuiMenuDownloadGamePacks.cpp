//
// Created by bkg2k on 05/05/23.
//

#include "GuiMenuDownloadGamePacks.h"
#include "systems/DownloaderManager.h"
#include "guis/GuiDownloader.h"
#include <systems/SystemManager.h>

GuiMenuDownloadGamePacks::GuiMenuDownloadGamePacks(WindowManager& window, SystemManager& systemManager)
  : GuiMenuBase(window, _("DOWNLOAD CONTENTS"), this)
  , mSystemManager(systemManager)
{
  for(int i = 0; i < (int)systemManager.GetAllSystemList().size(); ++i)
    if (systemManager.GetAllSystemList()[i]->Descriptor().HasDownloader())
      AddSubMenu(systemManager.GetAllSystemList()[i]->FullName(), i);
}

void GuiMenuDownloadGamePacks::SubMenuSelected(int id)
{
  mWindow.pushGui(new GuiDownloader(mWindow, *mSystemManager.GetAllSystemList()[id]));
}
