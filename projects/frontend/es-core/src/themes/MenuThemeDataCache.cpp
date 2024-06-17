//
// Created by bkg2k on 22/08/24.
//

#include "MenuThemeDataCache.h"
#include <themes/ThemeManager.h>

std::shared_ptr<TextureResource>& MenuThemeDataCache::GetIcon(MenuThemeData::MenuIcons::Type icon)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* flag = mIconToTextures.try_get(icon);
  if (flag == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(ThemeManager::Instance().Menu().Icons().FromType(icon), false, true, true);
    mIconToTextures.insert(icon, texture);
    flag = mIconToTextures.try_get(icon);
  }
  return *flag;
}

std::shared_ptr<TextureResource>& MenuThemeDataCache::GetElement(MenuThemeData::IconElement::Type element)
{
  Mutex::AutoLock lock(mLocker);
  std::shared_ptr<TextureResource>* flag = mElementToTextures.try_get(element);
  if (flag == nullptr)
  {
    // Load flag
    std::shared_ptr<TextureResource> texture = TextureResource::get(ThemeManager::Instance().Menu().Elements().FromType(element), false, true, true);
    mElementToTextures.insert(element, texture);
    flag = mElementToTextures.try_get(element);
  }
  return *flag;
}
