//
// Created by bkg2k on 15/04/23.
//
#pragma once

#include <games/FileData.h>
#include <systems/ArcadeGame.h>
#include <utils/storage/HashMap.h>
#include <utils/storage/Array.h>

//! Game database for a given emulator/core
class GameDatabase
{
  public:
    /*!
     * @brief Constructor
     * @param drivers Final drivers
     * @param games Arcade game spec list
     * @param lookup Lookup map
     */
    GameDatabase(String::List&& drivers, Array<ArcadeGame>&& games, HashMap<const FileData*, ArcadeGame*>&& lookup)
      : mDrivers(drivers)
      , mGames(games)
      , mLookup(lookup)
    {}

    /*!
     * Empty database constructor
     */
    GameDatabase() = default;

    /*!
     * @brief Lookup an arcade game from the source game
     * @param game Source game
     * @return Arcade game or null
     */
    ArcadeGame* LookupGame(const FileData& game)
    {
      ArcadeGame** arcadeGame = mLookup.try_get(&game);
      if (arcadeGame == nullptr) return nullptr;
      return *arcadeGame;
    }

  private:
    String::List mDrivers;                         //!< Final driver list
    Array<ArcadeGame> mGames;                      //!< Game list
    HashMap<const FileData*, ArcadeGame*> mLookup; //!< Reverse lookup FileData* => ArcadeGame
};
