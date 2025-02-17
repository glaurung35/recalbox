#pragma once

#include "BoardType.h"
#include "hardware/messaging/IHardwareNotifications.h"
#include "IBoardInterface.h"
#include "hardware/crt/ICrtInterface.h"
#include "Options.h"
#include <utils/cplusplus/StaticLifeCycleControler.h>
#include <hardware/messaging/HardwareMessageSender.h>

// Forward declaration
class InputCompactEvent;

class Board: public StaticLifeCycleControler<Board>
{
  public:
    /*!
     * @brief Constructor
     * @param notificationInterface Notification interface
     */
    explicit Board(IHardwareNotifications& notificationInterface, const Options& options);

    //! Destructor
    ~Board()
    {
      delete &mBoard;
      delete &mCrtBoard;
    }

    /*!
     * @brief Get board type
     * @return Board type
     */
    BoardType GetBoardType();

    /*!
     * @brief Get memory in megabyte
     * 0 means the memory amount is unknown
     * @return total memory in megabyte
     */
    int TotalMemory();

    /*!
     * @brief Check if the current board can have a CRT plugged in
     * @return True f the current board is compatible with CRT adapter, false otherwise
     */
    bool CanHaveCRTBoard();

    /*!
     * @brief Get Crt board interface
     * @return Crt board interface
     */
    ICrtInterface& CrtBoard() const { return mCrtBoard; }

    /*!
     * @brief Get brightness support
     * @return True if the current board support brightness, false otherwise
     */
    bool HasBrightnessSupport() { return mBoard.HasBrightnessSupport(); }

    /*!
     * @brief Set brightness
     * @param step Step value from 0 to 8
     */
    void SetBrightness(int step) { mBoard.SetBrightness(step); }

    /*!
     * @brief Set lowerst brightness available or even switch off the screen
     * @param step Step value from 0 to 8
     */
    void SetLowestBrightness() { mBoard.SetLowestBrightness(); };

    /*!
     * @brief Check if the current board has battery
     * @return
     */
    bool HasBattery() { return mBoard.HasBattery(); }

    /*!
     * @brief Get battery charge in percent
     * @return Battery charge (-1 = no battery)
     */
    int BatteryChargePercent() { return mBoard.BatteryChargePercent(); }

    /*!
     * @brief Check if the battery is charging
     * @return True = charging, False = discharging or no battery
     */
    bool IsBatteryCharging() { return mBoard.IsBatteryCharging(); }

    /*!
     * @brief Set CPU governance
     * @param cpuGovernance CPU governance
     */
    void SetCPUGovernance(IBoardInterface::CPUGovernance cpuGovernance) { mBoard.SetCPUGovernance(cpuGovernance); }

    /*!
     * @brief Check if this board has extra volume +/- buttons
     * @return True if such buttons are available, false otherwise
     */
    bool HasPhysicalVolumeButtons() { return mBoard.HasMappableVolumeButtons(); }

    /*!
     * @brief Check if this board has extra brightness +/- buttons
     * @return True if such buttons are available, false otherwise
     */
    bool HasPhysicalBrightnessButtons() { return mBoard.HasMappableBrightnessButtons(); }

    /*!
     * @brief Check if the current board supports suspend/resume operations
     * @return True if the board supports suspend/resume operations, false otherwise
     */
    bool HasSuspendResume() { return mBoard.HasSuspendResume(); }

    /*!
     * @brief Suspend!
     */
    void Suspend() { mBoard.Suspend(); }

    /*!
     * @brief Process special input if any
     * @param inputEvent Input to process
     * @return True if the input has been processed, false otherwise
     */
    bool ProcessSpecialInputs(InputCompactEvent& inputEvent, ISpecialGlobalAction* action) { return mBoard.ProcessSpecialInputs(inputEvent, action); }

    /*!
     * @brief Start optional global background processes
     * This method is called when ES starts
     */
    void StartGlobalBackgroundProcesses()
    {
      { LOG(LogInfo) << "[Hardware] Start global Hardware processes"; }
      return mBoard.StartGlobalBackgroundProcesses();
    }

    /*!
     * @brief Stop optional global background processes
     * This method is called when ES stops
     */
    void StopGlobalBackgroundProcesses()
    {
      { LOG(LogInfo) << "[Hardware] Stop global Hardware processes"; }
      return mBoard.StopGlobalBackgroundProcesses();
    }

    /*!
     * @brief Start optional in-game background processes.
     * This method is called when a game starts
     * @param sdlRunner Sdl2Runner object if the board need to interract with SDL inputs
     */
    void StartInGameBackgroundProcesses(Sdl2Runner& sdlRunner)
    {
      { LOG(LogInfo) << "[Hardware] Start in-game Hardware processes"; }
      return mBoard.StartInGameBackgroundProcesses(sdlRunner);
    }

    /*!
     * @brief Stop optional in-game background processes.
     * This method is called when a game stops
     * @param sdlRunner Sdl2Runner object if the board need to interract with SDL inputs
     */
    void StopInGameBackgroundProcesses(Sdl2Runner& sdlRunner)
    {
      { LOG(LogInfo) << "[Hardware] Stop in-game Hardware processes"; }
      return mBoard.StopInGameBackgroundProcesses(sdlRunner);
    }

    /*!
     * @brief The reboot or shutdown is managed by MainRunner, but the board can have some features to manage
     * @return True if a side effect has been triggered
     */
    bool OnRebootOrShutdown()
    {
      { LOG(LogInfo) << "[Hardware] Reboot or shutdown event passed to the board"; }
      return mBoard.OnRebootOrShutdown();
    }

    void HeadphonePlugged() { mBoard.HeadphonePlugged(); }
    void HeadphoneUnplugged() { mBoard.HeadphoneUnplugged(); }

    void SetFrontendCPUGovernor() { mBoard.SetFrontendCPUGovernor(); }
    /*!
    * @return the rotation capabilities for tate mode
    */
    RotationCapability GetRotationCapabilities() const
    {
      return mBoard.GetRotationCapabilities();
    }

    /*!
    * @return if board supports vulkan
    */
    bool HasVulkanSupport() const
    {
      return mBoard.HasVulkanSupport();
    }

  private:
    //! Options
    const Options& mOptions;
    //! Board type
    BoardType mType;
    //! Board memory (Only RPi for now)
    int mMemory;
    //! Synchronous message sender
    HardwareMessageSender mSender;
    //! Real hardware board interface implementation
    IBoardInterface& mBoard;
    //! CRT Interface
    ICrtInterface& mCrtBoard;

    //! Get board interface
    IBoardInterface& GetBoardInterface(HardwareMessageSender& messageSender);
    
    /*!
     * Raspberry model (real models)
     * https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
     */
    enum class RaspberryModel
    {
        OneA        = 0x00,
        OneB        = 0x01,
        OneAPlus    = 0x02,
        OneBPlus    = 0x03,
        TwoB        = 0x04,
        Alpha       = 0x05,
        OneCM1      = 0x06,
        TreeB       = 0x08,
        Zero        = 0x09,
        TreeCM3     = 0x0A,
        ZeroW       = 0x0C,
        TreeBPlus   = 0x0D,
        TreeAPlus   = 0x0E,
        TreeCM3Plus = 0x10,
        FourB       = 0x11,
        Zero2       = 0x12,
        FourHundred = 0x13,
        FourCM4     = 0x14,
        FiveB       = 0x17,
    };

    /*!
     * @brief Extract RPi modem
     * @param revision raw revision number
     * @return RPi model
     */
    static BoardType GetPiModel(unsigned int revision);

    /*!
     * @brief Extract RPi memory size
     * @param revision raw revision number
     * @return memory size en megabyte
     */
    static int GetPiMemory(unsigned int revision);

    /*!
     * @brief Get a valid instance of Crt board
     * @return Crt board implementation
     */
    ICrtInterface& GetCrtBoard();
};
