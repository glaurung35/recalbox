//
// Created by bkg2k on 07/03/24.
//

#include "FlagCaches.h"

std::shared_ptr<TextureResource>& FlagCaches::GetFlag(Regions::GameRegions region)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* flag = mRegionToTextures.try_get(region);
  if (flag == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(Path(":/regions/" + Regions::GameRegionsFromEnum(region) + ".svg"), false, true, true);
    mRegionToTextures.insert(region, texture);
    flag = mRegionToTextures.try_get(region);
  }
  return *flag;
}
