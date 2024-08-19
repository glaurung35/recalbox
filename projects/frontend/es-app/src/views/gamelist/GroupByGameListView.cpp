//
// Created by bkg2k on 31/05/23.
//

#include "GroupByGameListView.h"
#include <systems/arcade/ArcadeVirtualSystems.h>
#include "utils/locale/LocaleHelper.h"

GroupByGameListView::GroupByGameListView(WindowManager& window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver, FlagCaches& flagCache)
  : DetailedGameListView(window, systemManager, system, resolver, flagCache)
  , mDatabase(nullptr)
{
}

void GroupByGameListView::populateList(const FolderData& folder)
{
  mPopulatedFolder = &folder;

  // Default filter
  FileData::Filter includesFilter = FileData::Filter::Normal | FileData::Filter::Favorite;
  // Favorites only?
  if (RecalboxConf::Instance().GetFavoritesOnly()) includesFilter = FileData::Filter::Favorite;

  // Get items
  bool flatfolders = mSystem.IsAlwaysFlat() || (RecalboxConf::Instance().GetSystemFlatFolders(mSystem));
  FileData::List items;
  if (flatfolders) folder.GetItemsRecursivelyTo(items, includesFilter, mSystem.Excludes(), false);
  else folder.GetItemsTo(items, includesFilter, mSystem.Excludes(), true);

  // Check emptyness
  if (items.empty()) items.push_back(&mEmptyListItem); // Insert "EMPTY SYSTEM" item

  // Sort
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  FileSorts::Sorts sort = FileSorts::Clamp(RecalboxConf::Instance().GetSystemSort(mSystem), set);
  BuildAndSortGames(items, FileSorts::ComparerArcadeFromSort(sort), FileSorts::IsAscending(sort));
  BuildList();
}

void GroupByGameListView::BuildList()
{
  mList.clear();
  mHeaderText.setText(mSystem.FullName());

  // Region filtering?
  Regions::GameRegions currentRegion = Regions::Clamp((Regions::GameRegions)RecalboxConf::Instance().GetSystemRegionFilter(mSystem));
  bool activeRegionFiltering = false;
  if (currentRegion != Regions::GameRegions::Unknown)
  {
    Regions::List availableRegion = AvailableRegionsInGames(mGameList);
    // Check if our region is in the available ones
    for(Regions::GameRegions region : availableRegion)
    {
      activeRegionFiltering = (region == currentRegion);
      if (activeRegionFiltering) break;
    }
  }

  // Prepare manufacturer filtering
  std::vector<ArcadeDatabase::Manufacturer> manufacturerList = GetManufacturerList();
  HashSet<int> hiddenManufacturers;
  for(const ArcadeDatabase::Manufacturer& manufacturer : manufacturerList)
    if (RecalboxConf::Instance().IsInArcadeSystemHiddenManufacturers(mSystem, manufacturer.Name.empty() ? ArcadeVirtualSystems::sAllOtherManufacturers : manufacturer.Name))
      hiddenManufacturers.insert(manufacturer.Index);
  bool mustHideManufacturers = !hiddenManufacturers.empty();

  for (const String& alias : mAliases)
  {
    GameTuple gameTuple = mGamesMap[alias];
   // bool isFolded = gameTuple.mFolded;
    bool isFolded = !mAliasesIsFolded[alias];
    int colorIndexOffset = 0;
    if (gameTuple.mGamesList.empty())
    {
      continue;
    }

    // only one game with this alias, add regular game list item
    if (gameTuple.mGamesList.size() == 1)
    {
      FileData* game = gameTuple.mMainGame;
      mList.add(RecalboxConf::Instance().GetDisplayByFileName() ? game->Metadata().RomFileOnly().ToString() : game->Name(), game, colorIndexOffset + (game->IsFolder() ? 1 : 0), false);
      continue;
    }

    // multi games with same alias, add alias list item
    AliasData* aliasData = GetOrCreateAliasData(gameTuple.mMainGame->Parent(), alias);
    String icon = isFolded ? "+ " : "- ";
    mList.add(icon + alias, aliasData, colorIndexOffset, false);

    // Children?
    if (isFolded) continue;

    // add games group by alias
    for (FileData* game: gameTuple.mGamesList)
    {
      if (game == nullptr)
      {
        continue;
      }
      // Region filtering?
      colorIndexOffset = 0;
      if (activeRegionFiltering)
        if (!Regions::IsIn4Regions(game->Metadata().Region().Pack, currentRegion))
          colorIndexOffset = 2;
      // Store

      mList.add("    \u2022 " + DetailedGameListView::GetDisplayName(*game), game, colorIndexOffset, false);
    }

    if (mList.IsEmpty())
      mList.add(_S(mEmptyListItem.Name()), &mEmptyListItem, 0, true);
  }
}

// usefull for get always same object even after build list again.. need with setCursor() equals
AliasData* GroupByGameListView::GetOrCreateAliasData(FolderData* folder, const String& aliasName)
{
  AliasData* aliasData = mAliasDataCache[folder].try_get(aliasName);
  if (aliasData == nullptr)
  {
    mAliasDataCache[folder].insert(aliasName, *new AliasData(&mSystem, aliasName));
  }

  AliasData* cachedAliasData = mAliasDataCache[folder].try_get(aliasName);
  return cachedAliasData;
}

bool GroupByGameListView::HasMatchingManufacturer(const HashSet<int>& manufacturerSet, const ArcadeGame::LimitedManufacturerHolder& manufacturers)
{
  for(int i = manufacturers.Count(); --i >= 0; )
    if (manufacturerSet.contains(manufacturers.Manufacturer(i)))
      return true;
  return false;
}

String GroupByGameListView::getArcadeItemIcon(const ArcadeTupple& game)
{
  String result;

  // Open folder for folders
  if (game.mGame->IsFolder())
  {
    result.Append("\uF07C");

    // Crossed out eye for hidden things
    if (game.mGame->Metadata().Hidden()) result.Append("\uF070");
  }
  if (game.mGame->IsGame())
  {
    // Hierarchy
    if (game.mArcade == nullptr) result.Append("\uF1C2");
    else
      switch(game.mArcade->Hierarchy()) // ◀ ▶ ▲ ▼
      {
        case ArcadeGame::Type::Parent:
        {
          const ParentTupple& parent = *((const ParentTupple*)&game);
          result.Append("\uF1F0").Append(parent.mCloneList == nullptr ? "•" : (parent.mFolded ? "▶" : "▼")); break;
        }
        case ArcadeGame::Type::Clone: result.Append("    \uF1F1•"); break;
        case ArcadeGame::Type::Orphaned: result.Append("\uF1F2•"); break;
        case ArcadeGame::Type::Bios: result.Append("\uF1F3"); break;
      }
    // Crossed out eye for hidden things
    if (game.mGame->Metadata().Hidden()) result.Append("\uF070");
    // System icon, for Favorite games
    if (mSystem.IsVirtual() || game.mGame->Metadata().Favorite()) result.Append(game.mGame->System().Descriptor().IconPrefix());
  }

  return result.Append(' ');
}

String GroupByGameListView::GetDisplayName(const ArcadeTupple& game)
{
  if (RecalboxConf::Instance().GetArcadeUseDatabaseNames() && game.mArcade != nullptr)
    return game.mArcade->ArcadeName();
  return RecalboxConf::Instance().GetDisplayByFileName() ? game.mGame->Metadata().RomFileOnly().ToString() : game.mGame->Name(); // TODO: Use gugue new displayable name ASAP
}

String GroupByGameListView::GetIconifiedDisplayName(const ArcadeTupple& game)
{
  return getArcadeItemIcon(game).Append(GetDisplayName(game));
}

void GroupByGameListView::BuildAndSortGames(FileData::List& items, FileSorts::ComparerArcade comparer, bool ascending)
{
  HashMap<String, FileData::List> map;
  HashSet<String> aliases;
  mAliases.clear();
  mGamesMap.clear();

  // create alias list
  for(FileData* fileData : items)
  {
    mAliases.insert(GetAliasString(fileData));
  }

  // create games by alias map
  for(FileData* fileData : items)
  {
    const String alias = GetAliasString(fileData);
    mGamesMap[alias].mGamesList.push_back(fileData);
    mGamesMap[alias].mMainGame = fileData;
    mGamesMap[alias].mAlias = alias;
  }
}

void GroupByGameListView::AddSortedCategories(const std::vector<ParentTuppleList*>& categoryLists, FileSorts::ComparerArcade comparer, bool ascending)
{
  ArcadeTupplePointerList sortedList;
  for(ParentTuppleList* categoryList : categoryLists)
    for(ParentTupple& item : *categoryList) sortedList.push_back(&item);
  FileSorts::SortArcade(sortedList, comparer, ascending); // Sort bios
  for(ArcadeTupple* item : sortedList) mGameList.push_back(*((ParentTupple*)item));
}

void GroupByGameListView::AddSortedGames(HashSet<String> aliases, HashMap<String, FileData::List> map, FileSorts::ComparerArcade comparer, bool ascending)
{
//
//  std::vector<GamesTupples*> sortedList;
//  for (String alias: aliases)
//  {
//    mGamesMap[alias] = map.try_get(alias);
//  }

}

Regions::List GroupByGameListView::AvailableRegionsInGames(GroupByGameListView::ParentTuppleList& games)
{
  bool regionIndexes[256];
  memset(regionIndexes, 0, sizeof(regionIndexes));
  // Run through all games
  for(const ParentTupple& tupple : games)
  {
    unsigned int fourRegions = tupple.mGame->Metadata().Region().Pack;
    // Set the 4 indexes corresponding to all 4 regions (Unknown regions will all point to index 0)
    regionIndexes[(fourRegions >>  0) & 0xFF] = true;
    regionIndexes[(fourRegions >>  8) & 0xFF] = true;
    regionIndexes[(fourRegions >> 16) & 0xFF] = true;
    regionIndexes[(fourRegions >> 24) & 0xFF] = true;
  }
  // Rebuild final list
  Regions::List list;
  for(int i = 0; i < (int)sizeof(regionIndexes); ++i )
    if (regionIndexes[i])
      list.push_back((Regions::GameRegions)i);
  // Only unknown region?
  if (list.size() == 1 && regionIndexes[0])
    list.clear();
  return list;
}

bool GroupByGameListView::ProcessInput(const InputCompactEvent& event)
{
  FileData* fileData = DetailedGameListView::getCursor();
  if (event.ValidReleased() && getCursor()->IsAlias()) {
    auto* alias = (AliasData*) fileData;
    HandleFold(alias->AliasName());
    return false;
  }

  return DetailedGameListView::ProcessInput(event);
}

void GroupByGameListView::FoldAll()
{
  // Get cursor position - Get ancestor if its a clone
  FileData* item = getCursor();
  const ArcadeDatabase* database = item->System().ArcadeDatabases().LookupDatabase(*item->Parent());
  if (database == nullptr) return;

  const ArcadeGame* arcade = database->LookupGame(*item);
  if (arcade != nullptr)
    if (arcade->Hierarchy() == ArcadeGame::Type::Clone)
      item = (FileData*)arcade->Parent();

  // Fold all
  for(ParentTupple& parent : mGameList)
    if (parent.mCloneList != nullptr)
      if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Parent)
        parent.mFolded = true;

  // Rebuild the UI list
  BuildList();

  // Set cursor
  setCursor(item);
}

void GroupByGameListView::UnfoldAll()
{
  // Get cursor position
  FileData* item = getCursor();

  // Fold all
  for(ParentTupple& parent : mGameList)
    if (parent.mCloneList != nullptr)
      if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Parent)
        parent.mFolded = false;

  // Rebuild the UI list
  BuildList();

  // Set cursor
  setCursor(item);
}

void GroupByGameListView::HandleFold(const String& aliasKey)
{
  // Get cursor position - Get ancestor if its a clone
  mAliasesIsFolded[aliasKey] = !mAliasesIsFolded[aliasKey];
  FileData* currentCursor = DetailedGameListView::getCursor();
  BuildList();

  // Set cursor
  setCursor(currentCursor);
}

void GroupByGameListView::UnFold(const String& aliasKey)  {
  // Get cursor position - Get ancestor if its a clone
  mAliasesIsFolded[aliasKey] = true;

  BuildList();
}


void GroupByGameListView::Unfold()
{
  // Get cursor position
  FileData* item = getCursor();

  // Fold all
  bool rebuild =false;
  for(ParentTupple& parent : mGameList)
    if (parent.mGame == item)
      if (parent.mCloneList != nullptr)
        if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Parent)
        {
          parent.mFolded = false;
          rebuild = true;
        }

  // Rebuild the UI list
  if (rebuild) BuildList();

  // Set cursor
  setCursor(item);
}

Array<String::Unicode> GroupByGameListView::GetAvailableLetters()
{
  constexpr int UnicodeSize = 0x10000;
  FileData::List files = getFileDataList(); // All file
  Array<unsigned int> unicode;        // 1 bit per unicode char used
  unicode.ExpandTo(UnicodeSize / (sizeof(unsigned int) * 8));

  for (auto* file : files)
    if (file->IsGame())
    {
      const ArcadeTupple& currentArcade = Lookup(*file);
      if (currentArcade.mArcade != nullptr && currentArcade.mArcade->Hierarchy() == ArcadeGame::Type::Clone) continue; // Skip clones

      // Tag every first characters from every game name
      String::Unicode wc = String::UpperUnicode(file->Name().ReadFirstUTF8());
      if (wc < UnicodeSize) // Ignore extended unicodes
        unicode((int)(wc >> 5)) |= 1 << (wc & 0x1F);
    }

  // Rebuild a self-sorted unicode list with all tagged characters
  int unicodeOffset = 0;
  Array<String::Unicode> result;
  for(unsigned int i : unicode)
  {
    if (i != 0)
      for (int bit = 0; bit < 32; ++bit)
        if (((i >> bit) & 1) != 0)
          result.Add(unicodeOffset + bit);
    unicodeOffset += 32;
  }

  return result;
}

void GroupByGameListView::JumpToLetter(unsigned int unicode)
{
  for(int c = 0; c < (int)getCursorIndexMax(); ++c)
    if (getDataAt(c)->IsGame())
    {
      if (!getDataAt(c)->IsAlias())
      {
        continue;
      }
      if (String::UpperUnicode(getDataAt(c)->Name().ReadFirstUTF8()) == unicode)
      {
        setCursor(getDataAt(c));
        break;
      }
    }
}

void GroupByGameListView::JumpToNextLetter(bool forward)
{
  const ArcadeTupple& baseArcade = Lookup(*getCursor());
  const FileData* baseGame = baseArcade.mArcade != nullptr && baseArcade.mArcade->Hierarchy() == ArcadeGame::Type::Clone ? baseArcade.mArcade->Parent() : getCursor();
  UnicodeChar baseChar = String::UpperUnicode(LookupDisplayName(*baseGame).ReadFirstUTF8());
  int max = getCursorIndexMax() + 1;
  int step = max + (forward ? 1 : -1);

  int cursorIndex = getCursorIndex();
  for(int i = cursorIndex; (i = (i + step) % max) != cursorIndex; )
  {
    const ArcadeTupple& currentArcade = Lookup(*getDataAt(i));
    if (currentArcade.mArcade != nullptr && currentArcade.mArcade->Hierarchy() == ArcadeGame::Type::Clone) continue; // Skip clones
    if (String::UpperUnicode(LookupDisplayName(*getDataAt(i)).ReadFirstUTF8()) != baseChar)
    {
      setCursorIndex(i);
      break;
    }
  }
}

const ArcadeTupple& GroupByGameListView::Lookup(const FileData& item)
{
  for(const ParentTupple& parent : mGameList)
    if (parent.mGame == &item) return parent;
    else if (parent.mCloneList != nullptr)
      for(const ArcadeTupple& clone : *parent.mCloneList)
        if (clone.mGame == &item)
          return clone;

  { LOG(LogError) << "[ArcadeGameListView] Lookup FileData failed for game " << item.Name(); }
  static ArcadeTupple nullTupple(nullptr, nullptr);
  return nullTupple;
}

String GroupByGameListView::LookupDisplayName(const FileData& item)
{
  return GetDisplayName(Lookup(item));
}

std::vector<ArcadeDatabase::Manufacturer> GroupByGameListView::GetManufacturerList() const
{
  if (mDatabase == nullptr) return std::vector<ArcadeDatabase::Manufacturer>();
  std::vector<ArcadeDatabase::Manufacturer> result = mDatabase->GetLimitedManufacturerList();
  return result;
}

int GroupByGameListView::GetGameCountForManufacturer(int driverIndex) const
{
  int count = 0;
  for(const ParentTupple& parent : mGameList)
  {
    if (parent.mArcade == nullptr) continue;
    if (parent.mArcade->LimitedManufacturer().Contains(driverIndex)) count++;
    if (parent.mCloneList != nullptr)
      for (const ArcadeTupple& clone: *parent.mCloneList)
        if (clone.mArcade->LimitedManufacturer().Contains(driverIndex)) count++;
  }
  return count;
}

String GroupByGameListView::GetDisplayName(FileData& game)
{
  for(const ParentTupple& parent : mGameList)
    if (parent.mGame == &game) return GetIconifiedDisplayName(parent);
    else if (parent.mCloneList != nullptr)
      for(const ArcadeTupple& clone : *parent.mCloneList)
        if (clone.mGame == &game) return GetIconifiedDisplayName(clone);

  // Fallback
  return GetIconifiedDisplayName(ArcadeTupple(nullptr, &game));
}

String GroupByGameListView::GetDescription(FileData& game)
{
  String emulator;
  String core;
  if (const ArcadeDatabase* database = game.System().ArcadeDatabases().LookupDatabase(game, emulator, core); database != nullptr)
    if (const ArcadeGame* arcade = database->LookupGame(game); arcade != nullptr)
    {
      if (arcade->EmulationStatus() == ArcadeGame::Status::Imperfect)
        return (_F(_("{0} reports the emulation status of this game is 'imperfect'.")) / core).ToString().Append(String::LF, 2).Append(game.Metadata().Description());
      if (arcade->EmulationStatus() == ArcadeGame::Status::Preliminary)
        return (_F(_("{0} reports the emulation status of this game is 'preliminary'. You should expect issues such as bugs or even crashes!")) / core).ToString().Append(String::LF, 2).Append(game.Metadata().Description());
    }
  return game.Metadata().Description();
}

void GroupByGameListView::RefreshItem(FileData* game)
{
  if (game == nullptr || !game->IsGame()) { LOG(LogError) << "[DetailedGameListView] Trying to refresh null or empty item"; return; }

  int index = mList.Lookup(game);
  if (index < 0) { LOG(LogError) << "[DetailedGameListView] Trying to refresh a not found item"; return; }
  mList.changeTextAt(index, GetDisplayName(*game));
}

void GroupByGameListView::setCursor(FileData* cursor)
{

  UnFold(cursor->Metadata().Alias());

  if(!mList.setCursor(cursor, 0))
  {
    populateList(mSystem.MasterRoot());
    mList.setCursor(cursor);

    // update our cursor stack in case our cursor just got set to some folder we weren't in before
    if(mCursorStack.empty() || mCursorStack.top() != cursor->Parent())
    {
      std::stack<FolderData*> tmp;
      FolderData* ptr = cursor->Parent();
      while((ptr != nullptr) && !ptr->IsRoot())
      {
        tmp.push(ptr);
        ptr = ptr->Parent();
      }

      // flip the stack and put it in mCursorStack
      mCursorStack = std::stack<FolderData*>();
      while(!tmp.empty())
      {
        mCursorStack.push(tmp.top());
        tmp.pop();
      }
    }
  }
}

String GroupByGameListView::GetAliasString(FileData* fileData) {
  String alias = fileData->Metadata().Alias();

  if (alias.empty())
  {
    alias = RecalboxConf::Instance().GetDisplayByFileName() ? fileData->Metadata().RomFileOnly().ToString() : fileData->Name();
  }

  return alias;
}