//
// Created by gugue_u on 01/04/2022.
//

#pragma once


#include "RecalboxConf.h"
#include "systems/SystemData.h"
#include "systems/SystemManager.h"

class SuperGameBoyData
{
  public:
  SuperGameBoyData()
      : mEnableSuperGameBoy(RecalboxConf::Instance().GetSuperGameBoy() == "sgb"),
        mConfigured(false)
    {};

    bool Enabled() const { return mEnableSuperGameBoy; }
    const std::string Core(const FileData& game, const std::string& defaultCore) const {
      // Change to mgba if user did not overload the core for gameboy
      if(!game.System().Manager().Emulators().ConfigOverloaded(game))
        return "mgba";
      else
        return defaultCore;
    }
    void Enable(bool enabled) { mEnableSuperGameBoy = enabled; mConfigured = true; }
    bool ShouldAskForSuperGameBoy(const SystemData& system) const {
      return system.IsGameBoy() && (!mEnableSuperGameBoy && !mConfigured && RecalboxConf::Instance().GetSuperGameBoy() == "ask");
    }

  private:
    bool mEnableSuperGameBoy;
    bool mConfigured;
};
