//
// Created by xizor on 01/06/18.
//

#include <RecalboxConf.h>
#include <recalbox/RecalboxSystem.h>
#include <guis/GuiMsgBox.h>
#include <systems/SystemManager.h>
#include <utils/Zip.h>
#include "GuiHashStart.h"
#include "components/OptionListComponent.h"

GuiHashStart::GuiHashStart(WindowManager& window, SystemManager& systemManager)
  : Gui(window),
    mSystemManager(systemManager),
    mBusyAnim(window),
    mMenu(window, _("HASH NOW")),
    mState(State::Wait),
    mThreadPool(this, "Crc32", false),
    mTotalGames(0),
    mRemaininglGames(0)
{
  addChild(&mMenu); // #TODO: Rewrite using MenuBase

  mBusyAnim.setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());

  mFilter = std::make_shared<OptionListComponent<String> >(mWindow, _("FILTER"), false);
  mFilter->add(_("Only missing hashs"), "missing", true);
  mFilter->add(_("All Games"), "all", false);
  mMenu.addWithLabel(mFilter, _("FILTER"), String::Empty);

  // add systems (all with a platformid specified selected)
  mSystems = std::make_shared<OptionListComponent<SystemData*> >(mWindow, _("HASH THESE SYSTEMS"), true);
  for (auto* it : mSystemManager.VisibleSystemList())
  {
    if (it->Descriptor().HasNetPlayCores())
      mSystems->add(it->FullName(), it, true);
  }
  mMenu.addWithLabel(mSystems, _("SYSTEMS"), String::Empty);

  mMenu.addButton(_("START"), "start", [this]
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, _("THIS COULD TAKE A WHILE, CONFIRM?"), _("YES"), [this]
    {
      mState = State::Hashing;
      Start();
    }, _("NO"), [this] { mState = State::Exit; }));
  });
  mMenu.addButton(_("BACK"), "back", [this]
  { mState = State::Cancelled; });

  mMenu.setPosition((Renderer::Instance().DisplayWidthAsFloat() - mMenu.getSize().x()) / 2,
                    (Renderer::Instance().DisplayHeightAsFloat() - mMenu.getSize().y()) / 2);
}

bool GuiHashStart::ProcessInput(const InputCompactEvent& event)
{
  // Cancel or exit immediatelly on pressing A
  if (event.CancelReleased())
    mState = State::Cancelled;

  return Component::ProcessInput(event);
}

void GuiHashStart::Update(int deltaTime)
{
  // Avoid sleeping!
  mIsProcessing = true;

  switch(mState)
  {
    case State::Wait:
    {
      Component::Update(deltaTime);
      break;
    }
    case State::Hashing:
    {
      // Read summary text
      mMutex.Lock();
      String text = mSummaryText;
      mMutex.UnLock();

      mBusyAnim.setText(text);
      mBusyAnim.Update(deltaTime);
      break;
    }
    case State::Cancelled:
    {
      if (mRemaininglGames == 0)
        mState = State::Exit;
      break;
    }
    case State::Exit: break;
  }

  if (mState == State::Exit)
    Quit();
}

void GuiHashStart::Render(const Transform4x4f& parentTrans)
{
  Transform4x4f trans = parentTrans * getTransform();

  if (mState != State::Hashing)
    renderChildren(trans);

  Renderer::SetMatrix(trans);
  Renderer::DrawRectangle(0.f, 0.f, mSize.x(), mSize.y(), 0x00000011);

  if (mState == State::Hashing)
    mBusyAnim.Render(trans);
}

bool GuiHashStart::CollectHelpItems(Help& help)
{
  mMenu.CollectHelpItems(help);
  help.Set(Help::Cancel(), _("BACK"));
  return true;
}

void GuiHashStart::Start()
{
  bool forceAll = mFilter->getSelected() == "all";

  // Run through systems...
  for (auto* system : mSystems->getSelectedObjects())
    for(const RootFolderData* root : system->MasterRoot().SubRoots())
      if (!root->ReadOnly())
      {
        // Run through games
        FileData::List games = root->GetAllItems(true, system->Excludes());
        for (FileData* game : games)
          if (game->Metadata().RomCrc32() == 0 || forceAll)
            mThreadPool.PushFeed(game);
      }
  mTotalGames = mThreadPool.PendingJobs();
  mRemaininglGames = mTotalGames;

  // Run!
  if (mRemaininglGames != 0) mThreadPool.Run(-4, true);
  else mState = State::Exit;
}

FileData* GuiHashStart::ThreadPoolRunJob(FileData*& feed)
{
  if (mState == State::Hashing)
  {
    feed->CalculateHash();

    String busyText(feed->System().FullName());
    busyText.Append(' ')
            .Append(mTotalGames - mRemaininglGames)
            .Append('/')
            .Append(mTotalGames);

    // Write summary text
    mMutex.Lock();
    mSummaryText = busyText;
    mMutex.UnLock();
  }

  // Processed (or cancelled)
  mMutex.Lock();
  if (--mRemaininglGames == 0)
    mState = State::Exit;
  mMutex.UnLock();

  return feed;
}

void GuiHashStart::Quit()
{
  String message = (mTotalGames == 0) ? _("No missing hash found!") : _("%i missing hashes have been calculated!");
  message.Replace("%i", String(mTotalGames));
  mWindow.displayMessage(message);
  Close();
}

