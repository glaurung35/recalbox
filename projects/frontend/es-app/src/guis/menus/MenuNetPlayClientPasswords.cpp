#include <guis/menus/MenuNetPlayClientPasswords.h>
#include <emulators/run/NetPlayData.h>
#include <guis/menus/MenuNetPlayEditPasswords.h>
#include <views/ViewController.h>

MenuNetPlayClientPasswords::MenuNetPlayClientPasswords(WindowManager& window, LobbyGame& lobby)
  : Menu(window, _("JOIN NETPLAY GAME")),
    mLobbyGame(lobby)
{
  // Password type
  AddList<PasswordType>(_("JOIN AS"), (int)Components::JoinAs, nullptr,
                        { { _("PLAYER"), PasswordType::Player }, { _("VIEWER-ONLY"), PasswordType::Viewer } }, PasswordType::Player, PasswordType::Player);

  // Password selection for players
  AddList<int>(_("CHOOSE PLAYER PASSWORD"), (int)Components::Password, this, GetPasswords(), mConfiguration.GetNetplayPasswordClient(), -1);

  AddAction(_("JOIN GAME"), _("RUN"), (int)Components::Start, true, this);
  AddAction(_("CANCEL"), _(""), (int)Components::Cancel, false, this);
  AddAction(_("EDIT PASSWORDS"), _("EDIT"), (int)Components::Cancel, true, this);
}

SelectorEntry<int>::List MenuNetPlayClientPasswords::GetPasswords()
{
  SelectorEntry<int>::List list { { _("NONE"), -1 } };
  for(int i = 0; i < DefaultPasswords::sPasswordCount; i++)
  {
    String password = RecalboxConf::Instance().GetNetplayPasswords(i);
    if (password.empty()) password = DefaultPasswords::sDefaultPassword[i];
    list.push_back({ password, i });
  }
  return list;
}

void MenuNetPlayClientPasswords::MenuSingleChanged(int id, int index, const int& value)
{
  switch((Components)id)
  {
    case Components::Password: mConfiguration.SetNetplayPasswordClient(value).Save(); break;
    case Components::JoinAs:
    case Components::Start:
    case Components::Cancel:
    case Components::EditPasswords:
    default: break;
  }
}

void MenuNetPlayClientPasswords::MenuActionTriggered(int id)
{
  switch((Components)id)
  {
    case Components::Start:
    {
      // Run game
      if(mLobbyGame.mGame->IsGame())
      {
        String playerPassword, viewerPassword;
        PasswordType pt = AsList<PasswordType>((int)Components::JoinAs)->SelectedValue();
        if (int passwordIndex = AsList<int>((int)Components::Password)->SelectedValue(); passwordIndex >= 0)
          switch(pt)
          {
            case PasswordType::Player: playerPassword = mConfiguration.GetNetplayPasswords(passwordIndex); break;
            case PasswordType::Viewer: viewerPassword = mConfiguration.GetNetplayPasswords(passwordIndex); break;
          }

        bool mitm = mLobbyGame.mHostMethod == 3;
        String& ip = mitm ? mLobbyGame.mMitmIp : mLobbyGame.mIp;
        int port = mitm ? mLobbyGame.mMitmPort : mLobbyGame.mPort;

        GameLinkedData data(mLobbyGame.mCoreShortName, ip, port, playerPassword, viewerPassword, pt == PasswordType::Viewer);
        ViewController::Instance().Launch(mLobbyGame.mGame, data, Vector3f(), !ViewController::Instance().IsInVirtualSystem());
      }
      break;
    }
    case Components::Cancel: Close(); break;
    case Components::EditPasswords: mWindow.pushGui(new MenuNetPlayEditPasswords(mWindow)); break;
    case Components::JoinAs:
    case Components::Password:
    default: break;
  }
}
