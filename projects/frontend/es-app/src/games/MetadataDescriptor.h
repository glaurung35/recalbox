#pragma once

#include <utils/os/fs/Path.h>
#include <utils/Xml.h>
#include <utils/datetime/DateTime.h>
#include <games/classifications/Regions.h>
#include "ItemType.h"
#include "games/classifications/Genres.h"
#include "MetadataStringHolder.h"
#include "hardware/RotationType.h"
#include "SupportSides.h"

//#define _METADATA_STATS_

// Forward declaration (MetadataFieldDescriptor must include MetadataDescriptor)
class MetadataFieldDescriptor;

class MetadataDescriptor
{
  private:
    #ifdef _METADATA_STATS_
    static int LivingClasses;
    static int LivingFolders;
    static int LivingGames;
    #endif

    //! Game node <game></game>
    static const String GameNodeIdentifier;
    //! Folder node <folder></folder>
    static const String FolderNodeIdentifier;

    //! Game name string holder
    static MetadataStringHolder sNameHolder;
    //! Description string holder
    static MetadataStringHolder sDescriptionHolder;
    //! Developer string holder
    static MetadataStringHolder sDeveloperHolder;
    //! Publisher string holder
    static MetadataStringHolder sPublisherHolder;
    //! Publisher string holder
    static MetadataStringHolder sGenreHolder;
    //! Publisher string holder
    static MetadataStringHolder sEmulatorHolder;
    //! Publisher string holder
    static MetadataStringHolder sCoreHolder;
    //! Publisher string holder
    static MetadataStringHolder sRatioHolder;
    //! Path string holder
    static MetadataStringHolder sPathHolder;
    //! File string holder
    static MetadataStringHolder sFileHolder;

    // Please keep field ordered by type size to reduce alignment padding
    // 32 bits
    unsigned int                  mTimeStamp;    //!< Scraping timestamp
    MetadataStringHolder::Index32 mRomFile;      //!< Rom file
    MetadataStringHolder::Index32 mName;         //!< Name as simple string
    MetadataStringHolder::Index32 mDescription;  //!< Description, multiline text
    MetadataStringHolder::Index32 mImageFile;    //!< Image file
    MetadataStringHolder::Index32 mThumbnailFile;//!< Thumbnail file
    MetadataStringHolder::Index32 mVideoFile;    //!< Video file
    float                         mRating;       //!< Rating from 0.0 to 1.0
    unsigned int                  mReleaseDate;  //!< Release data (epoch)
    unsigned int                  mLastPlayed;   //!< Last time played (epoch)
    MetadataStringHolder::Index32 mGenre;        //!< Genres, comma separated
    MetadataStringHolder::Index32 mDeveloper;    //!< Developer name index
    MetadataStringHolder::Index32 mPublisher;    //!< Publisher name index
    int                           mPlayers;        //!< Players range: LSW:from - MSW:to (allow sorting by max players)
    int                           mRomCrc32;       //!< Rom Crc32
    Regions::RegionPack           mRegion;         //!< Rom/Game Region
    int                           mTotalPlayTime;  //<! Game tota play time
    // 16 bits
    MetadataStringHolder::Index16 mRomPath;        //!< Rom path
    MetadataStringHolder::Index16 mImagePath;      //!< Image path
    MetadataStringHolder::Index16 mThumbnailPath;  //!< Thumbnail path
    MetadataStringHolder::Index16 mVideoPath;      //!< Video path
    MetadataStringHolder::Index16 mEmulator;       //!< Specific emulator
    MetadataStringHolder::Index16 mCore;           //!< Specific core
    MetadataStringHolder::Index16 mLastPatchPath;  //!< Last patch path
    MetadataStringHolder::Index16 mLastPatchFile;  //!< Last patch file
    short                         mPlayCount;      //!< Play counter
    GameGenres                    mGenreId;        //!< Normalized Genre
    // 8 bits
    MetadataStringHolder::Index8  mRatio;          //!< Specific screen ratio
    // bit groups
    int                           mSupportIndex:5; //!< Support number (disk, tape, ...)
    int                           mSupportTotal:5; //!< Total Supports
    SupportSides                  mSupportSide:2;  //!< Support faces
    ItemType                      mType:3;         //!< Metadata type
    RotationType                  mRotation:2;     //!< Rotation flag
    bool                          mFavorite:1;     //!< Favorite game
    bool                          mHidden:1;       //!< Hidden game
    bool                          mAdult:1;        //!< Adult state
    bool                          mLatestVerion:1; //!< Lastest version of this game?
    bool                          mPreinstalled:1; //!< Preinstalled game?
    bool                          mNoGame:1;       //!< ?!
    bool                          mDirty:1;        //!< Dirty flag (modified data flag)

    //! Default value storage for fast default detection
    static const MetadataDescriptor& Default();

    /*!
     * Return the first static internal field descriptor reference
     * @param Type ObjectType from which to retrieve
     * @param count Number of actual field descriptor available
     * @return first static internal field descriptor reference
     */
    static const MetadataFieldDescriptor* GetMetadataFieldDescriptors(ItemType type, int& count);

    /*!
     * Convert integer range to string: low-high
     * @param range Integer range: Highest into MSB, Lowest into LSB
     * @return Converted string
     */
    static String IntToRange(int range);
    /*!
     * Convert a range X-Y to an int: Highest into MSB, Lowest into LSB (allow sorting by highest value)
     * @param range Range string
     * @param to destination int
     * @return True if the operation is successful. False otherwise.
     */
    static bool RangeToInt(const String& range, int& to);
    /*!
     * Convert int32 to Hexadecimal string
     * @param from Int32 value to convert to string
     * @param to Hexadecimal result string
     * @return True if the operation is successful. False otherwise.
     */
    static bool IntToHex(int from, String& to);
    /*!
     * Convert Hexa string into int32
     * @param from Hexadecimal string
     * @param to Target int32
     * @return True if the operation is successful. False otherwise.
     */
    static bool HexToInt(const String& from, int& to);
    /*!
     * Fast string to int conversion
     * @param from source string
     * @param to destination int
     * @param offset offset in source string
     * @param stop Stop char
     * @return True if the operation is successful. False otherwise.
     */
    static bool StringToInt(const String& from, int& to, int offset, char stop);
    /*!
     * Fast string to int conversion
     * @param from source string
     * @param to destination int
     * @return True if the operation is successful. False otherwise.
     */
    static bool StringToInt(const String& from, int& to);
    /*!
     * Fast string to float conversion
     * @param from source string
     * @param to destination float
     * @return True if the operation is successful. False otherwise.
     */
    static bool StringToFloat(const String& from, float& to);

  public:
    /*!
     * Destructor
     */
    ~MetadataDescriptor() = default;

    /*!
     * @brief Force default metadata initialization
     */
    static void InitializeDefaultMetadata() { Default(); }

    /*!
     * @brief Cleanup temporary data
     */
    static void CleanupHolders();

    /*!
     * @brief Initialize Holder, starting with no data at all
     */
    static void InitializeHolders();

    /*!
     * Default constructor
     */
    explicit MetadataDescriptor(const Path& path, const String& defaultName, ItemType type)
      : mTimeStamp(0)
      , mRomFile(0)
      , mName(0)
      , mDescription(0)
      , mImageFile(0)
      , mThumbnailFile(0)
      , mVideoFile(0)
      , mRating(0.0f)
      , mReleaseDate(0)
      , mLastPlayed(0)
      , mGenre(0)
      , mDeveloper(0)
      , mPublisher(0)
      , mPlayers((1 << 16) + 1)
      , mRomCrc32(0)
      , mRegion()
      , mTotalPlayTime(0)
      , mRomPath(0)
      , mImagePath(0)
      , mThumbnailPath(0)
      , mVideoPath(0)
      , mEmulator(0)
      , mCore()
      , mLastPatchPath(0)
      , mLastPatchFile(0)
      , mPlayCount(0)
      , mGenreId(GameGenres::None)
      , mRatio(0)
      , mSupportIndex(0)
      , mSupportTotal(0)
      , mSupportSide(SupportSides::None)
      , mType(type)
      , mRotation(RotationType::None)
      , mFavorite(false)
      , mHidden(false)
      , mAdult(false)
      , mLatestVerion(false)
      , mPreinstalled(false)
      , mNoGame(false)
      , mDirty(false)
    {
      SetRomPath(path);
      SetName(defaultName);
      #ifdef _METADATA_STATS_
      LivingClasses++;
      if (_Type == ItemType::Game) LivingGames++;
      if (_Type == ItemType::Folder) LivingFolders++;
      #endif
      UnsetDirty();
    }

    /*!
     * Move constructor
     * @param source  Source to move data from
     */
    MetadataDescriptor(const MetadataDescriptor& source) noexcept
    #ifdef _METADATA_STATS_
      : mName(source.mName),
        mDescription(source.mDescription),
        mImageFile(source.mImageFile),
        mThumbnailFile(source.mThumbnailFile),
        mVideoFile(source.mVideoFile),
        mRating(source.mRating),
        mReleaseDate(source.mReleaseDate),
        mLastPlayed(source.mLastPlayed),
        mGenre(source.mGenre),
        mDeveloper(source.mDeveloper),
        mPublisher(source.mPublisher),
        mRegion(source.mRegion),
        mPlayers(source.mPlayers),
        mRomCrc32(source.mRomCrc32),
        mImagePath(source.mImagePath),
        mThumbnailPath(source.mThumbnailPath),
        mVideoPath(source.mVideoPath),
        mEmulator(source.mEmulator),
        mCore(source.mCore),
        mPlaycount(source.mPlaycount),
        mGenreId(source.mGenreId),
        mRatio(source.mRatio),
        mTimeStamp(source.mTimeStamp),
        mFavorite(source.mFavorite),
        mHidden(source.mHidden),
        mAdult(source.mAdult),
        mLatestVerion(source.mLatestVerion),
        mNoGame(source.mNoGame),
        mDirty(source.mDirty),
        mType(source.mType),
        mRotation(source.mRotation),
        mTimePlayed(source.mTimePlayed),
    {
      LivingClasses++;
      if (_Type == ItemType::Game) LivingGames++;
      if (_Type == ItemType::Folder) LivingFolders++;
    }
    #else
      = default;
    #endif

    /*!
     * Assignment operator - Required by STL objects since a copy operator is defined
     * @param source Source to copy data from
     */
    MetadataDescriptor& operator = (const MetadataDescriptor& source)
    {
      if (&source == this) return *this;

      #ifdef _METADATA_STATS_
      LivingClasses--;
      if (_Type == ItemType::Game) LivingGames--;
      if (_Type == ItemType::Folder) LivingFolders--;
      #endif

      mRomFile       = source.mRomFile      ;
      mRomPath       = source.mRomPath      ;
      mName          = source.mName         ;
      mDescription   = source.mDescription  ;
      mImagePath     = source.mImagePath    ;
      mImageFile     = source.mImageFile    ;
      mThumbnailPath = source.mThumbnailPath;
      mThumbnailFile = source.mThumbnailFile;
      mVideoPath     = source.mVideoPath    ;
      mVideoFile     = source.mVideoFile    ;
      mGenre         = source.mGenre        ;
      mEmulator      = source.mEmulator     ;
      mCore          = source.mCore         ;
      mRatio         = source.mRatio        ;
      mDeveloper     = source.mDeveloper    ;
      mPublisher     = source.mPublisher    ;
      mRegion        = source.mRegion       ;
      mRating        = source.mRating       ;
      mGenreId       = source.mGenreId      ;
      mPlayers       = source.mPlayers      ;
      mReleaseDate   = source.mReleaseDate  ;
      mPlayCount     = source.mPlayCount    ;
      mLastPlayed    = source.mLastPlayed   ;
      mRomCrc32      = source.mRomCrc32     ;
      mFavorite      = source.mFavorite     ;
      mHidden        = source.mHidden       ;
      mAdult         = source.mAdult        ;
      mDirty         = source.mDirty        ;
      mLatestVerion  = source.mLatestVerion ;
      mPreinstalled  = source.mPreinstalled ;
      mNoGame        = source.mNoGame       ;
      mType          = source.mType         ;
      mRotation      = source.mRotation     ;
      mTotalPlayTime = source.mTotalPlayTime;
      mSupportIndex  = source.mSupportIndex ;
      mSupportTotal  = source.mSupportTotal ;
      mSupportSide   = source.mSupportSide  ;

      #ifdef _METADATA_STATS_
      if (_Type == ItemType::Game) LivingGames++;
      if (_Type == ItemType::Folder) LivingFolders++;
      #endif

      return *this;
    }

    /*!
     * Move operator
     * @param source Source to move data from
     */
    MetadataDescriptor& operator = (MetadataDescriptor&& source) noexcept
    {
      #ifdef _METADATA_STATS_
      LivingClasses--;
      if (_Type == ItemType::Game) LivingGames--;
      if (_Type == ItemType::Folder) LivingFolders--;
      #endif

      mRomFile       = source.mRomFile      ;
      mRomPath       = source.mRomPath      ;
      mName          = source.mName         ;
      mDescription   = source.mDescription  ;
      mImagePath     = source.mImagePath    ;
      mImageFile     = source.mImageFile    ;
      mThumbnailPath = source.mThumbnailPath;
      mThumbnailFile = source.mThumbnailFile;
      mVideoPath     = source.mVideoPath    ;
      mVideoFile     = source.mVideoFile    ;
      mEmulator      = source.mEmulator     ;
      mCore          = source.mCore         ;
      mLastPatchPath = source.mLastPatchPath;
      mLastPatchFile = source.mLastPatchFile;
      mRatio         = source.mRatio        ;
      mGenre         = source.mGenre        ;
      mDeveloper     = source.mDeveloper    ;
      mPublisher     = source.mPublisher    ;
      mRating        = source.mRating       ;
      mGenreId       = source.mGenreId      ;
      mPlayers       = source.mPlayers      ;
      mReleaseDate   = source.mReleaseDate  ;
      mPlayCount     = source.mPlayCount    ;
      mLastPlayed    = source.mLastPlayed   ;
      mRomCrc32      = source.mRomCrc32     ;
      mFavorite      = source.mFavorite     ;
      mHidden        = source.mHidden       ;
      mAdult         = source.mAdult        ;
      mDirty         = source.mDirty        ;
      mLatestVerion  = source.mLatestVerion ;
      mPreinstalled  = source.mPreinstalled ;
      mNoGame        = source.mNoGame       ;
      mType          = source.mType         ;
      mRotation      = source.mRotation     ;
      mTotalPlayTime = source.mTotalPlayTime;
      mSupportIndex  = source.mSupportIndex ;
      mSupportTotal  = source.mSupportTotal ;
      mSupportSide   = source.mSupportSide  ;

      #ifdef _METADATA_STATS_
      if (_Type == ItemType::Game) LivingGames++;
      if (_Type == ItemType::Folder) LivingFolders++;
      #endif

      return *this;
    }

    /*!
     * Deserialize data from a given Xml node
     * @param from XML Node to deserialize from
     * @param relativeTo Root path
     * @param timestamp last scraping timestamp
     * @return True of the node has been successfully deserialized
     */
    bool Deserialize(XmlNode from, const Path& relativeTo);

    /*!
     * Serialize internal data to XML node
     * @param relativeTo Root path
     * @return Serialized XML node
     */
    void Serialize(XmlNode parentTree, const Path& filePath, const Path& relativeTo) const;

    /*!
     * Merge value from the source metadata object into the current object
     * current fields ate replaced only if they have their default value.
     * @param source Metadata object from which to merge data
     */
    void Merge(const MetadataDescriptor& source);

    /*
     * Last scraping Timestamp
     */

    [[nodiscard]] unsigned int TimeStamp() const { return mTimeStamp; }
    void SetTimeStamp() { mTimeStamp = (unsigned int)DateTime().ToEpochTime(); }

    /*
     * Accessors
     */

    [[nodiscard]] ItemType Type() const { return mType; }

    [[nodiscard]] Path         Rom()           const { return sPathHolder.GetPath(mRomPath) / sFileHolder.GetString(mRomFile); }
    [[nodiscard]] Path         RomFolderOnly() const { return sPathHolder.GetPath(mRomPath); }
    [[nodiscard]] Path         RomFileOnly()   const { return sFileHolder.GetPath(mRomFile);        }
    [[nodiscard]] String       Name()          const { return sNameHolder.GetString(mName);                 }
    [[nodiscard]] String       Description()   const { return sDescriptionHolder.GetString(mDescription);   }
    [[nodiscard]] Path         Image()         const { return sPathHolder.GetPath(mImagePath) / sFileHolder.GetString(mImageFile); }
    [[nodiscard]] Path         Thumbnail()     const { return sPathHolder.GetPath(mThumbnailPath) / sFileHolder.GetString(mThumbnailFile); }
    [[nodiscard]] Path         Video()         const { return sPathHolder.GetPath(mVideoPath) / sFileHolder.GetString(mVideoFile); }
    [[nodiscard]] String       Developer()     const { return sDeveloperHolder.GetString(mDeveloper);       }
    [[nodiscard]] String       Publisher()     const { return sPublisherHolder.GetString(mPublisher);       }
    [[nodiscard]] String       Genre()         const { return sGenreHolder.GetString(mGenre);               }
    [[nodiscard]] String       Emulator()      const { return sEmulatorHolder.GetString(mEmulator);         }
    [[nodiscard]] String       Core()          const { return sCoreHolder.GetString(mCore);                 }
    [[nodiscard]] String       Ratio()         const { return sRatioHolder.GetString(mRatio);               }
    [[nodiscard]] Path         LastPatch()     const {return sPathHolder.GetPath(mLastPatchPath) / sFileHolder.GetString(mLastPatchFile); }

    [[nodiscard]] float              Rating()          const { return mRating;                           }
    [[nodiscard]] int                PlayerRange()     const { return mPlayers;                          }
    [[nodiscard]] int                PlayerMax()       const { return mPlayers >> 16;                    }
    [[nodiscard]] int                PlayerMin()       const { return mPlayers & 0xFFFF;                 }
    [[nodiscard]] unsigned int       ReleaseDateEpoc() const { return mReleaseDate;                      }
    [[nodiscard]] DateTime           ReleaseDate()     const { return DateTime((long long)mReleaseDate); }
    [[nodiscard]] int                PlayCount()       const { return mPlayCount;                        }
    [[nodiscard]] unsigned int       LastPlayedEpoc()  const { return mLastPlayed;                       }
    [[nodiscard]] DateTime           LastPlayed()      const { return DateTime((long long)mLastPlayed);  }
    [[nodiscard]] Regions::RegionPack Region()          const { return mRegion;                           }
    [[nodiscard]] int                RomCrc32()        const { return mRomCrc32;                         }
    [[nodiscard]] bool               Favorite()        const { return mFavorite;                         }
    [[nodiscard]] bool               Hidden()          const { return mHidden;                           }
    [[nodiscard]] bool               Adult()           const { return mAdult;                            }
    [[nodiscard]] bool               LatestVersion()   const { return mLatestVerion;                     }
    [[nodiscard]] bool               NoGame()          const { return mNoGame;                           }
    [[nodiscard]] GameGenres         GenreId()         const { return mGenreId;                          }
    [[nodiscard]] RotationType       Rotation()        const { return mRotation;                         }
    [[nodiscard]] int                TotalPlayTime()      const { return mTotalPlayTime;                       }
    [[nodiscard]] int                SupportIndex()    const { return mSupportIndex;                     }
    [[nodiscard]] int                SupportTotal()    const { return mSupportTotal;                     }
    [[nodiscard]] SupportSides       SupportSide()     const { return mSupportSide;                      }

    /*
     * Validators
     */

    [[nodiscard]] bool HasImage()     const { return (mImagePath     | mImageFile    ) > 0; }
    [[nodiscard]] bool HasThumnnail() const { return (mThumbnailPath | mThumbnailFile) > 0; }
    [[nodiscard]] bool HasVideo()     const { return (mVideoPath     | mVideoFile    ) > 0; }

    /*
     * String accessors
     */

    [[nodiscard]] String RomAsString()         const { return (sPathHolder.GetPath(mRomPath) / sFileHolder.GetString(mRomFile)).ToString(); }
    [[nodiscard]] String NameAsString()        const { return sNameHolder.GetString(mName);                 }
    [[nodiscard]] String EmulatorAsString()    const { return sEmulatorHolder.GetString(mEmulator);         }
    [[nodiscard]] String CoreAsString()        const { return sCoreHolder.GetString(mCore);                 }
    [[nodiscard]] String RatioAsString()       const { return sRatioHolder.GetString(mRatio, "auto"); }
    [[nodiscard]] String DescriptionAsString() const { return sDescriptionHolder.GetString(mDescription);   }
    [[nodiscard]] String ImageAsString()       const { return (sPathHolder.GetPath(mImagePath) / sFileHolder.GetString(mImageFile)).ToString(); }
    [[nodiscard]] String ThumbnailAsString()   const { return (sPathHolder.GetPath(mThumbnailPath) / sFileHolder.GetString(mThumbnailFile)).ToString(); }
    [[nodiscard]] String VideoAsString()       const { return (sPathHolder.GetPath(mVideoPath) / sFileHolder.GetString(mVideoFile)).ToString(); }
    [[nodiscard]] String DeveloperAsString()   const { return sDeveloperHolder.GetString(mDeveloper);       }
    [[nodiscard]] String PublisherAsString()   const { return sPublisherHolder.GetString(mPublisher);       }
    [[nodiscard]] String GenreAsString()       const { return sGenreHolder.GetString(mGenre);               }
    [[nodiscard]] String RegionAsString()      const { return Regions::Serialize4Regions(mRegion);          }

    [[nodiscard]] String RatingAsString()      const { return String(mRating, 2);                        }
    [[nodiscard]] String PlayersAsString()     const { return IntToRange(mPlayers);                                 }
    [[nodiscard]] String ReleaseDateAsString() const { return mReleaseDate != 0 ? DateTime((long long)mReleaseDate).ToCompactISO8601() : ""; }
    [[nodiscard]] String PlayCountAsString()   const { return String(mPlayCount);                           }
    [[nodiscard]] String LastPlayedAsString()  const { return mLastPlayed != 0 ? DateTime((long long)mLastPlayed).ToCompactISO8601() : ""; }
    [[nodiscard]] String FavoriteAsString()    const { return mFavorite ? "true" : "false";                         }
    [[nodiscard]] String RomCrc32AsString()    const { String r; IntToHex(mRomCrc32, r); return r;             }
    [[nodiscard]] String HiddenAsString()      const { return mHidden ? "true" : "false";                           }
    [[nodiscard]] String AdultAsString()       const { return mAdult ? "true" : "false";                            }
    [[nodiscard]] String GenreIdAsString()     const { return String((int)mGenreId);                           }
    [[nodiscard]] String LastPatchAsString()   const { return (sPathHolder.GetPath(mLastPatchPath) / sFileHolder.GetString(mLastPatchFile)).ToString(); }
    [[nodiscard]] String RotationAsString()    const { return RotationUtils::StringValue(mRotation); }
    [[nodiscard]] String TotalPlayTimeAsString()  const { return String(mTotalPlayTime);                        }

    /*
     * Setters
     */

    void SetRomPath(const Path& image)
    {
      mRomPath = sPathHolder.AddString16(image.Directory().ToString());
      mRomFile = sFileHolder.AddString32(image.Filename());
      mDirty = true;
    }
    void SetImagePath(const Path& image)
    {
      mImagePath = sPathHolder.AddString16(image.Directory().ToString());
      mImageFile = sFileHolder.AddString32(image.Filename());
      mDirty = true;
    }
    void SetThumbnailPath(const Path& thumbnail)
    {
      mThumbnailPath = sPathHolder.AddString16(thumbnail.Directory().ToString());
      mThumbnailFile = sFileHolder.AddString32(thumbnail.Filename());
      mDirty = true;
    }
    void SetVideoPath(const Path& video)
    {
      mVideoPath = sPathHolder.AddString16(video.Directory().ToString());
      mVideoFile = sFileHolder.AddString32(video.Filename());
      mDirty = true;
    }
    void SetLastPatch(const Path& patch)
    {
      mLastPatchPath = sPathHolder.AddString16(patch.Directory().ToString());
      mLastPatchFile = sFileHolder.AddString16(patch.Filename());
      mDirty = true;
    }
    void SetEmulator(const String& emulator)       { mEmulator     = sEmulatorHolder.AddString16(emulator);       mDirty = true; }
    void SetCore(const String& core)               { mCore         = sCoreHolder.AddString16(core);               mDirty = true; }
    void SetRatio(const String& ratio)             { mRatio        = sRatioHolder.AddString8(ratio);              mDirty = true; }
    void SetGenre(const String& genre)             { mGenre        = sGenreHolder.AddString32(genre);             mDirty = true; }
    void SetName(const String& name)               { mName         = sNameHolder.AddString32(name);               mDirty = true; }
    void SetDescription(const String& description) { mDescription  = sDescriptionHolder.AddString32(description); mDirty = true; }
    void SetReleaseDate(const DateTime& releasedate)    { mReleaseDate  = (int)releasedate.ToEpochTime();              mDirty = true; }
    void SetDeveloper(const String& developer)     { mDeveloper    = sDeveloperHolder.AddString32(developer);     mDirty = true; }
    void SetPublisher(const String& publisher)     { mPublisher    = sPublisherHolder.AddString32(publisher);     mDirty = true; }
    void SetRating(float rating)                        { mRating       = rating;                                      mDirty = true; }
    void SetPlayers(int min, int max)                   { mPlayers      = (max << 16) + min;                           mDirty = true; }
    void SetRegion(const Regions::RegionPack& regions)  { mRegion       = regions; mRegion.Sort();                     mDirty = true; }
    void SetRomCrc32(int romcrc32)                      { mRomCrc32     = romcrc32;                                    mDirty = true; }
    void SetFavorite(bool favorite)                     { mFavorite     = favorite;                                    mDirty = true; }
    void SetHidden(bool hidden)                         { mHidden       = hidden;                                      mDirty = true; }
    void SetAdult(bool adult)                           { mAdult        = adult;                                       mDirty = true; }
    void SetGenreId(GameGenres genre)                   { mGenreId      = genre;                                       mDirty = true; }
    void SetRotation(RotationType rotation)             { mRotation     = rotation;                                    mDirty = true; }
    void SetTotalPlayTime(int timePlayed)               { mTotalPlayTime= timePlayed;                                  mDirty = true; }
    // Volatiles flags - no dirtiness
    void SetPreinstalled(bool preinstalled)             { mPreinstalled = preinstalled;                                               }
    void SetLatestVersion(bool latestVersion)           { mLatestVerion = latestVersion;                                              }
    void SetNoGame(bool noGame)                         { mNoGame       = noGame;                                                     }
    void SetSupportIndex(int index)                     { mSupportIndex = index;                                                      }
    void SetSupportTotal(int total)                     { mSupportTotal = total;                                                      }
    void SetSupportSide(SupportSides side)              { mSupportSide  = side;                                                       }

    // Special setter to force dirty
    void SetDirty() { mDirty = true; }
    void UnsetDirty() { mDirty = false; }

    /*
     * Volatile setters - do not set the Dirty flag for auto-saving
     */

    void SetVolatileDescription(const String& description) { bool dirty = mDirty; SetDescription(description); mDirty = dirty; }
    void SetVolatileImagePath(const Path& image) { bool dirty = mDirty; SetImagePath(image); mDirty = dirty; }

    /*
     * String setters
     */

    void SetRomPathAsString(const String& image)           { SetRomPath(Path(image));           }
    void SetLastPatchAsString(const String& patch)         { SetLastPatch(Path(patch)); }
    void SetImagePathAsString(const String& image)         { SetImagePath(Path(image));         }
    void SetThumbnailPathAsString(const String& thumbnail) { SetThumbnailPath(Path(thumbnail)); }
    void SetVideoPathAsString(const String& video)         { SetVideoPath(Path(video));         }
    void SetReleaseDateAsString(const String& releasedate)
    {
      DateTime st;
      mReleaseDate = DateTime::FromCompactISO6801(releasedate, st) ? (int)st.ToEpochTime() : 0;
      mDirty = true;
    }
    void SetLastPlayedAsString(const String& lastplayed)
    {
      DateTime st;
      mLastPlayed = DateTime::FromCompactISO6801(lastplayed, st) ? (int)st.ToEpochTime() : 0;
      mDirty = true;
    }
    void SetRatingAsString(const String& rating)           { float f = 0.0f; if (StringToFloat(rating, f)) SetRating(f);              }
    void SetPlayersAsString(const String& players)         { if (!RangeToInt(players, mPlayers)) SetPlayers(1, 1);                    }
    void SetFavoriteAsString(const String& favorite)       { SetFavorite(favorite == "true");                                         }
    void SetHiddenAsString(const String& hidden)           { SetHidden(hidden == "true");                                             }
    void SetAdultAsString(const String& adult)             { SetAdult(adult == "true");                                             }
    void SetRomCrc32AsString(const String& romcrc32)       { int c = 0; if (HexToInt(romcrc32, c)) SetRomCrc32(c);                        }
    void SetPlayCountAsString(const String& playcount)     { int p = 0; if (StringToInt(playcount, p)) { mPlayCount = (short)p; mDirty = true; } }
    void SetGenreIdAsString(const String& genre)           { int g = 0; if (StringToInt(genre, g)) { mGenreId = (GameGenres)g; mDirty = true; } }
    void SetRegionAsString(const String& region)           { mRegion = Regions::Deserialize4Regions(region); mRegion.Sort(); mDirty = true; }
    void SetRotationAsString(const String& rotation)       { mRotation = RotationUtils::FromString(rotation); mDirty = true;}
    void SetTotalPlayTimeAsString(const String& timePlayed)   { int u = 0; if (StringToInt(timePlayed, u)) { mTotalPlayTime = u; mDirty = true; } }
    /*
     * Defaults
     */

    [[nodiscard]] bool IsDefaultRom()             const { return Default().mRomFile == mRomFile && Default().mRomPath == mRomPath; }
    [[nodiscard]] bool IsDefaultName()            const { return Default().mName == mName;               }
    [[nodiscard]] bool IsDefaultEmulator()        const { return Default().mEmulator == mEmulator;       }
    [[nodiscard]] bool IsDefaultCore()            const { return Default().mCore == mCore;               }
    [[nodiscard]] bool IsDefaultRatio()           const { return Default().mRatio == mRatio;           }
    [[nodiscard]] bool IsDefaultDescription()     const { return Default().mDescription == mDescription; }
    [[nodiscard]] bool IsDefaultImage()           const { return Default().mImageFile == mImageFile && Default().mImagePath == mImagePath; }
    [[nodiscard]] bool IsDefaultThumbnail()       const { return Default().mThumbnailFile == mThumbnailFile && Default().mThumbnailPath == mThumbnailPath; }
    [[nodiscard]] bool IsDefaultVideo()           const { return Default().mVideoFile == mVideoFile && Default().mVideoPath == mVideoPath; }
    [[nodiscard]] bool IsDefaultDeveloper()       const { return Default().mDeveloper == mDeveloper;     }
    [[nodiscard]] bool IsDefaultPublisher()       const { return Default().mPublisher == mPublisher;     }
    [[nodiscard]] bool IsDefaultGenre()           const { return Default().mGenre == mGenre;             }
    [[nodiscard]] bool IsDefaultRegion()          const { return Default().mRegion.Pack == mRegion.Pack; }
    [[nodiscard]] bool IsDefaultRating()          const { return Default().mRating == mRating;           }
    [[nodiscard]] bool IsDefaultPlayerRange()     const { return Default().mPlayers == mPlayers;         }
    [[nodiscard]] bool IsDefaultReleaseDateEpoc() const { return Default().mReleaseDate == mReleaseDate; }
    [[nodiscard]] bool IsDefaultPlayCount()       const { return Default().mPlayCount == mPlayCount;     }
    [[nodiscard]] bool IsDefaultLastPlayedEpoc()  const { return Default().mLastPlayed == mLastPlayed;   }
    [[nodiscard]] bool IsDefaultRomCrc32()        const { return Default().mRomCrc32 == mRomCrc32;       }
    [[nodiscard]] bool IsDefaultFavorite()        const { return Default().mFavorite == mFavorite;       }
    [[nodiscard]] bool IsDefaultHidden()          const { return Default().mHidden == mHidden;           }
    [[nodiscard]] bool IsDefaultAdult()           const { return Default().mAdult == mAdult;             }
    [[nodiscard]] bool IsDefaultGenreId()         const { return Default().mGenreId == mGenreId;         }
    [[nodiscard]] bool IsDefaultLastPath()        const { return Default().mLastPatchPath == mLastPatchPath && Default().mLastPatchFile == mLastPatchFile; }
    [[nodiscard]] bool IsDefaultRotation()        const { return Default().mRotation == mRotation; }
    [[nodiscard]] bool IsDefaultTotalPlayTime()   const { return Default().mTotalPlayTime == mTotalPlayTime; }
    [[nodiscard]] bool IsDefaultTate()            const { return Default().mRotation == mRotation; }

    /*
     * Convenient Accessors
     */

    [[nodiscard]] bool IsDirty()  const { return mDirty; }
    [[nodiscard]] bool HasSupportInformation() const { return mSupportSide != SupportSides::None || mSupportIndex != 0; }

    /*
     * Special modifiers
     */

    void IncPlayCount() { mPlayCount++; mDirty = true; }
    void SetLastPlayedNow() { mLastPlayed = (unsigned int)DateTime().ToEpochTime(); mDirty = true; }


    /*
     * Metadata FieldManagement Methods
     */

    /*!
     * Return the first static internal field descriptor reference
     * @param count Number of actual field descriptor available
     * @return first static internal field descriptor reference
     */
    const MetadataFieldDescriptor* GetMetadataFieldDescriptors(int& count) { return GetMetadataFieldDescriptors(mType, count); }

    /*
     * Index accessors
     */

    [[nodiscard]] MetadataStringHolder::Index32 FileIndex() const { return mRomFile; }
    [[nodiscard]] MetadataStringHolder::Index32 NameIndex() const { return mName; }
    [[nodiscard]] MetadataStringHolder::Index32 DescriptionIndex() const { return mDescription; }
    [[nodiscard]] MetadataStringHolder::Index32 DeveloperIndex() const { return mDeveloper; }
    [[nodiscard]] MetadataStringHolder::Index32 PublisherIndex() const { return mPublisher; }

    static int FileIndexCount() { return sFileHolder.ObjectCount(); }
    static int NameIndexCount() { return sNameHolder.ObjectCount(); }
    static int DescriptionIndexCount() { return sDescriptionHolder.ObjectCount(); }
    static int DeveloperIndexCount() { return sDeveloperHolder.ObjectCount(); }
    static int PublisherIndexCount() { return sPublisherHolder.ObjectCount(); }

    /*
     * Search
     */

    [[nodiscard]] bool IsMatchingFileIndex(MetadataStringHolder::Index32 index) const { return mRomFile == index; }
    [[nodiscard]] bool IsMatchingNameIndex(MetadataStringHolder::Index32 index) const { return mName == index; }
    [[nodiscard]] bool IsMatchingDescriptionIndex(MetadataStringHolder::Index32 index) const { return mDescription == index; }
    [[nodiscard]] bool IsMatchingDeveloperIndex(MetadataStringHolder::Index32 index) const { return mDeveloper == index; }
    [[nodiscard]] bool IsMatchingPublisherIndex(MetadataStringHolder::Index32 index) const { return mPublisher == index; }

    /*!
     * @brief Search text in game names
     * @param originaltext Text to search for
     * @param output Result container
     */
    static void SearchInNames(const String& originaltext, MetadataStringHolder::FoundTextList& output, int context) { return sNameHolder.FindText(originaltext, output, context); }

    /*!
     * @brief Search text in descriptions
     * @param originaltext Text to search for
     * @param output Result container
     */
    static void SearchInDescription(const String& originaltext, MetadataStringHolder::FoundTextList& output, int context) { return sDescriptionHolder.FindText(originaltext, output, context); }

    /*!
     * @brief Search text in developer names
     * @param originaltext Text to search for
     * @param output Result container
     */
    static void SearchInDeveloper(const String& originaltext, MetadataStringHolder::FoundTextList& output, int context) { return sDeveloperHolder.FindText(originaltext, output, context); }

    /*!
     * @brief Search text in publisher names
     * @param originaltext Text to search for
     * @param output Result container
     */
    static void SearchInPublisher(const String& originaltext, MetadataStringHolder::FoundTextList& output, int context) { return sPublisherHolder.FindText(originaltext, output, context); }

    /*!
     * @brief Search text in file names
     * @param originaltext Text to search for
     * @param output Result container
     */
    static void SearchInPath(const String& originaltext, MetadataStringHolder::FoundTextList& output, int context) { return sFileHolder.FindText(originaltext, output, context); }

    /*
     * Part comparers
     */

    [[nodiscard]] bool AreRomEqual(const MetadataDescriptor& other) const { return mRomFile == other.mRomFile && mRomPath == other.mRomPath; }
};

