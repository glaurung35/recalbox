//
// Created by bkg2k on 26/12/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <hardware/Board.h>
#include <CrtConf.h>
#include "../EmulatorData.h"
#include "RotationManager.h"

class CrtData
{
  public:
    //! Video system
    enum class CrtRegion
    {
      AUTO, //!< Automatic selection
      JP,  //!< Forced Japan
      US, //!< Forced US
      EU, //!< Forced Europe
    };

    enum class CrtVideoStandard
    {
        AUTO, //!< Automatic selection
        PAL,  //!< Forced Pal
        NTSC, //!< Forced Ntsc
    };

    enum class CrtMode
    {
      Auto, //!< Automatic selection for the screen. Will take in account the screen capabilities
      Force240p, //!< Force 240p (on 15khz screen -> avoid interlaced, on Multisync force 240p)
      Force480p, //<! Force 480p mode on 31kHz screens
      DoubleFreq //<! Force 240p@120Hz on 31Khz screens
    };

    static std::string CrtModeToString(CrtMode mode) {
      switch (mode)
      {
        case CrtMode::Force240p: return "240p";
        case CrtMode::Force480p: return "480p";
        case CrtMode::DoubleFreq: return "doublefreq";
        case CrtMode::Auto:
        default: return "auto";
      }
    }



    //! Default constructor
    CrtData()
      : mCrt(&Board::Instance().CrtBoard())
      , mConf(&CrtConf::Instance())
      , mRegionOrVideoStandardConfigured(false)
      , mForceResolutionConfigured(false)
      , mVideoStandard(CrtVideoStandard::AUTO)
      , mRegion(CrtRegion::AUTO)
      , mCrtMode(CrtMode::Auto)
    {
    }


    /*!
     * @brief Check if there is a CRT board and the user requested to choose individual NTSC options
     * @return True if the class needs to be configured, false otherwise
     */
    [[nodiscard]] bool IsRegionOrStandardConfigured() const
    {
      if (!mRegionOrVideoStandardConfigured)
        if (mCrt->IsCrtAdapterAttached())
          if (mConf->GetSystemCRTGameRegionSelect())
            return true;
      return false;
    }

    /*!
     * @brief Configure crt data
     * @param ntsc True for NTSC, false for PAL
     */
    void ConfigureVideoStandard(CrtVideoStandard standard)
    {
        mVideoStandard = standard;
        mRegionOrVideoStandardConfigured = true;
    }

    void ConfigureRegion(CrtRegion region)
    {
        mRegion = region;
        mRegionOrVideoStandardConfigured = true;
    }

    /*!
     * @brief Check if the target system requires choosing between PAL or NTSC
     * @param system target system
     * @return True if the choice is required, false otherwise
     */
    [[nodiscard]] bool MustChoosePALorNTSC(const SystemData& system) const
    {
      return system.Descriptor().CrtMultiRegion() &&        // System must support multi-region
             mCrt->IsCrtAdapterAttached() &&
             !mCrt->MustForce50Hz() && // & hardware must not force 50hz
             mCrt->GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHz15; // & and we are 15khz
    }

    /*!
     * @brief Check if there is a CRT board and the user requested to choose individual 480 or 240 options
     * @return True if the class needs to be configured, false otherwise
     */
    [[nodiscard]] bool IsForceResolutionSelectionConfigured() const
    {
      if (!mForceResolutionConfigured)
        if (mCrt->IsCrtAdapterAttached())
          if (mConf->GetSystemCRTGameResolutionSelect())
            return true;
      return false;
    }

    /*!
     * @brief Configure crt data
     * @param highRez True for 480, false for 240
     */
    void ConfigureForceResolution(CrtMode mode)
    {
      if (!mForceResolutionConfigured)
      {
        mCrtMode = mode;
        mForceResolutionConfigured = true;
      }
    }

    /*!
     * @brief check if the game is HD
     * @param game
     * @param emulator
     * @return
     */
    bool GameIsHD(FileData* game, const EmulatorData& emulator) const
    {
      bool gameIsHd = game->System().Descriptor().CrtHighResolution();
      if(game->System().IsArcade())
      {
        String emu = emulator.Emulator();
        String core =  emulator.Core();
        const ArcadeDatabase* database = game->System().ArcadeDatabases().LookupDatabase(*game, emu, core);
        if (database != nullptr){
          const ArcadeGame* arcade = database->LookupGame(*game);
          if(arcade != nullptr)
            gameIsHd |= (arcade->ScreenRotation() == RotationType::None && arcade->Height() >= 480);
        }
      }
      return gameIsHd;
    }

    /*!
     * @brief Check if the target system requires choosing between 240 or 480
     * @param game target game
     * @return True if the choice is required, false otherwise
     */
    [[nodiscard]] bool MustChooseResolution(FileData* game, const EmulatorData& emulator) const
    {
      bool gameIsHd = GameIsHD(game, emulator);
      if(RotationManager::IsTateOnYokoOrYokoOnTate(*game)) return false;
      // If 15Khz, the system must support high rez and the interlaced must be supported by board
      // If 31khz, the board must support 120Hz
      // If multisync1524, return true if the system supports hd and the 31 + 15 is supported
      // If multisync15 31 or trifreq, return true
      ICrtInterface::HorizontalFrequency freq = Board::Instance().CrtBoard().GetHorizontalFrequency();
      return (gameIsHd && freq == ICrtInterface::HorizontalFrequency::KHz15 && Board::Instance().CrtBoard().HasInterlacedSupport())
          || (freq == ICrtInterface::HorizontalFrequency::KHz31 && Board::Instance().CrtBoard().Has120HzSupport())
          || (gameIsHd && freq == ICrtInterface::HorizontalFrequency::KHzMulti1525)
          || freq >= ICrtInterface::HorizontalFrequency::KHzMulti1531;
    }

    /*
     * Accessors
     */

    [[nodiscard]] CrtMode GetCrtMode() const { return mCrtMode; }
    [[nodiscard]] CrtScanlines Scanlines(const SystemData& system) const
    {
      return ((mCrtMode == CrtMode::Auto || mCrtMode == CrtMode::Force480p) && !system.Descriptor().CrtHighResolution() &&
                    (Board::Instance().CrtBoard().GetHorizontalFrequency() >= ICrtInterface::HorizontalFrequency::KHzMulti1531)) ?
                    CrtConf::Instance().GetSystemCRTScanlines31kHz() : CrtScanlines::None;
    }
    [[nodiscard]] CrtVideoStandard VideoStandard() const { return mVideoStandard; }
    [[nodiscard]] CrtRegion Region() const { return mRegion; }

  private:
    //! ICrtInterface reference
    ICrtInterface* mCrt;
    //! Configuration
    CrtConf* mConf;
    //! NTSC configured
    bool mRegionOrVideoStandardConfigured;
    //! forced resolution configured
    bool mForceResolutionConfigured;
    //! Video system (default: auto
    CrtVideoStandard mVideoStandard;
    CrtRegion mRegion;
    //! 480? (default: 240p)
    CrtMode mCrtMode;
};