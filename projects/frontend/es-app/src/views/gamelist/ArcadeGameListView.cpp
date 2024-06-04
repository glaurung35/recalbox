//
// Created by bkg2k on 31/05/23.
//

#include "ArcadeGameListView.h"
#include <systems/arcade/ArcadeVirtualSystems.h>
#include "utils/locale/LocaleHelper.h"

ArcadeGameListView::ArcadeGameListView(WindowManager& window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver, PictogramCaches& flagCache)
  : DetailedGameListView(window, systemManager, system, resolver, flagCache)
  , mDatabase(nullptr)
  , mIsUsingArcadeDatabaseNames(RecalboxConf::Instance().GetArcadeUseDatabaseNames())
{
}

void ArcadeGameListView::populateList(const FolderData& folder)
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
  if (items.Empty()) items.Add(&mEmptyListItem); // Insert "EMPTY SYSTEM" item

  // Sort
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  FileSorts::Sorts sort = FileSorts::Clamp(RecalboxConf::Instance().GetSystemSort(mSystem), set);
  FileSorts& sorts = FileSorts::Instance();
  BuildAndSortArcadeGames(items, sorts.ComparerArcadeFromSort(sort), sorts.IsAscending(sort));
  BuildList();
}

void ArcadeGameListView::BuildList()
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

  // Add to list
  bool filterOutBios = RecalboxConf::Instance().GetArcadeViewHideBios();
  bool filterOutUnknown = RecalboxConf::Instance().GetArcadeViewHideNonWorking();
  bool onlyTate = RecalboxConf::Instance().GetShowOnlyTateGames();
  bool onlyYoko = RecalboxConf::Instance().GetShowOnlyYokoGames();
  for (const ParentTupple& parent : mGameList)
  {
    int colorIndexOffset = BaseColor;
    if (!parent.mGame->IsFolder())
    {
      if (parent.mArcade == nullptr && filterOutUnknown) continue;
      // Tate filtering only on parent - clones have presumably the same rotation :)
      if (onlyTate && !RotationUtils::IsTate(parent.mGame->Metadata().Rotation())) continue;
      if (onlyYoko && RotationUtils::IsTate(parent.mGame->Metadata().Rotation())) continue;
      // Non games
      if (parent.mArcade != nullptr)
      {
        if (filterOutBios)
          if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Bios) continue;
        if (mustHideManufacturers)
          if (HasMatchingManufacturer(hiddenManufacturers, parent.mArcade->LimitedManufacturer())) continue;
      }
      // Region highlighting?
      if (activeRegionFiltering)
        if (!Regions::IsIn4Regions(parent.mGame->Metadata().Region().Pack, currentRegion))
          colorIndexOffset = HighlightColor;
    }
    // Store
    mList.add(GetDisplayName(parent), (FileData*)parent.mGame, colorIndexOffset + (parent.mGame->IsFolder() ? FolderColor : GameColor), false);

    // Children?
    if (/*parent.mArcade != nullptr && */parent.mCloneList != nullptr)
      /*if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Parent)*/
        if (!parent.mFolded)
        {
          for (const ArcadeTupple& clone : *parent.mCloneList)
          {
            if (mustHideManufacturers)
              if (HasMatchingManufacturer(hiddenManufacturers, clone.mArcade->LimitedManufacturer())) continue;
            // Region filtering?
            colorIndexOffset = BaseColor;
            if (activeRegionFiltering)
              if (!Regions::IsIn4Regions(clone.mGame->Metadata().Region().Pack, currentRegion))
                colorIndexOffset = HighlightColor;
            // Store
            mList.add(GetDisplayName(clone), (FileData*)clone.mGame, colorIndexOffset, false);
          }
        }
  }

  // Check emptyness
  if (mList.IsEmpty())
    mList.add(_S(mEmptyListItem.Name()), &mEmptyListItem, 0, true);
}


bool ArcadeGameListView::HasMatchingManufacturer(const HashSet<int>& manufacturerSet, const ArcadeGame::LimitedManufacturerHolder& manufacturers)
{
  for(int i = manufacturers.Count(); --i >= 0; )
    if (manufacturerSet.contains(manufacturers.Manufacturer(i)))
      return true;
  return false;
}

void ArcadeGameListView::ArcadeUseDatabaseNamesConfigurationChanged(const bool& value)
{
  if (value != mIsUsingArcadeDatabaseNames)
  {
    mIsUsingArcadeDatabaseNames = value;
  }
}

String ArcadeGameListView::getArcadeItemIcon(const ArcadeTupple& game)
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

const ArcadeTupple* ArcadeGameListView::Lookup(const FileData& item)
{
  ArcadeTupple** tupple = mGameListLookup.try_get(&item);
  if (tupple != nullptr) return *tupple;
  { LOG(LogError) << "[ArcadeGameListView] Lookup FileData failed for game " << item.Name(); }
  return nullptr;
}

String ArcadeGameListView::GetUndecoratedDisplayName(const FileData& game)
{
  const ArcadeTupple* tupple = Lookup(game);
  if (tupple != nullptr) return GetUndecoratedDisplayName(*tupple);
  ArcadeTupple noParent(nullptr, &game);
  return GetUndecoratedDisplayName(noParent);
}

String ArcadeGameListView::GetDisplayName(const FileData& game)
{
  const ArcadeTupple* tupple = Lookup(game);
  if (tupple != nullptr) return GetDisplayName(*tupple);
  ArcadeTupple noParent(nullptr, &game);
  return GetDisplayName(noParent);
}

String ArcadeGameListView::GetUndecoratedDisplayName(const ArcadeTupple& game)
{
  if (mIsUsingArcadeDatabaseNames && game.mArcade != nullptr) return game.mArcade->ArcadeName();
  return DetailedGameListView::GetUndecoratedDisplayName(*game.mGame);
}

String ArcadeGameListView::GetDisplayName(const ArcadeTupple& game)
{
  return getArcadeItemIcon(game).Append(GetUndecoratedDisplayName(game));
}

void ArcadeGameListView::BuildAndSortArcadeGames(FileData::List& items, FileSorts::ComparerArcade comparer, bool ascending)
{
  // Split lists
  ParentTuppleList parents;
  HashMap<const FileData*, ArcadeTuppleList*> clones;
  ParentTuppleList orphaned;
  ParentTuppleList bios;
  ParentTuppleList notWorking;
  ParentTuppleList folders;
  // Virtual arcade only - Avoid too much lookups by keeping already found database localy
  // and by comparing parent pointers so that database changes only when parent changes
  HashMap<const FolderData*, const ArcadeDatabase*> mDatabaseLookup;
  FolderData* previousParent = nullptr;

  // Initialize database early for True arcade systems
  if (mSystem.IsTrueArcade())
  {
    mDatabase = mSystem.ArcadeDatabases().LookupDatabase(*mPopulatedFolder, mDefaultEmulator, mDefaultCore);
    if (mDatabase == nullptr || !mDatabase->IsValid())
      return DetailedGameListView::populateList(*mPopulatedFolder);
  }

  // Get initial fold status
  bool folded = RecalboxConf::Instance().GetArcadeViewFoldClones();

  mGameList.clear();
  for(FileData* item : items)
  {
    if (item->Parent() !=  previousParent)
    {
      previousParent = item->Parent();
      const ArcadeDatabase** db = mDatabaseLookup.try_get(previousParent);
      if (db != nullptr) mDatabase = *db;
      else
      {
        mDatabase = previousParent->System().ArcadeDatabases().LookupDatabase(*previousParent);
        mDatabaseLookup[previousParent] = mDatabase;
      }
    }
    // Safety check
    if (mDatabase == nullptr)
    {
      LOG(LogError) << "[ArcadeGameListView] No database for game " << item->RomPath().ToString() << " in system " << mSystem.FullName();
      continue;
    }
    // Lookup game from the current database
    const ArcadeGame* arcade = mDatabase->LookupGame(*item);
    // Distribute tuples in lists according to their type
    // Clones are stored in parent's lists
    if (arcade == nullptr)
    {
      if (item->IsFolder()) folders.push_back(ParentTupple(nullptr, item, false));
      else notWorking.push_back(ParentTupple(nullptr, item, false));
    }
    else
      switch(arcade->Hierarchy())
      {
        case ArcadeGame::Type::Parent: parents.push_back(ParentTupple(arcade, item, folded)); break;
        case ArcadeGame::Type::Clone:
        {
          if (clones[arcade->Parent()] == nullptr) clones[arcade->Parent()] = new ArcadeTuppleList();
          clones[arcade->Parent()]->push_back(ArcadeTupple(arcade, item)); break;
        }
        case ArcadeGame::Type::Orphaned: orphaned.push_back(ParentTupple(arcade, item, false)); break;
        case ArcadeGame::Type::Bios: bios.push_back(ParentTupple(arcade, item, false)); break;
      }
  }

  // Folders first
  AddSortedCategories({ &folders }, comparer, ascending);

  // Sorts parents / orphaned
  AddSortedCategories({ &parents, &orphaned }, comparer, ascending);

  // Insert clones as children
  for(ParentTupple& parent : mGameList)
  {
    if (parent.mGame->IsFolder()) continue; // Ignore folders
    if (parent.mArcade->Hierarchy() != ArcadeGame::Type::Parent) continue;
    ArcadeTuppleList** children = clones.try_get(parent.mGame);
    if (children != nullptr)
    {
      ArcadeTupplePointerList sortedList;
      for(ArcadeTupple& clone : **children) sortedList.push_back(&clone);
      FileSorts::SortArcade(sortedList, comparer, ascending); // Sort clones for a single parent
      ArcadeTuppleList* dynamicCloneList = new ArcadeTuppleList();
      for(ArcadeTupple* item : sortedList) dynamicCloneList->push_back(*item);
      parent.AddClones(dynamicCloneList);
      delete *children;
      clones.erase(parent.mGame);
    }
  }

  // Sort & add bios & unknowns
  AddSortedCategories({ &bios }, comparer, ascending);
  AddSortedCategories({ &notWorking }, comparer, ascending);

  // Build fast lookup
  for(ParentTupple& parent : mGameList)
  {
    mGameListLookup[parent.mGame] = &parent;
    if (parent.mCloneList != nullptr)
      for(ArcadeTupple& clone : *parent.mCloneList)
        mGameListLookup[clone.mGame] = &clone;
  }

  // For virtual arcade systems, cleanup database
  if (mSystem.IsVirtualArcade()) mDatabase = nullptr;
}

void ArcadeGameListView::AddSortedCategories(const std::vector<ParentTuppleList*>& categoryLists, FileSorts::ComparerArcade comparer, bool ascending)
{
  ArcadeTupplePointerList sortedList;
  for(ParentTuppleList* categoryList : categoryLists)
    for(ParentTupple& item : *categoryList) sortedList.push_back(&item);
  FileSorts::SortArcade(sortedList, comparer, ascending); // Sort bios
  for(ArcadeTupple* item : sortedList)
    mGameList.push_back(*((ParentTupple*)item));
}

Regions::List ArcadeGameListView::AvailableRegionsInGames(ArcadeGameListView::ParentTuppleList& games)
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

bool ArcadeGameListView::ProcessInput(const InputCompactEvent& event)
{
  if (event.AnyHotkeyCombination())
  {
    bool vertical = event.HotkeyDownReleased() || event.HotkeyUpReleased();
    if (event.HotkeyLeftReleased()  && !vertical) { Fold(); return true; }
    if (event.HotkeyRightReleased() && !vertical) { Unfold(); return true; }
    if (event.HotkeyUpReleased())                 { FoldAll(); return true; }
    if (event.HotkeyDownReleased())               { UnfoldAll(); return true; }
  }

  return DetailedGameListView::ProcessInput(event);
}

void ArcadeGameListView::FoldAll()
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

void ArcadeGameListView::UnfoldAll()
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

void ArcadeGameListView::Fold()
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
  bool rebuild =false;
  for(ParentTupple& parent : mGameList)
    if (parent.mCloneList != nullptr)
      if (parent.mArcade->Hierarchy() == ArcadeGame::Type::Parent)
        if (parent.mGame == item)
        {
          parent.mFolded = true;
          rebuild = true;
        }

  // Rebuild the UI list
  if (rebuild) BuildList();

  // Set cursor
  setCursor(item);
}

void ArcadeGameListView::Unfold()
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

Array<String::Unicode> ArcadeGameListView::GetAvailableLetters()
{
  constexpr int UnicodeSize = 0x10000;
  FileData::List files = getFileDataList(); // All file
  Array<unsigned int> unicode;        // 1 bit per unicode char used
  unicode.ExpandTo(UnicodeSize / (sizeof(unsigned int) * 8));

  for(const FileData* file : files)
    if (file->IsGame())
    {
      const ArcadeTupple* currentArcade = Lookup(*file);
      if (IsClone(currentArcade)) continue; // Skip clones

      // Tag every first characters from every game name
      String::Unicode wc = String::UpperUnicode((currentArcade != nullptr ? GetUndecoratedDisplayName(*currentArcade) : GetUndecoratedDisplayName(*file)).ReadFirstUTF8());
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

void ArcadeGameListView::JumpToLetter(unsigned int unicode)
{
  for(int c = 0; c < mList.Count(); ++c)
    if (FileData& file = *getDataAt(c); file.IsGame())
    {
      const ArcadeTupple* currentArcade = Lookup(file);
      if (IsClone(currentArcade)) continue; // Skip clones

      String::Unicode wc = String::UpperUnicode((currentArcade != nullptr ? GetUndecoratedDisplayName(*currentArcade) : GetUndecoratedDisplayName(file)).ReadFirstUTF8());
      if (wc == unicode)
      {
        setCursor(&file);
        break;
      }
    }
}

void ArcadeGameListView::JumpToNextLetter(bool forward)
{
  const ArcadeTupple* baseArcade = Lookup(*getCursor());
  // Clone? get parent
  if (IsClone(baseArcade)) baseArcade = Lookup(*baseArcade->mArcade->Parent());
  UnicodeChar baseChar = String::UpperUnicode((baseArcade != nullptr ? GetUndecoratedDisplayName(*baseArcade) : GetUndecoratedDisplayName(*getCursor())).ReadFirstUTF8());
  int max = mList.Count();
  int step = max + (forward ? 1 : -1);

  int cursorIndex = getCursorIndex();
  for(int i = cursorIndex; (i = (i + step) % max) != cursorIndex; )
  {
    const ArcadeTupple* currentArcade = Lookup(*getDataAt(i));
    if (IsClone(currentArcade)) continue; // Skip clones

    String::Unicode wc = String::UpperUnicode((currentArcade != nullptr ? GetUndecoratedDisplayName(*currentArcade) : GetUndecoratedDisplayName(*getDataAt(i))).ReadFirstUTF8());
    if (wc != baseChar)
    {
      setCursorIndex(i);
      break;
    }
  }
}

std::vector<ArcadeDatabase::Manufacturer> ArcadeGameListView::GetManufacturerList() const
{
  if (mDatabase == nullptr) return std::vector<ArcadeDatabase::Manufacturer>();
  std::vector<ArcadeDatabase::Manufacturer> result = mDatabase->GetLimitedManufacturerList();
  return result;
}

int ArcadeGameListView::GetGameCountForManufacturer(int driverIndex) const
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

String ArcadeGameListView::GetDescription(FileData& game)
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
