//
// Created by bkg2k on 07/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <utils/os/system/IThreadPoolWorkerInterface.h>
#include <guis/GuiWaitLongExecution.h>

// Forward declaration
class SystemManager;
class SystemData;

class MenuNetplay : public Menu
                     , private ISingleSelectorChanged<RecalboxConf::Relay>
                     , private ISwitchChanged
                     , private ISubMenuSelected
                     , private IEditableChanged
                     , private ILongExecution<bool, bool>
                     , private IThreadPoolWorkerInterface<FileData*, FileData*>
                     , private IActionTriggered
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuNetplay(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      Enabled,
      UserName,
      Port,
      Mitm,
      Passwords,
      Hash,
    };

    //! Thread pool for // CRC computations
    ThreadPool<FileData*, FileData*> mThreadPool;
    //! Event
    Signal mWaiter;
    //! Syncher
    Mutex mSyncher;

    //! System manager
    SystemManager& mSystemManager;

    //! Total games
    int mTotalGames;
    //! Remaining games
    int mRemainingGames;
    //! Sored progress
    int mPreviousProgressPercent;
    //! Reference to long operation object
    GuiWaitLongExecution<bool, bool>* mOperation;

    //! Get O/C List
    SelectorEntry<RecalboxConf::Relay>::List GetMitmEntries();

    /*
     * IEditableComponent implementation
     */

    void MenuEditableChanged(int id, const String& text) final;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * IOptionListComponent implementation
     */

    void MenuSingleChanged(int id, int index, const RecalboxConf::Relay& value) final;

    /*
     * ISWitchComponent implementation
     */

    void MenuSwitchChanged(int id, bool& status) final;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) final;

    /*
     * ILongExecution implementation
     */

    /*!
     * @brief Execture network operation
     * @param parameter Network operation required to execute
     * @return True if the operation was successful
     */
    bool Execute(GuiWaitLongExecution<bool, bool>& from, const bool& parameter) final;

    /*!
     * @brief Receive the status of network operations
     * @param parameter original input parameter
     * @param result Result state
     */
    void Completed(const bool& parameter, const bool& result) final { (void)parameter; (void)result; }

    /*
     * Thread implementation
     */

    /*!
     * @brief The main runner. Implement here the task to process a feed object
     * @param feed Feed object to process
     * @return Result Object
     */
    FileData* ThreadPoolRunJob(FileData*& feed) final;

    /*
     * Hash
     */
    void StartHashing();
};



