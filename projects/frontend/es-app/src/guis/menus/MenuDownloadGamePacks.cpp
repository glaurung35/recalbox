//
// Created by bkg2k on 05/05/23.
//

#include "MenuDownloadGamePacks.h"
#include "systems/DownloaderManager.h"
#include "guis/GuiDownloader.h"
#include "systems/SystemDeserializer.h"
#include <systems/SystemManager.h>

MenuDownloadGamePacks::MenuDownloadGamePacks(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("DOWNLOAD CONTENTS"))
  , mSystemManager(systemManager)
{
}

void MenuDownloadGamePacks::BuildMenuItems()
{
  SystemDeserializer deserializer;
  bool loaded = deserializer.LoadSystems();

  if (loaded)
    for (int index = 0; index < deserializer.Count(); ++index)
      if (SystemDescriptor descriptor; deserializer.Deserialize(index, descriptor))
        if (descriptor.HasDownloader())
        {
          AddAction(descriptor.FullName(), _("DOWNLOAD"), (int)mDescriptors.size(), true, this);
          mDescriptors.push_back(descriptor);
        }
}

void MenuDownloadGamePacks::MenuActionTriggered(int id)
{
  SystemData& target = mSystemManager.GetOrCreateSystem(mDescriptors[id]);
  mWindow.pushGui(new GuiDownloader(mWindow, target, mSystemManager));
}
