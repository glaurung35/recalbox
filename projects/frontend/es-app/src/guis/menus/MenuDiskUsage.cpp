//
// Created by bkg2k on 26/03/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuDiskUsage.h"
#include <guis/menus/base/ItemBar.h>

MenuDiskUsage::MenuDiskUsage(WindowManager& window, MountMonitor& mountMonitor)
  : Menu(window, _("DISK USAGE (FREE/TOTAL)"))
  , mMountMonitor(mountMonitor)
{
}

void MenuDiskUsage::BuildMenuItems()
{
  for(DeviceMount* device : mMountMonitor.AllMountPoints())
  {
    device->UpdateSize();
    AddBar(device->DisplayableDeviceName(), device->DisplayableFreeSpace(),
           device->TotalSize() != 0 ? 1.f - (float)device->FreeSize() / (float)device->TotalSize() : 0);
  }
}
