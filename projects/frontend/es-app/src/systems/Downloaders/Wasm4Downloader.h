//
// Created by bkg2k on 17/04/23.
//
#pragma once

#include "systems/BaseSystemDownloader.h"
#include "utils/network/Http.h"
#include <utils/sync/SyncMessageSender.h>
#include "utils/os/system/Thread.h"

enum class DownloadingGameState
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

class Wasm4Downloader : public BaseSystemDownloader
                      , private Thread
                      , private ISyncMessageReceiver<DownloadingGameState>
                      , private Http::IDownload
{
  public:
    /*!
     * @brief Constructor
     * @param updater UI update interface
     */
    Wasm4Downloader(SystemData& wasm4, IGuiDownloaderUpdater& updater);

    //! Destructor
    ~Wasm4Downloader() override { Thread::Stop(); }

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
    static constexpr const char* sRepoURL = "https://gitlab.com/recalbox/packages/game-providers/wasm4/-/archive/main/wasp4-main.zip";

    //! Http request object
    Http mRequest;

    //! Sync messager
    SyncMessageSender<DownloadingGameState> mSender;

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
    void ReceiveSyncMessage(const DownloadingGameState& code) override;


};
