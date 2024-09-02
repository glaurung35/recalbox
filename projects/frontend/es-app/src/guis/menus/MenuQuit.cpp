//
// Created by bkg2k on 12/02/2020.
//

#include <hardware/Case.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/menus/MenuQuit.h>
#include <guis/GuiMsgBox.h>
#include <MainRunner.h>


MenuQuit::MenuQuit(WindowManager& window)
  : Menu(window, _("QUIT"))
{
  if (Case::CurrentCase().CanShutdownFromMenu())
  {
    // Shutdown
    AddSubMenu(_("SHUTDOWN SYSTEM"), MenuThemeData::MenuIcons::Type::Shutdown, (int)Components::Shutdown, this,
               _("Shutdown Recalbox. All pending operations are completed before Recalbox is switched off"));

    // Fast Shutdown
    AddSubMenu(_("FAST SHUTDOWN SYSTEM"), MenuThemeData::MenuIcons::Type::Fastshutdown, (int)Components::FastShutdown, this,
               _("Quickly shutdown Recalbox. All pending operations are ignored and no change is saved!"));
  }

  // Reboot
  AddSubMenu(_("RESTART SYSTEM"), MenuThemeData::MenuIcons::Type::Restart, (int)Components::Reboot, this,
             _("Reboot Recalbox. All pending operations are completed before Recalbox restarts"));
}

void MenuQuit::PushQuitGui(WindowManager& window)
{
  window.pushGui(new MenuQuit(window));
}

void MenuQuit::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Shutdown:
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("REALLY SHUTDOWN?"), _("YES"),
                                        [] { MainRunner::RequestQuit(MainRunner::ExitState::Shutdown); }, _("NO"), nullptr));
      break;
    }
    case Components::FastShutdown:
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("REALLY SHUTDOWN WITHOUT SAVING METADATAS?"), _("YES"),
                                        [] { MainRunner::RequestQuit(MainRunner::ExitState::FastShutdown); }, _("NO"), nullptr));
      break;
    }
    case Components::Reboot:
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("REALLY RESTART?"), _("YES"),
                                        [] { MainRunner::RequestQuit(MainRunner::ExitState::NormalReboot); }, _("NO"), nullptr));
      break;
    }
  }
}
