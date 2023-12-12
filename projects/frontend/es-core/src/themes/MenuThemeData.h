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
      Path kodi;
      Path system;
      Path recalboxrgbdual;
      Path updates;
      Path games;
      Path controllers;
      Path ui;
      Path download;
      Path arcade;
      Path tate;
      Path sound;
      Path network;
      Path scraper;
      Path advanced;
      Path quit;
      Path restart;
      Path shutdown;
      Path fastshutdown;
      Path license;
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
