//
// Created by bkg2k on 17/03/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "ScreenScraperSingleEngine.h"
#include "games/adapter/GameAdapter.h"
#include <utils/Zip.h>
#include <utils/hash/Md5.h>
#include <utils/Files.h>
#include <utils/Log.h>
#include <systems/SystemData.h>

String ScreenScraperSingleEngine::sImageSubFolder("images");
String ScreenScraperSingleEngine::sThumbnailSubFolder("thumbnails");
String ScreenScraperSingleEngine::sVideoSubFolder("videos");
String ScreenScraperSingleEngine::sWheelSubFolder("wheels");
String ScreenScraperSingleEngine::sMarqueeSubFolder("marquees");
String ScreenScraperSingleEngine::sManualSubFolder("manuals");
String ScreenScraperSingleEngine::sMapSubFolder("maps");

void ScreenScraperSingleEngine::Initialize(bool noabort)
{
  mRunning = false;
  if (!noabort)
    mAbortRequest = false;
  mQuotaReached = false;
  mTextInfo = 0;
  mImages = 0;
  mVideos = 0;
  mMediaSize = 0;
}

ScrapeResult ScreenScraperSingleEngine::Scrape(ScrapingMethod method, FileData& game, MetadataType& updatedMetadata, ProtectedSet& md5Set)
{
  // Start
  mRunning = true;

  // Result
  ScrapeResult result = ScrapeResult::NotScraped;

  // Read only?
  if (game.TopAncestor().ReadOnly())
    return ScrapeResult::NotScraped;

  // Check if the game needs to be scraped
  if (NeedScraping(method, game))
    // Dummy loop, only there to be exited from everywhere
    for(;mRunning && !mAbortRequest;)
    {
      // Default return status
      ScreenScraperApis::Game gameResult;
      gameResult.mResult = ScrapeResult::NotFound;

      { LOG(LogDebug) << "[ScreenScraper] Start scraping data for " << game.RomPath().ToString(); }
      if (mAbortRequest) break;

      // Get file size
      long long size = GameAdapter(game).RomSize();
      if (size < 0) break;
      if (mAbortRequest) break;

      // Zip request
      switch (result = RequestZipGameInfo(gameResult, game, size))
      {
        case ScrapeResult::QuotaReached:
        case ScrapeResult::DiskFull:
        case ScrapeResult::FatalError: mAbortRequest = true; break; // General abort
        case ScrapeResult::NotScraped:
        case ScrapeResult::Ok: break;
        case ScrapeResult::NotFound:
        {
          // Normal file request
          switch (result = RequestGameInfo(gameResult, game, size))
          {
            case ScrapeResult::QuotaReached:
            case ScrapeResult::DiskFull:
            case ScrapeResult::FatalError: mAbortRequest = false; break; // General abort
            case ScrapeResult::NotFound:
            case ScrapeResult::NotScraped:
            case ScrapeResult::Ok: break;
          }
        }
      }
      if (mAbortRequest) break;

      // Something found?
      if (gameResult.mResult == ScrapeResult::Ok)
      {
        // Store text data
        MetadataType changes = StoreTextData(method, gameResult, game);
        updatedMetadata |= changes;
        if (mStageInterface != nullptr)
          mStageInterface->ScrapingStageCompleted(&game, IScraperEngineStage::Stage::Text, changes);
        if (mAbortRequest) break;

        // Request and store media
        changes = MetadataType::None;
        switch (DownloadAndStoreMedia(method, gameResult, game, changes, md5Set))
        {
          case ScrapeResult::QuotaReached:
          case ScrapeResult::DiskFull:
          case ScrapeResult::FatalError: mAbortRequest = true; break; // General abort
          case ScrapeResult::NotFound:
          case ScrapeResult::NotScraped:
          case ScrapeResult::Ok: updatedMetadata |= changes; break;
        }
        if (mAbortRequest) break;
      }

      break;
    }

  // EOS
  if (mStageInterface != nullptr)
    mStageInterface->ScrapingStageCompleted(&game, IScraperEngineStage::Stage::Completed, MetadataType::None);

  // Stop
  game.Metadata().SetTimeStamp();
  mRunning = false;
  return result;
}

String ScreenScraperSingleEngine::ComputeMD5(const Path& path)
{
  FILE* f = fopen(path.ToChars(), "rb");
  if (f != nullptr)
  {
    MD5 md5;
    char buffer[1 << 20]; // 1Mb buffer
    for(int read = 0; (read = (int)fread(buffer, 1, sizeof(buffer), f)) > 0; )
      md5.update(buffer, read);
    (void)fclose(f);
    md5.finalize();
    return md5.hexdigest();
  }
  return String();
}

ScrapeResult ScreenScraperSingleEngine::RequestGameInfo(ScreenScraperApis::Game& result, const FileData& game, long long size)
{
  // Get MD5
  const Path romPath(game.RomPath());
  String md5 = romPath.IsFile() ? ((size < sMaxMd5Calculation) ? ComputeMD5(romPath) : String::Empty) : String::Empty;
  { LOG(LogDebug) << "[ScreenScraper] MD5 of " << romPath.ToString() << " : " << md5; }

  // Get crc32
  String crc32;
  if (game.Metadata().RomCrc32() != 0) crc32 = game.Metadata().RomCrc32AsString();

  // Call!
  if (!mAbortRequest)
    result = mCaller.GetGameInformation(game, crc32, md5, size);

  return result.mResult;
}

ScrapeResult ScreenScraperSingleEngine::RequestZipGameInfo(ScreenScraperApis::Game& result, const FileData& game, long long size)
{
  if (game.RomPath().Extension().ToLowerCase() == ".zip")
  {
    Zip zip(game.RomPath());
    if (zip.Count() == 1) // Ignore multi-file archives
    {
      // Get real name
      Path filePath = zip.FileName(0);

      // Get MD5
      String md5 = zip.Md5(0);
      { LOG(LogDebug) << "[ScreenScraper] MD5 of " << filePath.ToString() << " [" << game.RomPath().ToString() << "] : " << md5; }

      // Get crc32
      unsigned int crc32i = zip.Crc32(0);
      String crc32(crc32i, 8, String::Hexa::None);

      // Call!
      if (!mAbortRequest)
        result = mCaller.GetGameInformation(game, crc32, md5, size);
    }
  }

  return result.mResult;
}

bool ScreenScraperSingleEngine::NeedScraping(ScrapingMethod method, FileData& game)
{
  const Path rootMediaPath = game.TopAncestor().RomPath() / "media";
  const String gameFile = game.RomPath().FilenameWithoutExtension();
  switch(method)
  {
    case ScrapingMethod::All: return true;
    case ScrapingMethod::AllIfNothingExists:
    {
      // Any missing text info = scrape
      if (!game.Metadata().Description().empty()) return false;
      if (game.Metadata().ReleaseDateEpoc() != 0) return false;
      if (!game.Metadata().Developer().empty()) return false;
      if (!game.Metadata().Publisher().empty()) return false;
      if (game.Metadata().GenreId() != GameGenres::None) return false;
      // Ignore crc & region since they are most likely set automatically
      //if (game.Metadata().RomCrc32() != 0) return false;
      //if (game.Metadata().Region().HasRegion()) return false;

      // Check required and missing media
      if (mConfiguration.GetImageType() != ScreenScraperEnums::ScreenScraperImageType::None)
        if (!game.Metadata().Image().IsEmpty()) return false;
      if (mConfiguration.GetThumbnailType() != ScreenScraperEnums::ScreenScraperImageType::None)
        if (!game.Metadata().Thumbnail().IsEmpty()) return false;
      if (mConfiguration.GetVideo() != ScreenScraperEnums::ScreenScraperVideoType::None)
        if (!game.Metadata().Video().IsEmpty()) return false;
      if (mConfiguration.GetWantMarquee())
      {
        Path marqueePath = rootMediaPath / sMarqueeSubFolder;
        if ((marqueePath / (gameFile + ".jpg")).Exists() ||
            (marqueePath / (gameFile + ".png")).Exists()) return false;
      }
      if (mConfiguration.GetWantWheel())
      {
        Path wheelPath = rootMediaPath / sWheelSubFolder;
        if ((wheelPath / (gameFile + ".jpg")).Exists() ||
            (wheelPath / (gameFile + ".png")).Exists()) return false;
      }
      if (mConfiguration.GetWantManual())
      {
        Path manuelPath = rootMediaPath / sManualSubFolder;
        if ((manuelPath / (gameFile + ".pdf")).Exists()) return false;
      }
      if (mConfiguration.GetWantMaps())
      {
        Path mapPath = rootMediaPath / sMapSubFolder;
        if ((mapPath / (gameFile + ".jpg")).Exists() ||
            (mapPath / (gameFile + ".png")).Exists()) return false;
      }
      if (mConfiguration.GetWantP2K())
      {
        if (game.P2KPath().Exists()) return false;
      }
      // TODO: Add more media checks here
      return true;
    }
    case ScrapingMethod::CompleteAndKeepExisting:
    case ScrapingMethod::CompleteAndReplaceExisting:
    {
      // Any missing text info = scrape
      if (game.Metadata().Name().empty()) return true;
      if (game.Metadata().Description().empty()) return true;
      if (game.Metadata().ReleaseDateEpoc() == 0) return true;
      if (game.Metadata().Developer().empty()) return true;
      if (game.Metadata().Publisher().empty()) return true;
      if (game.Metadata().GenreId() == GameGenres::None) return true;
      if (game.Metadata().RomCrc32() == 0) return true;
      if (!game.Metadata().Region().HasRegion()) return true;

      // Check required and missing media
      if (mConfiguration.GetImageType() != ScreenScraperEnums::ScreenScraperImageType::None)
        if (game.Metadata().Image().IsEmpty()) return true;
      if (mConfiguration.GetThumbnailType() != ScreenScraperEnums::ScreenScraperImageType::None)
        if (game.Metadata().Thumbnail().IsEmpty()) return true;
      if (mConfiguration.GetVideo() != ScreenScraperEnums::ScreenScraperVideoType::None)
        if (game.Metadata().Video().IsEmpty()) return true;
      if (mConfiguration.GetWantMarquee())
      {
        Path marqueePath = rootMediaPath / sMarqueeSubFolder;
        if ((!(marqueePath / (gameFile + ".jpg")).Exists()) &&
            (!(marqueePath / (gameFile + ".png")).Exists())) return true;
      }
      if (mConfiguration.GetWantWheel())
      {
        Path wheelPath = rootMediaPath / sWheelSubFolder;
        if ((!(wheelPath / (gameFile + ".jpg")).Exists()) &&
            (!(wheelPath / (gameFile + ".png")).Exists())) return true;
      }
      if (mConfiguration.GetWantManual())
      {
        Path manuelPath = rootMediaPath / sManualSubFolder;
        if (!(manuelPath / (gameFile + ".pdf")).Exists()) return true;
      }
      if (mConfiguration.GetWantMaps())
      {
        Path mapPath = rootMediaPath / sMapSubFolder;
        if ((!(mapPath / (gameFile + ".jpg")).Exists()) &&
            (!(mapPath / (gameFile + ".png")).Exists())) return true;
      }
      if (mConfiguration.GetWantP2K())
      {
        if (!game.P2KPath().Exists()) return true;
      }
      // TODO: Add more media checks here
      return false;
    }
    default: break;
  }

  // Unknown method
  { LOG(LogError) << "[ScreenScraper] Unknown scraping method"; }
  return false;
}

MetadataType ScreenScraperSingleEngine::StoreTextData(ScrapingMethod method, const ScreenScraperApis::Game& sourceData, FileData& game)
{
  MetadataType result = MetadataType::None;
  bool noKeep = method != ScrapingMethod::CompleteAndKeepExisting;
  // Name always scraped
  if (!sourceData.mName.empty())
  {
    game.Metadata().SetName(sourceData.mName);
    mTextInfo++;
    result |= MetadataType::Name;
  }
  // Store data only if they are not empty and not scraped if method is IncompleteKeep
  if (!sourceData.mSynopsis.empty())
    if (game.Metadata().IsDefaultDescription() || noKeep)
    {
      game.Metadata().SetDescription(sourceData.mSynopsis);
      mTextInfo++;
      result |= MetadataType::Synopsis;
    }
  if (!sourceData.mPublisher.empty())
    if (game.Metadata().IsDefaultPublisher() || noKeep)
    {
      game.Metadata().SetPublisher(sourceData.mPublisher);
      mTextInfo++;
      result |= MetadataType::Publisher;
    }
  if (!sourceData.mDeveloper.empty())
    if (game.Metadata().IsDefaultDeveloper() || noKeep)
    {
      game.Metadata().SetDeveloper(sourceData.mDeveloper);
      mTextInfo++;
      result |= MetadataType::Developer;
    }
  if (!sourceData.mPlayers.empty())
    if (game.Metadata().IsDefaultPlayerRange() || noKeep)
    {
      game.Metadata().SetPlayersAsString(sourceData.mPlayers);
      mTextInfo++;
      result |= MetadataType::Players;
    }
  if (sourceData.mReleaseDate.ToEpochTime() != 0)
    if (game.Metadata().IsDefaultReleaseDateEpoc() || noKeep)
    {
      game.Metadata().SetReleaseDate(sourceData.mReleaseDate);
      mTextInfo++;
      result |= MetadataType::ReleaseDate;
    }
  if (sourceData.mRating != 0.0f)
    if (game.Metadata().IsDefaultRating() || noKeep)
    {
      game.Metadata().SetRating(sourceData.mRating);
      mTextInfo++;
      result |= MetadataType::Rating;
    }
  if (!sourceData.mGenre.empty())
    if (game.Metadata().IsDefaultGenre() || noKeep)
    {
      game.Metadata().SetGenre(sourceData.mGenre);
      mTextInfo++;
      result |= MetadataType::Genre;
    }
  if (sourceData.mGenreId !=  GameGenres::None)
    if (game.Metadata().IsDefaultGenreId() || noKeep)
    {
      game.Metadata().SetGenreId(sourceData.mGenreId);
      game.Metadata().SetAdult(sourceData.mAdult);
      mTextInfo++;
      result |= MetadataType::GenreId;
    }
  if (sourceData.mRegions.HasRegion())
    if (!game.Metadata().IsDefaultRegion() || noKeep)
    {
      game.Metadata().SetRegion(sourceData.mRegions);
      mTextInfo++;
      result |= MetadataType::Region;
    }
  if (!sourceData.mCrc.empty()) // Always set CRC if not empty
  {
    game.Metadata().SetRomCrc32AsString(sourceData.mCrc);
    mTextInfo++;
    result |= MetadataType::Crc32;
  }

  // Store P2k file
  if (!sourceData.mP2k.empty())
  {
    Path p2kPath = game.P2KPath();
    if (!p2kPath.Exists() || noKeep)
    {
      Files::SaveFile(p2kPath, sourceData.mP2k);
      mTextInfo++;
      result |= MetadataType::P2K;
    }
  }
  // Store Rotation information
  if (!sourceData.mRotation.empty() &&
      game.Metadata().IsDefaultTate() &&
      game.System().IsTrueArcade())
  {
    game.Metadata().SetRotation(RotationUtils::FromAngle(sourceData.mRotation));
    mTextInfo++;
    result |= MetadataType::Rotation;
  }

  return result;
}

ScrapeResult ScreenScraperSingleEngine::DownloadMedia(const Path& AbsoluteImagePath, FileData& game,
                                                      const String& media, SetPathMethodType pathSetter,
                                                      ProtectedSet& md5Set, MediaType mediaType, bool& pathHasBeenSet)
{
  bool mediaIsPresent = md5Set.Exists(AbsoluteImagePath.ToString());
  if (mediaIsPresent)
  {
    if (pathSetter != nullptr)
    {
      (game.Metadata().*pathSetter)(AbsoluteImagePath);
      pathHasBeenSet = true;
    }
    return ScrapeResult::Ok;
  }
  if (!media.empty())
  {
    long long size = 0;

    switch(mCaller.GetMedia(game, media, AbsoluteImagePath, size))
    {
      case ScrapeResult::Ok:
      {
        if (pathSetter != nullptr && size != 0) // Ensure the media has a valid size before setting the path
        {
          (game.Metadata().*pathSetter)(AbsoluteImagePath);
          pathHasBeenSet = true;
        }
        if(mediaType == MediaType::Image) mImages++;
        if(mediaType == MediaType::Video) mVideos++;
        mMediaSize += size;
        break;
      }
      case ScrapeResult::NotScraped: break;
      case ScrapeResult::NotFound: { LOG(LogError) << "[ScreenScraper] Missing media!"; break; }
      case ScrapeResult::DiskFull: return ScrapeResult::DiskFull;
      case ScrapeResult::QuotaReached: return ScrapeResult::QuotaReached;
      case ScrapeResult::FatalError:  { LOG(LogError) << "[ScreenScraper] Fatal error getting media!"; break; }
    }
  }
  return ScrapeResult::Ok;
}

ScrapeResult
ScreenScraperSingleEngine::DownloadAndStoreMedia(FileData& game, bool noKeep, const Path& target, const String& subPath,
                                                 const String& name, MediaType mediaType, SetPathMethodType pathSetter,
                                                 const ScreenScraperApis::Game::MediaUrl::Media& mediaSource, ProtectedSet& md5Set,
                                                 bool& pathHasBeenSet)
{
  if (!mediaSource.IsValid()) return ScrapeResult::NotScraped;

  pathHasBeenSet = false;
  Path path = target / subPath / String(name).Append(' ').Append(mediaSource.mMd5).Append('.').Append(mediaSource.mFormat);
  bool exists = path.Exists();
  if (!exists || noKeep)
    return DownloadMedia(path, game, mediaSource.mUrl, pathSetter, md5Set, mediaType, pathHasBeenSet);

  if (pathSetter != nullptr)
  {
    (game.Metadata().*pathSetter)(path);
    pathHasBeenSet = true;
  }
  return ScrapeResult::Ok;
}

bool ScreenScraperSingleEngine::IsFatal(ScrapeResult result) const
{
  switch(result)
  {
    case ScrapeResult::QuotaReached:
    case ScrapeResult::DiskFull:
    case ScrapeResult::FatalError: return true;
    case ScrapeResult::Ok:
    case ScrapeResult::NotScraped:
    case ScrapeResult::NotFound:
    default: break;
  }
  return mAbortRequest;
}

ScrapeResult
ScreenScraperSingleEngine::DownloadAndStoreMedia(ScrapingMethod method, const ScreenScraperApis::Game& sourceData,
                                                 FileData& game, MetadataType& updatedMetadata, ProtectedSet& md5Set)
{
  bool ok = false;
  const Path rootFolder(game.TopAncestor().RomPath());
  const Path relativePath = game.RomPath().MakeRelative(rootFolder, ok);
  const String gameName = ok ? (relativePath.Directory() / game.RomPath().FilenameWithoutExtension()).ToString()
                                  : game.RomPath().FilenameWithoutExtension();
  const Path mediaFolder = rootFolder / "media";
  bool noKeep = method != ScrapingMethod::CompleteAndKeepExisting;

  // Main image
  updatedMetadata = MetadataType::None;
  MetadataType whatHaveBeenSet = MetadataType::None;
  bool pathHasBeenSet = false;
  ScrapeResult result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sImageSubFolder, sourceData.mScreenScraperName, MediaType::Image, &MetadataDescriptor::SetImagePath, sourceData.MediaSources.mImage, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Image;
  if (IsFatal(result)) return result;

  // Thumbnail image
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sThumbnailSubFolder, sourceData.mScreenScraperName, MediaType::Image, &MetadataDescriptor::SetThumbnailPath, sourceData.MediaSources.mThumbnail, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Image;
  if (IsFatal(result)) return result;

  // Notify main images
  updatedMetadata |= whatHaveBeenSet;
  if (mStageInterface != nullptr) mStageInterface->ScrapingStageCompleted(&game, IScraperEngineStage::Stage::Images, whatHaveBeenSet);

  // Video
  whatHaveBeenSet = MetadataType::None;
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sVideoSubFolder, sourceData.mScreenScraperName, MediaType::Video, &MetadataDescriptor::SetVideoPath, sourceData.MediaSources.mVideo, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Video;
  if (IsFatal(result)) return result;

  // Notify video
  updatedMetadata |= whatHaveBeenSet;
  if (mStageInterface != nullptr) mStageInterface->ScrapingStageCompleted(&game, IScraperEngineStage::Stage::Video, whatHaveBeenSet);

  // Marquee
  whatHaveBeenSet = MetadataType::None;
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sMarqueeSubFolder, sourceData.mScreenScraperName, MediaType::Image, nullptr, sourceData.MediaSources.mMarquee, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Marquee;
  if (IsFatal(result)) return result;

  // Wheels
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sWheelSubFolder, sourceData.mScreenScraperName, MediaType::Image, nullptr, sourceData.MediaSources.mWheel, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Wheels;
  if (IsFatal(result)) return result;

  // Manual
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sManualSubFolder, sourceData.mScreenScraperName, MediaType::Image, nullptr, sourceData.MediaSources.mManual, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Manual;
  if (IsFatal(result)) return result;

  // Maps
  pathHasBeenSet = false;
  result = DownloadAndStoreMedia(game, noKeep, mediaFolder, sMapSubFolder, sourceData.mScreenScraperName, MediaType::Image, nullptr, sourceData.MediaSources.mMaps, md5Set, pathHasBeenSet);
  if (pathHasBeenSet) whatHaveBeenSet |= MetadataType::Maps;
  if (IsFatal(result)) return result;

  // Notify extra media
  updatedMetadata |= whatHaveBeenSet;
  if (mStageInterface != nullptr) mStageInterface->ScrapingStageCompleted(&game, IScraperEngineStage::Stage::Extra, whatHaveBeenSet);

  // TODO: Add more image download & save here
  return ScrapeResult::Ok;
}
