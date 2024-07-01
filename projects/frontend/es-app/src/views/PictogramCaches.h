//
// Created by bkg2k on 07/03/24.
//
#pragma once

#include <utils/storage/HashMap.h>
#include <games/classifications/Regions.h>
#include <games/classifications/Genres.h>
#include <resources/TextureResource.h>
#include <memory>

class PictogramCaches
{
  public:
    /*!
     * @brief Get texture for the given region
     * @param region region to get flag texture from
     * @return Flag texture (or empty texture if the flag resource does not exist)
     */
    std::shared_ptr<TextureResource>& GetFlag(Regions::GameRegions region);

    /*!
     * @brief Get texture for the given genre
     * @param genre genre to get texture from
     * @return Genre texture (or empty texture if the genre resource does not exist)
     */
    std::shared_ptr<TextureResource>& GetGenre(GameGenres genres);

    /*!
     * @brief Get texture for the given players
     * @param playermin player min to get best texture from
     * @param playermax player max to get best texture from
     * @return Player texture (or empty texture if the player resource does not exist)
     */
    std::shared_ptr<TextureResource>& GetPlayers(int playermin, int playermax);

    //! Clean all flags to free memory
    void Clear()
    {
      mRegionToTextures.clear();
      mGenreToTextures.clear();
      mPlayersToTextures.clear();
    }

  private:
    //! region to flag texture
    HashMap<Regions::GameRegions, std::shared_ptr<TextureResource>> mRegionToTextures;
    //! GameGenre to genre texture
    HashMap<GameGenres, std::shared_ptr<TextureResource>> mGenreToTextures;
    //! GameGenre to genre texture
    HashMap<int, std::shared_ptr<TextureResource>> mPlayersToTextures;
    //! Hashmap locker
    Mutex mLocker;
};
