//
// Created by bkg2k on 17/04/23.
//
#pragma once

#include <guis/IGuiDownloaderUpdater.h>

class BaseSystemDownloader
{
  protected:
    //! UI interface
    IGuiDownloaderUpdater& mUpdater;
    //! Stop ASAP?
    bool mStopAsap;

  public:
    //! Default required destructor
    virtual ~BaseSystemDownloader() = default;

    //! Constructor
    explicit BaseSystemDownloader(IGuiDownloaderUpdater& updater)
      : mUpdater(updater)
      , mStopAsap(false)
    {};

    /*!
     * @brief Start download & install games
     */
    virtual void StartDownload() = 0;

    /*!
     * @brief User cancelled: must quit ASAP!
     */
    void MustExitAsap() { mStopAsap = true; };
};
