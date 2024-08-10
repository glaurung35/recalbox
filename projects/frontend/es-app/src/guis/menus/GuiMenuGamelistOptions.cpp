#include "GuiMenuGamelistOptions.h"
#include "guis/GuiSaveStates.h"
#include "guis/GuiDownloader.h"
#include "views/gamelist/ArcadeGameListView.h"
#include <systems/arcade/ArcadeVirtualSystems.h>
#include "GuiMenuArcade.h"
#include <guis/GuiSearch.h>
#include <MainRunner.h>
#include <views/gamelist/ISimpleGameListView.h>
#include <games/GameFilesUtils.h>
#include <components/SwitchComponent.h>
#include <guis/MenuMessages.h>
#include <guis/GuiMsgBox.h>
#include <guis/menus/GuiMenuQuit.h>
#include <guis/menus/GuiMenu.h>
#include <guis/menus/GuiMenuGamelistGameOptions.h>
#include <guis/menus/GuiMenuGamelistGameDeleteOptions.h>
#include <scraping/ScraperSeamless.h>

GuiMenuGamelistOptions::GuiMenuGamelistOptions(WindowManager& window, SystemData& system, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface, const IGlobalVariableResolver& resolver)
  :	GuiMenuBase(window, _("OPTIONS"), this)
  , mResolver(resolver)
  , mSystem(system)
  , mSystemManager(systemManager)
  , mGamelist(*ViewController::Instance().GetOrCreateGamelistView(&system))
  , mArcade(arcadeInterface)
{
  // edit game metadata
  bool nomenu = RecalboxConf::Instance().GetMenuType() == RecalboxConf::Menu::None;
  bool bartop = RecalboxConf::Instance().GetMenuType() == RecalboxConf::Menu::Bartop;
  if (!nomenu && !bartop)
  {
    String editTitle;
    const FileData* file = mGamelist.getCursor();
    bool isGame = mGamelist.getCursor()->IsGame();
    bool isFolder = mGamelist.getCursor()->IsFolder();
    if (isGame)  editTitle = file->TopAncestor().ReadOnly() ? _("NON EDITABLE GAME"): _("EDIT GAME");
    else if (isFolder) editTitle = _("EDIT FOLDER");
    if (!editTitle.empty()) mGame = AddSubMenu(editTitle, (int) Components::MetaData,_(MENUMESSAGE_GAMELISTOPTION_EDIT_METADATA_MSG));

    if (isGame || isFolder)
    {
      if (!mSystem.IsVirtual() && isGame && !mGamelist.getCursor()->TopAncestor().ReadOnly() &&
          !mSystem.IsScreenshots())
      {
        String text = _("DELETE GAME %s").Replace("%s", mGamelist.getCursor()->Name().ToUpperCaseUTF8());
        AddSubMenu(text, (int) Components::Delete, _(MENUMESSAGE_GAMELISTOPTION_DELETE_GAME_MSG));
      }

      if (mSystem.IsScreenshots())
      {
        AddSubMenu(_("DELETE SCREENSHOT"), (int) Components::DeleteScreeshot, _(MENUMESSAGE_GAMELISTOPTION_DELETE_SCREENSHOT_MSG));
      }

      if (RecalboxConf::Instance().GetAutorunEnabled() && isGame)
      {
        AddSwitch(_("BOOT ON THIS GAME"), mGamelist.getCursor()->RomPath().ToString() == RecalboxConf::Instance().GetAutorunGamePath(), (int) Components::AutorunGame, this, _(MENUMESSAGE_GAMELISTOPTION_BOOT_GAME_MSG));
      }

      if (!GameFilesUtils::GetGameSaveStateFiles(*mGamelist.getCursor()).empty())
        AddSubMenu(_("SAVE STATES"), (int) Components::SaveStates, _(MENUMESSAGE_GAMELISTOPTION_SAVE_STATES_MSG));
    }
  }

  RefreshGameMenuContext();

  // Downloader available?
  if (DownloaderManager::HasDownloader(mSystem))
    AddSubMenu(_("DOWNLOAD GAMES"),  (int)Components::Download, _(MENUMESSAGE_GAMELISTOPTION_DOWNLOAD_GAMES_MSG));

  // Jump to letter
	AddList<unsigned int>(_("JUMP TO LETTER"), (int)Components::JumpToLetter, this, GetLetterEntries(), _(MENUMESSAGE_GAMELISTOPTION_JUMP_LETTER_MSG));

  // open search wheel for this system
  if (!system.IsFavorite())
    AddSubMenu(_("SEARCH GAMES HERE"),  (int)Components::Search, _(MENUMESSAGE_GAMELISTOPTION_SEARCH_GAMES_MSG));

  // Sorting
	if (!system.IsSelfSorted())
	  mListSort = AddList<FileSorts::Sorts>(_("SORT GAMES BY"), (int)Components::Sorts, this, GetSortEntries(), _(MENUMESSAGE_GAMELISTOPTION_SORT_GAMES_MSG));

  // Global arcade option available on any arcade system, true or virtual
  if (system.IsArcade())
    AddSubMenu(_("ARCADE SETTINGS"), (int)Components::ArcadeOptions, _(MENUMESSAGE_ARCADE_HELP_MSG));

  // Decorations
  AddMultiList(_("DECORATIONS"), (int)Components::Decorations, this, GetDecorationEntries(), _(MENUMESSAGE_GAMELISTOPTION_DECORATIONS_MSG));

  // Region filter
  AddList<Regions::GameRegions>(_("HIGHLIGHT GAMES OF REGION..."), (int)Components::Regions, this, GetRegionEntries(), _(MENUMESSAGE_GAMELISTOPTION_FILTER_REGION_MSG));

  // flat folders
  if (!system.IsFavorite())
  {
    if (!system.IsAlwaysFlat())
      AddSwitch(_("SHOW FOLDERS CONTENT"), RecalboxConf::Instance().GetSystemFlatFolders(mSystem), (int) Components::FlatFolders, this, _(MENUMESSAGE_GAMELISTOPTION_SHOW_FOLDER_CONTENT_MSG));

    // favorites only
    AddSwitch(_("SHOW ONLY FAVORITES"), RecalboxConf::Instance().GetFavoritesOnly(), (int) Components::FavoritesOnly, this, _(MENUMESSAGE_UI_FAVORITES_ONLY_MSG));
    // favorites first
    if (!mSystem.IsArcade())
      AddSwitch(_("FAVORITES FIRST"), RecalboxConf::Instance().GetFavoritesFirst(), (int) Components::FavoritesFirst, this, _(MENUMESSAGE_UI_FAVORITES_FIRST_MSG));
  }
  // update game list
  if (!system.IsFavorite())
    AddSubMenu(_("UPDATE GAMES LISTS"), (int)Components::UpdateGamelist, _(MENUMESSAGE_UI_UPDATE_GAMELIST_HELP_MSG));

	if (!nomenu)
  {
    // Main menu
    AddSubMenu(_("MAIN MENU"), (int)Components::MainMenu, _(MENUMESSAGE_GAMELISTOPTION_MAIN_MENU_MSG));
    // QUIT
    AddSubMenu(_("QUIT"), (int)Components::Quit, _(MENUMESSAGE_QUIT_HELP_MSG));
  }
}

GuiMenuGamelistOptions::~GuiMenuGamelistOptions()
{
  // require list refresh
	mGamelist.onChanged(ISimpleGameListView::Change::Update);
}

void GuiMenuGamelistOptions::RefreshGameMenuContext()
{
  if (!mGame) return;

  const FileData* file = mGamelist.getCursor();
  if ((file == nullptr) || file->IsEmpty())
    mGame->setText(_("NO GAME"));
  else if (file->TopAncestor().ReadOnly())
    mGame->setText(_("NON EDITABLE GAME"));
  else if (file->IsGame())
  {
    String text = _("EDIT GAME %s").Replace("%s", file->Name().ToUpperCaseUTF8());
    mGame->setText(text);
  }
  else if (file->IsFolder())
  {
    String text = _("EDIT FOLDER %s").Replace("%s", file->Name().ToUpperCaseUTF8());
    mGame->setText(text);
  }
}

std::vector<GuiMenuBase::ListEntry<Regions::GameRegions>> GuiMenuGamelistOptions::GetRegionEntries()
{
  std::vector<GuiMenuBase::ListEntry<Regions::GameRegions>> list;

  Regions::GameRegions currentRegion = Regions::Clamp(RecalboxConf::Instance().GetSystemRegionFilter(mSystem));
  for(auto region : mGamelist.AvailableRegionsInGames())
  {
    String regionName = (region == Regions::GameRegions::Unknown) ? _("NONE") : Regions::RegionFullName(region);
    list.push_back({ regionName, region, region == currentRegion });
  }

  return list;
}

std::vector<GuiMenuBase::ListEntry<FileSorts::Sorts>> GuiMenuGamelistOptions::GetSortEntries()
{
  std::vector<GuiMenuBase::ListEntry<FileSorts::Sorts>> list;

  // Get & check sort id
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  const FileSorts::SortList availableSorts = FileSorts::AvailableSorts(set);
  FileSorts::Sorts currentSort = (FileSorts::Sorts)RecalboxConf::Instance().GetSystemSort(mSystem);
  if (!availableSorts.Contains(currentSort)) currentSort = FileSorts::Sorts::FileNameAscending;

  list.reserve(availableSorts.Count());
  FileSorts& sorts = FileSorts::Instance();
  for(FileSorts::Sorts sort : availableSorts)
    list.push_back({ sorts.Name(sort), sort, sort == currentSort });

  return list;
}

std::vector<GuiMenuBase::ListEntry<RecalboxConf::GamelistDecoration>> GuiMenuGamelistOptions::GetDecorationEntries()
{
  std::vector<GuiMenuBase::ListEntry<RecalboxConf::GamelistDecoration>> list;
  RecalboxConf::GamelistDecoration decorations = RecalboxConf::Instance().GetSystemGamelistDecoration(*ViewController::Instance().CurrentSystem());
  list.push_back({ "Region flags", RecalboxConf::GamelistDecoration::Regions, hasFlag(decorations, RecalboxConf::GamelistDecoration::Regions) });
  list.push_back({ "Players", RecalboxConf::GamelistDecoration::Players, hasFlag(decorations, RecalboxConf::GamelistDecoration::Players) });
  list.push_back({ "Genres", RecalboxConf::GamelistDecoration::Genre, hasFlag(decorations, RecalboxConf::GamelistDecoration::Genre) });
  return list;
}

std::vector<GuiMenuBase::ListEntry<unsigned int>> GuiMenuGamelistOptions::GetLetterEntries()
{
  std::vector<GuiMenuBase::ListEntry<unsigned int>> list;

  // Get available letters
  Array<String::Unicode> letters = mGamelist.GetAvailableLetters();
  if (!letters.Empty())
  {
    // Get current unicode char
    String::Unicode currentUnicode = String::UpperUnicode(mGamelist.getCursor()->Name().ReadFirstUTF8());
    // Build list
    for (String::Unicode unicode : letters)
      list.push_back({ String(unicode, 1), unicode, unicode == currentUnicode });
  }

  return list;
}

void GuiMenuGamelistOptions::Delete(FileData& game)
{
  (void)game.RomPath().Delete();
  if (game.Parent() != nullptr)
  {
    game.Parent()->RemoveChild(&game);
    game.System().RemoveArcadeReference(game);
  }

  mSystemManager.UpdateSystemsOnGameChange(&game, MetadataType::None, true);
}

void GuiMenuGamelistOptions::Modified(ISimpleGameListView* gamelistview, FileData& game)
{
  gamelistview->refreshList();
  gamelistview->setCursor(&game);
}

void GuiMenuGamelistOptions::OptionListComponentChanged(int id, int index, const unsigned int& value, bool quickChange)
{
  (void)quickChange;
  (void)index;
  if ((Components)id == Components::JumpToLetter)
  {
    FileSorts::Sorts sortId = mListSort->getSelected();

    // if sort is not alpha, need to force an alpha
    if (sortId != FileSorts::Sorts::FileNameAscending && sortId != FileSorts::Sorts::FileNameDescending)
    {
      sortId = FileSorts::Sorts::FileNameAscending;
      mListSort->select(sortId);
      // apply sort
      if (sortId != (FileSorts::Sorts)RecalboxConf::Instance().GetSystemSort(mSystem))
      {
        RecalboxConf::Instance().SetSystemSort(mSystem, sortId);
        mGamelist.onChanged(ISimpleGameListView::Change::Resort);
      }
    }

    mGamelist.JumpToLetter(value);
    RefreshGameMenuContext();
  }
}

void GuiMenuGamelistOptions::OptionListComponentChanged(int id, int index, const FileSorts::Sorts& value, bool quickChange)
{
  (void)quickChange;
  (void)index;
  if ((Components)id == Components::Sorts)
  {
    RecalboxConf::Instance().SetSystemSort(mSystem, value).Save();
    mGamelist.onChanged(ISimpleGameListView::Change::Resort);

    mGamelist.refreshList();
    RefreshGameMenuContext();
  }
}

void GuiMenuGamelistOptions::OptionListComponentChanged(int id, int index, const Regions::GameRegions& value, bool quickChange)
{
  (void)quickChange;
  (void)index;
  if ((Components)id == Components::Regions)
    RecalboxConf::Instance().SetSystemRegionFilter(mSystem, value).Save();
}

void GuiMenuGamelistOptions::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Download:
    {
      mWindow.pushGui(new GuiDownloader(mWindow, mSystem, mSystemManager));
      break;
    }
    case Components::MetaData:
    {
      FileData* file = mGamelist.getCursor();
      if ((file != nullptr) && (!file->IsEmpty()) && (!file->TopAncestor().ReadOnly()))
        mWindow.pushGui(new GuiMenuGamelistGameOptions(mWindow, mGamelist, mSystemManager, mSystem, *mGamelist.getCursor()));
      break;
    }
    case Components::MainMenu: mWindow.pushGui(new GuiMenu(mWindow, mSystemManager, mResolver)); Close(); break;
    case Components::Quit: GuiMenuQuit::PushQuitGui(mWindow); break;
    case Components::UpdateGamelist:
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("REALLY UPDATE GAMES LISTS ?"),
                      _("YES"), [] { MainRunner::RequestQuit(MainRunner::ExitState::Relaunch, true); },
                      _("NO"), nullptr ));
      break;
    }
    case Components::Delete:
    {
      FileData* game = mGamelist.getCursor();
      if (ScraperSeamless::Instance().IsScraping(game)) mWindow.pushGui(new GuiMsgBox(mWindow, _("This game is currently updating its metadata. Retry in a few seconds."), _("OK"), nullptr ));
      else mWindow.pushGui(new GuiMenuGamelistGameDeleteOptions(mWindow, mGamelist, *game));
      break;
    }
    case Components::DeleteScreeshot:
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("DELETE SCREENSHOT, CONFIRM?"), _("YES"), [this]
      {
        (void)mGamelist.getCursor()->RomPath().Delete();
        RootFolderData::DeleteChild(mGamelist.getCursor());
        mSystemManager.UpdateSystemsOnGameChange(mGamelist.getCursor(), MetadataType::None, true);
        mWindow.CloseAll();
      }, _("NO"), {}));
      break;
    }
    case Components::SaveStates:
    {
      FileData* game = mGamelist.getCursor();
      mWindow.pushGui(new GuiSaveStates(mWindow, mSystemManager, *game, nullptr, true));
      break;
    }
    case Components::Search:
    {
      mWindow.pushGui(new GuiSearch(mWindow, mSystemManager));
      break;
    }
    case Components::ArcadeOptions:
    {
      mWindow.pushGui(new GuiMenuArcade(mWindow, mSystemManager, mArcade));
      break;
    }
    case Components::JumpToLetter:
    case Components::Sorts:
    case Components::Regions:
    case Components::FavoritesOnly:
    case Components::FavoritesFirst:
    case Components::FlatFolders:
    case Components::AutorunGame:
    case Components::Decorations:
    default: break;
  }
}

void GuiMenuGamelistOptions::SwitchComponentChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::FlatFolders: RecalboxConf::Instance().SetSystemFlatFolders(mSystem, status).Save(); break;
    case Components::FavoritesOnly:
    {
      RecalboxConf::Instance().SetFavoritesOnly(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no favorite games in any system!"));
        RecalboxConf::Instance().SetFavoritesOnly(!status);
        status = false;
      }
      break;
    }
    case Components::FavoritesFirst:
    {
      RecalboxConf::Instance().SetFavoritesFirst(status).Save();
      break;
    }
    case Components::AutorunGame:
    {
      FileData* game = mGamelist.getCursor();
      if (game->IsGame())
      {
        if (status)
          RecalboxConf::Instance().SetAutorunGamePath(game->RomPath().ToString())
                                  .SetAutorunSystemUUID(game->System().Descriptor().GUID())
                                  .Save();
        else
          RecalboxConf::Instance().SetAutorunGamePath(String::Empty)
                                  .SetAutorunSystemUUID(String::Empty)
                                  .Save();
      }
    }
    case Components::Download:
    case Components::Regions:
    case Components::Sorts:
    case Components::JumpToLetter:
    case Components::Search:
    case Components::MetaData:
    case Components::MainMenu:
    case Components::UpdateGamelist:
    case Components::Delete:
    case Components::DeleteScreeshot:
    case Components::SaveStates:
    case Components::Quit:
    case Components::ArcadeOptions:
    case Components::Decorations:
    default: break;
  }

  mGamelist.refreshList();
  RefreshGameMenuContext();
}

void GuiMenuGamelistOptions::OptionListMultiComponentChanged(int id, const std::vector<RecalboxConf::GamelistDecoration>& value)
{
  switch((Components)id)
  {
    case Components::Decorations:
    {
      RecalboxConf::GamelistDecoration decorations = RecalboxConf::GamelistDecoration::None;
      for(RecalboxConf::GamelistDecoration deco : value) decorations |= deco;
      RecalboxConf::Instance().SetSystemGamelistDecoration(*ViewController::Instance().CurrentSystem(), decorations);
      break;
    }
    case Components::Download:
    case Components::JumpToLetter:
    case Components::Sorts:
    case Components::Regions:
    case Components::FlatFolders:
    case Components::FavoritesOnly:
    case Components::FavoritesFirst:
    case Components::MetaData:
    case Components::UpdateGamelist:
    case Components::Delete:
    case Components::DeleteScreeshot:
    case Components::SaveStates:
    case Components::MainMenu:
    case Components::Quit:
    case Components::Search:
    case Components::ArcadeOptions:
    case Components::AutorunGame:
    default: break;
  }
}

