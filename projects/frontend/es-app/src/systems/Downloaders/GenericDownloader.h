//
// Created by bkg2k on 17/04/23.
//
#pragma once

#include "guis/IGuiDownloaderUpdater.h"
#include "systems/SystemData.h"
#include "systems/BaseSystemDownloader.h"
#include "utils/network/Http.h"
#include "utils/os/system/Thread.h"

enum class GenericDownloadingGameState
{
    // Actions
    Downloading,       //!< Downloading games
    Extracting,        //!< Extracting
    UpdatingMetadata,  //!< Update metadata
    // End
    Completed,         //!< Completed
    // Errors
    WriteOnlyShare,    //!< Share is write only!
    DownloadError,     //!< Error downloading file(s)
};

class GenericDownloader : public BaseSystemDownloader
                        , private Thread
                        , private ISyncMessageReceiver<GenericDownloadingGameState>
                        , private Http::IDownload
{
  public:
    GenericDownloader(SystemData& system, IGuiDownloaderUpdater& updater);

    /*
     * ISystemDownloader implementation
     */

    //! Start downloading & installing games
    void StartDownload() override;

    /*
     * Thread Implementation
     */

    /*!
     * @brief Main thread routine
     */
    void Run() override;

    /*
     * Http::IDownload implementation
     */

    /*!
     * @brief Notify of download progress
     * @param http HTTP request
     * @param currentSize downloaded bytes
     * @param expectedSize total expected bytes
     */
    void DownloadProgress(const Http& http, long long currentSize, long long expectedSize) override;

  private:
    //! Game fetching URL
    static constexpr const char* sRepoBaseURL = "https://gitlab.com/recalbox/packages/game-providers/%s/-/archive/main/wasp4-main.zip";

    //! Http request object
    Http mRequest;

    //! Sync messager
    SyncMessageSender<GenericDownloadingGameState> mSender;

    //! Time reference
    DateTime mTimeReference;

    //! Wasm4 system reference
    SystemData& mSystem;

    //! File length
    long long mTotalSize;
    //! Downloaded length
    long long mCurrentSize;

    //! Extracted games
    int mGames;

    /*!
     * @brief Receive synchronous code
     */
    void ReceiveSyncMessage(const GenericDownloadingGameState& code) override;
};
