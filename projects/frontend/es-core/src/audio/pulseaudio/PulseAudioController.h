//
// Created by bkg2k on 19/12/2020.
//
#pragma once

#include <audio/IAudioController.h>
#include <audio/IAudioNotification.h>
#include <pulse/pulseaudio.h>
#include <vector>
#include <utils/os/system/Thread.h>
#include <utils/os/system/Signal.h>
#include <utils/sync/SyncMessageSender.h>

class PulseAudioController: public IAudioController
                          , private Thread
                          , private ISyncMessageReceiver<void>

{
  public:
    /*!
     * @brief Default constructor
     */
    PulseAudioController();

    /*!
     * @brief Destructor
     */
    ~PulseAudioController() override;

    /*
     * Audio Controller implementation
     */

    /*!
     * @brief Return device list using identifier/displayable name
     * @return Device list
     */
    IAudioController::DeviceList GetPlaybackList() override;

    /*!
     * @brief Set default playback device by name
     * @param playbackName device name
     * @return Actual selected device name
     */
    String SetDefaultPlayback(const String& playbackName) override;

    /*!
     * @brief Get current running playback name
     * @return Actual playback name in the form of card:profile:port
     */
    String GetActivePlaybackName() override;

    /*!
     * @brief Get volume level, from 0 to 100
     * @return Volume level
     */
    int GetVolume() override;

    /*!
     * @brief Set global volume from 0 to 100
     * @param volume Volume to set
     */
    void SetVolume(int volume) override;

    /*!
     * @brief Force the implementation to refresh all its internal objects
     */
    void Refresh() override;

    /*!
     * @brief Disable notification on sink or card change
     */
    void DisableNotification() override;

    /*!
     * @brief Enable notification on sink or card change
     */
    void EnableNotification() override;

  private:
    //! Timeout
    static constexpr int sTimeOut = 800; //! 800ms timeout

    struct Profile
    {
      String Name;        //!< Device name
      String Description; //!< Description
      int Priority;            //!< Profile priority
      bool Available;          //!< Profile available
    };

    struct Port
    {
      std::vector<Profile> Profiles; //!< Available profile list for this port
      String Name;              //!< Device name
      String Description;       //!< Description
      int InternalIndex;             //!< Internal index (not PA index)
      int Priority;                  //!< Priority
      AudioIcon Icon;                //!< Icon
      bool Available;                //!< Available? (plugged)
    };

    struct Sink
    {
      String Name;                   //!< Device name
      String Description;            //!< Device description
      std::vector<Port> Ports;            //!< Available port list
      int Channels;                       //!< Channel count
      int Index;                          //!< Device index in pulseaudio context
      int CardIndex;                      //!< Card ID this sink is linked to
      int State;                          //!< Current sink state (RUNNING, SUSPENDED, ...)
      String ActivePort;             //!< Selected port
    };

    struct Card
    {
      std::vector<Port> Ports;       //!< Available port list
      std::vector<Profile> Profiles; //!< Available profile list
      String Name;              //!< Card name
      String Description;       //!< Card Description
      int Index;                     //!< Device index in pulseaudio context
      bool HasActiveProfile;         //!< Has an active profile already set?
      String ActiveProfile;     //!< Name of active profile
    };

    typedef struct ServerInfo
    {
      String DefaultSinkName; //!< Name of the current selected and running sink
    } ServerInfo;

    //! Pulseaudio connection state
    enum class ConnectionState
    {
      NotConnected, //!< Not yet connected
      Ready,        //!< Ready!
      Closed,       //!< Connection close
    };

    //! Source enumeration state state
    enum class EnumerationState
    {
      Starting,    //!< Just start!
      Enumerating, //!< Enumerating devices
      Complete,    //!< Enumeration complete
    };

    //! APIs Syncer
    Mutex mAPISyncer;

    //! Card list
    std::vector<Card> mCards;
    //! Sink list
    std::vector<Sink> mSinks;
    //! Server Information
    ServerInfo mServerInfo;
    //! Internal Syncer
    Mutex mSyncer;

    //! Connection state
    ConnectionState mConnectionState;

    //! PulseAudio Context
    pa_context* mPulseAudioContext;
    //! PulseAudio Mainloop handle
    pa_mainloop* mPulseAudioMainLoop;
    //! Signal
    Signal mSignal;

    //! Notifier Sync messager
    SyncMessageSender<void> mEvent;

    //! Audio notification
    IAudioNotification* mNotificationInterface;

    //! Enable/disable notifications
    bool mNotification;

    /*!
     * @brief Initialize all
     */
    void Initialize();

    /*!
     * @brief Finalize all
     */
    void Finalize();

    /*
     * Tools
     */

    const Card* FirstCard();

    static const Port* FirstPort(const Card& card);

    const Card* LookupCard(const String& name);

    static const Port* LookupPort(const Card& card, const String& name);

    static bool HasPort(const Sink& sink, const Port& port);

    const Sink* LookupSink(const String& name);

    const Sink* GetSinkFromCardPort(const Card* card, const Port* port);

    const Sink * GetSinkFromName(const String& name);

    static const Profile* LookupProfile(const Card& card, const String& name);

    void UpdateDefaultSink();

    static void AddSpecialPlaybacks(IAudioController::DeviceList& list);

    bool IsPortAvailable(const String& portName);

    /*!
     * @brief Give the oportunity to process special playback regarding the current hardware
     * set the flag allprocessed to true will stop the playback processing right after this method
     * not tring to use common PulseAudio API
     * @param originalPlaybackName Required playback
     * @param allprocessed if set to true, the playback processing will stop right after this method call
     * @return modified or unmodified playback name
     */
    String AdjustSpecialPlayback(const String& originalPlaybackName, bool& allprocessed);

    /*
     * Pulse Audio callback
     */

    /*!
     * @brief Pulseaudio state callback
     * @param context Pulseaudio context
     * @param userdata This
     */
    static void ContextStateCallback(pa_context *context, void *userdata);

    /*!
     * @brief Callback used to retrieve Sinks
     * @param context Pulseaudio context
     * @param info Sink information structure
     * @param eol End-of-list flag
     * @param userdata This
     */
    static void EnumerateSinkCallback(pa_context *context, const pa_sink_info *info, int eol, void *userdata);

    /*!
     * @brief Callback used to retrieve Cards
     * @param context Pulseaudio context
     * @param info Sink information structure
     * @param eol End-of-list flag
     * @param userdata This
     */
    static void EnumerateCardCallback(pa_context* context, const pa_card_info* info, int eol, void* userdata);

    /*!
     * @brief Callback called when profile is set
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void SetProfileCallback(pa_context *context, int success, void *userdata);

    /*!
     * @brief Callback called when the sink changed
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void SetSinkCallback(pa_context *context, int success, void *userdata);

    /*!
     * @brief Callback called when the port changed
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void SetPortCallback(pa_context *context, int success, void *userdata);

    /*!
     * @brief Callback called when a sink is muted/unmuted
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void SetMuteCallback(pa_context *context, int success, void *userdata);

    /*!
     * @brief Subscription callback
     * @param context Pulseaudio context
     * @param type Event type
     * @param index Object index
     * @param userdata This
     */
    static void SubscriptionCallback(pa_context *context, pa_subscription_event_type_t type, uint32_t index, void *userdata);

    /*!
     * @brief Callback called when volume is changed
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void SetVolumeCallback(pa_context *context, int success, void *userdata);

    /*!
     * @brief Callback called to get server information
     * @param context Pulseaudio context
     * @param success Success flag
     * @param userdata This
     */
    static void GetServerInfoCallback(pa_context *context, const pa_server_info *i, void *userdata);

    /*
     * Thread implementation
     */

    //! Break blocking method in thread loop
    void Break() override;

    //! Thread loop
    void Run() override;

    /*
     * PulseAudio api
     */

    //! Connect to pulse Audio server
    void PulseContextConnect();

    //! Disconnect from Pulse Audio server
    void PulseContextDisconnect();

    //! Enumerates all cards and thair sub objects
    void PulseEnumerateCards();

    //! Enumerates outputs (sinks)
    void PulseEnumerateSinks();

    //! Subscribe to all pulse audio events
    void PulseSubscribe();

    //! Activate best profile for profile-less cards
    void SetDefaultProfiles();

    /*!
     * @brief Get overall best profile, regarding the given information
     * @param card Card (may be null)
     * @param port Port (may be null)
     * @return Best profile or null if no profile is available
     */
    const Profile* GetBestProfile(const Card*& card, const Port*& port);

    /*!
     * @brief Get overall best profile, regarding the given information
     * @param card Card
     * @param port Port
     * @return Best profile or null if no profile is available
     */
    const Profile* GetBestProfile2(const Card* card, const Port* port);

    /*!
     * @brief Build the best card name from the card property set
     * @param info Card info structure
     * @return Card name
     */
    static String GetCardDescription(const pa_card_info& info);

    /*!
     * @brief Get card by card index
     * @param index Card index
     * @return Card
     */
    const Card* GetCardByIndex(int index);

    /*!
     * @brief Build the best port name from the port properties
     * @param info Port info structure
     * @return Port name
     */
    static String GetPortDescription(const pa_card_port_info& info, AudioIcon& icon);
    static String GetPortDescription(const pa_sink_port_info& info, AudioIcon& icon);

    /*!
     * @brief Get icon from port information
     * @param info Port info structure
     * @return Icon
     */
    static AudioIcon GetPortIcon(const pa_card_port_info& info);
    static AudioIcon GetPortIcon(const pa_sink_port_info& info);

    /*!
     * @brief Count available profiles for a specific port
     * @param Port
     * @return uint count
     */
    static uint CountAvailableProfiles(const Port& port);

    /*!
     * @brief Receive synchronous events
     */
    void ReceiveSyncMessage();

    /*!
     * @brief Set notification callback to call when a sink is added or removed
     */
    void SetNotificationCallback(IAudioNotification* callback) override { mNotificationInterface = callback; }

    /*!
     * @brief Clear notification callback
     */
    void ClearNotificationCallback() override { mNotificationInterface = nullptr; }

    /*!
     * @brief Set output port name of the current sink
     */
    void SetOutputPort(const String portName);
    int GetMusicVolume();
    void SetMusicVolume(const int);
};
