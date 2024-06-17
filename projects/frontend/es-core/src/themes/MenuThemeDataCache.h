//
// Created by bkg2k on 22/08/24.
//
#pragma once

#include <utils/storage/HashMap.h>
#include <games/classifications/Regions.h>
#include <resources/TextureResource.h>
#include <themes/MenuThemeData.h>
#include <memory>

class MenuThemeDataCache
{
  public:
    MenuThemeDataCache(const MenuThemeData& menutheme)
      : mTheme(menutheme)
    {}

    /*!
     * @brief Get texture for the menu icon
     * @param icon icon type to get menu icon texture from
     * @return menu icon texture
     */
    std::shared_ptr<TextureResource>& GetIcon(MenuThemeData::MenuIcons::Type icon);

    /*!
     * @brief Get texture for the menu element
     * @param element element type to get menu element texture from
     * @return menu element texture
     */
    std::shared_ptr<TextureResource>& GetElement(MenuThemeData::IconElement::Type element);

    //! Clean all texture to free memory
    void Clear()
    {
      mIconToTextures.clear();
      mElementToTextures.clear();
    }

  private:
    //! icon to texture
    HashMap<MenuThemeData::MenuIcons::Type, std::shared_ptr<TextureResource>> mIconToTextures;
    //! element to texture
    HashMap<MenuThemeData::IconElement::Type, std::shared_ptr<TextureResource>> mElementToTextures;
    //! Hashmap locker
    Mutex mLocker;
    //! Menu theme reference
    const MenuThemeData& mTheme;
};
