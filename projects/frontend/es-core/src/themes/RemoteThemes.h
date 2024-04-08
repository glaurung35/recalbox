//
// Created by bkg2k on 08/04/24.
//
#pragma once

#include <themes/ThemeData.h>
#include <utils/SectionFile.h>

class RemoteThemes
{
  public:
    /*!
     * @brief Fetch remote theme list & information
     * @return True if the operation is successful, false on error
     */
    bool FetchRemoteThemes();

    //! Get remote theme count
    [[nodiscard]] int Count() const { return (int)mRemoteThemeList.size(); }

  private:
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
                    const String& version, const String& minrecalbox)
          : mFolder(folder)
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

      private:
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

    //! Remote theme list
    std::vector<RemoteTheme> mRemoteThemeList;

    //! Fetch from a given repository url
    bool FetchFrom(const String& url);

    /*!
     * @brief Deserialize theme data
     * @param sections SectionFile content
     * @param sectionName Section name to extract theme from
     */
    void Deserialize(const SectionFile& sections, const String& sectionName);

    static bool DeserializeImages(const SectionFile& sections, const String& sectionName, const String& suffix, int index, RemoteTheme::ImageLists& output);
};
