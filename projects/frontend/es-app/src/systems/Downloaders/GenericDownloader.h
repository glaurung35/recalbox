//
// Created by bkg2k on 17/04/23.
//
#pragma once

#include "guis/IGuiDownloaderUpdater.h"
#include "systems/SystemData.h"
#include "systems/BaseSystemDownloader.h"

class GenericDownloader : public BaseSystemDownloader
{
  public:
    GenericDownloader(const SystemData& system, IGuiDownloaderUpdater& updater);

    /*
     * ISystemDownloader implementation
     */

    //! Start downloading & installing games
    void StartDownload() override;

  private:
    //! Target system
    const SystemData& mSystem;
};
