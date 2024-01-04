#pragma once

#include <themes/ThemeData.h>
#include <resources/Font.h>
#include "utils/cplusplus/INoCopy.h"

class MenuThemeData : private INoCopy
{
  public:
    struct MenuElement
    {
      unsigned int color = 0xFFFFFFFF;
      unsigned int selectedColor = 0x808080FF;
      unsigned int selectorColor = 0xC0C0C0FF;
      unsigned int separatorColor = 0x404040FF;
      Path path;
      Path fadePath;
      std::shared_ptr<Font> font;
    };

    struct MenuSize
    {
      float height;
    };

    struct IconElement
    {
      Path button;
      Path button_filled;
      Path on;
      Path off;
      Path option_arrow;
      Path arrow;
      Path knob;
    };

    struct MenuIcons
    {
      public:
        enum class Type
        {
          Kodi,
          System,
          RecalboxRGBDual,
          Updates,
          Games,
          Controllers,
          Ui,
          Download,
          Arcade,
          Tate,
          Sound,
          Network,
          Scraper,
          Advanced,
          Quit,
          Restart,
          Shutdown,
          Fastshutdown,
          License,
        };
        [[nodiscard]] const Path& FromType(Type type) const
        {
          switch(type)
          {
            case Type::Kodi: return mKodi;
            case Type::System: return mSystem;
            case Type::RecalboxRGBDual: return mRecalboxRGBDual;
            case Type::Updates: return mUpdates;
            case Type::Games: return mGames;
            case Type::Controllers: return mControllers;
            case Type::Ui: return mUI;
            case Type::Download: return mDownload;
            case Type::Arcade: return mArcade;
            case Type::Tate: return mTate;
            case Type::Sound: return mSound;
            case Type::Network: return mNetwork;
            case Type::Scraper: return mScraper;
            case Type::Advanced: return mAdvanced;
            case Type::Quit: return mQuit;
            case Type::Restart: return mRestart;
            case Type::Shutdown: return mShutdown;
            case Type::Fastshutdown: return mFastShutdown;
            case Type::License: return mLicense;
            default: break;
          }
          abort();
        }
      private:
        Path mKodi;
        Path mSystem;
        Path mRecalboxRGBDual;
        Path mUpdates;
        Path mGames;
        Path mControllers;
        Path mUI;
        Path mDownload;
        Path mArcade;
        Path mTate;
        Path mSound;
        Path mNetwork;
        Path mScraper;
        Path mAdvanced;
        Path mQuit;
        Path mRestart;
        Path mShutdown;
        Path mFastShutdown;
        Path mLicense;

        //! Allow access to the private area to the enlosing class
        friend class MenuThemeData;
    };

    //! Default constructor
    MenuThemeData()
      : mSize()
    {
      Reset();
    }

    /*!
     * @brief Load theme data from global theme
     * @param mainTheme Main theme data
     */
    void Load(ThemeData& mainTheme);

    //! Reset data to default values
    void Reset();

    /*
     * Getters
     */

    [[nodiscard]] const MenuElement& Background() const { return mBackground; }
    [[nodiscard]] const MenuElement& Title() const { return mTitle; }
    [[nodiscard]] const MenuElement& Footer() const { return mFooter; }
    [[nodiscard]] const MenuElement& Text() const { return mText; }
    [[nodiscard]] const MenuElement& SmallText() const { return mTextSmall; }
    [[nodiscard]] const MenuIcons&   Icons() const { return mIconSet; }
    [[nodiscard]] const IconElement& Elements() const { return mIconElement; }
    [[nodiscard]] float              Height() const { return mSize.height; }

  private:
    //! Menu background font/colors
    MenuElement mBackground;
    //! Menu title font/colors
    MenuElement mTitle;
    //! Menu footer font/colors
    MenuElement mFooter;
    //! Menu main text font/colors
    MenuElement mText;
    //! Menu small text font/colors
    MenuElement mTextSmall;
    //! Menu icon set path
    MenuIcons mIconSet;
    //! Menu icons elements path
    IconElement mIconElement;
    //! Menu sizes
    MenuSize mSize;
};
