//
// Created by bkg2k on 20/08/24.
//
#pragma once

#include <utils/String.h>
#include <themes/MenuThemeData.h>
#include "IOverlay.h"

// Forward declaration
class ItemHeader;
class ItemSubMenu;
class ItemSwitch;
class ItemSlider;

//! Menu item structure
class ItemBase : public IOverlay
{
  public:
    enum class Type : char
    {
      Header,   //!< Header
      SubMenu,  //!< Sub menu
      InfoText, //!< Non interactive text
      Switch,   //!< On/Off switch
      Slider,   //!< Slider
      Editor,   //!< Text editor
      List,     //!< Single value among many
      Multi,    //!< Multiple values among many
      Rating,   //!< Rating stars
    };

    //! Get item type
    [[nodiscard]] Type ItemType() const { return mType; }
    //! Get item identifier
    [[nodiscard]] int Identifier() const { return mIdentifier; }
    //! Check if this item has a valid icon
    [[nodiscard]] bool HasValidIcon() const { return mHasIcon; }
    //! Icon
    MenuThemeData::MenuIcons::Type Icon() const { return mIcon; }
    //! Label
    const String& Label() const { return mLabel; }
    //! Help
    const String& Help() const { return mHelp; }

    /*!
     * @brief Notification of an input event
     * @param event Compact event
     * @return Implementation must return true if it consumed the event.
     */
    virtual bool ProcessInput(const InputCompactEvent& event) = 0;

    /*!
     * @brief Called once per frame, after Update.
     * @param area Area to draw within
     */
    virtual void Render(const Rectangle& area) = 0;

    /*
     * Filtering
     */

    //! Check if this item is selectable
    [[nodiscard]] bool IsSelectable() const { return !mIsUnselectable; }

    //! Check of the item is displayable - TODO: implement filtering here
    [[nodiscard]] bool IsDisplayable() const { return true; }

    /*
     * Identification
     */

    bool IsHeader() const { return mType == Type::Header; }
    bool IsSubMenu() const { return mType == Type::SubMenu; }
    bool IsInfoText() const { return mType == Type::InfoText; }
    bool IsSwitch() const { return mType == Type::Switch; }
    bool IsSlider() const { return mType == Type::Slider; }
    bool IsEditor() const { return mType == Type::Editor; }
    bool IsList() const { return mType == Type::List; }
    bool IsMulti() const { return mType == Type::Multi; }
    bool IsRating() const { return mType == Type::Rating; }

    /*
     * Get typed children
     */

    ItemHeader* AsHeader() { return mType == Type::Header ? (ItemHeader*)(this) : nullptr; }
    ItemSubMenu* AsSubMenu() { return mType == Type::SubMenu ? (ItemSubMenu*)(this) : nullptr; }
    //bool IsInfoText() const { return mType == Type::InfoText; }
    ItemSwitch* AsSwitch() { return mType == Type::Switch ? (ItemSwitch*)(this) : nullptr; }
    ItemSlider* AsSlider() { return mType == Type::Slider ? (ItemSlider*)(this) : nullptr; }
    //bool IsEditor() const { return mType == Type::Editor; }
    //bool IsList() const { return mType == Type::List; }
    //bool IsMulti() const { return mType == Type::Multi; }
    //bool IsRating() const { return mType == Type::Rating; }

  protected:
    //! Fire callback
    virtual void Fire() = 0;

    //! Minimal constructor
    ItemBase(Type type, const String& label, const MenuThemeData& theme, int identifier, const String& help, bool unselectable)
      : mType(type)
      , mHasIcon(false)
      , mIsUnselectable(unselectable)
      , mIdentifier(identifier)
      , mIcon(MenuThemeData::MenuIcons::Type(0))
      , mLabel(label)
      , mHelp(help)
      , mTheme(theme)
    {}

    //! With icon
    ItemBase(Type type, const String& label, const MenuThemeData& theme, int identifier, MenuThemeData::MenuIcons::Type icon, const String& help, bool unselectable)
      : mType(type)
      , mHasIcon(true)
      , mIsUnselectable(unselectable)
      , mIdentifier(identifier)
      , mIcon(icon)
      , mLabel(label)
      , mHelp(help)
      , mTheme(theme)
    {}

    Type mType;                           //!< Menu item type
    bool mHasIcon;                        //!< Has a valid icon?
    bool mIsUnselectable;                 //!< True if the item is shown but not selectable
    int  mIdentifier;                     //!< Identifier from the template enum
    MenuThemeData::MenuIcons::Type mIcon; //!< Optionnal icon type
    String mLabel;                        //!< Menu item label
    String mHelp;                         //!< Optional help string
    const MenuThemeData& mTheme;          //!< Menu theme
};

