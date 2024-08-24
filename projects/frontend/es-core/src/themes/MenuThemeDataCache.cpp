//
// Created by bkg2k on 22/08/24.
//

#include "MenuThemeDataCache.h"
#include <themes/ThemeManager.h>

std::shared_ptr<TextureResource>& MenuThemeDataCache::GetIcon(MenuThemeData::MenuIcons::Type icon, int width, int height)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* resource = mIconToTextures.try_get(icon);
  if (resource == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(ThemeManager::Instance().Menu().Icons().FromType(icon), false, true, true);
    if ((width | height) != 0) texture->rasterizeAt(width, height);
    mIconToTextures.insert(icon, texture);
    resource = mIconToTextures.try_get(icon);
  }
  return *resource;
}

std::shared_ptr<TextureResource>& MenuThemeDataCache::GetElement(MenuThemeData::IconElement::Type element, int width, int height)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* resource = mElementToTextures.try_get(element);
  if (resource == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(ThemeManager::Instance().Menu().Elements().FromType(element), false, true, true);
    if ((width | height) != 0) texture->rasterizeAt(width, height);
    mElementToTextures.insert(element, texture);
    resource = mElementToTextures.try_get(element);
  }
  return *resource;
}

std::shared_ptr<TextureResource>& MenuThemeDataCache::GetFromPath(const Path& path, int width, int height)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* resource = mPathToTextures.try_get(path);
  if (resource == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(path, false, true, true);
    texture->rasterizeAt(width, height);
    mPathToTextures.insert(path, texture);
    resource = mPathToTextures.try_get(path);
  }
  return *resource;
}
