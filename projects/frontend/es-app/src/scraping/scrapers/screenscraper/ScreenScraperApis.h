//
// Created by bkg2k on 07/12/2019.
//
#pragma once

#include <utils/String.h>
#include "utils/network/HttpClient.h"
#include <utils/datetime/DateTime.h>
#include <rapidjson/document.h>
#include <scraping/ScrapeResult.h>
#include <games/classifications/Genres.h>
#include <games/classifications/Regions.h>
#include <scraping/scrapers/screenscraper/IConfiguration.h>
#include <scraping/scrapers/screenscraper/IEndPointProvider.h>

class ScreenScraperApis
{
  public:
    //! Game object
    struct Game
    {
      //! Result
      ScrapeResult mResult = ScrapeResult::NotFound;
      //! ScreenScraper Game Name
      String mScreenScraperName;
      //! Game Name
      String mName;
      //! Developer
      String mDeveloper;
      //! Publisher
      String mPublisher;
      //! Synopsis
      String mSynopsis;
      //! Players
      String mPlayers;
      //! Genre
      String mGenre;
      //! Rom regions
      Regions::RegionPack mRegions;
      //! Rom region
      String mCrc;
      //! Pad2Keyboard
      String mP2k;
      //! Rotation
      String mRotation;
      //! Editor
      DateTime mReleaseDate;
      //! Rating
      float mRating;
      //! Normalized genre
      GameGenres mGenreId;
      //! Adult state
      bool mAdult;

      //! Media Urls
      struct MediaUrl
      {
        struct Media
        {
          //! source Url
          String mUrl;
          //! file format
          String mFormat;
          //! md5
          String mMd5;
          //! size
          long long mSize;

          [[nodiscard]] bool IsValid() const
          {
            return !mUrl.empty() &&
                   !mFormat.empty() &&
                   !mMd5.empty() &&
                   mSize != 0;
          }
        };

        //! Main image
        Media mImage;
        //! Thumbnail image
        Media mThumbnail;
        //! Video
        Media mVideo;
        //! Marquee
        Media mMarquee;
        //! Wheel (clear logo)
        Media mWheel;
        //! Game Manual
        Media mManual;
        //! Game Maps
        Media mMaps;
      }
      MediaSources;
    };

  private:
    //! Credential interface
    IConfiguration& mConfiguration;
    //! Endpoint provider
    IEndPointProvider& mEndPointProvider;

    // Client initialized status
    bool mClientInitialized;

    //! Htpp client
    HttpClient mClient;

    /*!
     * @brief Deserialize a gameinfo request JSON result into a Game object
     * @param json Input json
     * @param game Output Game object
     * @param path Rom path
     * @param md5 file md5
     * @param size file size in byte
     */
    void DeserializeGameInformationInner(const rapidjson::Value& json, Game& game, const Path& path, const String& md5, long long size);

    /*!
     * @brief Deserialize a gameinfo request JSON result into a Game object
     * @param json Input json
     * @param game Output Game object
     * @param path Rom path
     * @param md5 file md5
     * @param size file size in byte
     */
    void DeserializeGameInformationOuter(const String& json, Game& game, const Path& path, const String& md5, long long size);

    /*!
     * @brief Lookup best matching rom entry in the 'roms' array, according to md5, then filename & size
     * @param json json node
     * @param filename rom filename
     * @param md5 rom md5
     * @param size rom size in byte
     * @return Found node or empty node
     */
    static const rapidjson::Value& LookupBestRomEntry(const rapidjson::Value& json, const String& filename, const String& md5, long long size);

    /*!
     * @brief Select best region regarding configuration & availability
     * @param romRegions Regions from screenscraper
     * @param path File name to extract region from
     * @param favoriteRegion User favorite regios
     * @return Serialized best region
     */
    static String GetRequiredRegion(Regions::RegionPack regions, const Path& path, Regions::GameRegions favoriteRegion);

    /*!
     * @brief Get all regions for a given game
     * @param romRegions Regions extracted from rom information
     * @param path File path
     * @return PAckled regions
     */
    static Regions::RegionPack GetRegions(Regions::RegionPack romRegions, const Path& path);

    /*!
     * @brief Extract the best-matching text from an array of regionalized texts, regarding the given preferred region
     * @param array Json array of regionalized texts
     * @param requiredRegion Required region
     * @return Best-matching text
     */
    static String ExtractRegionalizedText(const rapidjson::Value& array, const String& requiredRegion);

    /*!
     * @brief Extract the best-matching text from an array of localized texts, regarding the given preferred language
     * @param array Json array of localized texts
     * @param preferedlanguage Preferred language
     * @return Best-matching text
     */
    static String ExtractLocalizedText(const rapidjson::Value& array, const String& preferedlanguage);

    /*!
     * @brief Extract the best-matching media URL, given the type, parent type and prefered region.
     * @param medias Media name
     * @param type Type
     * @param region Preferred region
     * @param format If an url is found, this string contains the media format (jpg, png, mp4, ...)
     * @return Best-matching url
     */
    static String ExtractMedia(const rapidjson::Value& medias, const char* type, const String& region, String& format, long long& size, String&  md5);

    /*!
     * @brief Extract best matching genre texts, comma-separated.
     * @param array Json array of genres
     * @param preferedlanguage Prefered language
     * @return Genre strings, comma-separated
     */
    static String ExtractLocalizedGenre(const rapidjson::Value& array, const String& preferedlanguage);

    /*!
     * @brief Extract a normalized game genre
     * @param array Json array of genres
     * @return GameGenres value
     */
    static GameGenres ExtractNormalizedGenre(const rapidjson::Value& array);

    /*!
     * @brief Extract the adult state
     * @param array Json array of genres
     * @return True if the game is flagged Adult, false otherwise
     */
    static bool ExtractAdultState(const rapidjson::Value& array);

    /*!
     * @brief Decode inner string encoding, including XML and X encoding like
     * &quot; \u0260 and \r\n
     * @param raw Raw string, decoded in place
     */
    static void DecodeString(String& raw);

    /*!
     * @brief Return a clean game name, removing all non file authorized chars
     * @param source Original game
     * @return Clean name
     */
    static String CleanGameName(const String& source);

    /*!
     * @brief Check if the http client is initialized & initialize if required
     */
    void InitializeClient()
    {
      if (!mClientInitialized)
      {
        if (mEndPointProvider.RequireBasicAuth())
          mClient.SetBasicAuth(mConfiguration.GetLogin(), mConfiguration.GetPassword());
        if (mEndPointProvider.RequireBearer())
          mClient.SetBearer(mConfiguration.GetBearer());
        mClientInitialized = true;
      }
    }

  public:
    /*!
     * @brief Constructor
     * @param credentials
     */
    explicit ScreenScraperApis(IConfiguration* credentials, IEndPointProvider* endPointProvider)
      : mConfiguration(*credentials)
      , mEndPointProvider(*endPointProvider)
      , mClientInitialized(false)
    {
    }

    //! Get user
    ScreenScraperUser GetUserInformation();

    /*!
     * @brief Get Game informations
     * @param file Game to work on
     * @param crc32 optionnal CRC32
     * @param md5 optionnal MD5
     * @return Game structure with the result code in the mResult field
     */
    Game GetGameInformation(const FileData& file, const String& crc32, const String& md5, long long size);

    /*!
     * @brief Download a media into a target file
     * @param mediaurl Media url
     * @param to Filepath
     * @return ScrapeResult Request status
     */
    ScrapeResult GetMedia(const FileData& game, const String& mediaurl, const Path& to, long long& size);
};

