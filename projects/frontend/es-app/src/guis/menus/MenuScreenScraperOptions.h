//
// Created by gugue_u on 03/01/2021.
//

#pragma once

#include <guis/menus/base/Menu.h>
#include <systems/SystemManager.h>
#include <scraping/scrapers/IScraperEngine.h>
#include <scraping/scrapers/screenscraper/ScreenScraperApis.h>
#include "guis/menus/base/IEditableChanged.h"

class MenuScreenScraperOptions : public Menu
                               , private IEditableChanged
                               , private ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperImageType>
                               , private ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperVideoType>
                               , private ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperRegionPriority>
                               , private ISingleSelectorChanged<Languages>
                               , private ISingleSelectorChanged<Regions::GameRegions>
                               , private ISwitchChanged
{
  public:
    explicit MenuScreenScraperOptions(WindowManager& window, SystemManager& systemManager, ScraperType scraperType);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
        Image,
        Thumbnail,
        Video,
        Manuals,
        Maps,
        PK2,
        RegionPriority,
        Region,
        Language,
        Login,
        Password,
    };

    //! System Manager instance
    SystemManager& mSystemManager;

    //! Scraper type
    ScraperType mType;


    //! Get login regarding scraper type
    String GetLogin();

    //! Get password regarding scraper type
    String GetPassword();

    //! Get login regarding scraper type
    void SetLogin(const String& login);

    //! Get password regarding scraper type
    void SetPassword(const String& password);

    static SelectorEntry<ScrapingMethod>::List GetScrapingMethods();
    static SelectorEntry<SystemData*>::List GetSystemsEntries();
    static SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List GetImagesEntries();
    static SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List GetThumbnailsEntries();
    static SelectorEntry<ScreenScraperEnums::ScreenScraperVideoType>::List GetVideosEntries();
    static SelectorEntry<ScreenScraperEnums::ScreenScraperRegionPriority>::List GetRegionOptionsEntries();
    static SelectorEntry<Regions::GameRegions>::List GetRegionsEntries();
    static SelectorEntry<Languages>::List GetLanguagesEntries();

    /*
     * IEditableChanged implementation
     */

    void MenuEditableChanged(int id, const String& text) override;

    /*
     * ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperImageType> implementations
     */

    void MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperImageType& value) override;

    /*
     * ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperVideoType> implementations
     */

    void MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperVideoType& value) override;

    /*
     * ISingleSelectorChanged<ScreenScraperEnums::ScreenScraperRegionPriority> implementations
     */

    void MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperRegionPriority& value) override;

    /*
     * ISingleSelectorChanged<Regions::GameRegions> implementations
     */

    void MenuSingleChanged(int id, int index, const Regions::GameRegions& value) override;

    /*
     * ISingleSelectorChanged<Languages> implementations
     */

    void MenuSingleChanged(int id, int index, const Languages& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};