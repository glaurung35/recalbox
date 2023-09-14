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
  private:
    bool systemIsDreamcastArcade(const std::string& systemName) const
    {
      return systemName == "atomiswave" || systemName == "naomigd" || systemName == "naomi";
    }
  public:
    JammaData() {};
    bool ShouldConfigureJammaConfiggen() const {
      return  (Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma ||
               Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJammaV2);
    }

    bool ShouldSwitchTo6ButtonLayout(const FileData& game, const EmulatorData& emulator) const {
      return  ShouldConfigureJammaConfiggen() && CrtConf::Instance().GetSystemCRTJamma6Btns() &&
               !(emulator.Emulator() == "libretro" &&
               (emulator.Core() == "fbneo" || emulator.Core() == "mame2015" || systemIsDreamcastArcade(game.System().Name()))
               );
    }
    std::string JammaControlType(const FileData& game, const EmulatorData& emulator) const {
      if(ShouldSwitchTo6ButtonLayout(game, emulator)) {
        return "6btns";
      } else {
        return CrtConf::Instance().GetSystemCRTJammaNeogeoLayout();
      }
    }
};
