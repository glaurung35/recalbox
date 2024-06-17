//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/ISliderChanged.h>

//! Submenu item
class ItemSlider : public ItemBase
{
  public:
    ItemSlider(const String& label, const MenuThemeData& theme, ISliderChanged* interface, int identifier, float min, float max, float inc, float value, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, help, unselectable)
      , mInterface(interface)
      , mMinimum(min)
      , mMaximum(max)
      , mIncrement(inc)
      , mValue(value)
    {}
    ItemSlider(const String& label, const MenuThemeData& theme, ISliderChanged* interface, int identifier, MenuThemeData::MenuIcons::Type icon, float min, float max, float inc, float value, const String& help, bool unselectable)
      : ItemBase(Type::SubMenu, label, theme, identifier, icon, help, unselectable)
      , mInterface(interface)
      , mMinimum(min)
      , mMaximum(max)
      , mIncrement(inc)
      , mValue(value)
    {}

  private:
    //! Interface
    ISliderChanged* mInterface;
    //! Value min
    float mMinimum;
    //! Value max
    float mMaximum;
    //! Increment
    float mIncrement;
    //! Value
    float mValue;

    //! Fire callback
    void Fire() override { mInterface->MenuSliderMoved(Identifier(), mValue); }
};
