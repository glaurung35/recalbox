//
// Created by bkg2k on 08/04/24.
//
#pragma once

#include <themes/ThemeData.h>
#include <utils/SectionFile.h>
#include "rapidjson/document.h"
#include "IRemoteThemeDownloadNotifier.h"
#include <utils/os/system/Thread.h>
#include <utils/os/system/Signal.h>
#include <utils/sync/SyncMessageSender.h>

class RemoteThemes : public Thread
                   , private ISyncMessageReceiver<void>
{
  public:
    //! Constructor
    explicit RemoteThemes(IRemoteThemeDownloadNotifier* notifier)
      : mNotifier(notifier)
      , mSender(*this)
    {
      Thread::Start("rmt-themes");
    }

    //! Destructor
    ~RemoteThemes() override
    {
      Thread::Stop();
    }

    /*!
     * @brief Fetch remote theme list & information
     * @return True if the operation is successful, false on error
     */
    bool FetchRemoteThemes();

    /*!
     * @brief Start fetching illustration for the given theme index
     * @param themeIndex Theme index
     * @param crt Current display is on CRT?
     * @param tate Current display is TATE?
     */
    void FetchIllustration(int themeIndex, bool crt, bool tate);

    //! Get remote theme count
    [[nodiscard]] int Count() const { return (int)mRemoteThemeList.size(); }

  private:
    //! Download folder
    static constexpr const char* sDownloadFolder = "/recalbox/share/extractions";
    //! Public repo
    static constexpr const char* sPublicRepository = "https://gitlab.com/recalbox/contents/themes";
    //! Private repo
    static constexpr const char* sPrivateRepository = "https://gitlab.com/recalbox/contents/patronthemes";

    //! Remote theme descriptor
    class RemoteTheme
    {
      public:
        //! Maximum images
        static constexpr int sMaxImages = 8;

        //! Image list container
        struct ImageLists
        {
          String::List mSystemListImages;  //!< System list image files, relative to theme folder
          String::List mGameListImages;    //!< Game list image files, relative to theme folder
          String::List mMenuImages;        //!< Menu list image files, relative to theme folder

          // Check emptiness of all lists
          [[nodiscard]] bool IsEmpty() const { return mSystemListImages.empty() && mGameListImages.empty() && mMenuImages.empty(); }
        };

        //! Sub theme
        class SubTheme
        {
          public:
            //! Constructor
            SubTheme(const String& zipFile, ThemeData::Compatibility compatibility, ThemeData::Resolutions resolutions)
              : mZippedTheme(zipFile)
              , mCompatibility(compatibility)
              , mResolutions(resolutions)
            {}

            //! Get Zipped folder
            [[nodiscard]] const String& ZippedFolder() const { return mZippedTheme; }
            //! Get display compatibility
            [[nodiscard]] ThemeData::Compatibility Compatibility() const { return mCompatibility; }
            //! Get resolutions
            [[nodiscard]] ThemeData::Resolutions Resolutions() const { return mResolutions; }

          private:
            String mZippedTheme;                     //!< Zipped theme file, relative to it's top folder
            ThemeData::Compatibility mCompatibility; //!< Display compatibility
            ThemeData::Resolutions mResolutions;     //!< Resolution
        };

        //! Constructor
        RemoteTheme(const String& folder, const String& author, const String& name, const String& description,
                    const String& version, const String& minrecalbox, const String& baseUrl)
          : mBaseUrl(baseUrl)
          , mFolder(folder)
          , mAuthor(author)
          , mName(name)
          , mDescription(description)
          , mVersion(0x100)
          , mMinimumRecalboxVersion(0x902)
        {
          if (String major, minor; version.Extract('.', major, minor, true))
            mVersion = (major.AsInt() << 8) + minor.AsInt();
          if (String major, minor; minrecalbox.Extract('.', major, minor, true))
            mMinimumRecalboxVersion = (major.AsInt() << 8) + minor.AsInt();
        }

        //! Get base url
        [[nodiscard]] const String& BaseUrl() const { return mBaseUrl; }
        //! Get theme folder
        [[nodiscard]] const String& ThemeFolder() const { return mFolder; }
        //! Get Author
        [[nodiscard]] const String& Author() const { return mAuthor; }
        //! Get Theme nameAuthor
        [[nodiscard]] const String& Name() const { return mName; }
        //! Get Theme description
        [[nodiscard]] const String& Description() const { return mDescription; }
        //! Get Theme version
        [[nodiscard]] int Version() const { return mVersion; }
        //! Get Minimal Recalbox version required to handle this theme
        [[nodiscard]] int MinimumRecalboxVersion() const { return mMinimumRecalboxVersion; }

        /*!
         * @brief Add a new subtheme
         * @param subtheme Subtheme to add
         */
        void AddSubTheme(const String& zipfile, ThemeData::Compatibility compatibility, ThemeData::Resolutions resolutions)
        {
          mSubThemeList.push_back(SubTheme(zipfile, compatibility, resolutions));
        }

        //! Get sub theme count
        [[nodiscard]] int SubThemeCount() const { return (int)mSubThemeList.size(); }

        //! Get sub theme count
        [[nodiscard]] const SubTheme& GetSubThemeAt(int index) const { return mSubThemeList[index]; }

        //! Overall Compatibility
        [[nodiscard]] ThemeData::Compatibility Compatibility() const
        {
          ThemeData::Compatibility result = ThemeData::Compatibility::None;
          for(const SubTheme& sub : mSubThemeList)
            result |= sub.Compatibility();
          return result;
        }

        //! Overall Compatibility
        [[nodiscard]] ThemeData::Resolutions Resolutions() const
        {
          ThemeData::Resolutions result = ThemeData::Resolutions::None;
          for(const SubTheme& sub : mSubThemeList)
            result |= sub.Resolutions();
          return result;
        }

        void SetImageLists(ImageLists&& hdmi, ImageLists&& hdmiTate, ImageLists&& crt, ImageLists&& crtTate)
        {
          mImagesHdmi = std::move(hdmi);
          mImagesHdmiTate = std::move(hdmiTate);
          mImagesCrt = std::move(crt);
          mImagesCrtTate = std::move(crtTate);
        }

        /*!
         * @brief Select best non-empty image lists regarding the given display state, crt & tate mode.
         * @param crt True if we look for CRT images first
         * @param tate True if we look for TATE image first
         * @return Best ImageLists found
         */

        const RemoteTheme::ImageLists& SelectImageLists(bool crt, bool tate);

      private:
        String mBaseUrl;                       //!< Theme repository base URL
        String mFolder;                        //!< Theme folder, relative to the top folder
        String mAuthor;                        //!< Author name
        String mName;                          //!< Theme name
        String mDescription;                   //!< Free text description
        int mVersion;                          //!< Theme version
        int mMinimumRecalboxVersion;           //!< Minimal Recalbox version to handle this theme properly
        ImageLists mImagesHdmi;                //!< Image list for HDMI screens
        ImageLists mImagesHdmiTate;            //!< Image list for HDMI screens in TATE mode
        ImageLists mImagesCrt;                 //!< Image list for CRT screens
        ImageLists mImagesCrtTate;             //!< Image list for CRT screens in TATE mode
        std::vector<SubTheme> mSubThemeList;   //!< Sub themes
    };

    //! Queue object
    class Download
    {
      public:
        //! Default constructor
        Download()
          : mThemeIndex(0)
          , mType(RemoteIllustrationType::SystemList)
          , mIndex(0)
        {
        }

        /*!
         * @brief Constructor
         * @param url Url to download illustration from
         * @param path File to store illustration into
         * @param themeIndex Theme index
         * @param type Illustration type
         * @param index Illustration index
         */
        Download(String&& url, Path&& path, int themeIndex, RemoteIllustrationType type, int index)
          : mUrl(std::move(url))
          , mPath(std::move(path))
          , mThemeIndex(themeIndex)
          , mType(type)
          , mIndex(index)
        {
        }

        //! Get url
        [[nodiscard]] const String& Url() const { return mUrl; }
        //! Get path
        [[nodiscard]] const Path& FilePath() const { return mPath; }
        //! Get theme index
        [[nodiscard]] int ThemeIndex() const { return mThemeIndex; }
        //! Get illustration type
        [[nodiscard]] RemoteIllustrationType Type() const { return mType; }
        //! Get illustration index
        [[nodiscard]] int Index() const { return mIndex; }

      private:
        String mUrl;                  //!< Url to download illustration from
        Path   mPath;                 //!< File to store illustration into
        int    mThemeIndex;           //!< Theme index
        RemoteIllustrationType mType; //!< Illustration type
        int    mIndex;                //!< Illustration index
    };

    //! Remote theme list
    std::vector<RemoteTheme> mRemoteThemeList;

    //! Notifier callback
    IRemoteThemeDownloadNotifier* mNotifier;

    //! Syncronous event
    SyncMessageSender<void> mSender;

    //! Download complete
    std::vector<Download> mCompleted;
    //! Download queue
    std::vector<Download> mQueue;
    //! Download signal
    Signal mSignal;
    //! Queue guardian
    Mutex mLocker;

    //! Fetch from a given repository url
    bool FetchFrom(const String& url);

    /*!
     * @brief Deserialize theme data
     * @param theme JSON object to deserialize
     * @param index theme index (for logging)
     * @param optional True if the object is optional and must not log errors
     */
    void Deserialize(rapidjson::Value& theme, const String& url, int index);

    /*!
     * @brief Deserialize a whole illustration node
     * @param theme Theme JSON object
     * @param name Illustration node name
     * @param sectionName Image list name
     * @param index theme index (for logging)
     * @param optional True if the object is optional and must not log errors
     * @return True if the illustration node has been deserialized, false if any error occurred
     */
    static bool DeserializeImages(rapidjson::Value& theme, const char* name, RemoteTheme::ImageLists& sectionName, const String& url, int index);

    /*!
     * @brief Get the named array from the given JSON theme
     * @param theme Theme JSON node
     * @param name Name of the object to lookup
     * @param object Pointer to the object from theme object if found. Nullptr otherwise
     * @param url source url (for logging)
     * @param index theme index (for logging)
     * @param optional True if the object is optional and must not log errors
     * @return True if the array exists, false otherwise
     */
    static bool GetObject(rapidjson::Value& theme, const char* name, rapidjson::Value*& object, const String& url, int index, bool optional);

    /*!
     * @brief Get the named array from the given JSON theme
     * @param theme Theme JSON node
     * @param name Name of the array to lookup
     * @param array Pointer to the array from JSON object if found. Nullptr otherwise
     * @param url source url (for logging)
     * @param index theme index (for logging)
     * @return True if the array exists, false otherwise
     */
    static bool GetArray(rapidjson::Value& theme, const char* name, rapidjson::Value*& array, const String& url, int index);

    /*!
     * @brief Deserialize string. If the string is unavailable, log error and return false
     * @param object Parent JSON node
     * @param name Name of the string
     * @param string Output string
     * @param url source url (for logging)
     * @param index theme index (for logging)
     * @return True if the string exists and has been deserialized, false otherwise
     */
    static bool DeserializeStringMandatory(rapidjson::Value& object, const char* name, String& string, const String& url, int index);

    /*!
     * @brief Deserialize string. If the string is unavailable, return the default value
     * @param object Parent JSON node
     * @param name Name of the string
     * @param string Output string
     * @param defaultValue Default value if the string does not exist in the JSON node
     * @return Always true
     */
    static bool DeserializeStringOptional(rapidjson::Value& object, const char* name, String& string, const String& defaultValue);

    /*!
     * @brief Deserialize an array of string
     * @param object Parent JSON node
     * @param name Name of the array
     * @param list Output list filled with deserialized strings
     * @param url source url (for logging)
     * @param index theme index (for logging)
     * @return True if the array exists and has been deserialized (even empty), false otherwise
     */
    static bool DeserializeArrayOfString(rapidjson::Value& object, const char* name, String::List& list, const String& url, int index);

    /*!
     * @brief Push all existing images from an image list into the dowload queue
     * @param imageList
     * @param themeIndex
     */
    void PushImageListsToDownloadQueue(const RemoteTheme::ImageLists& imageList, int themeIndex);

    /*!
     * @brief Push a single image into the download queue. If the image is already downloaded, call the notifier immediately
     * @param baseUrl Base url including required folders
     * @param filename Illustration filename
     * @param themeIndex Theme index
     * @param type Illustration type
     * @param index Illustration index
     */
    void PushImageToDownloadQueue(const String& baseUrl, const String& filename, int themeIndex, RemoteIllustrationType type, int index);

    /*
     * Thread implementation
     */

    //! Download thread runner
    void Run() override;

    //! Break
    void Break() override { mSignal.Fire(); }

    /*
     * Synchronous event implementation
     */

    /*!
     * @brief Receive synchronous message
     */
    void ReceiveSyncMessage() override;
};
