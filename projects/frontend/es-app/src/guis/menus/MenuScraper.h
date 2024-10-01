//
// Created by gugue_u on 03/01/2021.
//

#pragma once

#include <guis/menus/base/Menu.h>
#include <systems/SystemManager.h>
#include <scraping/ScraperTools.h>
#include <scraping/ScraperFactory.h>

class MenuScraper : public Menu
                  , private ISingleSelectorChanged<ScraperNameOptions>
                  , private ISingleSelectorChanged<ScraperType>
                  , private ISwitchChanged
                  , private ISubMenuSelected
                  , private IActionTriggered
{
  public:
    explicit MenuScraper(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
        Scraper,
        ScraperAuto,
        ScraperOptions,
        ScrapeNameFrom,
        ScrapingMethod,
        Systems,
        Run,
    };

    ItemSelector<ScraperType>* mScrapers;
    ItemSelector<ScrapingMethod>* mScrapingMethod;
    ItemSelector<SystemData*>* mSystems;

    //! SystemManager reference
    SystemManager& mSystemManager;

    static ItemSelector<ScrapingMethod>::List GetScrapingMethods();
    ItemSelector<SystemData*>::List GetSystemsEntries();
    static ItemSelector<ScraperType>::List GetScrapersEntries();
    static ItemSelector<ScraperNameOptions>::List GetNameOptionsEntries();

    void SubMenuSelected(int id) override;

    /*
     * ISingleSelectorChanged<ScraperNameOptions> implementation
     */

    void MenuSingleChanged(int id, int index, const ScraperNameOptions& value) final;

    /*
     * ISingleSelectorChanged<ScraperFactory::ScraperType> implementation
     */

    void MenuSingleChanged(int id, int index, const ScraperType& value) final;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) final;

    /*
     * IActionInterface
     */

    void MenuActionTriggered(int id) final;
};