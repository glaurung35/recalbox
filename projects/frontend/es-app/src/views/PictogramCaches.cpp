//
// Created by bkg2k on 07/03/24.
//

#include "PictogramCaches.h"

std::shared_ptr<TextureResource>& PictogramCaches::GetFlag(Regions::GameRegions region)
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

std::shared_ptr<TextureResource>& PictogramCaches::GetGenre(GameGenres genre)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* ptexture = mGenreToTextures.try_get(genre);
  if (ptexture == nullptr)
  {
    // Load genre
    std::shared_ptr<TextureResource> texture = TextureResource::get(Path(String(Genres::GetResourcePath(genre).ToString()).Replace("/genre/", "/genreflags/")), false, true, true);
    mGenreToTextures.insert(genre, texture);
    ptexture = mGenreToTextures.try_get(genre);
  }
  return *ptexture;
}

std::shared_ptr<TextureResource>& PictogramCaches::GetPlayers(int playermin, int playermax)
{
  // Some control first
  if (playermin == 0xFFFF) { playermin = playermax; playermax = 0; }
  playermin = Math::clampi(playermin, 1, 12);
  if (playermax < playermin && playermax != 0) playermax = playermin;

  int playerMinMax = (playermin << 16) | playermax;
  std::shared_ptr<TextureResource>* pPlayers = mPlayersToTextures.try_get(playerMinMax);
  if (pPlayers == nullptr)
  {
    String resourceName(":/players/");
    resourceName.Append(playermin);
    if (playermin != playermax)
      switch(playermin)
      {
        case 1:
        case 2:
        case 3:
        case 4: if (playermax > playermin) resourceName.Append("to").Append(playermax); else resourceName.Append("plus"); break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11: if (playermax == playermin) resourceName.Append("plus"); break;
        case 12:
        default: break;
      }
    // Load players
    std::shared_ptr<TextureResource> texture = TextureResource::get(Path(resourceName.Append(".svg")), false, true, true);
    mPlayersToTextures.insert(playerMinMax, texture);
    pPlayers = mPlayersToTextures.try_get(playerMinMax);
  }
  return *pPlayers;
}
