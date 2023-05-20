//
// Created by gugue_u on 01/04/2022.
//

#pragma once


#include "hardware/crt/CrtAdapterType.h"
#include "hardware/Board.h"
#include "CrtConf.h"
#include "emulators/EmulatorData.h"

class JammaData
{
  public:
    JammaData() {};
    bool ShouldConfigureJammaConfiggen() const {
      return  (Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma ||
               Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJammaPoll);
    }
    bool ShouldSwitchTo6ButtonLayout(const EmulatorData& emulator) const {
      return  ShouldConfigureJammaConfiggen() && CrtConf::Instance().GetSystemCRTJamma6Btns() &&
               !(emulator.Emulator() == "libretro" && (emulator.Core() == "fbneo" || emulator.Core() == "mame2015" ));
    }
    std::string JammaControlType(const EmulatorData& emulator) const {
      if(ShouldSwitchTo6ButtonLayout(emulator)) {
        return "6btns";
      } else {
        return CrtConf::Instance().GetSystemCRTJammaNeogeoLayout();
      }
    }
};
