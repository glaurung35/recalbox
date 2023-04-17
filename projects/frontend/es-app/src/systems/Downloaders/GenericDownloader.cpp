//
// Created by bkg2k on 17/04/23.
//

#include "GenericDownloader.h"

GenericDownloader::GenericDownloader(const SystemData& system, IGuiDownloaderUpdater& updater)
  : BaseSystemDownloader(updater)
  , mSystem(system)
{
}

void GenericDownloader::StartDownload()
{

}
