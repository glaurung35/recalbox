#include "SystemData.h"
#include <systems/SystemManager.h>
#include "audio/AudioManager.h"
#include "games/GameFilesUtils.h"
#include <usernotifications/NotificationManager.h>
#include <utils/Files.h>

SystemData::SystemData(SystemManager& systemManager, const SystemDescriptor& descriptor, Properties properties)
  : mSystemManager(systemManager)
  , mDescriptor(descriptor)
  , mRootOfRoot(mRootOfRoot, RootFolderData::Ownership::None, RootFolderData::Types::None, Path(), *this)
  , mProperties(properties)
  , mFixedSort(FileSorts::Sorts::FileNameAscending)
  , mArcadeDatabases(*this)
  , mSensitivity(MetadataType::None)
  , mVirtualType(VirtualSystemType::None)
{
}

SystemData::SystemData(SystemManager& systemManager, const SystemDescriptor& descriptor, Properties properties, MetadataType sensitivity, VirtualSystemType virtualType, FileSorts::Sorts fixedSort)
  : mSystemManager(systemManager)
  , mDescriptor(descriptor)
  , mRootOfRoot(mRootOfRoot, RootFolderData::Ownership::None, RootFolderData::Types::None, Path(), *this)
  , mProperties(properties)
  , mFixedSort(fixedSort)
  , mArcadeDatabases(*this)
  , mSensitivity(sensitivity)
  , mVirtualType(virtualType)
{
}

void SystemData::populateFolder(RootFolderData& root, FileData::StringMap& doppelgangerWatcher)
{
  { LOG(LogInfo) << "[Gamelist] " << root.System().FullName() << ": Searching games/roms in " << root.RomPath().ToString() << "..."; }

  try
  {
    String ignoreList(','); ignoreList.Append(mDescriptor.IgnoredFiles()).Append(',');
    root.PopulateRecursiveFolder(root, mDescriptor.Extension().ToLowerCase(), ignoreList, doppelgangerWatcher);
  }
  catch (std::exception& ex)
  {
    { LOG(LogError) << "[Gamelist] Reading folder \"" << root.RomPath().ToString() << "\" has raised an error!"; }
    { LOG(LogError) << "[Gamelist] Exception: " << ex.what(); }
  }
}

Path SystemData::getGamelistPath(const RootFolderData& root, bool forWrite)
{
  Path filePath = root.RomPath() / "gamelist.xml";

  if (forWrite) // Write mode, ensure folder exist
  {
    if (!filePath.Directory().Exists())
      (void)filePath.Directory().CreatePath();
  }

  return filePath;
}

FileData* SystemData::LookupOrCreateGame(RootFolderData& topAncestor, const Path& rootPath, const Path& path, ItemType type, FileData::StringMap& doppelgangerWatcher) const
{
  if (!path.StartWidth(rootPath))
  {
    { LOG(LogError) << "[Gamelist] File path \"" << path.ToString() << "\" is outside system path \"" <<  rootPath.ToString() << "\""; }
    return nullptr;
  }

  bool isVirtual = IsVirtual();

  int itemStart = rootPath.ItemCount();
  int itemLast = path.ItemCount() - 1;
  FolderData* treeNode = &topAncestor;
  for (int itemIndex = itemStart; itemIndex <= itemLast; ++itemIndex)
  {
    // Get the key for duplicate detection. MUST MATCH KEYS USED IN populateRecursiveFolder.populateRecursiveFolder - Always fullpath
    String key = path.UptoItem(itemIndex);
    FileData** itemFound = doppelgangerWatcher.try_get(key);
    FileData* item = itemFound != nullptr ? *itemFound : nullptr;

    // Some ScummVM folder/games may create inconsistent folders
    if (!treeNode->IsFolder()) return nullptr;

    // Last path component?
    if (itemIndex == itemLast)
    {
      if (type == ItemType::Game) // Final file
      {
        FileData* game = item;
        if (game == nullptr && !isVirtual)
        {
          // Add final game
          game = new FileData(path, topAncestor);
          doppelgangerWatcher[key] = game;
          treeNode->AddChild(game, true);
        }
        // Virtual systems use the doppleganger map in a reverse way:
        // Game to insert are already in the map
        // and are added as parent-less game in the virtual system
        else if (game != nullptr && isVirtual)
        {
          // Add existing game & remove from doppleganger
          treeNode->AddChild(game, false);
          doppelgangerWatcher.erase(key);
        }
        return game;
      }

      // Final folder (scraped obviously)
      FolderData* folder = (FolderData*) item;
      if (folder == nullptr)
      {
        // Create missing folder in both case, virtual or not
        folder = new FolderData(Path(key), topAncestor);
        doppelgangerWatcher[key] = folder;
        treeNode->AddChild(folder, true);
      }
      return folder;
    }

    // Intermediate path
    FolderData* folder = (FolderData*) item;
    if (folder == nullptr)
    {
      // Create missing folder in both case, virtual or not
      folder = new FolderData(Path(key), topAncestor);
      doppelgangerWatcher[key] = folder;
      treeNode->AddChild(folder, true);
    }
    treeNode = folder;
  }

  return nullptr;
}

void SystemData::ParseGamelistXml(RootFolderData& root, FileData::StringMap& doppelgangerWatcher, bool forceCheckFile)
{
  try
  {
    Path xmlpath = getGamelistPath(root, false);
    if (!xmlpath.Exists()) return;

    XmlDocument gameList;
    XmlResult result = gameList.load_file(xmlpath.ToChars());
    if (!result)
    {
      { LOG(LogError) << "[Gamelist] Could not parse " << xmlpath.ToString() << " file!"; }
      return;
    }

    String ignoreList(','); ignoreList.Append(mDescriptor.IgnoredFiles()).Append(',');

    const Path relativeTo(root.RomPath());
    XmlNode games = gameList.child("gameList");
    HashSet<String> blacklist;

    if (games != nullptr)
    {
      // build game subfiles blacklist
      String extensions = mDescriptor.Extension();
      if (GameFilesUtils::ContainsMultiDiskFile(extensions))
        for (const XmlNode fileNode: games.children())
        {
          Path path = relativeTo / Xml::AsString(fileNode, "path", "");
          GameFilesUtils::ExtractUselessFiles(path, blacklist);
        }

      for (const XmlNode fileNode: games.children())
      {
        ItemType type = ItemType::Game;
        String name = fileNode.name();
        if (name == "folder") type = ItemType::Folder;
        else if (name != "game") continue; // Unknown node

        Path path = relativeTo / Xml::AsString(fileNode, "path", "");
        if (forceCheckFile)
          if (!path.Exists())
            continue;

        // Force to hide ignored files
        const String fileName = path.Filename();
        int p = ignoreList.Find(fileName);
        if (p > 0 && ignoreList[p-1] == ',')
          if (ignoreList[p + fileName.length()] == ',')
            continue;

        if (blacklist.contains(path.ToString()))
          continue;

        FileData* file = LookupOrCreateGame(root, relativeTo, path, type, doppelgangerWatcher);
        if (file == nullptr)
        {
          { LOG(LogError) << "[Gamelist] Error finding/creating FileData for \"" << path.ToString() << "\", skipping."; }
          continue;
        }

        // load the metadata
        file->Metadata().Deserialize(fileNode, relativeTo);
      }
    }
  }
  catch (std::exception& ex)
  {
    { LOG(LogError) << "[Gamelist] System \"" << Name() << "\" has raised an error while reading its gamelist.xml!"; }
    { LOG(LogError) << "[Gamelist] Exception: " << ex.what(); }
  }
}

void SystemData::UpdateGamelistXml()
{
  // We do this by reading the XML again, adding changes and then writing it back,
  // because there might be information missing in our systemdata which would then miss in the new XML.
  // We have the complete information for every game though, so we can simply remove a game
  // we already have in the system from the XML, and then add it back from its GameData information...
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    if (!root->ReadOnly() && root->IsDirty())
      try
      {
        /*
         * Get all folder & games in a flat storage
         */
        FileData::List fileList = root->GetAllItemsRecursively(true, FileData::Filter::None);
        FileData::List folderList = root->GetAllFolders();
        // Nothing to process?
        if (fileList.empty() && !root->HasDeletedChildren())
          return;

        /*
         * Create gamelist
         */
        Path xmlReadPath = getGamelistPath(*root, false);
        XmlDocument document;
        XmlNode gameList = document.append_child("gameList");

        /*
         * Serialize folder and game nodes
         */
        Path rootPath = root->RomPath();
        for (const FileData* folder : folderList)
          folder->Metadata().Serialize(gameList, folder->RomPath(), rootPath);
        for (FileData* file : fileList)
        {
          Path path(file->RomPath());
          if (root->GetDeletedChildren().contains(file))
            continue;
          file->Metadata().Serialize(gameList, path, rootPath);
          file->Metadata().UnsetDirty();
        }

        /*
         * Custom thread-safe writer
         */
        struct XmlWriter : public pugi::xml_writer
        {
          String mOutput;
          void write(const void* data, size_t size) override { mOutput.Append((const char*)data, (int)size); }
        }
        Writer;

        /*
         * Write the list.
         * At this point, we're sure at least one node has been updated (or added and updated).
         */
        Path xmlWritePath(getGamelistPath(*root, true));
        (void)xmlWritePath.Directory().CreatePath();
        mSystemManager.AddWatcherIgnoredFiles(xmlWritePath.ToString());
        document.save(Writer);

        // Save
        Path xmlTempPath = xmlWritePath.ChangeExtension(".tmp");
        if (Files::SaveFile(xmlTempPath, Writer.mOutput))
        {
          // Force sync to disk
          sync();
          // Check size
          if (xmlTempPath.Size() > 0)
          {
            // Delete old list & rename tmp => xml
            (void)xmlWritePath.Delete();
            if (Path::Rename(xmlTempPath, xmlWritePath))
            {
              // Sync again
              sync();
              { LOG(LogInfo) << "[Gamelist] Saved gamelist.xml for system " << FullName() << ". Updated items: " << fileList.size() << "/" << fileList.size(); }
            }
            else { LOG(LogInfo) << "[Gamelist] Failed to rename " << xmlTempPath << " into " << xmlWritePath; }
          }
          else { LOG(LogError) << "[Gamelist] Null file size! " << xmlTempPath.ToString(); }
        }
        else { LOG(LogError) << "[Gamelist] Failed to save " << xmlTempPath.ToString(); }
      }
      catch (std::exception& e)
      {
        { LOG(LogError) << "[Gamelist] Something went wrong while saving " << FullName() << " : " << e.what(); }
      }
}

bool SystemData::IsAutoScrapable() const
{
  return (mProperties & Properties::GameInPng) != 0;
}

bool SystemData::IsFavorite() const
{
  return (mProperties & Properties::Favorite) != 0;
}

bool SystemData::IsPorts() const
{
  return (mProperties & Properties::Ports) != 0;
}

bool SystemData::IsScreenshots() const
{
    return (mProperties & Properties::ScreenShots) != 0;
}

bool SystemData::IsLastPlayed() const
{
  return (mSensitivity & MetadataType::LastPlayed) != 0;
}

bool SystemData::IsVirtual() const
{
  return (mProperties & Properties::Virtual) != 0;
}

bool SystemData::IsSelfSorted() const
{
  return (mProperties & Properties::FixedSort) != 0;
}

bool SystemData::IsAlwaysFlat() const
{
  return (mProperties & Properties::AlwaysFlat) != 0;
}

bool SystemData::IsSearchable() const
{
  return (mProperties & Properties::Searchable) != 0;
}

void SystemData::BuildDoppelgangerMap(FileData::StringMap& doppelganger, bool includefolder) const
{
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    root->BuildDoppelgangerMap(doppelganger, includefolder);
}

void SystemData::UpdateLastPlayedGame(FileData& updated)
{
  // Update game
  updated.Metadata().SetLastPlayedNow();

  // Build the doppelganger map
  FileData::StringMap map;
  BuildDoppelgangerMap(map, false);
  // If the game is already here, exit
  Path path(updated.RomPath());
  if (map.contains(path.ToString())) return;

  // Prepare a one game map
  map.clear();
  map[path.ToString()] = &updated;
  // Add the virtual game
  RootFolderData* root = GetRootFolder(RootFolderData::Types::Virtual);
  if (root != nullptr)
    LookupOrCreateGame(*root, updated.TopAncestor().RomPath(), path, ItemType::Game, map);
}

RootFolderData* SystemData::GetRootFolder(RootFolderData::Types type)
{
  for(RootFolderData* rootFolder : mRootOfRoot.SubRoots())
    if (rootFolder->RootType() == type)
      return rootFolder;
  return nullptr;
}

RootFolderData* SystemData::GetRootFolder(const Path& root)
{
  for(RootFolderData* rootFolder : mRootOfRoot.SubRoots())
    if (rootFolder->RomPath() == root)
      return rootFolder;
  return nullptr;
}

RootFolderData& SystemData::CreateRootFolder(const Path& startpath, RootFolderData::Ownership childownership, RootFolderData::Types type)
{
  RootFolderData* newRoot = new RootFolderData(mRootOfRoot, childownership, type, startpath, *this);
  mRootOfRoot.AddSubRoot(newRoot);
  return *newRoot;
}

RootFolderData& SystemData::LookupOrCreateRootFolder(const Path& startpath, RootFolderData::Ownership childownership,
                                                    RootFolderData::Types type)
{
  RootFolderData* lookup = GetRootFolder(startpath);
  if (lookup != nullptr) return *lookup;

  return CreateRootFolder(startpath, childownership, type);
}

bool SystemData::HasGame() const
{
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    if (root->HasGame())
      return true;
  return false;
}

int SystemData::GameCount(int& favorites, int& hidden) const
{
  int result = 0;
  favorites = hidden = 0;
  FileData::Filter excludes = Excludes();
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    result += root->CountAllGamesAndFavoritesAndHidden(excludes, favorites, hidden);
  return result;
}

FileData::List SystemData::getFavorites() const
{
  FileData::Filter filter = FileData::Filter::Favorite;
  FileData::Filter excludes = Excludes();
  FileData::List result;
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    root->GetItemsRecursivelyTo(result, filter, excludes, false);
  return result;
}

FileData::List SystemData::getAllGames() const
{
  FileData::List result;
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    root->GetItemsRecursivelyTo(result, FileData::Filter::All, FileData::Filter::None, false);
  return result;
}

bool SystemData::HasVisibleGame() const
{
  FileData::TopLevelFilter filter = FileData::BuildTopLevelFilter();
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    if (root->HasVisibleGame(filter)) return true;
  return false;
}

bool SystemData::HasScrapableGame() const
{
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    if (root->HasSacrapableGame()) return true;

  return false;
}

Path::PathList SystemData::WritableGamelists()
{
  Path::PathList result;
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    if (root->Normal())
      result.push_back(getGamelistPath(*root, true));
  return result;
}

FileData::List SystemData::getTopGamesAndFolders() const
{
  FileData::List result;
  FileData::Filter excludes = Excludes();
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    root->GetItemsTo(result, FileData::Filter::All, excludes, true);
  return result;
}

bool SystemData::IncludeAdultGames() const
{
  return !RecalboxConf::Instance().GetSystemFilterAdult(*this);
}

FileData::Filter SystemData::Excludes() const
{
  RecalboxConf& conf = RecalboxConf::Instance();

  // Default excludes filter
  FileData::Filter excludesFilter = FileData::Filter::None;
  // ignore hidden?
  if (!conf.GetShowHidden())
    excludesFilter |= FileData::Filter::Hidden;
  // ignore non Latest version
  if (conf.GetShowOnlyLatestVersion())
    excludesFilter |= FileData::Filter::NotLatest;
  // ignore pre installed games ?
  if (conf.GetGlobalHidePreinstalled())
    excludesFilter |= FileData::Filter::PreInstalled;
  // ignore adult games ?
  if(!IncludeAdultGames())
    excludesFilter |= FileData::Filter::Adult;
  // ignore no games ?
  if(conf.GetHideNoGames())
    excludesFilter |= FileData::Filter::NoGame;
  // ignore board games ?
  if(conf.GetHideBoardGames())
    excludesFilter |= FileData::Filter::Board;
  // ignore yoko games ?
  if(conf.GetTateOnly())
    excludesFilter |= FileData::Filter::Yoko;
  // ignore less than 3 players games
  if(conf.GetShowOnly3PlusPlayers())
    excludesFilter |= FileData::Filter::OneAndTwoPlayers;
  return excludesFilter;
}

void SystemData::LookupGames(FolderData::FastSearchContext context, const MetadataStringHolder::IndexAndDistance& index, FileData::List& games) const
{
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    switch(context)
    {
      case FolderData::FastSearchContext::Path: root->LookupGamesFromPath(index, games); break;
      case FolderData::FastSearchContext::Name: root->LookupGamesFromName(index, games); break;
      case FolderData::FastSearchContext::Description: root->LookupGamesFromDescription(index, games); break;
      case FolderData::FastSearchContext::Developer: root->LookupGamesFromDeveloper(index, games); break;
      case FolderData::FastSearchContext::Publisher: root->LookupGamesFromPublisher(index, games); break;
      case FolderData::FastSearchContext::All: root->LookupGamesFromAll(index, games); break;
      default: break;
    }
}

void SystemData::BuildFastSearchSeries(FolderData::FastSearchItemSerie& into, FolderData::FastSearchContext context) const
{
  for(const RootFolderData* root : mRootOfRoot.SubRoots())
    switch(context)
    {
      case FolderData::FastSearchContext::Path: root->BuildFastSearchSeriesPath(into); break;
      case FolderData::FastSearchContext::Name: root->BuildFastSearchSeriesName(into); break;
      case FolderData::FastSearchContext::Description: root->BuildFastSearchSeriesDescription(into); break;
      case FolderData::FastSearchContext::Developer: root->BuildFastSearchSeriesDeveloper(into); break;
      case FolderData::FastSearchContext::Publisher: root->BuildFastSearchSeriesPublisher(into); break;
      case FolderData::FastSearchContext::All:
      default: break;
    }
}

void SystemData::RemoveArcadeReference(const FileData& game)
{
  mArcadeDatabases.RemoveGame(game);
}

bool SystemData::Rotatable() const
{
  return mDescriptor.Name() == SystemManager::sTateSystemShortName;
}
