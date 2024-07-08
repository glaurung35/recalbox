//
// Created by bkg2k on 30/05/24.
//
#pragma once

#include "GuiMenuBase.h"
#include "usernotifications/ScriptOutputListenerInterface.h"
#include "guis/GuiASyncWaiter.h"

class GuiMenuUserScripts : public GuiMenuBase
                         , public IGuiMenuBase
                         , private ScriptOutputListenerInterface
                         , private Thread
                         , private ISyncMessageReceiver<void>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuUserScripts(WindowManager& window);

    //! Destructor
    ~GuiMenuUserScripts()
    {
      Thread::Stop();
    }

  private:
    //! Script events
    enum class ScriptEvent
    {
      Start, //!< Script just started
      Line,  //!< Script just emitted a line to stdout
      Stop,  //!< Script stopped
    };

    //! Synchronous event
    SyncMessageSender<void> mSender;

    //! Waiter ui
    GuiASyncWaiter* mWaiter;

    //! Script index to run/running
    int mScriptIndexToRun;

    //! Pending packet data
    struct EventData
    {
      //! Packet type
      ScriptEvent mType;
      //! Current script path
      Path mPath;
      //! Current script attribute
      ScriptAttributes mAttributes;
      //! Last line the script wrote to stdout
      String mLastLine;
      //! Complete stdout
      String mStdOut;
      //! Complete stderr
      String mStdErr;
      //! Script error
      bool mError;

      // Start constructor
      EventData(const Path& path, ScriptAttributes attributes)
        : mType(ScriptEvent::Start)
        , mPath(path)
        , mAttributes(attributes)
        , mError(false)
      {}

      // Line constructor
      EventData(const Path& path, ScriptAttributes attributes, const String& line)
        : mType(ScriptEvent::Line)
        , mPath(path)
        , mAttributes(attributes)
        , mLastLine(line)
        , mError(false)
      {}

      // Stop constructor
      EventData(const Path& path, ScriptAttributes attributes, const String& output, bool error, const String& errors)
        : mType(ScriptEvent::Stop)
        , mPath(path)
        , mAttributes(attributes)
        , mStdOut(output)
        , mStdErr(errors)
        , mError(error)
      {}
    };

    //! Pending packets
    std::vector<EventData> mPendingData;
    //! Pending data guardian
    Mutex mGuardian;


    /*!
     * @brief Extract script name from file path
     * @param script Script file path
     * @return Script name
     */
    String ExtractScriptName(const Path& script);

    /*
     * Menu implementation
     */

    /*!
     * @brief Called when a script is lselected
     * @param id Script index
     */
    void SubMenuSelected(int id) override;

    /*
     * Script implementation
     */

    /*!
     * @brief Notify start of a script
     * @param script Script path
     * @param attributes Attributes
     */
    void ScriptStarts(const Path& script, ScriptAttributes attributes) final;

    /*!
     * @brief Receive raw string output from the currently running user script
     * @param line Line string
     */
    void ScriptOutputLine(const Path& script, ScriptAttributes attributes, const String& line) final;

    /*!
     * @brief Notify the script has completed
     * @param error True is error occured
     * @param errors Content of stderr if available and if the script was run synchronously. Empty otherwise
     */
    void ScriptCompleted(const Path& script, ScriptAttributes attributes, const String& output, bool error, const String& errors) final;

    /*
     * Thread implementation
     */

    void Run() override;

    /*
     * Synchronous event implementation
     */

    /*!
     * @brief Receive synchronous message
     */
    void ReceiveSyncMessage() override;
};
