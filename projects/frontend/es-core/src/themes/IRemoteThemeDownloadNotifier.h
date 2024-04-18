//
// Created by bkg2k on 17/04/24.
//
#pragma once

#include "RemoteIllustrationType.h"
#include "utils/os/fs/Path.h"

class IRemoteThemeDownloadNotifier
{
  public:
    //! Default virtual destructor
    virtual ~IRemoteThemeDownloadNotifier() = default;

    /*!
     * @brief Called when a remotye illustration has been downloaded and is ready
     * @param themeIndex Theme index
     * @param type Illustration type
     * @param index Illustration index
     * @param path Illustration path
     */
    virtual void IllustrationReady(int themeIndex, RemoteIllustrationType type, int index, const Path& path) = 0;
};
