//
// Created by bkg2k on 30/10/2020.
//

#include <utils/locale/LocaleHelper.h>
#include <audio/AudioController.h>
#include <audio/AudioManager.h>
#include "MenuSound.h"
#include "MenuSoundPair.h"
#include "guis/MenuMessages.h"
#include <guis/GuiMsgBox.h>
#include <recalbox/RecalboxSystem.h>
#include <guis/menus/base/ItemSlider.h>

MenuSound::MenuSound(WindowManager& window)
  : Menu(window, _("SOUND SETTINGS"))
{
}

void MenuSound::BuildMenuItems()
{
  // Force configuration to reload so that any external modified volume is set properly
  RecalboxConf::Instance().ForceReload();

  // Volume
  mVolume = AddSlider(_("SYSTEM VOLUME"), 0.f, 100.f, 1.f, 80.f, (float)AudioController::Instance().GetVolume(), "%", (int)Components::Volume, this, _(MENUMESSAGE_SOUND_VOLUME_HELP_MSG));
  mMusicVolume = AddSlider(_("MUSIC VOLUME"), 0.f, 100.f, 1.f, (float)AudioController::Instance().GetMusicVolume(), 80.f, "%", (int)Components::MusicVolume, this, _(MENUMESSAGE_SOUND_MUSIC_VOLUME_HELP_MSG));

  // AudioMode
  SelectorEntry<AudioMode>::List list = GetAudioModeEntries();
  AddList<AudioMode>(_("AUDIO MODE"), (int)Components::AudioMode, this, list, RecalboxConf::Instance().GetAudioMode(), AudioMode::MusicsXorVideosSound, _(MENUMESSAGE_SOUND_MODE_HELP_MSG));

  // Audio device
  if (RecalboxConf::Instance().GetMenuType() != RecalboxConf::Menu::Bartop)
    mOutputDevice = AddList<String>(_("OUTPUT DEVICE"), (int)Components::Output, this, GetOutputEntries(), AudioController::Instance().GetActivePlaybackName(), String::Empty, _(MENUMESSAGE_SOUND_DEVICE_HELP_MSG));

  // Bluetooth pairing
  AddSubMenu(_("PAIR A BLUETOOTH AUDIO DEVICE"), (int)Components::Pair, this, _(MENUMESSAGE_SOUND_BT_HELP_MSG));

  // Set PulseAudioCallback to update the menu
  AudioController::Instance().EnableNotification();
  AudioController::Instance().SetNotificationCallback(this);
}

MenuSound::~MenuSound()
{
  AudioController::Instance().DisableNotification();
  AudioController::Instance().ClearNotificationCallback();
}

SelectorEntry<String>::List MenuSound::GetOutputEntries()
{
  SelectorEntry<String>::List list;
  // Add all remaining stuff
  for(const DeviceDescriptor& playback : AudioController::Instance().GetPlaybackList())
    list.push_back({ playback.DisplayableName, playback.InternalName });
  return list;
}

SelectorEntry<AudioMode>::List MenuSound::GetAudioModeEntries()
{
  SelectorEntry<AudioMode>::List list;
  list.push_back({ _("Musics or videos sound") , AudioMode::MusicsXorVideosSound });
  list.push_back({ _("Musics and videos sound"), AudioMode::MusicsAndVideosSound });
  list.push_back({ _("Musics only")            , AudioMode::MusicsOnly           });
  list.push_back({ _("Videos sound only")      , AudioMode::VideosSoundOnly      });
  list.push_back({ _("No sound")               , AudioMode::None                 });
  return list;
}

void MenuSound::Refresh()
{
  mOutputDevice->ChangeSelectionItems(GetOutputEntries());
}

void MenuSound::Update(int deltaTime)
{
  Menu::Update(deltaTime);

  // Synchronize volume bar if the volume is modified elsewhere
  int realVolume = AudioController::Instance().GetVolume();
  if (realVolume != mVolume->Value())
  {
    mVolume->SetValue((float)realVolume, false);
    RecalboxConf::Instance().SetAudioVolume(realVolume).Save();
  }

  realVolume = AudioController::Instance().GetMusicVolume();
  if (realVolume != mMusicVolume->Value())
  {
    mMusicVolume->SetValue((float) realVolume, false);
    RecalboxConf::Instance().SetAudioMusicVolume(realVolume).Save();
  }
}

void MenuSound::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  if ((Components)id == Components::Output)
  {
    AudioController::Instance().DisableNotification();
    AudioController::Instance().SetDefaultPlayback(value);
    AudioController::Instance().EnableNotification();
    RecalboxConf::Instance().SetAudioOuput(value).Save();
  }
}

void MenuSound::MenuSingleChanged(int id, int index, const AudioMode& value)
{
  (void)index;
  if ((Components)id == Components::AudioMode)
  {
    const bool currentCanPlayMusic = AudioModeTools::CanPlayMusic();
    RecalboxConf::Instance().SetAudioMode(value).Save();
    if (!currentCanPlayMusic && AudioModeTools::CanPlayMusic())
      AudioManager::Instance().StartPlaying(ThemeManager::Instance().Main());
    else if (currentCanPlayMusic && !AudioModeTools::CanPlayMusic())
      AudioManager::Instance().StopAll();
  }
}

void MenuSound::MenuSliderMoved(int id, float value)
{
  if ((Components)id == Components::Volume && AudioController::Instance().GetVolume() != Math::roundi(value))
  {
    AudioController::Instance().SetVolume(Math::roundi(value));
    if (RecalboxConf::Instance().GetAudioVolume() != Math::roundi(value))
      RecalboxConf::Instance().SetAudioVolume(Math::roundi(value)).Save();
  }
  if ((Components)id == Components::MusicVolume && AudioController::Instance().GetMusicVolume() != Math::roundi(value))
  {
    AudioController::Instance().SetMusicVolume(Math::roundi(value));
  }
}

void MenuSound::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Volume:
    case Components::MusicVolume:
    case Components::AudioMode:
    case Components::Output:
      break;
    case Components::Pair:
      StartScanningDevices();
      break;
  }
}

void MenuSound::StartScanningDevices()
{
  mWindow.pushGui((new GuiWaitLongExecution<bool, String::List>(mWindow, *this))->Execute(false, _("DISCOVERING BLUETOOTH AUDIO DEVICES...")));
}

String::List MenuSound::Execute(GuiWaitLongExecution<bool, String::List>&, const bool&)
{
  return RecalboxSystem::scanBluetooth();
}

void MenuSound::Completed(const bool&, const String::List& result)
{
  mWindow.pushGui(result.empty()
                  ? (Gui*)new GuiMsgBox(mWindow, _("NO AUDIO DEVICE FOUND"), _("OK"))
                  : (Gui*)new MenuSoundPair(mWindow, result));
}

void MenuSound::NotifyAudioChange()
{
  Refresh();
}
