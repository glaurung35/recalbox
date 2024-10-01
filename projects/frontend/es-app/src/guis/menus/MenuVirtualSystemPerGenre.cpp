//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/MenuVirtualSystemPerGenre.h>
#include <systems/SystemManager.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/GuiMsgBox.h>
#include <MainRunner.h>

MenuVirtualSystemPerGenre::MenuVirtualSystemPerGenre(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("VIRTUAL SYSTEMS PER GENRE"))
  , mSystemManager(systemManager)
{
}

void MenuVirtualSystemPerGenre::BuildMenuItems()
{
  // All games
  for(const GameGenres genre : Genres::GetOrderedList())
  {
    String shortName = Genres::GetShortName(genre);
    String longName = Genres::GetFullName(genre);
    String prefix = Genres::IsSubGenre(genre) ? "   \u21B3 " : "";
    Path icon = Path(Genres::GetResourcePath(genre));
    String identifier = SystemManager::BuildGenreSystemName(genre);
    bool value = RecalboxConf::Instance().IsInCollectionGenre(identifier);
    mGenres[identifier] = value;
    AddSwitch(icon, prefix + _S(longName), value, (int)genre, this, _S(longName));
  }
}

void MenuVirtualSystemPerGenre::MenuSwitchChanged(int id, bool& status)
{
  GameGenres genre = (GameGenres)id;
  String identifier = SystemManager::BuildGenreSystemName(genre);
  mGenres[identifier] = status;
  // Update list
  String::List list;
  for(const auto& it : mGenres)
    if (it.second)
      list.push_back(it.first);
  // Save and update systems
  RecalboxConf::Instance().SetCollectionGenre(list);
  mSystemManager.UpdateVirtualGenreSystemsVisibility(genre, status ? SystemManager::Visibility::ShowAndSelect : SystemManager::Visibility::Hide);
}
