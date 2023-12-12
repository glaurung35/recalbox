//
// Created by bkg2k on 12/12/23.
//
#pragma once

#include "ThemeData.h"
#include "MenuThemeData.h"
#include <utils/cplusplus/StaticLifeCycleControler.h>

// forward declaration
class SystemData;

class ThemeManager : public StaticLifeCycleControler<ThemeManager>
{
  public:
    //! Default theme file
    static constexpr const char* sRootThemeFile = "theme.xml";
    //! Default system folder
    static constexpr const char* sDefaultSystemThemeFolder = "default";

    //! List of theme w/ name
    typedef HashMap<String, Path> ThemeList;

    //! Default constructor
    ThemeManager();

    //! Destructor
    ~ThemeManager();

    /*!
     * @brief Load main theme & menu theme
     */
    void LoadMainTheme();

    /*!
     * @brief Load system theme
     * @param system System
     */
    void LoadSystemTheme(const SystemData* system);

    /*!
     * @brief Theme or theme configuration has changed, reload main, menu & all existing system themes
     * then, apply theme in all UIs
     * @param window Window manager
     */
    void ThemeChanged(WindowManager& window);

    /*!
     * @brief Get a list of available theme
     * @return Theme list
     */
    static ThemeList AvailableThemes();

    /*
     * Getters
     */

    //! Main theme
    [[nodiscard]] const ThemeData& Main() const { return mMain; }

    //! Menu theme
    [[nodiscard]] const MenuThemeData& Menu() const { return mMenu; }

    //! System theme
    const ThemeData& System(const SystemData* system) { return CreateOrGetSystem(system); }

  private:
    //! Main theme
    ThemeData mMain;
    //! Menu theme data
    MenuThemeData mMenu;
    //! System themes
    HashMap<const SystemData*, ThemeData*> mSystem;

    //! Current theme root path
    Path mRootPath;

    /*!
     * @brief Create or get existing system theme
     * @param system System
     * @return ThemeData instance
     */
    ThemeData& CreateOrGetSystem(const SystemData* system);
};

