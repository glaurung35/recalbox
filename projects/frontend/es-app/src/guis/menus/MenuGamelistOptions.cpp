#include "MenuGamelistOptions.h"
#include "guis/GuiSaveStates.h"
#include "guis/GuiDownloader.h"
#include "views/gamelist/ArcadeGameListView.h"
#include "MenuArcade.h"
#include <guis/GuiSearch.h>
#include <MainRunner.h>
#include <games/GameFilesUtils.h>
#include <guis/MenuMessages.h>
#include <guis/GuiMsgBox.h>
#include <guis/menus/MenuQuit.h>
#include <guis/menus/MenuMain.h>
#include <guis/menus/MenuGamelistGameOptions.h>
#include <guis/menus/MenuGamelistGameDeleteOptions.h>
#include <scraping/ScraperSeamless.h>
#include <guis/menus/base/ItemSubMenu.h>

MenuGamelistOptions::MenuGamelistOptions(WindowManager& window, SystemData& system, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface, const IGlobalVariableResolver& resolver)
  :	Menu(window, _("OPTIONS"))
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
    if (!editTitle.empty()) mGame = AddSubMenu(editTitle, (int) Components::MetaData, this, _(MENUMESSAGE_GAMELISTOPTION_EDIT_METADATA_MSG));

    if (isGame || isFolder)
    {
      if (!mSystem.IsVirtual() && isGame && !mGamelist.getCursor()->TopAncestor().ReadOnly() &&
          !mSystem.IsScreenshots())
      {
        String text = _("DELETE GAME %s").Replace("%s", mGamelist.getCursor()->Name().ToUpperCaseUTF8());
        AddSubMenu(text, (int) Components::Delete, this, _(MENUMESSAGE_GAMELISTOPTION_DELETE_GAME_MSG));
      }

      if (mSystem.IsScreenshots())
      {
        AddSubMenu(_("DELETE SCREENSHOT"), (int) Components::DeleteScreeshot, this, _(MENUMESSAGE_GAMELISTOPTION_DELETE_SCREENSHOT_MSG));
      }

      if (RecalboxConf::Instance().GetAutorunEnabled() && isGame)
      {
        AddSwitch(_("BOOT ON THIS GAME"), mGamelist.getCursor()->RomPath().ToString() == RecalboxConf::Instance().GetAutorunGamePath(), (int) Components::AutorunGame, this, _(MENUMESSAGE_GAMELISTOPTION_BOOT_GAME_MSG));
      }

      if (!GameFilesUtils::GetGameSaveStateFiles(*mGamelist.getCursor()).empty())
        AddSubMenu(_("SAVE STATES"), (int) Components::SaveStates, this, _(MENUMESSAGE_GAMELISTOPTION_SAVE_STATES_MSG));
    }
  }

  RefreshGameMenuContext();

  // Downloader available?
  if (DownloaderManager::HasDownloader(mSystem))
    AddSubMenu(_("DOWNLOAD GAMES"),  (int)Components::Download, this, _(MENUMESSAGE_GAMELISTOPTION_DOWNLOAD_GAMES_MSG));

  // search others version of current game
  if(!mGamelist.getCursor()->Metadata().Alias().empty())
    AddSubMenu(_("SEARCH OTHER VERSIONS"), (int) Components::SearchSiblings, this,  _("MENUMESSAGE_GAMELISTOPTION_SEARCH_SIBLINGS_MSG"));

  // Jump to letter
	AddList<unsigned int>(_("JUMP TO LETTER"), (int)Components::JumpToLetter, this, GetLetterEntries(), String::UpperUnicode(mGamelist.getCursor()->Name().ReadFirstUTF8()), 0, _(MENUMESSAGE_GAMELISTOPTION_JUMP_LETTER_MSG));

  // open search wheel for this system
  if (!system.IsFavorite())
    AddSubMenu(_("SEARCH GAMES HERE"),  (int)Components::Search, this, _(MENUMESSAGE_GAMELISTOPTION_SEARCH_GAMES_MSG));

  // Sorting
	if (!system.IsSelfSorted())
	  mListSort = AddList<FileSorts::Sorts>(_("SORT GAMES BY"), (int)Components::Sorts, this, GetSortEntries(), RecalboxConf::Instance().GetSystemSort(mSystem), FileSorts::Sorts::FileNameAscending, _(MENUMESSAGE_GAMELISTOPTION_SORT_GAMES_MSG));

  // Global arcade option available on any arcade system, true or virtual
  if (system.IsArcade())
    AddSubMenu(_("ARCADE SETTINGS"), (int)Components::ArcadeOptions, this, _(MENUMESSAGE_ARCADE_HELP_MSG));

  // Decorations
  AddMultiList(_("DECORATIONS"), (int)Components::Decorations, this, GetDecorationEntries(), _(MENUMESSAGE_GAMELISTOPTION_DECORATIONS_MSG));

  // Region filter
  AddList<Regions::GameRegions>(_("HIGHLIGHT GAMES OF REGION..."), (int)Components::Regions, this, GetRegionEntries(), Regions::Clamp(RecalboxConf::Instance().GetSystemRegionFilter(mSystem)), Regions::GameRegions::Unknown, _(MENUMESSAGE_GAMELISTOPTION_FILTER_REGION_MSG));

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
    AddSubMenu(_("UPDATE GAMES LISTS"), (int)Components::UpdateGamelist, this, _(MENUMESSAGE_UI_UPDATE_GAMELIST_HELP_MSG));

	if (!nomenu)
  {
    // Main menu
    AddSubMenu(_("MAIN MENU"), (int)Components::MainMenu, this, _(MENUMESSAGE_GAMELISTOPTION_MAIN_MENU_MSG));
    // QUIT
    AddSubMenu(_("QUIT"), (int)Components::Quit, this, _(MENUMESSAGE_QUIT_HELP_MSG));
  }
}

MenuGamelistOptions::~MenuGamelistOptions()
{
  // require list refresh
	mGamelist.onChanged(ISimpleGameListView::Change::Update);
}

void MenuGamelistOptions::RefreshGameMenuContext()
{
  if (!mGame) return;

  const FileData* file = mGamelist.getCursor();
  if ((file == nullptr) || file->IsEmpty()) mGame->ChangeLabel(_("NO GAME"));
  else if (file->TopAncestor().ReadOnly())  mGame->ChangeLabel(_("NON EDITABLE GAME"));
  else if (file->IsGame())
  {
    String text = _("EDIT GAME %s").Replace("%s", file->Name().ToUpperCaseUTF8());
    mGame->ChangeLabel(text);
  }
  else if (file->IsFolder())
  {
    String text = _("EDIT FOLDER %s").Replace("%s", file->Name().ToUpperCaseUTF8());
    mGame->ChangeLabel(text);
  }
}

SelectorEntry<Regions::GameRegions>::List MenuGamelistOptions::GetRegionEntries()
{
  SelectorEntry<Regions::GameRegions>::List list;
  for(auto region : mGamelist.AvailableRegionsInGames())
  {
    String regionName = (region == Regions::GameRegions::Unknown) ? _("NONE") : Regions::RegionFullName(region);
    list.push_back({ regionName, region });
  }
  return list;
}

SelectorEntry<FileSorts::Sorts>::List MenuGamelistOptions::GetSortEntries()
{
  SelectorEntry<FileSorts::Sorts>::List list;

  // Get & check sort id
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  const Array<FileSorts::Sorts>& availableSorts = FileSorts::AvailableSorts(set);

  list.reserve(availableSorts.Count());
  FileSorts& sorts = FileSorts::Instance();
  for(FileSorts::Sorts sort : availableSorts)
    list.push_back({ sorts.Name(sort), sort });

  return list;
}

SelectorEntry<RecalboxConf::GamelistDecoration>::List MenuGamelistOptions::GetDecorationEntries()
{
  SelectorEntry<RecalboxConf::GamelistDecoration>::List list;
  RecalboxConf::GamelistDecoration decorations = RecalboxConf::Instance().GetSystemGamelistDecoration(*ViewController::Instance().CurrentSystem());
  list.push_back({ "Region flags", RecalboxConf::GamelistDecoration::Regions, hasFlag(decorations, RecalboxConf::GamelistDecoration::Regions) });
  list.push_back({ "Players", RecalboxConf::GamelistDecoration::Players, hasFlag(decorations, RecalboxConf::GamelistDecoration::Players) });
  list.push_back({ "Genres", RecalboxConf::GamelistDecoration::Genre, hasFlag(decorations, RecalboxConf::GamelistDecoration::Genre) });
  return list;
}

SelectorEntry<String::Unicode>::List MenuGamelistOptions::GetLetterEntries()
{
  SelectorEntry<String::Unicode>::List list;

  // Get available letters
  Array<String::Unicode> letters = mGamelist.GetAvailableLetters();
  if (!letters.Empty())
  {
    // Build list
    for (String::Unicode unicode : letters)
      list.push_back({ String(unicode, 1), unicode });
  }

  return list;
}

void MenuGamelistOptions::Delete(FileData& game)
{
  (void)game.RomPath().Delete();
  if (game.Parent() != nullptr)
  {
    game.Parent()->RemoveChild(&game);
    game.System().RemoveArcadeReference(game);
  }

  mSystemManager.UpdateSystemsOnGameChange(&game, MetadataType::None, true);
}

void MenuGamelistOptions::Modified(ISimpleGameListView* gamelistview, FileData& game)
{
  gamelistview->refreshList();
  gamelistview->setCursor(&game);
}

void MenuGamelistOptions::MenuSingleChanged(int id, int index, const unsigned int& value)
{
  (void)index;
  if ((Components)id == Components::JumpToLetter)
  {
    FileSorts::Sorts sortId = mListSort->SelectedValue();

    // if sort is not alpha, need to force an alpha
    if (sortId != FileSorts::Sorts::FileNameAscending && sortId != FileSorts::Sorts::FileNameDescending)
    {
      sortId = FileSorts::Sorts::FileNameAscending;
      mListSort->SetSelectedItemValue(sortId, false);
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

void MenuGamelistOptions::MenuSingleChanged(int id, int index, const FileSorts::Sorts& value)
{
  (void)index;
  if ((Components)id == Components::Sorts)
  {
    RecalboxConf::Instance().SetSystemSort(mSystem, value).Save();
    mGamelist.onChanged(ISimpleGameListView::Change::Resort);

    mGamelist.refreshList();
    RefreshGameMenuContext();
  }
}

void MenuGamelistOptions::MenuSingleChanged(int id, int index, const Regions::GameRegions& value)
{
  (void)index;
  if ((Components)id == Components::Regions)
    RecalboxConf::Instance().SetSystemRegionFilter(mSystem, value).Save();
}

void MenuGamelistOptions::SubMenuSelected(int id)
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
        mWindow.pushGui(new MenuGamelistGameOptions(mWindow, mGamelist, mSystemManager, mSystem, *mGamelist.getCursor()));
      break;
    }
    case Components::MainMenu: mWindow.pushGui(new MenuMain(mWindow, mSystemManager, mResolver)); Close(); break;
    case Components::Quit: MenuQuit::PushQuitGui(mWindow); break;
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
      else mWindow.pushGui(new MenuGamelistGameDeleteOptions(mWindow, mGamelist, *game));
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
      mWindow.pushGui(new MenuArcade(mWindow, mSystemManager, mArcade));
      break;
    }
    case Components::SearchSiblings:
    {
      std::string alias = mGamelist.getCursor()->Metadata().Alias();
      SearchForcedOptions forcedOptions = SearchForcedOptions(alias, FolderData::FastSearchContext::Alias, true);
      mWindow.pushGui(new GuiSearch(mWindow, mSystemManager, &forcedOptions));
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

void MenuGamelistOptions::MenuSwitchChanged(int id, bool& status)
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
    case Components::SearchSiblings:
      break;
    case Components::Decorations:
    default: break;
  }

  mGamelist.refreshList();
  RefreshGameMenuContext();
}

void MenuGamelistOptions::MenuMultiChanged(int id, int index, const std::vector<RecalboxConf::GamelistDecoration>& value)
{
  (void)index;
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
    case Components::SearchSiblings:
    default: break;
  }
}

