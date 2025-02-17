//
// Created by bkg2k on 02/09/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <WindowManager.h>
#include <systems/SystemManager.h>
#include <emulators/run/GameLinkedData.h>
#include <scraping/scrapers/IScraperEngineFreezer.h>
#include "sdl2/ISdl2EventNotifier.h"

class GameRunner : public StaticLifeCycleControler<GameRunner>
                 , public IScraperEngineFreezer
{
  public:
    /*!
     * @brief Constructor
     * @param window Window manager
     * @param systemManager System manager
     */
    GameRunner(WindowManager* window, SystemManager& systemManager, ISdl2EventNotifier& sdl2Callback)
      : StaticLifeCycleControler<GameRunner>("GameRunner")
      , mWindowManager(window)
      , mSystemManager(systemManager)
      , mSdl2Callback(sdl2Callback)
    {
    }

    /*!
     * @brief Run game
     * @param game Game to run
     * @param emulator Emulator data
     * @param netplay Netplay data
     * @return True if the game ran fine, false otherwise
     */
    bool RunGame(FileData& game, const EmulatorData& emulator, const GameLinkedData& data);

    /*!
     * @brief Create basic command line for starting a game
     * @param game Game to run
     * @param emulator Emulator data
     * @param core the core to run
     * @param data game linked data
     * @param mapper
     * @param debug debug logs
     * @param demo is it for a demo ?
     * @return The command line
     */
    String CreateCommandLine(const FileData& game, const EmulatorData& emulator, const String& core, const GameLinkedData& data,const InputMapper& mapper, bool debug, bool demo);

    /*!
     * @brief Initialize demo launching
     * @return Controller configuration string
     */
    void SubSystemPrepareForRun();

    /*!
     * @brief Finalize demo run
     */
    void SubSystemRestore();

    /*!
     * @brief Run a game demo
     * @param game Game to run
     * @param emulator Emulator data
     * @param duration Duraction in second
     * @param infoscreenduration Info screen duration in second
     * @return True if the demo exited on user request
     */
    bool DemoRunGame(const FileData& game, const EmulatorData& emulator, int duration, int infoscreenduration);

    /*!
     * @brief Run kodi
     * @return True if Kodi ran fine, false otherwise
     */
    bool RunKodi();

    /*!
     * @brief Instruct threads that a game is currently running
     * @return True if a game is currently running, false otherwise
     */
    static bool IsGameRunning() { return sGameIsRunning; }

    void SetWindowManager(WindowManager *window) { mWindowManager = window; }

  private:
    //! CPU Governance file
    static constexpr const char* sGovernanceFile = "/recalbox/share_init/system/configs/.governances.cfg";

    //! Window manager reference
    WindowManager* mWindowManager;
    //! System manager reference
    SystemManager& mSystemManager;
    //! SDL callback interface
    ISdl2EventNotifier& mSdl2Callback;

    //! Game running flag
    static bool sGameIsRunning;

    /*!
     * @brief Build CRT options from the CRT interface
     * @param system System
     * @param data CRT Data
     * @param demo starting a demo
     * @return Option string
     */
    static String BuildCRTOptions(const SystemData& system, const CrtData& data, RotationType rotation, const bool demo);

    /*!
    * @brief Automatic Game running flag management
    */
    class GameRunning
    {
      public:
        //! Constructor - Set the flag
        GameRunning() { sGameIsRunning = true; }
        //! Destructor - Reset the flag
        ~GameRunning() { sGameIsRunning = false; }
    };

    class ThreadRunner : private Thread
    {
      public:
        /*!
         * @brief Constructor
         * @param runner Runner to stop when the game is over
         * @param command Command to execute
         * @param debug Debug flag
         */
        ThreadRunner(Sdl2Runner& runner, const String& command, bool debug)
          : mRunner(runner)
          , mCommand(command)
          , mDebug(debug)
          , mExitCode(0)
        {
          Thread::Start("GameRun");
        }

        //! Get exit code
        [[nodiscard]] int ExitCode() const { return mExitCode; }

      private:
        //! Runner to stop when the game is over
        Sdl2Runner& mRunner;
        //! Command to execute
        String mCommand;
        //! Debug flag
        bool        mDebug;
        //! Exit code
        int         mExitCode;

        /*
         * Thread implementation
         */

        // Run the thread
        void Run() override;
    };

    /*!
     * @brief Run system command and capture output
     * @param cmd_utf8 Command to execute
     * @param debug log output?
     * @return Return code
     */
    static int Run(const String& cmd_utf8, bool debug);

    /*!
     * @brief Get energy governance for a particular core/emulator
     * @param core Core/Emulateur
     * @return Governance
     */
    static IBoardInterface::CPUGovernance GetGovernance(const String& core);

    /*!
     * @brief Get netplay option regarding the given netplay object
     * @param netplay Netplay object
     * @return Netplay option string
     */
    static String NetplayOption(const FileData& game, const NetPlayData& netplay);

    /*
     * IScraperEngineFreezer implementation
     */

    bool MustFreeze() override { return sGameIsRunning; }
};



