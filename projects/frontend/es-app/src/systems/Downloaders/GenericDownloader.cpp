//
// Created by bkg2k on 17/04/23.
//

#include "GenericDownloader.h"
#include "utils/locale/LocaleHelper.h"
#include "utils/Files.h"
#include "utils/Zip.h"

GenericDownloader::GenericDownloader(SystemData& system, IGuiDownloaderUpdater& updater)
  : BaseSystemDownloader(updater)
  , mSender(*this)
  , mSystem(system)
  , mTotalSize(0)
  , mCurrentSize(0)
  , mGames(0)
{
}

void GenericDownloader::StartDownload()
{
  // start the thread if not aleady done
  Thread::Start("generic-dl");
}

void GenericDownloader::Run()
{
  String title(_("DOWNLOADING GAMES FOR %s"));
  mUpdater.UpdateTitleText(title.Replace("%s", mSystem.FullName()));
  mUpdater.UpdateMainText(_("Downloading free games from Recalbox repositories. Please wait..."));
  { LOG(LogDebug) << "[GenericDownloader] Download files for " << mSystem.FullName(); }

  // Seek for the right folder
  const RootFolderData& roots = mSystem.MasterRoot();
  RootFolderData* targetRoot = nullptr;
  Path output;
  for(RootFolderData* root : roots.SubRoots())
    if (!root->ReadOnly() && root->RomPath().StartWidth("/recalbox/share/"))
    {
      output = root->RomPath();
      targetRoot = root;
    }
  if (output.IsEmpty()) { mSender.Send(GenericDownloadingGameState::WriteOnlyShare); return; }

  // Get destination filename
  Path destination("/recalbox/share/system/download.tmp");
  { LOG(LogDebug) << "[GenericDownloader] Target path " << destination.ToString(); }

  // Source URL
  String source(sRepoBaseURL);
  source.Replace("%s", mSystem.Name());

  // Download
  if (mStopAsap) return;
  destination.Delete();
  mTimeReference = DateTime();
  if (!mRequest.Execute(source, destination, this)) { mSender.Send(GenericDownloadingGameState::DownloadError); return; }


  // Extract
  { LOG(LogDebug) << "[GenericDownloader] Copying games"; }
  Zip zip(destination);
  // Fill in storage
  mTotalSize = zip.Count();
  mCurrentSize = 0;
  String gamelist;
  for(int i = (int)mTotalSize; --i >= 0;)
  {
    if (mStopAsap) return;
    Path relativePath(zip.FileName(i));
    Path destinationPath = output / relativePath.FromItem(1);
    { LOG(LogDebug) << "[GenericDownloader] File: " << destinationPath.ToString(); }
    if (relativePath.Filename() == "gamelist.xml") gamelist = zip.Content(i);
    else
    {
      destinationPath.Directory().CreatePath();
      Files::SaveFile(destinationPath, zip.Content(i));
      if (mSystem.Descriptor().Extension().Contains(relativePath.Extension().ToLowerCase())) mGames++;
    }
    mCurrentSize++;
    mSender.Send(GenericDownloadingGameState::Extracting);
    usleep(20000); // Let display refreshing
  }

  // Populate
  { LOG(LogDebug) << "[GenericDownloader] Populate gamelist"; }
  XmlDocument gameList;
  XmlResult result = gameList.load_string(gamelist.data());
  if (result)
  {
    XmlNode games = gameList.child("gameList");
    mTotalSize = std::distance(games.children().begin(), games.children().end());
    mCurrentSize = 0;
    mSender.Send(GenericDownloadingGameState::UpdatingMetadata);
    FileData::StringMap doppleGanger;
    if (mStopAsap) return;
    targetRoot->BuildDoppelgangerMap(doppleGanger, true);
    if (mStopAsap) return;
    String ignoreList(',');
    ignoreList.Append(mSystem.Descriptor().IgnoredFiles()).Append(',');
    targetRoot->PopulateRecursiveFolder(*targetRoot, Strings::ToLowerASCII(mSystem.Descriptor().Extension()), ignoreList, doppleGanger);

    for (const XmlNode fileNode: games.children())
    {
      // Build a temporary game object...
      if (String(fileNode.name()) == "game")
      {
        Path path = output / Xml::AsString(fileNode, "path", "");
        FileData game(path, *targetRoot);
        // ...and deserialize metadata into
        game.Metadata().Deserialize(fileNode, output);
        // Lookup real game
        FileData* realGame = targetRoot->LookupGameByFilePath(path.ToString());
        if (realGame == nullptr)
        { LOG(LogError) << "[GenericDownloader] Cannot lookup real game!"; }
        else
        {
          // Merge data from
          realGame->Metadata().Merge(game.Metadata());
          // Special process for game name
          String name(realGame->Name());
          if (name.empty() || name == realGame->RomPath().FilenameWithoutExtension())
            realGame->Metadata().SetName(game.Name());
        }
      }
      mCurrentSize++;
      mSender.Send(GenericDownloadingGameState::UpdatingMetadata);
      usleep(20000); // Let display refreshing
    }
  }
  else { LOG(LogError) << "[GenericDownloader] Cannot load remote gamelist!"; }

  // Delete temp file
  destination.Delete();
  mSender.Send(GenericDownloadingGameState::Completed);
}

void GenericDownloader::DownloadProgress(const Http &http, long long int currentSize, long long int expectedSize)
{
  (void)http;
  // Store data and synchronize
  mTotalSize = expectedSize;
  mCurrentSize = currentSize;
  mSender.Send(GenericDownloadingGameState::Downloading);
}

void GenericDownloader::ReceiveSyncMessage(const GenericDownloadingGameState &code)
{
  switch(code)
  {
    case GenericDownloadingGameState::Downloading:
    {
      // Load size into progress bar component
      mUpdater.UpdateProgressbar(mCurrentSize, mTotalSize);

      // Elapsed time
      if (mCurrentSize != 0 && mCurrentSize < mTotalSize)
      {
          TimeSpan elapsed = DateTime() - mTimeReference;
          TimeSpan eta((elapsed.TotalMilliseconds() * (mTotalSize - mCurrentSize)) / mCurrentSize);

          std::string text = _("Downloading... Estimated time: %s");
          Strings::ReplaceAllIn(text, "%s", eta.ToTimeString());
          mUpdater.UpdateETAText(text);
      }
      break;
    }
    case GenericDownloadingGameState::Extracting:
    {
      // Load size into progress bar component
      mUpdater.UpdateProgressbar(mCurrentSize, mTotalSize);

      std::string text = _("Installing %s games");
      Strings::ReplaceAllIn(text, "%s", String(mGames));
      mUpdater.UpdateETAText(text);
      break;
    }
    case GenericDownloadingGameState::UpdatingMetadata:
    {
      // Load size into progress bar component
      mUpdater.UpdateProgressbar(mCurrentSize, mTotalSize);

      std::string text = _("Updating metadata...");
      Strings::ReplaceAllIn(text, "%s", String(mCurrentSize));
      if (mCurrentSize == 0) text = _("Refreshing gamelist...");
      mUpdater.UpdateETAText(text);
      break;
    }
    case GenericDownloadingGameState::Completed:
    {
      mUpdater.DownloadComplete(mSystem);
      break;
    }
    case GenericDownloadingGameState::WriteOnlyShare:
    {
      mUpdater.UpdateETAText("Can't write games to share!");
      break;
    }
    case GenericDownloadingGameState::DownloadError:
    {
      mUpdater.UpdateETAText("Error downloading games! Retry later.");
      break;
    }
  }
}
