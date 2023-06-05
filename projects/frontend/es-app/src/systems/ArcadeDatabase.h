//
// Created by bkg2k on 15/04/23.
//
#pragma once

#include <games/FileData.h>
#include <systems/ArcadeGame.h>
#include <utils/storage/HashMap.h>
#include <utils/storage/Array.h>

//! Game database for a given emulator/core
class ArcadeDatabase
{
  public:
    /*!
     * @brief Constructor
     * @param drivers Final drivers
     * @param games Arcade game spec list
     * @param lookup Lookup map
     */
    ArcadeDatabase(String::List&& drivers, Array<ArcadeGame>&& games)
      : mDrivers(drivers)
      , mGames(games)
      , mLookup()
    {
      // Build lookup
      for(int i = games.Count(); --i >= 0;)
        mLookup.insert(&games(i).Game(), i);
    }

    /*!
     * Empty database constructor
     */
    ArcadeDatabase() = default;

    /*!
     * @brief Lookup an arcade game from the source game
     * @param game Source game
     * @return Arcade game or null
     */
    [[nodiscard]] const ArcadeGame* LookupGame(const FileData& game) const
    {
      int* index = mLookup.try_get(&game);
      if (index == nullptr) return nullptr;
      return &mGames.ConstRef(*index);
    }

    /*!
     * @brief Remove any reference to the given game.
     * Delete direct arcade gama associated, and all children orphans
     * @param game FileData reference to remove reference to
     */
    void Remove(const FileData& game)
    {
      for(int i = mGames.Count(); --i >= 0; )
      {
        ArcadeGame& arcade = mGames(i);
        if (&arcade.Game() == &game) mGames.Delete(i);
        else if (arcade.Parent() == &game) arcade.MakeOrphan();
      }
    }

    /*!
     * @brief Check if this database is valid and contains games
     * @return True if there is at least one game
     */
    [[nodiscard]] bool IsValid() const { return !mGames.Empty(); }

    /*!
     * @brief Check if games in this database can be filtered (more than one driver)
     * @return True if there is more than one driver
     */
    [[nodiscard]] bool CanBeFiltered() const { return mDrivers.size() > 1; }

  private:
    String::List mDrivers;                 //!< Final driver list
    Array<ArcadeGame> mGames;              //!< Game list
    HashMap<const FileData*, int> mLookup; //!< Reverse lookup FileData* => ArcadeGame
};
