//
// Created by bkg2k on 07/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuNetplay.h"
#include <guis/MenuMessages.h>
#include <utils/locale/LocaleHelper.h>
#include <systems/SystemManager.h>
#include <components/SwitchComponent.h>
#include <guis/GuiNetPlayEditPasswords.h>
#include <guis/GuiArcadeVirtualKeyboard.h>
#include <guis/GuiHashStart.h>
#include <guis/GuiMsgBox.h>

MenuNetplay::MenuNetplay(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("NETPLAY SETTINGS"))
  , mThreadPool(this, "Crc32", false)
  , mSystemManager(systemManager)
  , mTotalGames(0)
  , mRemainingGames(0)
  , mPreviousProgressPercent(0)
  , mOperation(nullptr)
{
  // Netplay Enabled
  AddSwitch(_("NETPLAY"), RecalboxConf::Instance().GetNetplayEnabled(), (int)Components::Enabled, this, _(MENUMESSAGE_NP_ONOFF_HELP_MSG));

  // netplay username
  AddEditable(_("NICKNAME"), RecalboxConf::Instance().GetNetplayLogin(), (int)Components::UserName, this, _(MENUMESSAGE_NP_NICKNAME_HELP_MSG), false);

  // netplay port
  AddEditable(_("PORT"), String(RecalboxConf::Instance().GetNetplayPort()), (int)Components::Port, this, _(MENUMESSAGE_NP_PORT_HELP_MSG), false);

  //mitm
  AddList<RecalboxConf::Relay>(_("NETPLAY MITM"), (int)Components::Mitm, this, GetMitmEntries(), RecalboxConf::Instance().GetNetplayRelay(), RecalboxConf::Relay::None, _(MENUMESSAGE_NP_RELAY_HELP_MSG));

  // Password edition
  AddSubMenu(_("PREDEFINED PASSWORDS"), (int)Components::Passwords, this, _(MENUMESSAGE_NP_PASSWORDS_HELP_MSG));

  // Hash roms
  AddAction(_("HASH ROMS"), _("RUN"),  (int)Components::Hash, true, this, _(MENUMESSAGE_NP_HASH_HELP_MSG));
}

SelectorEntry<RecalboxConf::Relay>::List MenuNetplay::GetMitmEntries()
{
  return SelectorEntry<RecalboxConf::Relay>::List
  {
    { _("NONE"), RecalboxConf::Relay::None },
    { _("NEW YORK"), RecalboxConf::Relay::NewYork },
    { _("MADRID"), RecalboxConf::Relay::Madrid },
    { _("MONTREAL"), RecalboxConf::Relay::Montreal },
    { _("SAOPAULO"), RecalboxConf::Relay::Saopaulo },
  };
}

void MenuNetplay::MenuEditableChanged(int id, const String& text)
{
  switch((Components)id)
  {
    case Components::UserName:
    {
      RecalboxConf::Instance().SetNetplayLogin(text).Save();
      break;
    }
    case Components::Port:
    {
      int p = text.AsInt(0, RecalboxConf::sNetplayDefaultPort);
      if ((unsigned int)p > 65535) p = 65535;
      RecalboxConf::Instance().SetNetplayPort(p).Save();
      break;
    }
    case Components::Enabled:
    case Components::Mitm:
    case Components::Passwords:
    case Components::Hash: break;
  }
}

void MenuNetplay::SubMenuSelected(int id)
{
  if ((Components)id == Components::Passwords) mWindow.pushGui(new GuiNetPlayEditPasswords(mWindow));
}

void MenuNetplay::MenuActionTriggered(int id)
{
  if ((Components)id == Components::Hash) mWindow.pushGui(new GuiHashStart(mWindow, mSystemManager));
}

void MenuNetplay::MenuSingleChanged(int id, int index, const RecalboxConf::Relay& value)
{
  (void)index;
  if ((Components)id == Components::Mitm)
    RecalboxConf::Instance().SetNetplayRelay(value).Save();
}

void MenuNetplay::MenuSwitchChanged(int id, bool& status)
{
  RecalboxConf::Instance().SetNetplayEnabled(status).Save();
  if ((Components)id == Components::Enabled && status)
  {
    bool needHashRefresh = false;
    for(SystemData* system : mSystemManager.AllSystems())
      if (system->Descriptor().HasNetPlayCores())
        if (system->MasterRoot().HasMissingHashRecursively())
        {
          needHashRefresh = true;
          break;
        }
    if (needHashRefresh)
      mWindow.pushGui(new GuiMsgBox(mWindow, _("Some games are not netplay ready yet."), _("OK"), [this] { StartHashing(); }));
  }
}

void MenuNetplay::StartHashing()
{
  mWindow.pushGui((new GuiWaitLongExecution<bool, bool>(mWindow, *this))->Execute(true, _("Preparing Games...")));
}

bool MenuNetplay::Execute(GuiWaitLongExecution<bool, bool>& from, const bool& parameter)
{
  (void)from;
  (void)parameter;

  // Just sleep 2s to make the long operation bar readable :)
  Thread::Sleep(2000);

  // Collect non-hashed roms...
  FileData::List list;
  for(SystemData* system : mSystemManager.AllSystems())
    if (system->Descriptor().HasNetPlayCores())
      system->MasterRoot().getMissingHashRecursively(list);
  // Inject into thread pool
  for(FileData* game : list)
    mThreadPool.PushFeed(game);

  mTotalGames = mThreadPool.PendingJobs();
  mRemainingGames = mTotalGames;

  // Run!
  mOperation = &from;
  if (mRemainingGames != 0) mThreadPool.Run(-4, true);
  mWaiter.WaitSignal();
  mOperation = nullptr;

  return true;
}

FileData* MenuNetplay::ThreadPoolRunJob(FileData*& feed)
{
  feed->CalculateHash();
  {
    Mutex::AutoLock locker(mSyncher);
    // Update
    if (--mRemainingGames <= 0)
      mWaiter.Fire();
    else
    {
      int percent = 100 - (mRemainingGames * 100) / mTotalGames;
      if (percent != mPreviousProgressPercent)
      {
        mPreviousProgressPercent = percent;
        mOperation->SetText(_("Preparing Games...").Append(percent).Append('%'));
      }
    }
  }
  return feed;
}


