//
// Created by digitalLumberjack on 20/06/2022.
//

#pragma once
#include "hardware/IBoardInterface.h"
#include "utils/os/system/Thread.h"
#include "PiPowerAndTempThread.h"

class PiBoard : public IBoardInterface
{
  public:
    explicit PiBoard(HardwareMessageSender &messageSender, BoardType boardType)
        : IBoardInterface(messageSender), powerThread(messageSender, boardType)
    {
    }
  /*!
  * @brief Has vulkan support
  */
    bool HasVulkanSupport() final
    { return mBoardType == BoardType::Pi5 || mBoardType == BoardType::Pi4; }

  private:
    PiPowerAndTempThread powerThread;
    BoardType mBoardType;
};
