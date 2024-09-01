//
// Created by gugue_u on 03/01/2021.
//

#include <guis/menus/MenuScraper.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>
#include <scraping/ScraperFactory.h>
#include <scraping/ScraperTools.h>
#include <guis/GuiScraperRun.h>
#include <guis/GuiMsgBox.h>
#include "MenuNetwork.h"
#include "MenuScreenScraperOptions.h"
#include <emulators/run/GameRunner.h>
#include <patreon/PatronInfo.h>

MenuScraper::MenuScraper(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("SCRAPER")),
  mSystemManager(systemManager)
{
  mScrapers = AddList<ScraperType>(_("SCRAPE FROM"), (int)Components::Scraper, this, GetScrapersEntries(), RecalboxConf::Instance().GetScraperSource(), ScraperType::ScreenScraper, _(MENUMESSAGE_SCRAPER_FROM_HELP_MSG));

  if (PatronInfo::Instance().IsPatron())
    AddSwitch(_("AUTOMATIC SCRAPING"), RecalboxConf::Instance().GetScraperAuto(), (int)Components::ScraperAuto, this, _(MENUMESSAGE_SCRAPER_AUTO_HELP_MSG));

  AddSubMenu(_("SCRAPER OPTIONS"), (int)Components::ScraperOptions, this, _(MENUMESSAGE_SCRAPER_OPTIONS_HELP_MSG));

  AddList<ScraperNameOptions>(_("GET GAME NAME FROM"), (int)Components::ScrapeNameFrom, this,
                              GetNameOptionsEntries(), RecalboxConf::Instance().GetScraperNameOptions(), ScraperNameOptions::GetFromScraper, _(MENUMESSAGE_SCRAPER_GET_NAME_FROM_HELP_MSG));

  mScrapingMethod = AddList<ScrapingMethod>(_("FILTER"), (int)Components::ScrapingMethod, nullptr, GetScrapingMethods(), ScrapingMethod::AllIfNothingExists, ScrapingMethod::AllIfNothingExists, _(MENUMESSAGE_SCRAPER_FILTER_HELP_MSG));

  mSystems = AddMultiList<SystemData*>(_("SYSTEMS"), (int)Components::Systems, nullptr, GetSystemsEntries(), _(MENUMESSAGE_SCRAPER_SYSTEMS_HELP_MSG));

  AddAction(_("SCRAPE NOW"), _("RUN"), (int)Components::Run, true, this, _("Run the scraper !"));
}

SelectorEntry<ScraperType>::List MenuScraper::GetScrapersEntries()
{
  SelectorEntry<ScraperType>::List list;
  for(const auto& kv : ScraperFactory::GetScraperList())
    list.push_back({ kv.second, kv.first });
  return list;
}

SelectorEntry<ScraperNameOptions>::List MenuScraper::GetNameOptionsEntries()
{
  SelectorEntry<ScraperNameOptions>::List list;
  list.push_back({ _("Scraper results"), ScraperNameOptions::GetFromScraper });
  list.push_back({ _("Raw filename"), ScraperNameOptions::GetFromFilename });
  list.push_back({ _("Undecorated filename"), ScraperNameOptions::GetFromFilenameUndecorated });
  return list;
}

SelectorEntry<ScrapingMethod>::List MenuScraper::GetScrapingMethods()
{
  SelectorEntry<ScrapingMethod>::List list;
  list.push_back({ _("All Games"), ScrapingMethod::All });
  list.push_back({ _("Only missing image"), ScrapingMethod::AllIfNothingExists });
  return list;
}

SelectorEntry<SystemData*>::List MenuScraper::GetSystemsEntries()
{
  SelectorEntry<SystemData*>::List list;
  for(SystemData* system : mSystemManager.VisibleSystemList())
  {
    if (!system->IsScreenshots()) // Only screenshot doesn't need to be scraped
      if (system->HasScrapableGame())
        list.push_back({ system->FullName(), system, false });
  }
  return list;
}

void MenuScraper::MenuSingleChanged(int id, int index, const ScraperType& value)
{
  (void)index;
  if ((Components)id == Components::Scraper)
    RecalboxConf::Instance().SetScraperSource(value).Save();
}

void MenuScraper::MenuSingleChanged(int id, int index, const ScraperNameOptions & value)
{
  (void)index;
  if ((Components)id == Components::ScrapeNameFrom)
    RecalboxConf::Instance().SetScraperNameOptions(value).Save();
}

void MenuScraper::SubMenuSelected(int id)
{
  if ((Components)id == Components::ScraperOptions)
    switch (mScrapers->SelectedValue())
    {
      case ScraperType::TheGameDB:
      case ScraperType::Recalbox:
      case ScraperType::ScreenScraper:
          mWindow.pushGui(new MenuScreenScraperOptions(mWindow, mSystemManager, RecalboxConf::Instance().GetScraperSource())); break;
    }
}

void MenuScraper::MenuActionTriggered(int id)
{
  if (id == (int)Components::Run)
  {
    if (mSystems->SelectedCount() == 0)
    {
      String text = _("Please select one or more systems to scrape!");
      GuiMsgBox* msgBox = new GuiMsgBox(mWindow, text, _("OK"), nullptr);
      mWindow.pushGui(msgBox);
    }
    else
      GuiScraperRun::CreateOrShow(mWindow, mSystemManager, mSystems->AllSelectedValuesAsArray(),
                                  mScrapingMethod->SelectedValue(), &GameRunner::Instance(),
                                  Renderer::Instance().DisplayHeightAsInt() <= 576);
  }
}

void MenuScraper::MenuSwitchChanged(int id, bool& status)
{
  if ((Components)id == Components::ScraperAuto)
    RecalboxConf::Instance().SetScraperAuto(status).Save();
}
