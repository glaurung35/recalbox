//
// Created by bkg2k on 12/03/23.
//
#pragma once

#include <utils/String.h>
#include "ArcadeGame.h"
#include "utils/storage/Set.h"
#include "ArcadeDatabase.h"

// Forward declaration
class SystemData;

class ArcadeDatabaseManager
{
  public:
    /*!
     * @brief Load arcade databases for a given system
     * @param parentSystem Parent system
     */
    explicit ArcadeDatabaseManager(SystemData& parentSystem);

    /*!
     * @brief Load all arcade database
     */
    void LoadDatabases();

    /*!
     * @brief Lookup an arcade database for the given system
     * regarding only default system configuration to find out what emulator & core to use
     * @param system System to get database from
     * @return GameDatabase or null
     */
    const GameDatabase* LookupDatabase(SystemData& system);

  private:
    //! Raw driver structure
    struct RawDriver
    {
      String mName;   //!< Driver name
      int mIndex;     //!< Driver index
      int mGameCount; //!< Game using this driver
    };

    //! System reference
    SystemData& mSystem;

    //! Arcade database typedef
    typedef HashMap<String, GameDatabase> Databases;
    //! Database per core per emulator
    Databases mDatabases;
    //! Loaded?
    bool mReady;

    /*!
     * @brief Load flat database for the given emulator/core
     * @param emulator Emulator name
     * @param core Core name
     * @param splitDriverString raw split driver string list
     * @param ignoredDriverString raw ignored driver string list
     * @result GameDatabase
     */
    GameDatabase LoadFlatDatabase(const String& emulator, const String& core, const String& splitDriverString, const String& ignoredDriverString, int limit);

    /*!
     * @brief Deserializea line into an ArcadeGame structure
     * @param database Target database
     * @param line Text line to deserialize
     * @param map Map used to lookup FileData
     * @param drivers raw driver map used in loading process
     * @param nextDriverIndex next driver index
     */
    static void DeserializeTo(Array<ArcadeGame>& games, const String& line, const HashMap<String, FileData*>& map,
                              HashMap<const FileData*, ArcadeGame*>& lookups, HashMap<String, RawDriver>& drivers,
                              const HashSet<String>& splitDrivers, const HashSet<String>& ignoreDrivers, int& nextDriverIndex);

    /*!
     * @brief Build final driver list & remap drivers in existing games
     * @param map Raw driver map
     * @param array Game array
     * @param limit maximum final drivers
     * @param rawDriverCount raw driver count
     * @return Final driver list
     */
    static String::List BuildAndRemapDrivers(const HashMap<String, RawDriver>& map, Array<ArcadeGame>& games, int limit, int rawDriverCount);

    /*!
     * @brief Try to assign best matching names to unamed games
     * regarding the core the game is assigned to (default, or forced in any way)
     */
    void AssignNames();
};
