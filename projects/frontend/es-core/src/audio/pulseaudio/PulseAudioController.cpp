#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-function-cognitive-complexity"
//
// Created by bkg2k on 19/12/2020.
//

#include "PulseAudioController.h"
#include <pulse/pulseaudio.h>
#include <utils/locale/LocaleHelper.h>
#include <utils/math/Misc.h>
#include <RecalboxConf.h>
#include <hardware/Board.h>
#include <SDL_mixer.h>

PulseAudioController::PulseAudioController()
  : mConnectionState(ConnectionState::NotConnected)
  , mPulseAudioContext(nullptr)
  , mPulseAudioMainLoop(nullptr)
  , mEvent(*this)
  , mNotificationInterface(nullptr)
  , mNotification(false)
{
  Thread::Start("PulseAudio");
  Initialize();
}

PulseAudioController::~PulseAudioController()
{
  Finalize();
  Thread::Stop();
}

void PulseAudioController::Initialize()
{
  // Connect to pulseaudio server
  PulseContextConnect();
  // Enumerate all sinks and cards
  Refresh();
  // Subscribe to changes
  //PulseSubscribe();

  { LOG(LogInfo) << "[PulseAudio] Initialized."; }
}

void PulseAudioController::Finalize()
{
  PulseContextDisconnect();
}

void PulseAudioController::SetProfileCallback(pa_context *context, int success, void *userdata)
{
  (void)context;
  (void)success;

  { LOG(LogDebug) << "[PulseAudio] Set Profile result: " << (success != 0 ? "SUCCESS" : "FAIL"); }

  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Result - don't care about the actual result
  This.mSignal.Fire();
}

void PulseAudioController::SetVolumeCallback(pa_context *context, int success, void *userdata)
{
  (void)context;
  (void)success;

  { LOG(LogDebug) << "[PulseAudio] Set Volume result: " << (success != 0 ? "SUCCESS" : "FAIL"); }

  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Result - don't care about the actual result
  This.mSignal.Fire();
}

void PulseAudioController::SetMuteCallback(pa_context *context, int success, void *userdata)
{
  (void)context;
  (void)success;

  { LOG(LogDebug) << "[PulseAudio] Set Mute result: " << (success != 0 ? "SUCCESS" : "FAIL"); }

  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Result - don't care about the actual result
  This.mSignal.Fire();
}

void PulseAudioController::SetSinkCallback(pa_context *context, int success, void *userdata)
{
  (void)context;
  (void)success;

  { LOG(LogDebug) << "[PulseAudio] Set Sink result: " << (success != 0 ? "SUCCESS" : "FAIL"); }

  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Result - don't care about the actual result
  This.mSignal.Fire();
}

void PulseAudioController::SetPortCallback(pa_context *context, int success, void *userdata)
{
  (void)context;
  (void)success;

  { LOG(LogDebug) << "[PulseAudio] Set Port result: " << (success != 0 ? "SUCCESS" : "FAIL"); }

  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Result - don't care about the actual result
  This.mSignal.Fire();
}

void PulseAudioController::ContextStateCallback(pa_context *context, void *userdata)
{
  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Get state
  pa_context_state_t state = pa_context_get_state(context);
	switch(state)
	{
		case PA_CONTEXT_FAILED:
		case PA_CONTEXT_TERMINATED:
    {
      { LOG(LogInfo) << "[PulseAudio] Disconnected from PulseAudio server."; }
      This.mConnectionState = ConnectionState::Closed;
      This.mSignal.Fire();
      break;
    }
		case PA_CONTEXT_READY:
    {
      { LOG(LogInfo) << "[PulseAudio] Connected to PulseAudio server."; }
      This.mConnectionState = ConnectionState::Ready;
      This.mSignal.Fire();

      // Set callback
      pa_context_set_subscribe_callback(context, SubscriptionCallback, userdata);
      // Set events mask and enable event callback.
      pa_operation *o = pa_context_subscribe(context, (pa_subscription_mask_t)(PA_SUBSCRIPTION_MASK_SINK), nullptr, nullptr);

      if (o != nullptr)
      {
        { LOG(LogInfo) << "[PulseAudio] Subscribed to card and sink event."; }
        pa_operation_unref(o);
      }
      break;
    }
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
    default: break;
	}
}

void PulseAudioController::SubscriptionCallback(pa_context *context, pa_subscription_event_type_t t, uint32_t index, void *userdata)
{
  (void)context;
  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;
  (void)This;

  unsigned int facility = t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;
  unsigned int type = (pa_subscription_event_type_t)(t & PA_SUBSCRIPTION_EVENT_TYPE_MASK);

  const char* typeStr = "UNKNOWN TYPE";;
  switch(type)
  {
    case PA_SUBSCRIPTION_EVENT_NEW: typeStr = "NEW"; break;
    case PA_SUBSCRIPTION_EVENT_CHANGE: typeStr = "CHANGE"; break;
    case PA_SUBSCRIPTION_EVENT_REMOVE: typeStr = "REMOVE"; break;
    default: break;
  }
  const char* eventStr = "UNKNOWN EVENT";;
  switch(facility)
  {
    case PA_SUBSCRIPTION_EVENT_SINK: eventStr = "SINK"; break;
    case PA_SUBSCRIPTION_EVENT_SOURCE: eventStr = "SOURCE"; break;
    case PA_SUBSCRIPTION_EVENT_SINK_INPUT: eventStr = "SINK_INPUT"; break;
    case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT: eventStr = "SOURCE_OUTPUT"; break;
    case PA_SUBSCRIPTION_EVENT_MODULE: eventStr = "MODULE"; break;
    case PA_SUBSCRIPTION_EVENT_CLIENT: eventStr = "CLIENT"; break;
    case PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE: eventStr = "SAMPLE CACHE"; break;
    case PA_SUBSCRIPTION_EVENT_SERVER: eventStr = "SERVER"; break;
    case PA_SUBSCRIPTION_EVENT_AUTOLOAD: eventStr = "AUTOLOAD"; break;
    case PA_SUBSCRIPTION_EVENT_CARD: eventStr = "CARD"; break;
    default: break;
  }

  { LOG(LogDebug) << "[PulseAudio] Event received Type: " << typeStr << " - Event: " << eventStr << " - Index: " << index; }

  // Emit signal when a sink is added or removed
  // all chances are the new sink is now the default one
  // UI may need to refresh that change
  if (This.mNotification && (type == PA_SUBSCRIPTION_EVENT_NEW || type == PA_SUBSCRIPTION_EVENT_REMOVE)
      &&
      (facility == PA_SUBSCRIPTION_EVENT_SINK))
  {
    { LOG(LogDebug) << "[PulseAudio] Sink altered, send event"; }
    // Notify of sink/card change
    This.mEvent.Send();
  }
}

void PulseAudioController::GetServerInfoCallback(pa_context *context, const pa_server_info* i, void* userdata)
{
  (void)context;
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // Store default sink name
  This.mSyncer.Lock();
  This.mServerInfo.DefaultSinkName = i->default_sink_name;
  This.mSyncer.UnLock();

  This.mSignal.Fire();
}

AudioIcon PulseAudioController::GetPortIcon(const pa_sink_port_info& info)
{
#if PA_CHECK_VERSION(14,0,0)
  switch(info.type)
  {
    case PA_DEVICE_PORT_TYPE_SPEAKER: return AudioIcon::Speakers;
    case PA_DEVICE_PORT_TYPE_HEADPHONES: return AudioIcon::Headphones;
    case PA_DEVICE_PORT_TYPE_HDMI:
    case PA_DEVICE_PORT_TYPE_TV:
    case PA_DEVICE_PORT_TYPE_VIDEO:
      return AudioIcon::Screens;
    default: break;
  }
#else
  (void)info;
#endif
  return AudioIcon::Unidentified;
}

AudioIcon PulseAudioController::GetPortIcon(const pa_card_port_info& info)
{
  const char* icon = pa_proplist_gets(info.proplist, "device.icon_name");
  if (icon == nullptr) return AudioIcon::Unidentified;

  if (strcmp(icon, "audio-speakers") == 0) return AudioIcon::Speakers;
  if (strcmp(icon, "audio-headphones") == 0) return AudioIcon::Headphones;
  if (strcmp(icon, "video-display") == 0) return AudioIcon::Screens;

  return AudioIcon::Unidentified;
}

void PulseAudioController::EnumerateCardCallback(pa_context* context, const pa_card_info* info, int eol, void* userdata)
{
  (void)context;
  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // If eol is set to a positive number, you're at the end of the list
  if (eol > 0)
  {
    This.mSignal.Fire();
    return;
  }

  // New card
  Card newCard;
  newCard.Name = info->name;
  newCard.Description = GetCardDescription(*info);
  newCard.Index = (int)info->index;
  newCard.HasActiveProfile = false;
  if (info->active_profile != nullptr)
    newCard.ActiveProfile = info->active_profile->name;
  else
    newCard.ActiveProfile = "";
  newCard.Ports.clear();

  { LOG(LogDebug) << "[PulseAudio] Card #" << newCard.Index << " : " << newCard.Name; }

  // Active profile
  String activeProfileName;
  if (info->active_profile2 != nullptr)
  {
    activeProfileName = info->active_profile2->name;
    { LOG(LogDebug) << "[PulseAudio] Active Profile: " << info->active_profile2->description << " ("
                    << info->active_profile2->name << ") - Available "
                    << (info->active_profile2->available != 0 ? "YES" : "NO") << " - Priority "
                    << info->active_profile2->priority; }
  }

  // Card Profiles
  for(int i = (int)info->n_profiles; --i >= 0; )
  {
    pa_card_profile_info2& profileInfo = *info->profiles2[i];
    if (profileInfo.n_sinks <= 0) continue; // Ignore source-only profiles

    Profile newProfile;
    newProfile.Name = profileInfo.name;
    newProfile.Description = profileInfo.description;
    newProfile.Available = profileInfo.available != 0;
    newProfile.Priority = (int)profileInfo.priority;

    { LOG(LogDebug) << "[PulseAudio]  Card Profile " << newProfile.Description << " (" << newProfile.Name << ") - Available " << (newProfile.Available ? "YES" : "NO") << " - Priority " << newProfile.Priority; }

    // Check if this profile is the active one
    newCard.HasActiveProfile |= (newProfile.Name == activeProfileName);

    // Add profile to the card
    newCard.Profiles.push_back(newProfile);
  }

  // Ports (device outputs)
  for(int i = (int)info->n_ports; --i >= 0; )
  {
    pa_card_port_info& portInfo = *info->ports[i];
    if ((portInfo.direction & PA_DIRECTION_OUTPUT) == 0) continue; // Ignore non-output ports

    Port newPort;
    newPort.Name = portInfo.name;
    newPort.Icon = GetPortIcon(portInfo);
    newPort.Description = GetPortDescription(portInfo, newPort.Icon);
    newPort.Available = portInfo.available != PA_PORT_AVAILABLE_NO;
    newPort.Priority = (int)portInfo.priority;
    newPort.InternalIndex = (int)newCard.Ports.size();
    newPort.Profiles.clear();

    { LOG(LogDebug) << "[PulseAudio]   Port " << newPort.Description << " (" << newPort.Name << ") - Available " << (newPort.Available ? "YES" : "NO") << " - Priority " << newPort.Priority; }

    // Port's supported profiles
    for(int p = (int)portInfo.n_profiles; --p >= 0; )
    {
      pa_card_profile_info2& profileInfo = *portInfo.profiles2[p];
      if (profileInfo.n_sinks <= 0) continue; // Ignore source-only profiles

      Profile newProfile;
      newProfile.Name = profileInfo.name;
      newProfile.Description = profileInfo.description;
      newProfile.Available = profileInfo.available != 0;
      newProfile.Priority = (int)profileInfo.priority;

      { LOG(LogDebug) << "[PulseAudio]     Profile " << newProfile.Description << " (" << newProfile.Name << ") - Available " << (newProfile.Available ? "YES" : "NO") << " - Priority " << newProfile.Priority; }

      // Check if this profile is the active one
      newCard.HasActiveProfile |= (newProfile.Name == activeProfileName);

      // Add profile to the port
      newPort.Profiles.push_back(newProfile);
    }

    // Add port to the card
    newCard.Ports.push_back(newPort);
  }

  // Store card
  This.mSyncer.Lock();
  This.mCards.push_back(newCard);
  This.mSyncer.UnLock();
}

void PulseAudioController::EnumerateSinkCallback(pa_context* context, const pa_sink_info* info, int eol, void* userdata)
{
  (void)context;
  // Get class
  PulseAudioController& This = *(PulseAudioController*)userdata;

  // If eol is set to a positive number, you're at the end of the list
  if (eol > 0)
  {
    This.mSignal.Fire();
    return;
  }

  Sink newSink;
  newSink.Name = info->name;
  newSink.Description = info->description;
  newSink.Index = (int)info->index;
  newSink.CardIndex = (int)info->card;
  newSink.Channels = info->channel_map.channels;
  newSink.State = info->state;
  if (info->active_port != nullptr)
    newSink.ActivePort = info->active_port->name;
  else
    newSink.ActivePort = "";
  newSink.Ports.clear();

  { LOG(LogInfo) << "[PulseAudio] Sink #" << newSink.Index << ' ' << newSink.Name << " found."; }

  // Collect available ports
  for(int i = (int)info->n_ports; --i >= 0; )
  {
    pa_sink_port_info& portInfo = *info->ports[i];

    Port newPort;
    newPort.Name = portInfo.name;
    newPort.Icon = GetPortIcon(portInfo);
    newPort.Description = GetPortDescription(portInfo, newPort.Icon);
    newPort.Available = portInfo.available != PA_PORT_AVAILABLE_NO;
    newPort.Priority = (int)portInfo.priority;
    newSink.Ports.push_back(newPort);
    { LOG(LogInfo) << "[PulseAudio] Port " << info->ports[i]->name << " - " << info->ports[i]->description; }
  }

  This.mSyncer.Lock();
  This.mSinks.push_back(newSink);
  This.mSyncer.UnLock();
}

void PulseAudioController::AddSpecialPlaybacks(IAudioController::DeviceList& list)
{
  (void)list;
  /*switch(Board::Instance().GetBoardType())
  {
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::PCx86:
    case BoardType::PCx64:
    default: break;
  }*/
}

IAudioController::DeviceList PulseAudioController::GetPlaybackList()
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  Mutex::AutoLock lock(mSyncer);
  DeviceList result;

  AddSpecialPlaybacks(result);

  for(const Sink& sink: mSinks)
  {
    const Card* card = GetCardByIndex(sink.CardIndex);
    // Classic audio sink (card + port + profile)
    if (card != nullptr && !card->Ports.empty())
    {
      for(const Port& port : card->Ports)
      {
        bool available = port.Available;

        // card + port
        String device;
        switch(port.Icon)
        {
          case AudioIcon::Auto: device = " \uf1e0 "; break;
          case AudioIcon::Unidentified: device = " \uf1b0 "; break;
          case AudioIcon::Speakers: device = " \uf1e1 "; break;
          case AudioIcon::Headphones: device = " \uf1e2 "; break;
          case AudioIcon::Screens: device = " \uf1e3 "; break;
        }
        device.Append(port.Description + " - " + card->Description);
        String displayable = available ? String("\u26ab ").Append(device).Append(" \u26ab") : String("\u26aa ").Append(device).Append(" \u26aa") ;
        const Profile* selectedProfile = GetBestProfile2(card, &port);
        if (selectedProfile != nullptr)
          result.push_back({ displayable, card->Name + ':' + port.Name+':'+selectedProfile->Name, AudioIcon::Auto });

        if (CountAvailableProfiles(port) > 1)
        {
          // profiles of card + ports
          for(const Profile& profile : port.Profiles)
          {
            if (! profile.Available)
              continue;

            String deviceDesc = " ↳ " + profile.Description;
            if (selectedProfile != nullptr)
              if (profile.Name == selectedProfile->Name)
                deviceDesc.Append(" \uf006");
            String displayableString = available ? String("      \u26ab ").Append(deviceDesc).Append(" \u26ab") : String("      \u26aa ").Append(deviceDesc).Append(" \u26aa") ;
            result.push_back({displayableString, card->Name + ':' + port.Name + ':' + profile.Name, AudioIcon::Auto });
          }
        }
      }
    }
    // PulseAudio Filter (eg. stereo to mono)
    else
      result.push_back({ String("\u26ab ").Append(sink.Description).Append(" \u26ab"), sink.Name+':', AudioIcon::Auto });
  }

  return result;
}

uint PulseAudioController::CountAvailableProfiles(const Port& port)
{
  uint availableProfiles = 0;
  for(const Profile& profile : port.Profiles)
  {
    if (profile.Available)
      availableProfiles++;
  }
  return availableProfiles;
}

const PulseAudioController::Card* PulseAudioController::FirstCard()
{
  if (!mCards.empty())
    return mCards.data();

  return nullptr;
}

const PulseAudioController::Port* PulseAudioController::FirstPort(const Card& card)
{
  if (!card.Ports.empty())
    return card.Ports.data();

  return nullptr;
}

const PulseAudioController::Card* PulseAudioController::LookupCard(const String& name)
{
  for(const Card& card : mCards)
    if (name == card.Name)
      return &card;

  return nullptr;
}

const PulseAudioController::Port* PulseAudioController::LookupPort(const Card& card, const String& name)
{
  for(const Port& port : card.Ports)
    if (name == port.Name)
      return &port;

  return nullptr;
}

const PulseAudioController::Profile* PulseAudioController::LookupProfile(const Card& card, const String& name)
{
  for(const Port& port : card.Ports)
    for(const Profile& profile : port.Profiles)
      if (name == profile.Name)
        return &profile;

  return nullptr;
}

bool PulseAudioController::HasPort(const PulseAudioController::Sink& sink, const PulseAudioController::Port& port)
{
  for(const Port& sinkPort : sink.Ports)
    if (sinkPort.Name == port.Name)
      return true;

  return false;
}

const PulseAudioController::Sink* PulseAudioController::LookupSink(const String& name)
{
  for(const Sink& sink : mSinks)
    if (name == sink.Name)
      return &sink;

  return nullptr;
}

const PulseAudioController::Sink* PulseAudioController::GetSinkFromCardPort(const Card* card, const Port* port)
{
  for(const Sink& sink : mSinks)
    if (card->Index == sink.CardIndex)
      for (const Port& sinkPort : sink.Ports)
        if (sinkPort.Name == port->Name)
          return &sink;

  return nullptr;
}

const PulseAudioController::Sink* PulseAudioController::GetSinkFromName(const String& name)
{
  for(const Sink& sink : mSinks)
    if (sink.Name == name)
      return &sink;

  return nullptr;
}

bool PulseAudioController::IsPortAvailable(const String& portName)
{
  bool available = false;
  for(const Card& card : mCards)
    for(const Port& port : card.Ports)
      if (port.Name == portName)
        available = port.Available;

  return available;
}

void PulseAudioController::UpdateDefaultSink()
{

  { LOG(LogInfo) << "[PulseAudio] Get server info"; }

  pa_operation* serverInfoOp = pa_context_get_server_info(mPulseAudioContext, GetServerInfoCallback, this);
  // Wait for response
  mSignal.WaitSignal(sTimeOut);
  // Release
  pa_operation_unref(serverInfoOp);
}

String PulseAudioController::GetActivePlaybackName()
{
  String playbackName;
  String sinkName ;

  Refresh();
  sinkName = mServerInfo.DefaultSinkName; // DefaultSinkName is updated by event subscription

  LOG(LogDebug) << "[PulseAudio] Default sink name is '" << sinkName << '\'';

  // return special playback name (like auto)
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG351V:
    case BoardType::RG351P:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::PCx86:
    case BoardType::PCx64:
    default: break;
  }

  if (sinkName.empty())
    return sinkName;

  const PulseAudioController::Sink* sink = GetSinkFromName(sinkName);

  if (sink == nullptr) {
    LOG(LogDebug) << "[PulseAudio] Sink not found... bailing out";
    return "";
  }
  const Card* card = GetCardByIndex(sink->CardIndex);

  // Classic audio sink (card + port + profile)
  if (card != nullptr && !card->Ports.empty())
  {
    playbackName.Append(card->Name).Append(':');
    playbackName.Append(GetSinkFromName(sinkName)->ActivePort);
    playbackName.Append(':');
    playbackName.Append(card->ActiveProfile);
  }
  // PulseAudio Filter (eg. stereo to mono)
  else
    playbackName.Append(sinkName).Append(':');

  LOG(LogDebug) << "[PulseAudio] Playback name is '" << playbackName << '\'';
  return playbackName;
}

String PulseAudioController::AdjustSpecialPlayback(const String& originalPlaybackName, bool& allprocessed)
{
  allprocessed = false;

  switch(Board::Instance().GetBoardType())
  {
    case BoardType::RG351V:
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    {
      if (originalPlaybackName == "alsa_card.platform-rk817-sound:rk817-headphones:output:analog-stereo")
      {
        if (system("amixer sset 'Playback Path' HP") != 0)
        { LOG(LogError) << "[PulseAudio] Error setting playback path"; }
      }
      else if (originalPlaybackName == "alsa_card.platform-rk817-sound:rk817-speaker:output:analog-stereo")
      {
        if (system("amixer sset 'Playback Path' SPK") != 0)
        { LOG(LogError) << "[PulseAudio] Error setting playback path"; }
      }
      else { LOG(LogError) << "[PulseAudio] Unreconized output: " << originalPlaybackName; }
      break;
    }
    case BoardType::RG353V:
    case BoardType::RG353P:
    case BoardType::RG353M:
    case BoardType::RG503:
    {
      // we force headphone output so that rg353m can be switched to spk by the software after the init/detection of headphone
      if (system("amixer sset 'Playback Path' HP") != 0)
      { LOG(LogError) << "[PulseAudio] Error setting playback path"; }
      break;
    }
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::PCx86:
    case BoardType::PCx64:
    default: break;
  }

  return originalPlaybackName;
}

String PulseAudioController::SetDefaultPlayback(const String& originalPlaybackName)
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  bool allProcessed = false;
  bool isAudioDeviceSink = true;
  String playbackName = AdjustSpecialPlayback(originalPlaybackName, allProcessed);
  if (allProcessed) return playbackName; // AjustSpecialPlayback did some tricks, no need to go further

  const Card* card = nullptr;
  const Sink* sink = nullptr;
  const Port* port = nullptr;
  const Profile* profile = nullptr;
  {
    Mutex::AutoLock lock(mSyncer);

    { LOG(LogInfo) << "[PulseAudio] Switching to " << playbackName; }

    String deviceName;
    String portName;
    if (!playbackName.Extract(':', deviceName, portName, true))
    { LOG(LogError) << "[PulseAudio] Invalid playbackname: " << playbackName; }

    sink = LookupSink(deviceName); // lookup existing sink in case of filter
    card = LookupCard(deviceName);

    // Bail out if sink or card not available anymore
    // This can happend when migrating or when audio cards have changed
    if (sink == nullptr and card == nullptr)
    {
      { LOG(LogWarning) << "[PulseAudio] Invalid sink or card: " << deviceName; }
      return playbackName;
    }

    // Card is set in the audio.device configuation
    // Set best profile of card (higher priority)
    if (card != nullptr)
    {
      String portName2;
      String profileName;
      if (!portName.Extract(':', portName2, profileName, true))
        portName2 = portName; // no profile given
      port = LookupPort(*card, portName2);
      profile = LookupProfile(*card, profileName);
      // Get best profile regarding selected card/port
      if (port == nullptr) { LOG(LogError) << "[PulseAudio] No port '" << portName2 << "' available on sound card " << card->Description; return playbackName; }
      if (profile == nullptr)
      {
        profile = GetBestProfile2(card, port);
        if (profile == nullptr) { LOG(LogError) << "[PulseAudio] No profile available!"; return playbackName; } // should never happend
      }

      { LOG(LogInfo) << "[PulseAudio] Activating profile " << profile->Name << " for card #" << card->Index << ' ' << card->Name; }

      pa_operation* profileOp = pa_context_set_card_profile_by_index(mPulseAudioContext, card->Index, profile->Name.data(), SetProfileCallback,this);
      // Wait for response
      mSignal.WaitSignal(sTimeOut);
      // Release
      pa_operation_unref(profileOp);

      // need to reload sinks after changing card profile
      isAudioDeviceSink = false;
    }
  }

  if (!isAudioDeviceSink)
    PulseEnumerateSinks();

  {
    Mutex::AutoLock lock(mSyncer);
    // profile was changed, audio.device is a card:port:profile, sink must be found
    if (sink == nullptr)
      sink = GetSinkFromCardPort(card, port);
    // Set port
    if (port != nullptr)
    {
      if (sink == nullptr)
        { LOG(LogError) << "[PulseAudio] No sink found!"; return playbackName; } // should never happend again

      { LOG(LogDebug) << "[PulseAudio] Switching sink '" << sink->Name << "' to port " << port->Name; }
      pa_operation* op = pa_context_set_sink_port_by_name(mPulseAudioContext, sink->Name.data(), port->Name.data(), SetPortCallback, this);
      // Wait for result
      mSignal.WaitSignal(sTimeOut);
      // Release
      pa_operation_unref(op);
      { LOG(LogInfo) << "[PulseAudio] Sink '" << sink->Name << "' has been switched to port " << port->Name; }
    }

    // Set sink the default one
    pa_operation* op = pa_context_set_default_sink(mPulseAudioContext, sink->Name.data(), SetSinkCallback, this);
    // Wait for result
    mSignal.WaitSignal(sTimeOut);
    // Release
    pa_operation_unref(op);
     { LOG(LogDebug) << "[PulseAudio] Sink '" << sink->Name << "' has been set as default sink."; }

    // Unmute sink
    op = pa_context_set_sink_mute_by_name(mPulseAudioContext, sink->Name.data(), 0, SetMuteCallback, this);
    // Wait for result
    mSignal.WaitSignal(sTimeOut);
    // Release
    pa_operation_unref(op);
     { LOG(LogDebug) << "[PulseAudio] Sink '" << sink->Name << "' has been unmuted."; }
  }

  // Return new  playback
  if (isAudioDeviceSink)
  {
    if (port != nullptr)
      return String(sink->Name).Append(':').Append(port->Name);
    return String(sink->Name).Append(':');
  }
  return String(card->Name).Append(':').Append(port->Name).Append(':').Append(profile->Name);
}

int PulseAudioController::GetVolume()
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  return Math::clampi(RecalboxConf::Instance().AsInt("audio.volume"),0, 100);
}

void PulseAudioController::SetVolume(int volume)
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  if (mPulseAudioContext == nullptr) return;

  volume = Math::clampi(volume, 0, 100);

  Mutex::AutoLock lock(mSyncer);

  for(Sink& sink : mSinks)
  {
    pa_cvolume volumeStructure;
    pa_cvolume_init(&volumeStructure);
    pa_cvolume_set(&volumeStructure, sink.Channels, (PA_VOLUME_NORM * volume) / 100);

    // Set volume
    pa_operation* op = pa_context_set_sink_volume_by_index(mPulseAudioContext, sink.Index, &volumeStructure, SetVolumeCallback, this);
    // Wait for result
    mSignal.WaitSignal(sTimeOut);
    // Release
    pa_operation_unref(op);
  }
}

int PulseAudioController::GetMusicVolume()
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  return Math::clampi(RecalboxConf::Instance().GetAudioMusicVolume(), 0, 100);
}

void PulseAudioController::SetMusicVolume(int volume)
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);

  volume = Math::clampi(volume, 0, 100);
  Mix_VolumeMusic((volume*MIX_MAX_VOLUME)/100);
  RecalboxConf::Instance().SetAudioMusicVolume(Math::roundi(volume)).Save();
}

void PulseAudioController::Break()
{
  pa_mainloop_quit(mPulseAudioMainLoop, 1);
}

void PulseAudioController::Run()
{

  // Create a mainloop API and connection to the default server
  mPulseAudioMainLoop = pa_mainloop_new();
  pa_mainloop_api* pa_mlapi = pa_mainloop_get_api(mPulseAudioMainLoop);
  mPulseAudioContext = pa_context_new(pa_mlapi, "Recalbox");
  // This function defines a callback so the server will tell us it's state.
  pa_context_set_state_callback(mPulseAudioContext, ContextStateCallback, this);
  // This function connects to the pulse server
  pa_context_connect(mPulseAudioContext, nullptr, pa_context_flags::PA_CONTEXT_NOFLAGS, nullptr);

  // Thread loop
  int result = 0;
  pa_mainloop_run(mPulseAudioMainLoop, &result);

  // Deinit
  pa_context_unref(mPulseAudioContext);
  pa_mainloop_free(mPulseAudioMainLoop);
}

void PulseAudioController::PulseContextConnect()
{
  // Wait for response
  mSignal.WaitSignal(sTimeOut);
  { LOG(LogInfo) << "[PulseAudio] Connection to PulseAudio complete."; }
}

void PulseAudioController::PulseContextDisconnect()
{
  // Disconnect from pulse server
  pa_context_disconnect(mPulseAudioContext);
  { LOG(LogInfo) << "[PulseAudio] Disconnected to Server."; }
}

const PulseAudioController::Profile* PulseAudioController::GetBestProfile2(const PulseAudioController::Card* targetCard, const PulseAudioController::Port* targetPort)
{
  int priority = -1;

  if (targetCard == nullptr || targetPort == nullptr)
    return nullptr;

  // both target card/port cannot be null behind the test above
  const PulseAudioController::Profile* bestProfile = nullptr;
  { LOG(LogDebug) << "[PulseAudio] Get Best profile for Card:Port : " << targetCard->Description << ":" << targetPort->Description; }

  // Card loop
  for(const Card& card : mCards)
  {
    // Card is non-null? Only target the selected card
    if (targetCard != &card) continue;

    for (const Port& port : card.Ports)
    {
      // Port is non-null? Only target the selected port
      if (targetPort != &port) continue;

      for(const Profile& profile : port.Profiles)
      {
        // Get the highest priority port with profile available
        if (profile.Priority > priority && profile.Available)
        {
          priority = profile.Priority;
          bestProfile = &profile;
          { LOG(LogDebug) << "[PulseAudio] Get Best profile result : " << targetCard->Name << ":"  << targetPort->Name << ":" << profile.Name << " with priority=" << profile.Priority; }
        }
      }
    }
  }
  return bestProfile;
}

const PulseAudioController::Profile* PulseAudioController::GetBestProfile(const PulseAudioController::Card*& targetCard, const PulseAudioController::Port*& targetPort)
{
  int priority = -1;
  const Port* selectedPort = nullptr;
  const Card* selectedCard = nullptr;

  { LOG(LogDebug) << "[PulseAudio] Get Best profile for Card : " << ((targetCard != nullptr) ? targetCard->Description : "NULL"); }
  { LOG(LogDebug) << "[PulseAudio] Get Best profile for Port : " << ((targetPort != nullptr) ? targetPort->Description : "NULL"); }

  // Card loop
  for(const Card& card : mCards)
  {
    // Card is non-null? Only target the selected card
    if (targetCard != nullptr && targetCard != &card) continue;

    for (const Port& port : card.Ports)
    {
      // Port is non-null? Only target the selected port
      if (targetPort != nullptr && targetPort != &port) continue;

      bool available = false;
      for(const Profile& profile : port.Profiles)
        available |= profile.Available;

      // Get the highest priority port with profile available
      if (port.Priority > priority && available)
      {
        priority = port.Priority;
        selectedPort = &port;
        selectedCard = &card;
        { LOG(LogDebug) << "[PulseAudio] Get Best profile, card - port selected : " << selectedCard->Description << " - " << selectedPort->Description; }
      }
    }
  }

  // Store selected
  targetCard = selectedCard;
  targetPort = selectedPort;

  // Check port
  if (selectedPort == nullptr || selectedCard == nullptr)
  { LOG(LogWarning) << "[PulseAudio] No port with available profiles!"; return nullptr; }

  // Check available profiles
  if (selectedPort->Profiles.empty())
  { LOG(LogWarning) << "[PulseAudio] Card #" << selectedCard->Index << ' ' << selectedCard->Name << ", Port " << selectedPort->Description << " has no profile!"; return nullptr; }

  // Seek for the highest profile
  const Profile* selectedProfile = selectedPort->Profiles.data();
  for (const Profile& profile : selectedPort->Profiles)
    selectedProfile = (profile.Priority > selectedProfile->Priority) ? &profile : selectedProfile;

  { LOG(LogDebug) << "[PulseAudio] Get Best profile : " << selectedProfile->Description; }

  return selectedProfile;
}

void PulseAudioController::SetDefaultProfiles()
{
  for(Card& card : mCards)
  {
    if (card.HasActiveProfile) continue;
    if (card.Ports.empty()) { LOG(LogWarning) << "[PulseAudio] Card #" << card.Index << ' ' << card.Name << " has no port!"; continue; }

    { LOG(LogInfo) << "[PulseAudio] Card #" << card.Index << ' ' << card.Name << " has no profile activated."; }

    const Card* cardTemp = &card;
    const Port* portTemp = nullptr;
    const Profile* selectedProfile = GetBestProfile(cardTemp, portTemp);
    if (selectedProfile == nullptr) continue;

    // Activate selected profile
    { LOG(LogInfo) << "[PulseAudio] Activating profile " << selectedProfile->Description << " for card #" << card.Index << ' ' << card.Name; }

    pa_operation* profileOp = pa_context_set_card_profile_by_index(mPulseAudioContext, card.Index, selectedProfile->Name.data(), SetProfileCallback, this);
    // Wait for response
    mSignal.WaitSignal(sTimeOut);
    // Release
    pa_operation_unref(profileOp);
  }
}

void PulseAudioController::PulseEnumerateSinks()
{
  mSyncer.Lock();
  mSinks.clear();
  mSyncer.UnLock();

  // Enumerate cards
  { LOG(LogInfo) << "[PulseAudio] Enumerating Sinks."; }
  pa_operation* sinkOp = pa_context_get_sink_info_list(mPulseAudioContext, EnumerateSinkCallback, this);
  // Wait for response
  mSignal.WaitSignal(sTimeOut);
  // Release
  pa_operation_unref(sinkOp);
}

void PulseAudioController::PulseEnumerateCards()
{
  mSyncer.Lock();
  mCards.clear();
  mSyncer.UnLock();

  // Enumerate cards
  { LOG(LogInfo) << "[PulseAudio] Enumerating Cards."; }
  pa_operation* cardOp = pa_context_get_card_info_list(mPulseAudioContext, EnumerateCardCallback, this);
  // Wait for response
  mSignal.WaitSignal(sTimeOut);
  // Release
  pa_operation_unref(cardOp);

  // Activate the best profile on every card if they do not have default profile
  SetDefaultProfiles();

}

void PulseAudioController::PulseSubscribe()
{
  { LOG(LogInfo) << "[PulseAudio] Subscribing to events"; }

  pa_context_set_subscribe_callback(mPulseAudioContext, SubscriptionCallback, this);
  pa_context_subscribe(mPulseAudioContext, PA_SUBSCRIPTION_MASK_ALL, nullptr, nullptr);
}

String PulseAudioController::GetCardDescription(const pa_card_info& info)
{
  String result;

  // Get best name
  const char* cardName = pa_proplist_gets(info.proplist, "alsa.card_name");
  if (cardName == nullptr) cardName = pa_proplist_gets(info.proplist, "bluez.alias");
  if (cardName == nullptr) cardName = pa_proplist_gets(info.proplist, "device.description");
  if (cardName == nullptr) cardName = "Unlabelled device";

  // Change
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    {
      result = cardName;
      result.Remove("bcm2835").Replace("vc4-hdmi", "HDMI");
      break;
    }
    case BoardType::RG351V:
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::PCx86:
    case BoardType::PCx64:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    default:
    {
      result = cardName;
      break;
    }
  }

  if (result.empty()) result.Append("Card #").Append(info.index);

  return result.Trim();
}

const PulseAudioController::Card* PulseAudioController::GetCardByIndex(int index)
{
  for(Card& card : mCards)
    if (card.Index == index)
      return &card;
  return nullptr;
}

String PulseAudioController::GetPortDescription(const pa_sink_port_info& info, AudioIcon& icon)
{
  (void)icon;
  String result = info.description;

  // Change
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::RG351V:
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::PCx86:
    case BoardType::PCx64:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    default: break;
  }

  return result;
}

String PulseAudioController::GetPortDescription(const pa_card_port_info& info, AudioIcon& icon)
{
  (void)icon;
  String result = info.description;
  //String low = result.LowerCase();

  // Change
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::RG351V:
    case BoardType::PCx86:
    case BoardType::PCx64:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    default: break;
  }

  return result;
}

void PulseAudioController::Refresh()
{
  // API Sync'
  Mutex::AutoLock locker(mAPISyncer);
  { LOG(LogDebug) << "[PulseAudio] Refresh in progress."; }
  if (mConnectionState == ConnectionState::Ready)
  {
    // Update default sink name
    UpdateDefaultSink();

    PulseEnumerateSinks();
    PulseEnumerateCards();
  }
  { LOG(LogDebug) << "[PulseAudio] Refresh complete."; }
}

void PulseAudioController::DisableNotification()
{
  Mutex::AutoLock locker(mAPISyncer);
  mNotification = false;
}

void PulseAudioController::EnableNotification()
{
  Mutex::AutoLock locker(mAPISyncer);
  mNotification = true;
}

void PulseAudioController::ReceiveSyncMessage()
{
  // Get new sink/card and default sink name
  { LOG(LogDebug) << "[PulseAudio] Received sync message."; }
  Refresh();

  if (mNotificationInterface != nullptr && mNotification) {
    { LOG(LogDebug) << "[PulseAudio] Send notification on sink change"; }
    mNotificationInterface->NotifyAudioChange();
  }
}

void PulseAudioController::SetOutputPort(String portName)
{
  pa_operation* op = pa_context_set_sink_port_by_name(mPulseAudioContext, mServerInfo.DefaultSinkName.c_str(), portName.c_str(), SetPortCallback, this);
  // Wait for result
  mSignal.WaitSignal(sTimeOut);
  // Release
  pa_operation_unref(op);
}
#pragma clang diagnostic pop
