//
// Created by bkg2k on 12/03/23.
//

#include "ArcadeDatabaseManager.h"
#include "emulators/EmulatorManager.h"
#include <utils/Files.h>
#include <systems/SystemManager.h>

ArcadeDatabaseManager::ArcadeDatabaseManager(SystemData& parentSystem)
  : mSystem(parentSystem)
  , mReady(false)
{
}

ArcadeDatabaseManager::~ArcadeDatabaseManager()
{
  for(const auto& kv : mDatabases)
    delete kv.second;
}

void ArcadeDatabaseManager::LoadDatabases()
{
  if (mReady) return;

  // Valid arcade system?
  if (mSystem.Descriptor().Type() != SystemDescriptor::SystemType::Arcade) return;
  if (!mSystem.HasGame()) return;

  // Load all possible database
  for(int e = mSystem.Descriptor().EmulatorTree().Count(); --e >= 0;)
  {
    const EmulatorDescriptor& emulator = mSystem.Descriptor().EmulatorTree().EmulatorAt(e);
    for (int c = emulator.CoreCount(); --c >= 0;)
    {
      mDatabases[String(emulator.Name()).Append('|').Append(emulator.CoreNameAt(c))] =
        LoadFlatDatabase(emulator.Name(), emulator.CoreNameAt(c), emulator.CoreFlatDatabase(c), emulator.CoreSplitDrivers(c),
                         emulator.CoreIgnoreDrivers(c), emulator.CoreDriverLimit(c) + 1 /* driver 0 is "all-what's-remaining" */);
    }
  }

  AssignNames();
  ArcadeGame::Finalize();

  mReady = true;
}

ArcadeDatabase* ArcadeDatabaseManager::LoadFlatDatabase(const String& emulator, const String& core, const String& databaseFilename, const String& splitDriverString, const String& ignoredDriverString, int limit)
{
  // Get database
  Path database("/recalbox/system/arcade/flats/" + databaseFilename);

  // Build special sets
  HashSet<String> splitDrivers;
  for(const String& split : splitDriverString.Split(','))
    splitDrivers.insert_unique(split);
  HashSet<String> ignoredDrivers;
  for(const String& ignored : ignoredDriverString.Split(','))
    ignoredDrivers.insert_unique(ignored);

  // Load all lines
  String::List lines = Files::LoadAllFileLines(database);
  if (!lines.empty()) lines.erase(lines.begin()); // Remove header
  if (lines.empty())
  {
    LOG(LogError) << "[Arcade] Invalid database: " << database.ToString();
    return new ArcadeDatabase();
  }

  // Setup romname => FileData map for fast lookup
  HashMap<String, FileData*> map;
  class MapBuilder : public IParser
  {
    private:
      HashMap<String, FileData*>* mMap;

    public:
      explicit MapBuilder(HashMap<String, FileData*>& map) : mMap(&map) {}

      void Parse(FileData& game) override
      {
        if (game.IsGame())
          mMap->insert_unique(game.Metadata().RomFileOnly().FilenameWithoutExtension(), &game);
      }
  } Mapper(map);
  mSystem.MasterRoot().ParseAllItems(Mapper);

  // Prepare raw drivers
  HashMap<String, RawDriver> rawDrivers;
  rawDrivers.insert_unique("", { "", 0, 0 }); // all "others"
  int nextIndex = 1;

  // Build game array
  Array<ArcadeGame> games((int)lines.size(), 1, false); // Allocate all
  for(int i = (int)lines.size(); --i >= 0; )
    if (const String& line = lines[i]; !line.empty())
      DeserializeTo(games, line, map, rawDrivers, splitDrivers, ignoredDrivers, nextIndex);

  // Build final drivers
  String::List finalDrivers = BuildAndRemapDrivers(rawDrivers, games, limit, nextIndex);

  #if DEBUG
  { printf("%s\n", (String("First most populated arcade systems for ") + emulator + "-" + core).c_str()); }
  for(int i = 1; i < (int)finalDrivers.size(); ++i)
  { printf("%s\n", (String("  #") + i + " - " + finalDrivers[i]).c_str()); }
  #endif

  return new ArcadeDatabase(std::move(finalDrivers), std::move(games));
}

void ArcadeDatabaseManager::DeserializeTo(Array<ArcadeGame>& games, const String& line, const HashMap<String, FileData*>& map,
                                          HashMap<String, RawDriver>& drivers, const HashSet<String>& splitDrivers,
                                          const HashSet<String>& ignoreDrivers, int& nextDriverIndex)
{
  // Field positions
  enum
  {
    fZip = 0,
    fName,
    fType,
    fDriver,
    fRotation,
    fWidth,
    fHeight,
    fFrequency,
    fBios,
    fParent,
    fStatus,
    fFieldCount,
  };

  // Get fields
  String::List fields = line.Split('|');
  if (fields.size() != fFieldCount)
  { LOG(LogError) << "[Arcade] Invalid line: " << line; return; }

  // Try to lookup game
  FileData** fileData = map.try_get(fields[fZip]);
  if (fileData == nullptr) return; // This game is not in the rom folders
  FileData* realGame = *fileData;

  // Parent?
  FileData** parentNull = map.try_get(fields[fParent]);
  FileData* parent = parentNull != nullptr ? *parentNull : nullptr;

  // Raw driver
  String driver = fields[fDriver];
  if (int p = driver.Find('/'); p >= 0)
    driver.Delete(p, INT32_MAX);
  if (ignoreDrivers.contains(driver)) driver.clear(); // Sent to "others" driver 0
  if (splitDrivers.contains(driver)) driver = fields[fDriver].Remove(".cpp"); // Keep the whole name
  RawDriver* rawDriver = drivers.try_get(driver);
  if (rawDriver == nullptr)
  {
    drivers.insert(driver, { driver, nextDriverIndex++, 0});
    rawDriver = drivers.try_get(driver);
  }
  rawDriver->mGameCount++;

  // Data
  ArcadeGame::Rotation rotation = ArcadeGame::RotationFromString(fields[fRotation]);
  ArcadeGame::Type type = ArcadeGame::TypeFromString(fields[fType], parent != nullptr);
  ArcadeGame::Status status = ArcadeGame::StatusFromString(fields[fStatus]);

  games.Add(ArcadeGame(realGame, parent, fields[fName], 0, type, status, rotation));
  //lookups.insert(realGame, &games(games.Count() - 1));
}

String::List ArcadeDatabaseManager::BuildAndRemapDrivers(const HashMap<String, RawDriver>& rawDrivers, Array<ArcadeGame>& games, int limit, int rawDriverCount)
{
  // Build and sort a list of all drivers
  std::vector<RawDriver> sortedDrivers;
  for(const auto& kv : rawDrivers)
    sortedDrivers.push_back(kv.second);
  std::sort(sortedDrivers.begin(), sortedDrivers.end(), [](const RawDriver& a, const RawDriver& b) { return a.mGameCount > b.mGameCount; });

  // Build final list by selecting the top most used drivers
  // Also build a remap list where the index at pos X is remapped to X + 1
  String::List finalDrivers;
  Array<int> indexRemapper(rawDriverCount);
  indexRemapper.Insert(0, 0, rawDriverCount);
  finalDrivers.push_back(String::Empty); // Driver 0 = all others
  for(int i = 0; i < limit; ++i)
  {
    if (i >= (int)sortedDrivers.size()) break;
    RawDriver driver = sortedDrivers[i];
    if (driver.mIndex == 0) // Driver 0 has already been added, remove it
    {
      sortedDrivers.erase(sortedDrivers.begin() + i);
      if (i >= (int)sortedDrivers.size()) break;
      driver = sortedDrivers[i];
    }
    finalDrivers.push_back(driver.mName);
    indexRemapper(driver.mIndex) = i;
  }

  // Log
  if (Log::getReportingLevel() == LogLevel::LogDebug)
  {
    String log("[Arcade] Most used drivers: ");
    for (int i = 0; i < (int) finalDrivers.size(); ++i)
      log.Append(i == 0 ? "" : ",").Append(finalDrivers[i]);
    LOG(LogDebug) << log;
  }

  // Remap indexes using the remapper
  for(int i = games.Count(); --i >= 0;)
    games(i).RemapDriverTo(indexRemapper[games[i].Driver()]);

  // Count "all others". If noone in this driver, remove it and decrease all drivers
  int zeroCount = 0;
  for(int i = games.Count(); --i >= 0;)
    if (games(i).Driver() == 0)
      zeroCount++;
  if (zeroCount == 0)
  {
    finalDrivers.erase(finalDrivers.begin());
    for(int i = games.Count(); --i >= 0;)
      games(i).RemapDriverTo(games(i).Driver() - 1);
  }

  return finalDrivers;
}

void ArcadeDatabaseManager::AssignNames()
{
  class Renaming : public IParser
  {
    private:
      Databases& mDatabase;

    public:
      explicit Renaming(Databases& databases) : mDatabase(databases) {}

      void Parse(FileData& game) override
      {
        EmulatorManager emulators;
        String emulator, core;
        if (game.IsGame())
          // Game is unamed?
          if (game.Metadata().Name().empty())
            // Get emulator & core for this game, regarding all default and override configurations
            if (emulators.GetGameEmulator(game, emulator, core))
              // Lookup emulator hashmap
              if (ArcadeDatabase** gameDatabase = mDatabase.try_get(emulator.Append('|').Append(core)); gameDatabase != nullptr)
                // Has Game a matching ArcadeGame?
                if (const ArcadeGame* arcadeGame = (*gameDatabase)->LookupGame(game); arcadeGame != nullptr)
                  game.Metadata().SetName(arcadeGame->ArcadeName());
      }
  } Renamer(mDatabases);
  mSystem.MasterRoot().ParseAllItems(Renamer);
}

const ArcadeDatabase* ArcadeDatabaseManager::LookupDatabase() const
{
  String emulator, core;
  if (mSystem.Manager().Emulators().GetDefaultEmulator(mSystem, emulator, core))
  {
    ArcadeDatabase** database = mDatabases.try_get(emulator.Append('|').Append(core));
    if (database != nullptr) return *database;
  }
  return nullptr;
}

const ArcadeDatabase* ArcadeDatabaseManager::LookupDatabase(const FolderData& folder) const
{
  String emulator, core;
  if (mSystem.Manager().Emulators().GetGameEmulator(folder, emulator, core))
  {
    ArcadeDatabase** database = mDatabases.try_get(emulator.Append('|').Append(core));
    if (database != nullptr) return *database;
  }
  return nullptr;
}

void ArcadeDatabaseManager::RemoveGame(const FileData& game)
{
  for(const auto& database : mDatabases)
    database.second->Remove(game);
}


