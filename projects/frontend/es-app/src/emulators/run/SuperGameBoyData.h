//
// Created by gugue_u on 01/04/2022.
//

#pragma once


#include "RecalboxConf.h"
#include "systems/SystemData.h"

class SuperGameBoyData
{
  public:
  SuperGameBoyData()
      : mEnableSuperGameBoy(RecalboxConf::Instance().GetSuperGameBoy() == "sgb"),
        mConfigured(false)
    {};

    bool Enabled() const { return mEnableSuperGameBoy; }
    const std::string Core() const {
      BoardType board = Board::Instance().GetBoardType();
      if(board == BoardType::Pi0 || board == BoardType::Pi1 || board == BoardType::Pi02 || board == BoardType::Pi3)
      {
        return "mgba";
      }else {
        return "mesen_s";
      }
    }
    void Enable(bool enabled) { mEnableSuperGameBoy = enabled; mConfigured = true; }
    bool ShouldAskForSuperGameBoy(const SystemData& system) const {
      return system.IsGameBoy() && (!mEnableSuperGameBoy && !mConfigured && RecalboxConf::Instance().GetSuperGameBoy() == "ask");
    }

  private:
    bool mEnableSuperGameBoy;
    bool mConfigured;
};
