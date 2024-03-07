//
// Created by bkg2k on 07/03/24.
//
#pragma once

#include <utils/storage/HashMap.h>
#include <games/classifications/Regions.h>
#include <resources/TextureResource.h>
#include <memory>

class FlagCaches
{
  public:
    /*!
     * @brief Get texture for the given region
     * @param region region to get flag texture from
     * @return Flkag texture (or empty texture if the flag resource does not exist
     */
    std::shared_ptr<TextureResource>& GetFlag(Regions::GameRegions region);

    //! Clean all flags to free memory
    void Clear() { mRegionToTextures.clear(); }

  private:
    //! region to flag texture
    HashMap<Regions::GameRegions, std::shared_ptr<TextureResource>> mRegionToTextures;
    //! Hashmap locker
    Mutex mLocker;
};
