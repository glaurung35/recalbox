//
// Created by gugue_u on 03/01/2021.
//

#include <scraping/ScraperFactory.h>
#include <guis/GuiScraperRun.h>
#include "MenuScreenScraperOptions.h"
#include <guis/MenuMessages.h>

MenuScreenScraperOptions::MenuScreenScraperOptions(WindowManager& window, SystemManager& systemManager, ScraperType scraperType)
  : Menu(window, _("SCREENSCRAPER OPTIONS"))
  , mSystemManager(systemManager)
  , mType(scraperType)
{
  AddList<ScreenScraperEnums::ScreenScraperImageType>(_("SELECT IMAGE TYPE"), (int)Components::Image, this, GetImagesEntries(), RecalboxConf::Instance().GetScreenScraperMainMedia(), ScreenScraperEnums::ScreenScraperImageType::MixV2,_(MENUMESSAGE_SCREENSCRAPER_IMAGE_HELP_MSG));
  AddList<ScreenScraperEnums::ScreenScraperVideoType>(_("SELECT VIDEO TYPE"), (int)Components::Video, this, GetVideosEntries(), RecalboxConf::Instance().GetScreenScraperVideo(), ScreenScraperEnums::ScreenScraperVideoType::None, _(MENUMESSAGE_SCREENSCRAPER_VIDEO_HELP_MSG));
  AddList<ScreenScraperEnums::ScreenScraperImageType>(_("SELECT THUMBNAIL TYPE"), (int)Components::Thumbnail, this, GetThumbnailsEntries(), RecalboxConf::Instance().GetScreenScraperThumbnail(), ScreenScraperEnums::ScreenScraperImageType::None, _(MENUMESSAGE_SCREENSCRAPER_THUMBNAIL_HELP_MSG));

  AddList<ScreenScraperEnums::ScreenScraperRegionPriority>(_("SELECT REGION PRIORITY"), (int)Components::RegionPriority, this, GetRegionOptionsEntries(), RecalboxConf::Instance().GetScreenScraperRegionPriority(), ScreenScraperEnums::ScreenScraperRegionPriority::DetectedRegion, _(MENUMESSAGE_SCREENSCRAPER_PRIORITY_HELP_MSG));
  AddList<Regions::GameRegions>(_("SELECT FAVORITE REGION"), (int)Components::Region, this, GetRegionsEntries(), RecalboxConf::Instance().GetScreenScraperRegion(), Regions::GameRegions::WOR, _(MENUMESSAGE_SCREENSCRAPER_REGION_HELP_MSG));
  AddList<Languages>(_("SELECT FAVORITE LANGUAGE"), (int)Components::Language, this, GetLanguagesEntries(), LanguagesTools::GetScrapingLanguage(), Languages::EN, _(MENUMESSAGE_SCREENSCRAPER_LANGUAGE_HELP_MSG));

  AddSwitch(_("DOWNLOAD GAME MANUALS"), RecalboxConf::Instance().GetScreenScraperWantManual(), (int)Components::Manuals, this, _(MENUMESSAGE_SCREENSCRAPER_MANUALS_HELP_MSG));
  AddSwitch(_("DOWNLOAD GAME MAPS"), RecalboxConf::Instance().GetScreenScraperWantMaps(), (int)Components::Maps, this, _(MENUMESSAGE_SCREENSCRAPER_MAPS_HELP_MSG));
  AddSwitch(_("INSTALL PAD-2-KEYBOARD CONFIGURATIONS"), RecalboxConf::Instance().GetScreenScraperWantP2K(), (int)Components::PK2, this, _(MENUMESSAGE_SCREENSCRAPER_P2K_HELP_MSG));

  AddEditable(_("USERNAME"), GetLogin(), (int)Components::Login, this, _(MENUMESSAGE_SCREENSCRAPER_USERNAME_HELP_MSG), false, mType != ScraperType::ScreenScraper,
              _("Username not required for the selected scraper"));
  AddEditable(_("PASSWORD"), GetPassword(), (int)Components::Password, this, _(MENUMESSAGE_SCREENSCRAPER_PASSWORD_HELP_MSG), true, mType != ScraperType::ScreenScraper,
              _("Password not required for the selected scraper"));
}

SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List MenuScreenScraperOptions::GetImagesEntries()
{
  SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List list;
  list.push_back({ _("In-game screenshot"), ScreenScraperEnums::ScreenScraperImageType::ScreenshotIngame });
  list.push_back({ _("Title screenshot"),   ScreenScraperEnums::ScreenScraperImageType::ScreenshotTitle });
  list.push_back({ _("Clear logo"),         ScreenScraperEnums::ScreenScraperImageType::Wheel });
  list.push_back({ _("Marquee"),            ScreenScraperEnums::ScreenScraperImageType::Marquee });
  list.push_back({ _("box2D"),              ScreenScraperEnums::ScreenScraperImageType::Box2d });
  list.push_back({ _("box3d"),              ScreenScraperEnums::ScreenScraperImageType::Box3d });
  list.push_back({ _("ScreenScraper Mix V1"),ScreenScraperEnums::ScreenScraperImageType::MixV1 });
  list.push_back({ _("ScreenScraper Mix V2"), ScreenScraperEnums::ScreenScraperImageType::MixV2 });
  return list;
}

SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List MenuScreenScraperOptions::GetThumbnailsEntries()
{
  SelectorEntry<ScreenScraperEnums::ScreenScraperImageType>::List list;
  list.push_back({ _("No thumbnail"),       ScreenScraperEnums::ScreenScraperImageType::None });
  list.push_back({ _("In-game screenshot"), ScreenScraperEnums::ScreenScraperImageType::ScreenshotIngame });
  list.push_back({ _("Title screenshot"),   ScreenScraperEnums::ScreenScraperImageType::ScreenshotTitle });
  list.push_back({ _("Clear logo"),         ScreenScraperEnums::ScreenScraperImageType::Wheel });
  list.push_back({ _("Marquee"),            ScreenScraperEnums::ScreenScraperImageType::Marquee });
  list.push_back({ _("box2D"),              ScreenScraperEnums::ScreenScraperImageType::Box2d });
  list.push_back({ _("box3d"),              ScreenScraperEnums::ScreenScraperImageType::Box3d });
  list.push_back({ _("ScreenScraper Mix V1"), ScreenScraperEnums::ScreenScraperImageType::MixV1 });
  list.push_back({ _("ScreenScraper Mix V2"), ScreenScraperEnums::ScreenScraperImageType::MixV2 });
  return list;
}

SelectorEntry<ScreenScraperEnums::ScreenScraperVideoType>::List MenuScreenScraperOptions::GetVideosEntries()
{
  SelectorEntry<ScreenScraperEnums::ScreenScraperVideoType>::List list;
  list.push_back({ _("No video"), ScreenScraperEnums::ScreenScraperVideoType::None });
  list.push_back({ String(_("Optimized video")).Append(" (").Append(_("RECOMMENDED")).Append(')'), ScreenScraperEnums::ScreenScraperVideoType::Optimized });
  list.push_back({ _("Original video"), ScreenScraperEnums::ScreenScraperVideoType::Raw});
  return list;
}

SelectorEntry<ScreenScraperEnums::ScreenScraperRegionPriority>::List MenuScreenScraperOptions::GetRegionOptionsEntries()
{
  SelectorEntry<ScreenScraperEnums::ScreenScraperRegionPriority>::List list;
  list.push_back({_("DETECTED REGION"), ScreenScraperEnums::ScreenScraperRegionPriority::DetectedRegion });
  list.push_back({_("FAVORITE REGION"), ScreenScraperEnums::ScreenScraperRegionPriority::FavoriteRegion });
  return list;
}

SelectorEntry<Regions::GameRegions>::List MenuScreenScraperOptions::GetRegionsEntries()
{
  SelectorEntry<Regions::GameRegions>::List list;
  for(Regions::GameRegions region : Regions::AvailableRegions())
  {
    if(Regions::GameRegions::Unknown == region) continue;
    list.push_back({ Regions::RegionFullName(region), region });
  }
  return list;
}

SelectorEntry<Languages>::List MenuScreenScraperOptions::GetLanguagesEntries()
{
  SelectorEntry<Languages>::List list;
  for(Languages language : LanguagesTools::AvailableLanguages())
  {
    if (language == Languages::Unknown) continue;
    list.push_back({ LanguagesTools::LanguagesFullName(language), language });
  }
  return list;
}

void MenuScreenScraperOptions::MenuEditableChanged(int id, const String& text)
{
  if ((Components)id == Components::Login) SetLogin(text);
  else if ((Components)id == Components::Password) SetPassword(text);
}

void MenuScreenScraperOptions::MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperImageType& value)
{
  (void)index;
  if((Components)id == Components::Image)
    RecalboxConf::Instance().SetScreenScraperMainMedia(value).Save();
  if((Components)id == Components::Thumbnail)
    RecalboxConf::Instance().SetScreenScraperThumbnail(value).Save();
}

void MenuScreenScraperOptions::MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperVideoType& value)
{
  (void)index;
  if((Components)id == Components::Video)
    RecalboxConf::Instance().SetScreenScraperVideo(value).Save();
}

void MenuScreenScraperOptions::MenuSwitchChanged(int id, bool& status)
{
  if ((Components)id == Components::Manuals) RecalboxConf::Instance().SetScreenScraperWantManual(status).Save();;
  if ((Components)id == Components::Maps) RecalboxConf::Instance().SetScreenScraperWantMaps(status).Save();;
  if ((Components)id == Components::PK2) RecalboxConf::Instance().SetScreenScraperWantP2K(status).Save();;
}

void MenuScreenScraperOptions::MenuSingleChanged(int id, int index, const ScreenScraperEnums::ScreenScraperRegionPriority& value)
{
  (void)index;
  if((Components)id == Components::RegionPriority)
    RecalboxConf::Instance().SetScreenScraperRegionPriority(value).Save();
}

void MenuScreenScraperOptions::MenuSingleChanged(int id, int index, const Regions::GameRegions& value)
{
  (void)index;
  if((Components)id == Components::Region)
    RecalboxConf::Instance().SetScreenScraperRegion(value).Save();
}

void MenuScreenScraperOptions::MenuSingleChanged(int id, int index, const Languages& value)
{
  (void)index;
  if((Components)id == Components::Language)
    RecalboxConf::Instance().SetScreenScraperLanguage(value).Save();
}

String MenuScreenScraperOptions::GetLogin()
{
  switch(mType)
  {
    case ScraperType::ScreenScraper: return RecalboxConf::Instance().GetScreenScraperLogin();
    case ScraperType::Recalbox:
    case ScraperType::TheGameDB:
    default: break;
  }
  return String();
}

String MenuScreenScraperOptions::GetPassword()
{
  switch(mType)
  {
    case ScraperType::ScreenScraper: return RecalboxConf::Instance().GetScreenScraperPassword();
    case ScraperType::Recalbox:
    case ScraperType::TheGameDB:
    default: break;
  }
  return String();
}

void MenuScreenScraperOptions::SetLogin(const String& login)
{
  switch(mType)
  {
    case ScraperType::ScreenScraper: RecalboxConf::Instance().SetScreenScraperLogin(login).Save(); break;
    case ScraperType::Recalbox:
    case ScraperType::TheGameDB:
    default: break;
  }
}

void MenuScreenScraperOptions::SetPassword(const String& password)
{
  switch(mType)
  {
    case ScraperType::ScreenScraper: RecalboxConf::Instance().SetScreenScraperPassword(password).Save(); break;
    case ScraperType::Recalbox:
    case ScraperType::TheGameDB:
    default: break;
  }
}
