#pragma once

#include <scraping/scrapers/IScraperEngineFreezer.h>
#include "guis/menus/base/Menu.h"

// Forward declarations
class WindowManager;
class SystemManager;

class MenuMain : public Menu
              , private ISubMenuSelected
{
  public:
    //! Constructor
    MenuMain(WindowManager& window, SystemManager& systemManager, const IGlobalVariableResolver& resolver);

  private:
    enum class Components
    {
      Kodi,
      System,
      Update,
      RecalboxRGBDual,
      Games,
      ContentDoanwloader,
      Controllers,
      UISettings,
      Arcade,
      Tate,
      Sound,
      Network,
      Scraper,
      Advanced,
      Bios,
      License,
      Quit,
    };

    //! Global variable resolver
    const IGlobalVariableResolver& mResolver;
    //! SystemManager instance
    SystemManager& mSystemManager;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*!
     * @brief Do its job
     * @param input Input string
     * @param key Key
     * @return Result string
     */
    static String ScrambleSymetric2(const String& input, const String& key);
};
