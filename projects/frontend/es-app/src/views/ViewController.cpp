#include <RecalboxConf.h>
#include "systems/SystemData.h"
#include "views/ViewController.h"

#include "views/gamelist/ArcadeGameListView.h"
#include "guis/GuiDetectDevice.h"
#include "animations/LaunchAnimation.h"
#include "animations/MoveCameraAnimation.h"
#include "animations/LambdaAnimation.h"

#include "guis/menus/GuiMenuSoftpatchingLauncher.h"
#include "RotationManager.h"
#include "guis/GuiSaveStates.h"
#include "guis/menus/GuiFastMenuList.h"

#include <MainRunner.h>
#include <bios/BiosManager.h>
#include <guis/GuiMsgBox.h>

#include <games/GameFilesUtils.h>

ViewController::ViewController(WindowManager& window, SystemManager& systemManager)
	: StaticLifeCycleControler<ViewController>("ViewController")
	, Gui(window)
  , mNoView(window)
  , mGameToLaunch(nullptr)
  , mLaunchCameraTarget()
  , mCheckFlags(LaunchCheckFlags::None)
  , mForceGoToGame(false)
	, mSystemManager(systemManager)
	, mCurrentView(&mSplashView)
  , mPreviousView(&mSplashView)
  , mCurrentSystem(nullptr)
	, mSystemListView(window, systemManager)
	, mSplashView(window)
	, mGameClipView(window, systemManager)
  , mCrtView(window)
	, mCamera(Transform4x4f::Identity())
	, mFadeOpacity(0)
	, mLockInput(false)
  , mLastGameLaunched(0LL)
  , mFrequencyLastChoiceMulti60(0)
  , mFrequencyLastChoice(0)
  , mResolutionLastChoice(0)
  , mSuperGameboyLastChoice(0)
  , mSoftPatchingLastChoice(0)
  , mSender(*this)
  , mNextItem(nullptr)
  , mTransition(Transition::None)
{
  // Set interfaces
  systemManager.SetProgressInterface(&mSplashView);
  systemManager.SetLoadingPhaseInterface(&mSplashView);
  systemManager.SetChangeNotifierInterface(this);

  // Splash
  mSplashView.setPosition(0,0);

  // default view mode
  ChangeView(IView::ViewType::SplashScreen, nullptr, Transition::None);

  //! Centralized thread that process slow information for gamelists
  Thread::Start("gamelist-slow");
}

ViewController::~ViewController()
{
  // Stop gamelist thread
  Thread::Stop();

  for(const auto& view : mGameListViews)
    delete view.second;
}

void ViewController::goToStart()
{
  CheckFilters();

  String systemName = RecalboxConf::Instance().GetStartupSelectedSystem();
  int index = systemName.empty() ? -1 : mSystemManager.getVisibleSystemIndex(systemName);
  SystemData* selectedSystem = index < 0 ? nullptr : mSystemManager.VisibleSystemList()[index];

  if ((selectedSystem == nullptr) || !selectedSystem->HasVisibleGame())
    selectedSystem = mSystemManager.FirstNonEmptySystem();

  if (selectedSystem == nullptr)
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, "Your filters preferences hide all your games !\nThe filters will be reseted and recalbox will be reloaded.", _("OK"), [] { ResetFilters();}));
    return;
  }

  if (RecalboxConf::Instance().GetStartupHideSystemView())
    goToGameList(selectedSystem);
  else
  {
    if (RecalboxConf::Instance().GetStartupStartOnGamelist())
      goToGameList(selectedSystem);
    else
    {
      mSystemListView.SetProgressInterface(&mSplashView);
      mSystemListView.populate();
      mSystemListView.SetProgressInterface(nullptr);
      goToSystemView(selectedSystem);
    }
  }
}

bool ViewController::CheckFilters()
{
  if(mSystemManager.FirstNonEmptySystem() == nullptr)
  {
    ResetFilters();
    mWindow.pushGui(new GuiMsgBox(mWindow, "Your filters preferences hide all your games !\nAll filters have been reseted.", _("OK"), []{}));
    return false;
  }
  return true;
}

void ViewController::ResetFilters()
{
  RecalboxConf& conf = RecalboxConf::Instance();

  conf.SetShowOnlyLatestVersion(false);
  conf.SetFavoritesOnly(false);
  conf.SetGlobalHidePreinstalled(false);
  conf.SetHideNoGames(false);
  conf.SetFilterAdultGames(false);

  conf.SetCollectionPorts(true);
}

void ViewController::goToQuitScreen()
{
  mSplashView.Quit();
  ChangeView(IView::ViewType::SplashScreen, nullptr, Transition::None);
  mCamera.translation().Set(0,0,0);
}

void ViewController::goToSystemView(SystemData* system, Transition forcedTransition)
{
  CheckFilters();
  mSystemListView.setPosition((float)mSystemManager.SystemAbsoluteIndex(system) * Renderer::Instance().DisplayWidthAsFloat(), mSystemListView.getPosition().y());

  if (!system->HasVisibleGame()) {
    system = mSystemManager.FirstNonEmptySystem();
  }

  ChangeView(IView::ViewType::SystemList, system, forcedTransition);

  NotificationManager::Instance().Notify(*system, Notification::SystemBrowsing);
}

void ViewController::goToGameClipView()
{
  if (isViewing(IView::ViewType::GameClip)) return; // #TODO: avoid ths method being called in loop

  ChangeView(IView::ViewType::GameClip, nullptr, Transition::None);
  NotificationManager::Instance().Notify(Notification::StartGameClip);
  mGameClipView.Reset();
}

void ViewController::goToCrtView(CrtView::CalibrationType screenType)
{
  ChangeView(IView::ViewType::CrtCalibration, nullptr, Transition::None);
  mCrtView.Initialize(screenType);
}

void ViewController::selectGamelistAndCursor(FileData *file)
{
  SystemData& system = file->System();
  goToGameList(&system);
  ISimpleGameListView* view = GetOrCreateGamelistView(&system);
  view->setCursorStack(file);
  view->setCursor(file);
}

SystemData* ViewController::goToNextGameList()
{
	assert(mCurrentView->GetViewType() == IView::ViewType::GameList);
	SystemData* system = mCurrentSystem;
	assert(system);

  CheckFilters();
  SystemData* next = mSystemManager.NextVisible(system);
	while(!next->HasVisibleGame())
    next = mSystemManager.NextVisible(next);

  AudioManager::Instance().StartPlaying(next->Theme());

	goToGameList(next, Transition::Rightward);
  return next;
}

void ViewController::goToPrevGameList()
{
	assert(mCurrentView->GetViewType() == IView::ViewType::GameList);
	SystemData* system = mCurrentSystem;
	assert(system);

  CheckFilters();
  SystemData* prev = mSystemManager.PreviousVisible(system);
	while(!prev->HasVisibleGame()) {
		prev = mSystemManager.PreviousVisible(prev);
	}

  AudioManager::Instance().StartPlaying(prev->Theme());

	goToGameList(prev, Transition::Leftward);
}

void ViewController::goToGameList(SystemData* system, Transition forcedTransition)
{
	RotationType rotation = RotationType::None;
	if(system->Rotatable() && RotationManager::ShouldRotateTateEnter(rotation))
	{
		mWindow.Rotate(rotation);
	}
	/*if (mCurrentView->GetViewType() != IView::ViewType::GameList)
	{
		// move system list
		int sysId = mSystemManager.SystemAbsoluteIndex(system);
    mSystemListView.setPosition((float)sysId * Renderer::Instance().DisplayWidthAsFloat(), mSystemListView.getPosition().y());
    mCamera.translation().x() = -mSystemListView.getPosition().x();
  }*/

	if (mInvalidGameList[system])
	{
		mInvalidGameList[system] = false;
    if (!GetOrReCreateGamelistView(system))
    {
      // if listview has been reload due to last game has been deleted,
      // we have to stop the previous goToGameList process because current
      // system will no longer exists in the available list
      return;
    }
	}

  ChangeView(IView::ViewType::GameList, system, forcedTransition);

  NotificationManager::Instance().Notify(*GetOrCreateGamelistView(system)->getCursor(), Notification::GamelistBrowsing);
  // for reload cursor video path if present
  GetOrCreateGamelistView(system)->DoUpdateGameInformation(false);
}

void ViewController::PlayViewTransition(int elapsed)
{
  static constexpr float sTiming = 5000.f;
  mTransitionElapsedTime += elapsed;
  float factor = Math::clamp((float)mTransitionElapsedTime, 0.f, sTiming) / sTiming;
  switch(mTransition)
  {
    case Transition::Upward:
    {
      float offset = mPreviousView->GetGui().getSize().y() * factor;
      mPreviousView->GetGui().setPosition(0, -offset);
      mCurrentView->GetGui().setPosition(0, mPreviousView->GetGui().getSize().y() - offset);
      break;
    }
    case Transition::Downward:
    {
      float offset = mPreviousView->GetGui().getSize().y() * factor;
      mPreviousView->GetGui().setPosition(0, offset);
      mCurrentView->GetGui().setPosition(0, -mCurrentView->GetGui().getSize().y() + offset);
      break;
    }
    case Transition::Leftward:
    {
      float offset = mPreviousView->GetGui().getSize().x() * factor;
      mPreviousView->GetGui().setPosition(offset, 0);
      mCurrentView->GetGui().setPosition(-mCurrentView->GetGui().getSize().x() + offset, 0);
      break;
    }
    case Transition::Rightward:
    {
      float offset = mPreviousView->GetGui().getSize().x() * factor;
      mPreviousView->GetGui().setPosition(-offset, 0);
      mCurrentView->GetGui().setPosition(mPreviousView->GetGui().getSize().x() - offset, 0);
      break;
    }
    case Transition::Fade:
    {
      factor *= 2.f;
      if (factor > 1.f) factor = 2.f - factor;
      mFadeOpacity = factor;
      break;
    }
    case Transition::Instant:
    {
      mTransition = Transition::None;
      break;
    }
    case Transition::None:
    default:break;
  }
  if (mTransitionElapsedTime >= 500)
    mTransition = Transition::None;
}

void ViewController::InitializeViewTransition(ViewController::Transition forcedTransition)
{
  IView* source = mPreviousView;
  IView* destination = mCurrentView;
  Transition transition = Transition::None;

  switch(IView::ViewType from = source->GetViewType(); from)
  {
    case IView::ViewType::None:break;
    case IView::ViewType::SplashScreen:
    {
      if (destination->GetViewType() == IView::ViewType::SplashScreen) return; // Setup: no transition
      transition = Transition::Upward;
      break;
    }
    case IView::ViewType::SystemList:
    case IView::ViewType::GameList:
    {
      switch(destination->GetViewType())
      {
        case IView::ViewType::None: return; // Hu?!
        case IView::ViewType::SplashScreen:
        {
          transition = Transition::Downward;
          break;
        }
        case IView::ViewType::SystemList:
        {
          if (from == IView::ViewType::SystemList) transition = forcedTransition; // Syslist => syslist = nope
          else transition = Transition::Downward;
          break;
        }
        case IView::ViewType::GameList:
        {
          if (from == IView::ViewType::SystemList) transition = Transition::Upward;
          else
          {
            // Both are Gamelist views, check indexes
            int indexFrom = mSystemManager.SystemAbsoluteIndex(&((ISimpleGameListView*) source)->System());
            int indexTo = mSystemManager.SystemAbsoluteIndex(&((ISimpleGameListView*) destination)->System());
            transition = indexFrom < indexTo ? Transition::Leftward : Transition::Rightward;
          }
          break;
        }
        case IView::ViewType::GameClip:
        case IView::ViewType::CrtCalibration:
        {
          // Entering Gameclip or CRT Calibration makes them being shown immediately
          transition = Transition::Instant;
          break;
        }
      }
      break;
    }
    case IView::ViewType::GameClip:
    case IView::ViewType::CrtCalibration:
    {
      // Exiting from Gameclip or CRT Calibration makes the previous view to be shown immediately
      transition = Transition::Instant;
      break;
    }
  }

  mTransition = transition;
  mTransitionElapsedTime = 0;
  switch(transition)
  {
    case Transition::Upward:
    {
      mPreviousView->GetGui().setPosition(0, 0);
      mCurrentView->GetGui().setPosition(0, mPreviousView->GetGui().getSize().y());
      break;
    }
    case Transition::Downward:
    {
      mPreviousView->GetGui().setPosition(0, 0);
      mCurrentView->GetGui().setPosition(0, -mPreviousView->GetGui().getSize().y());
      break;
    }
    case Transition::Leftward:
    {
      mPreviousView->GetGui().setPosition(0, 0);
      mCurrentView->GetGui().setPosition(-mPreviousView->GetGui().getSize().y(), 0);
      break;
    }
    case Transition::Rightward:
    {
      mPreviousView->GetGui().setPosition(0, 0);
      mCurrentView->GetGui().setPosition(mPreviousView->GetGui().getSize().y(), 0);
      break;
    }
    case Transition::Fade:
    {
      mCurrentView->GetGui().setPosition(0, 0);
      break;
    }
    case Transition::Instant:
    case Transition::None:
    default:
    {
      mCurrentView->GetGui().setPosition(0, 0);
      mTransition = Transition::None;
      break;
    }
  }
}

void ViewController::Launch(FileData* game, const GameLinkedData& data, const Vector3f& cameraTarget, bool forceGoToGame)
{
  // Avoid launch repeat
  DateTime now;
  if ((now - mLastGameLaunched).TotalMilliseconds() < 2000) return;
  mLastGameLaunched = now;

  if (!game->IsGame())
  {
    { LOG(LogError) << "[ViewController] Tried to launch something that isn't a game"; }
    return;
  }

  mGameLinkedData = data;
  mGameToLaunch = game;
  mLaunchCameraTarget = cameraTarget;
  mCheckFlags = LaunchCheckFlags::None;
  mForceGoToGame = forceGoToGame;
  LaunchCheck();
}

bool ViewController::CheckBiosBeforeLaunch()
{
  const BiosList& biosList = BiosManager::Instance().SystemBios(mGameToLaunch->System().Name());
  if (biosList.TotalBiosKo() != 0)
  {
    // Build emulator name
    EmulatorData emulator = EmulatorManager::GetGameEmulator(*mGameToLaunch);
    String emulatorString = emulator.Emulator();
    if (emulator.Emulator() != emulator.Core()) emulatorString.Append('/').Append(emulator.Core());
    // Build text
    String text = _("At least one mandatory BIOS is missing for %emulator%!\nYour game '%game%' will very likely not run at all until required BIOS are put in the expected folder.\n\nDo you want to launch the game anyway?")
                  .Replace("%emulator%", emulatorString)
                  .Replace("%game%", mGameToLaunch->Name());
    // Show the dialog box
    Gui* gui = new GuiMsgBox(mWindow, text, _("YES"), [this] { LaunchCheck(); }, _("NO"), nullptr);
    mWindow.pushGui(gui);
    return false;
  }
  return true;
}

void ViewController::FastMenuLineSelected(int menuIndex, int itemIndex)
{
  switch((FastMenuType)menuIndex)
  {
    case FastMenuType::Frequencies:
    {
      static CrtData::CrtVideoStandard videoStandards[] =
      {
        CrtData::CrtVideoStandard::AUTO,
        CrtData::CrtVideoStandard::NTSC,
        CrtData::CrtVideoStandard::NTSC,
        CrtData::CrtVideoStandard::PAL,
      };
      mGameLinkedData.ConfigurableCrt().ConfigureRegion(CrtData::CrtRegion::AUTO);
      if ((unsigned int)itemIndex < sizeof(videoStandards))
        mGameLinkedData.ConfigurableCrt().ConfigureVideoStandard(videoStandards[itemIndex]);
      else { LOG(LogError) << "[ViewController] Unprocessed fast menu item!"; return; }
      LaunchCheck();
      mFrequencyLastChoice = itemIndex;
      break;
    }
    case FastMenuType::FrequenciesMulti60:
    {
      static CrtData::CrtRegion regions[] =
      {
        CrtData::CrtRegion::AUTO,
        CrtData::CrtRegion::US,
        CrtData::CrtRegion::JP,
        CrtData::CrtRegion::EU,
      };
      static CrtData::CrtVideoStandard videoStandards[sizeof(regions)] =
      {
        CrtData::CrtVideoStandard::AUTO,
        CrtData::CrtVideoStandard::NTSC,
        CrtData::CrtVideoStandard::NTSC,
        CrtData::CrtVideoStandard::PAL,
      };
      if ((unsigned int)itemIndex < sizeof(regions))
      {
        mGameLinkedData.ConfigurableCrt().ConfigureRegion(regions[itemIndex]);
        mGameLinkedData.ConfigurableCrt().ConfigureVideoStandard(videoStandards[itemIndex]);
      }
      else { LOG(LogError) << "[ViewController] Unprocessed fast menu item!"; return; }
      LaunchCheck();
      mFrequencyLastChoiceMulti60 = itemIndex;
      break;
    }
    case FastMenuType::CrtResolution:
    {
      mGameLinkedData.ConfigurableCrt().ConfigureHighResolution(itemIndex != 0);
      LaunchCheck();
      mResolutionLastChoice = itemIndex;
      break;
    }
    case FastMenuType::SuperGameboy:
    {
      mGameLinkedData.ConfigurableSuperGameBoy().Enable(itemIndex != 0);
      LaunchCheck();
      mSuperGameboyLastChoice = itemIndex;
    }
  }
}

void ViewController::SaveStateSlotSelected(int slot)
{
  mGameLinkedData.ConfigurableSaveState().SetSlotNumber(slot);
  LaunchCheck();
}

void ViewController::SoftPathingDisabled()
{
  mGameLinkedData.ConfigurablePatch().SetDisabledSoftPatching(true);
  mSoftPatchingLastChoice = 0;
  LaunchCheck();
}

void ViewController::SoftPatchingSelected(const Path& path)
{
  mGameLinkedData.ConfigurablePatch().SetPatchPath(path);
  mSoftPatchingLastChoice = 1;
  LaunchCheck();
}

bool ViewController::CheckSoftPatching(const EmulatorData& emulator)
{
  if (bool coreIsSoftpatching = mGameToLaunch->System().Descriptor().IsSoftpatching(emulator.Emulator(), emulator.Core()); coreIsSoftpatching)
    switch(RecalboxConf::Instance().GetGlobalSoftpatching())
    {
      case RecalboxConf::SoftPatching::Auto:
      {
        if (!GameFilesUtils::HasAutoPatch(mGameToLaunch))
        {
          Path priorityPath = GameFilesUtils::GetSubDirPriorityPatch(mGameToLaunch);
          if (!priorityPath.IsEmpty() && priorityPath.Exists())
            mGameLinkedData.ConfigurablePatch().SetPatchPath(priorityPath);
        }
        break;
      }
      case RecalboxConf::SoftPatching::LaunchLast:
      {
        Path lastPatchPath = mGameToLaunch->Metadata().LastPatch();

        if (lastPatchPath.Exists())
        {
          mGameLinkedData.ConfigurablePatch().SetPatchPath(lastPatchPath);
          break;
        }
        else if (lastPatchPath.Filename() == "original")
        {
          mGameLinkedData.ConfigurablePatch().SetDisabledSoftPatching(true);
          break;
        }
        // if no patch are configured yet go to next SoftPatching::Select case
        [[fallthrough]];
      }
      case RecalboxConf::SoftPatching::Select:
      {
        std::vector<Path> patches = GameFilesUtils::GetSoftPatches(mGameToLaunch);
        if (!mGameLinkedData.ConfigurablePatch().IsConfigured() && !patches.empty())
        {
          mWindow.pushGui(new GuiMenuSoftpatchingLauncher(mWindow, *mGameToLaunch, std::move(patches), mSoftPatchingLastChoice, this));
          return true;
        }
        break;
      }
      case RecalboxConf::SoftPatching::Disable:
      default:
      {
        mGameLinkedData.ConfigurablePatch().SetDisabledSoftPatching(true);
        break;
      }
    }

  // No soft patching
  return false;
}

void ViewController::LaunchCheck()
{
  EmulatorData emulator = EmulatorManager::GetGameEmulator(*mGameToLaunch);
  if (!emulator.IsValid())
  {
    {
      LOG(LogError) << "[ViewController] Empty emulator/core when running " << mGameToLaunch->RomPath().ToString()
                    << '!';
    }
    return;
  }

  // Check bios
  if ((mCheckFlags & LaunchCheckFlags::Bios) == 0)
    if (mCheckFlags |= LaunchCheckFlags::Bios; !CheckBiosBeforeLaunch())
      return;

  // Refresh rate choice
  if ((mCheckFlags & LaunchCheckFlags::Frequency) == 0)
    if (mCheckFlags |= LaunchCheckFlags::Frequency; mGameLinkedData.Crt().IsRegionOrStandardConfigured())
      if (mGameLinkedData.Crt().MustChoosePALorNTSC(mGameToLaunch->System()))
      {
        if (mGameToLaunch->System().Name() == "megadrive")
          mWindow.pushGui(new GuiFastMenuList(mWindow, this, _("Game refresh rate"), mGameToLaunch->Name(),
                                              (int) FastMenuType::FrequenciesMulti60,
                                              {{_("AUTO")},
                                               {_("60Hz (US)")},
                                               {_("60Hz (JP)")},
                                               {_("50Hz (EU)")}}, mFrequencyLastChoiceMulti60));
        else
          mWindow.pushGui(new GuiFastMenuList(mWindow, this, _("Game refresh rate"), mGameToLaunch->Name(),
                                              (int) FastMenuType::Frequencies,
                                              {{_("AUTO")},
                                               {_("60Hz")},
                                               {_("50Hz")}}, mFrequencyLastChoice));
        return;
      }

  // CRT Resolution choice
  if ((mCheckFlags & LaunchCheckFlags::CrtResolution) == 0)
  {
    if (mCheckFlags |= LaunchCheckFlags::CrtResolution; mGameLinkedData.Crt().IsResolutionSelectionConfigured())
    {
      const bool is31kHz = Board::Instance().CrtBoard().GetHorizontalFrequency() ==
                           ICrtInterface::HorizontalFrequency::KHz31;
      const bool supports120Hz = Board::Instance().CrtBoard().Has120HzSupport();
      const bool isMultiSync = Board::Instance().CrtBoard().MultiSyncEnabled();
      if (mGameLinkedData.Crt().MustChooseHighResolution(mGameToLaunch->System()))
      {
        mWindow.pushGui(new GuiFastMenuList(mWindow, this, _("Game resolution"), mGameToLaunch->Name(),
                                            (int) FastMenuType::CrtResolution,
                                            {{(is31kHz && supports120Hz) ? "240p@120" : "240p"},
                                             {(is31kHz || isMultiSync) ? "480p" : "480i"}}, mResolutionLastChoice));
        return;
      }
    }
    else
    {
      mGameLinkedData.ConfigurableCrt().AutoConfigureHighResolution(mGameToLaunch->System());
    }
  }


  if ((mCheckFlags & LaunchCheckFlags::SoftPatching) == 0)
    if (mCheckFlags |= LaunchCheckFlags::SoftPatching; CheckSoftPatching(emulator))
      return;

  // SuperGameBoy choice
  if ((mCheckFlags & LaunchCheckFlags::SuperGameboy) == 0)
    if(mCheckFlags |= LaunchCheckFlags::SuperGameboy; mGameLinkedData.SuperGameBoy().ShouldAskForSuperGameBoy(mGameToLaunch->System()))
    {
      mWindow.pushGui(new GuiFastMenuList(mWindow, this, _("GameBoy Mode"), mGameToLaunch->Name(), (int)FastMenuType::SuperGameboy,
                                          { { "GameBoy", _("Start the game standard Game Boy mode") }, { "SuperGameBoy", _("Start the game in Super Game Boy mode") } }, mSuperGameboyLastChoice));
      return;
    }

  // Save state slot
  if ((mCheckFlags & LaunchCheckFlags::SaveState) == 0)
    if (mCheckFlags |= LaunchCheckFlags::SaveState; EmulatorManager::GetGameEmulator(*mGameToLaunch).IsLibretro() && RecalboxConf::Instance().GetGlobalShowSaveStateBeforeRun())
      if (!GameFilesUtils::GetGameSaveStateFiles(*mGameToLaunch).empty())
      {
        mWindow.pushGui(new GuiSaveStates(mWindow, mSystemManager, *mGameToLaunch, this, false));
        return;
      }

  LaunchAnimated(emulator);
}

void ViewController::LaunchActually(const EmulatorData& emulator)
{
  DateTime start;
  GameRunner::Instance().RunGame(*mGameToLaunch, emulator, mGameLinkedData);
  if (mForceGoToGame)
    selectGamelistAndCursor(mGameToLaunch);
  TimeSpan elapsed = DateTime() - start;

  if (elapsed.TotalMilliseconds() <= 3000) // 3s
  {
    // Build text
    String text = _("It seems that your game didn't start at all!\n\nIt's most likely due to either:\n- bad rom\n- missing/bad mandatory bios files\n- missing/bad optional BIOS files (but required for this very game)");
    // Show the dialog box
    Gui* gui = new GuiMsgBox(mWindow, text, _("OK"), TextAlignment::Left);
    mWindow.pushGui(gui);
  }
}

void ViewController::LaunchAnimated(const EmulatorData& emulator)
{
  Vector3f center = mLaunchCameraTarget.isZero() ? Vector3f(Renderer::Instance().DisplayWidthAsFloat() / 2.0f, Renderer::Instance().DisplayHeightAsFloat() / 2.0f, 0) : mLaunchCameraTarget;

	Transform4x4f origCamera = mCamera;
	origCamera.translation() = -mCurrentView->GetGui().getPosition();

	center += mCurrentView->GetGui().getPosition();
	stopAnimation(1); // make sure the fade in isn't still playing
	mLockInput = true;

  String transitionTheme = ThemeData::getCurrent().getTransition();
  if (transitionTheme.empty()) transitionTheme = RecalboxConf::Instance().GetThemeTransition();

	auto launchFactory = [this, origCamera, &emulator] (const std::function<void(std::function<void()>)>& backAnimation)
	{
		return [this, origCamera, backAnimation, emulator]
		{
		  LaunchActually(emulator);

      mCamera = origCamera;
			backAnimation([this] { mLockInput = false; });
      if (isViewing(IView::ViewType::GameList))
        ((ISimpleGameListView*)mCurrentView)->DoUpdateGameInformation(true);

			// Re-sort last played system if it exists
      mSystemManager.UpdateSystemsOnGameChange(mGameToLaunch, MetadataType::LastPlayed, false);
		};
	};

	if(transitionTheme == "fade")
	{
		// fade out, launch game, fade back in
		auto fadeFunc = [this](float t) {
			mFadeOpacity = lerp<float>(0.0f, 1.0f, t);
		};
		setAnimation(new LambdaAnimation(fadeFunc, 800), 0, launchFactory([this, fadeFunc](const std::function<void()>& finishedCallback) {
			setAnimation(new LambdaAnimation(fadeFunc, 800), 0, finishedCallback, true);
		}));
	} else if (transitionTheme == "slide"){

		// move camera to zoom in on center + fade out, launch game, come back in
		setAnimation(new LaunchAnimation(mCamera, mFadeOpacity, center, 1500), 0, launchFactory([this, center](const std::function<void()>& finishedCallback) {
			setAnimation(new LaunchAnimation(mCamera, mFadeOpacity, center, 1000), 0, finishedCallback, true);
		}));
	} else {
		setAnimation(new LaunchAnimation(mCamera, mFadeOpacity, center, 10), 0, launchFactory([this, center](const std::function<void()>& finishedCallback) {
			setAnimation(new LaunchAnimation(mCamera, mFadeOpacity, center, 10), 0, finishedCallback, true);
		}));
	}
}

ISimpleGameListView* ViewController::GetOrCreateGamelistView(SystemData* system)
{
	//if we already made one, return that one
	auto exists = mGameListViews.find(system);
	if(exists != mGameListViews.end())
		return exists->second;

	//if we didn't, make it, remember it, and return it
	ISimpleGameListView* view =
    (system->Descriptor().IsArcade() && RecalboxConf::Instance().GetArcadeViewEnhanced() && !(system->Name() == "daphne")) ?
    new ArcadeGameListView(mWindow, mSystemManager, *system) :
    new DetailedGameListView(mWindow, mSystemManager, *system);
  view->Initialize();
	view->setTheme(system->Theme());

  int id = mSystemManager.SystemAbsoluteIndex(system);
	view->setPosition((float)id * Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat() * 2);

	addChild(view);

	mGameListViews[system] = view;
	mInvalidGameList[system] = false;
	return view;
}

bool ViewController::ProcessInput(const InputCompactEvent& event)
{
	if (mLockInput) return true;

	/* if we receive a button pressure for a non-configured joystick, suggest the joystick configuration */
	if (event.AskForConfiguration())
	{
		mWindow.pushGui(new GuiDetectDevice(mWindow, false, nullptr));
		return true;
	}

  return mCurrentView->GetGui().ProcessInput(event);
}

void ViewController::Update(int deltaTime)
{
  PlayViewTransition(deltaTime);
  mCurrentView->GetGui().Update(deltaTime);
	updateSelf(deltaTime);
}

void ViewController::Render(const Transform4x4f& parentTrans)
{
  if (mTransition != Transition::None)
    if (mPreviousView != nullptr)
      mPreviousView->GetGui().Render(parentTrans);
  if (mCurrentView != nullptr)
    mCurrentView->GetGui().Render(parentTrans);

  /*switch(mCurrentView->GetViewType())
  {
    case IView::ViewType::GameClip:
    case IView::ViewType::CrtCalibration: mCurrentView->GetGui().Render(parentTrans); break;
    // All the following view have been drawn before as they are part of the "global" visible area
    case IView::ViewType::SystemList:
    case IView::ViewType::GameList:
    case IView::ViewType::SplashScreen:
    case IView::ViewType::None:
    default:
    {
      Transform4x4f trans = mCamera * parentTrans;
      Transform4x4f transInverse(Transform4x4f::Identity());
      transInverse.invert(trans);

      // camera position, position + size
      Vector3f viewStart = transInverse.translation();
      Vector3f viewEnd = transInverse * Vector3f(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat(), 0);

      int vpl = (int)viewStart.x();
      int vpu = (int)viewStart.y();
      int vpr = (int)viewEnd.x() - 1;
      int vpb = (int)viewEnd.y() - 1;

      // Draw splash
      for(;;)
      {
        // clipping - only y
        const Vector3f& position = mSplashView.getPosition();
        const Vector2f& size = mSplashView.getSize();

        int gu = (int)position.y();
        int gb = (int)position.y() + (int)size.y() - 1;

        if (gb < vpu) break;
        if (gu > vpb) break;

        mSplashView.Render(trans);
        break;
      }

      // Draw systemview
      for(;;)
      {
        // clipping - only y
        const Vector3f& position = mSystemListView.getPosition();
        const Vector2f& size = mSystemListView.getSize();

        int gu = (int)position.y();
        int gb = (int)position.y() + (int)size.y() - 1;

        if (gb < vpu) break;
        if (gu > vpb) break;

        mSystemListView.Render(trans);
        break;
      }

      // Draw gamelists
      for (auto& mGameListView : mGameListViews)
      {
        // clipping
        const Vector3f& position = mGameListView.second->getPosition();
        const Vector2f& size = mGameListView.second->getSize();

        int gl = (int)position.x();
        int gu = (int)position.y();
        int gr = (int)position.x() + (int)size.x() - 1;
        int gb = (int)position.y() + (int)size.y() - 1;

        if (gr < vpl) continue;
        if (gl > vpr) continue;
        if (gb < vpu) continue;
        if (gu > vpb) continue;

        mGameListView.second->Render(trans);
      }

      break;
    }
  }*/

	if(mWindow.peekGui() == nullptr) // TODO:: dafuk?!
		mWindow.renderHelpPromptsEarly();

	// fade out
	if(mFadeOpacity != 0.0)
	{
		Renderer::SetMatrix(parentTrans);
		Renderer::DrawRectangle(0, 0, Renderer::Instance().DisplayWidthAsInt(), Renderer::Instance().DisplayHeightAsInt(), 0x00000000 | (unsigned char)(mFadeOpacity * 255));
	}
}

bool ViewController::GetOrReCreateGamelistView(SystemData* system, bool reloadTheme)
{
  for (auto& it : mGameListViews)
    if (it.first == system)
    {
      ISimpleGameListView* view = it.second;
      bool isCurrent = (mCurrentView == view);
      FileData *cursor = view->Count() != 0 ? view->getCursor() : nullptr;
      mGameListViews.erase(system);

      if (reloadTheme) system->loadTheme();

      if (system->HasVisibleGame())
      {
        ISimpleGameListView* newView = GetOrCreateGamelistView(system);
        newView->setCursor(cursor);
        if (isCurrent) mCurrentView = newView;
        mGameListViews[system] = newView;
        return true;
      }
    }
  return false;
}

void ViewController::InvalidateGamelist(const SystemData* system)
{
	for (auto& mGameListView : mGameListViews)
		if (system == (mGameListView.first))
		{
			mInvalidGameList[mGameListView.first] = true;
			break;
		}
}

void ViewController::InvalidateAllGamelistsExcept(const SystemData* systemExclude)
{
	for (auto& mGameListView : mGameListViews)
		if (systemExclude != (mGameListView.first))
			mInvalidGameList[mGameListView.first] = true;
}

bool ViewController::getHelpPrompts(Help& help)
{
	return mCurrentView != nullptr && mCurrentView->GetGui().getHelpPrompts(help);
}

void ViewController::ApplyHelpStyle()
{
	if (mCurrentView != nullptr)
    mCurrentView->GetGui().ApplyHelpStyle();
}

void ViewController::ChangeView(IView::ViewType newViewMode, SystemData* targetSystem, Transition forcedTransition)
{
  // Save previous mode & deinit
  mPreviousView = mCurrentView;
  mCurrentView->GetGui().onHide();
  switch(mCurrentView->GetViewType())
  {
    case IView::ViewType::None:
    case IView::ViewType::SplashScreen:
    case IView::ViewType::SystemList:
    case IView::ViewType::GameList:
    case IView::ViewType::CrtCalibration: break;
    case IView::ViewType::GameClip:
    {
      WakeUp();
      if(AudioMode::MusicsXorVideosSound == RecalboxConf::Instance().GetAudioMode())
        AudioManager::Instance().StartPlaying(mCurrentSystem->Theme());
      break;
    }
  }

  // Process new mode
  switch(newViewMode)
  {
    case IView::ViewType::None: return; // lol
    case IView::ViewType::SplashScreen:
    {
      mCurrentView = &mSplashView;
      break;
    }
    case IView::ViewType::SystemList:
    {
      mCurrentView = &mSystemListView;
      mSystemListView.goToSystem(targetSystem, false);
      mCurrentSystem = targetSystem;
      break;
    }
    case IView::ViewType::GameList:
    {
      mCurrentView = GetOrCreateGamelistView(targetSystem);
      mCurrentSystem = targetSystem;
      break;
    }
    case IView::ViewType::GameClip:
    {
      if (AudioMode::MusicsXorVideosSound == RecalboxConf::Instance().GetAudioMode())
        AudioManager::Instance().StopAll();
      mCurrentView = &mGameClipView;
      break;
    }
    case IView::ViewType::CrtCalibration:
    {
      mCurrentView = &mCrtView;
      break;
    }
  }
  mCurrentView->GetGui().onShow();
  InitializeViewTransition(forcedTransition);
  updateHelpPrompts();
}

void ViewController::BackToPreviousView()
{
  if (mPreviousView->GetViewType() == IView::ViewType::None) return; // Previous mode not initialized
  ChangeView(mPreviousView->GetViewType(), mCurrentSystem, Transition::None);
  mPreviousView = &mNoView; // Reset previous mode so that you cannot go back until next forward move
}

void ViewController::SelectSystem(SystemData* system)
{
  if (isViewing(IView::ViewType::SystemList)) mSystemListView.goToSystem(system, false);
  else if (isViewing(IView::ViewType::GameList)) goToGameList(system);
}

void ViewController::ShowSystem(SystemData* system)
{
  mSystemListView.addSystem(system);
  mSystemListView.Sort();
  InvalidateAllGamelistsExcept(nullptr);
  GetOrCreateGamelistView(system);
}

void ViewController::HideSystem(SystemData* system)
{
  // Remove system in system view
  mSystemListView.removeSystem(system);

  // Are we on the gamelist that just need to be removed?
  if (isViewing(IView::ViewType::GameList))
    if (&((ISimpleGameListView*)mCurrentView)->System() == system)
    {
      SystemData* nextSystem = goToNextGameList();
      mSystemListView.goToSystem(nextSystem, false);
    }
}

void ViewController::UpdateSystem(SystemData* system)
{
  InvalidateGamelist(system);
  // Force refresh if we're on this system's view
  ISimpleGameListView** view = mGameListViews.try_get(system);
  if (view != nullptr)
    if (*view == mCurrentView)
    {
      int index = (*view)->getCursorIndex();
      (*view)->refreshList();
      (*view)->setCursorIndex(index);
    }
}

void ViewController::SystemShownWithNoGames(SystemData* system)
{
  String text = (_F(_("System \"{0}\" has no visible game yet!\n\nIt will show up automatically as soon as it has visible games.")) / system->FullName()).ToString();
  mWindow.InfoPopupAddRegular(text, 10, PopupType::Recalbox, false);
}

void ViewController::ToggleFavorite(FileData* game, bool forceStatus, bool forcedStatus)
{
  // Toggle favorite
  game->Metadata().SetFavorite(forceStatus ? forcedStatus : !game->Metadata().Favorite());

  // Fire dynamic system refresh
  mSystemManager.UpdateSystemsOnGameChange(game, MetadataType::Favorite, false);

  // Refresh game in its regular view
  ISimpleGameListView** view = mGameListViews.try_get(&game->System());
  if (view != nullptr) (*view)->RefreshItem(game);

  // Info popup
  String message = game->Metadata().Favorite() ? _("Added to favorites") : _("Removed from favorites");
  mWindow.InfoPopupAddRegular(message.Append(":\n").Append(game->Name()), RecalboxConf::Instance().GetPopupHelp(), PopupType::None, false);
}

void ViewController::Run()
{
  while(IsRunning())
  {
    mSignal.WaitSignal();
    if (!IsRunning()) return; // Destructor called :)
    for(bool working = true; working; )
    {
      mLocker.Lock();
      const FileData* item = mNextItem;
      mNextItem = nullptr;
      mLocker.UnLock();
      if (item == nullptr) working = false;
      else if (item->IsFolder())
      {
        FolderData* folder = (FolderData*)item;
        int count = folder->CountAll(false, folder->System().Excludes());

        class Filter : public IFilter
        {
          private:
            Path mPath[sFoldersMaxGameImageCount];
            int mCount;

          public:
            Filter() : mCount(0) {}

            bool ApplyFilter(const FileData& file) override
            {
              if (mCount < sFoldersMaxGameImageCount)
                if (file.HasThumbnailOrImage())
                  mPath[mCount++] = file.ThumbnailOrImagePath();
              return false;
            }

            Path& GetPath(int index) { return mPath[index]; }
        } filter;
        folder->CountFilteredItemsRecursively(&filter, false);

        // Store results
        mLocker.Lock();
        for(int i = sFoldersMaxGameImageCount; --i >= 0;)
          mLastFolderImagePath[i] = std::move(filter.GetPath(i));
        mLocker.UnLock();
        mSender.Send({ item, count, false, &mLastFolderImagePath });
      }
      else if (item->IsGame())
      {
        if (item->HasP2K()) // Send message only if p2k is available
          mSender.Send({ item, 0, true, &mLastFolderImagePath } );
      }
    }
  }
}

void ViewController::ReceiveSyncMessage(const SlowDataInformation& data)
{
  if (isViewing(IView::ViewType::GameList))
    ((ISimpleGameListView*)mCurrentView)->UpdateSlowData(data);
}

void ViewController::FetchSlowDataFor(FileData* data)
{
  Mutex::AutoLock locker(mLocker);
  mNextItem = data;
  mSignal.Fire();
}

void ViewController::RequestSlowOperation(ISlowSystemOperation* interface, ISlowSystemOperation::List systems, bool autoSelectMonoSystem)
{
  String text = systems.Count() > 1 ?
                _("INITIALIZING SYSTEMS...") :
                (_F(_("INITIALIZING SYSTEM {0}")) / systems.First()->FullName()).ToString();
  auto* gui = new GuiWaitLongExecution<DelayedSystemOperationData, bool>(mWindow, *this);
  gui->Execute({ std::move(systems), interface, autoSelectMonoSystem }, text);
  mWindow.pushGui(gui);
}

bool ViewController::Execute(GuiWaitLongExecution<DelayedSystemOperationData, bool>& from,
                             const DelayedSystemOperationData& parameter)
{
  (void)from;
  if (parameter.mSlowIMethodInterface != nullptr)
    parameter.mSlowIMethodInterface->SlowPopulateExecute(parameter.mSystemList);
  return true;
}

void ViewController::Completed(const DelayedSystemOperationData& parameter, const bool& result)
{
  (void)result;
  if (parameter.mSlowIMethodInterface != nullptr)
    parameter.mSlowIMethodInterface->SlowPopulateCompleted(parameter.mSystemList, parameter.autoSelectMonoSystem);
}

