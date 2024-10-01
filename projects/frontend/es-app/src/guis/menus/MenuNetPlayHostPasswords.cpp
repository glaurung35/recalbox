#include "guis/menus/MenuNetPlayHostPasswords.h"
#include <guis/menus/MenuNetPlayEditPasswords.h>
#include <views/ViewController.h>

MenuNetPlayHostPasswords::MenuNetPlayHostPasswords(WindowManager& window, FileData& game)
  : Menu(window, _("GAME PROTECTION"))
  , mGame(game)
{
}

void MenuNetPlayHostPasswords::BuildMenuItems()
{
  // Use password for players
  AddSwitch(_("USE PLAYER PASSWORD"), RecalboxConf::Instance().GetNetplayPasswordUseForPlayer(), (int)Components::UseForPlayers, this);
  // Use password for viewers
  AddSwitch(_("USE PLAYER PASSWORD"), RecalboxConf::Instance().GetNetplayPasswordUseForPlayer(), (int)Components::UseForViewers, this);

  // Password selection for players
  AddList<int>(_("CHOOSE PLAYER PASSWORD"), (int)Components::LastForPlayers, this, GetPasswords(), RecalboxConf::Instance().GetNetplayPasswordLastForPlayer(), 0);
  // Password selection for viewers
  AddList<int>(_("CHOOSE VIEWER-ONLY PASSWORD"), (int)Components::LastForViewers, this, GetPasswords(), RecalboxConf::Instance().GetNetplayPasswordLastForViewer(), 0);

  AddAction(_("START GAME"), _("RUN"), (int)Components::Start, true, this);
  AddAction(_("CANCEL"), _(""), (int)Components::Cancel, false, this);
  AddAction(_("EDIT PASSWORDS"), _("EDIT"), (int)Components::EditPasswords, true, this);
}

SelectorEntry<int>::List MenuNetPlayHostPasswords::GetPasswords()
{
  SelectorEntry<int>::List list;
  for(int i = 0; i < DefaultPasswords::sPasswordCount; i++)
  {
    String password = RecalboxConf::Instance().GetNetplayPasswords(i);
    if (password.empty()) password = DefaultPasswords::sDefaultPassword[i];
    list.push_back({ password, i });
  }
  return list;
}

void MenuNetPlayHostPasswords::MenuSwitchChanged(int id, bool& state)
{
  switch((Components)id)
  {
    case Components::UseForPlayers: mConfiguration.SetNetplayPasswordUseForPlayer(state).Save(); break;
    case Components::UseForViewers: mConfiguration.SetNetplayPasswordUseForViewer(state).Save(); break;
    case Components::LastForPlayers:
    case Components::LastForViewers:
    case Components::Start:
    case Components::Cancel:
    case Components::EditPasswords:
    default: break;
  }
}

void MenuNetPlayHostPasswords::MenuSingleChanged(int id, int index, const int& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::UseForPlayers:
    case Components::UseForViewers:break;
    case Components::LastForPlayers: mConfiguration.SetNetplayPasswordLastForPlayer(value).Save(); break;
    case Components::LastForViewers: mConfiguration.SetNetplayPasswordLastForViewer(value).Save(); break;
    case Components::Start:
    case Components::Cancel:
    case Components::EditPasswords:
    default: break;
  }
}

void MenuNetPlayHostPasswords::MenuActionTriggered(int id)
{
  switch((Components)id)
  {
    case Components::Start:
    {
      GameLinkedData data(RecalboxConf::Instance().GetNetplayPort(),
                          mConfiguration.GetNetplayPasswords(mConfiguration.GetNetplayPasswordLastForPlayer()),
                          mConfiguration.GetNetplayPasswords(mConfiguration.GetNetplayPasswordLastForViewer()));
      ViewController::Instance().Launch(&mGame, data, Vector3f(), !ViewController::Instance().IsInVirtualSystem());
      break;
    }
    case Components::Cancel: Close(); break;
    case Components::EditPasswords: mWindow.pushGui(new MenuNetPlayEditPasswords(mWindow)); break;
    case Components::UseForPlayers:
    case Components::UseForViewers:
    case Components::LastForPlayers:
    case Components::LastForViewers:
    default: break;
  }
}

