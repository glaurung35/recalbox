//
// Created by bkg2k on 19/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/MenuSwitchKodiNetplay.h>
#include <guis/GuiNetPlay.h>
#include <utils/locale/LocaleHelper.h>
#include <systems/SystemManager.h>
#include <emulators/run/GameRunner.h>

MenuSwitchKodiNetplay::MenuSwitchKodiNetplay(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("KODI/NETPLAY"))
  , mSystemManager(systemManager)
{
}

void MenuSwitchKodiNetplay::BuildMenuItems()
{
  // Start kodi
  AddSubMenu("\uF1c3 " + _("KODI MEDIA CENTER"), (int)Components::Kodi, this);

  // Start Netplay
  AddSubMenu("\uF1c4 " + _("NETPLAY LOBBY"), (int)Components::Netplay, this);
}

void MenuSwitchKodiNetplay::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Kodi:
    {
      if (!GameRunner::Instance().RunKodi())
      { LOG(LogWarning) << "[Kodi] Kodi terminated with non-zero result!"; }
      break;
    }
    case Components::Netplay:
    {
      mWindow.pushGui(new GuiNetPlay(mWindow, mSystemManager));
      Close();
      break;
    }
  }
}
