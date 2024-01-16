//
// Created by bkg2k on 12/12/23.
//
#pragma once

#include "ThemeData.h"
#include "MenuThemeData.h"
#include "IThemeSwitchable.h"
#include "ThemeFileCache.h"
#include "guis/GuiSyncWaiter.h"
#include <utils/os/system/ThreadPool.h>
#include <utils/cplusplus/StaticLifeCycleControler.h>

// forward declaration
class SystemData;

class ThemeManager : public StaticLifeCycleControler<ThemeManager>
                   , private IThreadPoolWorkerInterface<const SystemData*, const SystemData*>
                   , public IThemeSwitchTick
{
  public:
    //! Default theme folder
    static constexpr const char* sDefaultThemeFolder = "recalbox-next";
    //! Default theme file
    static constexpr const char* sRootThemeFile = "theme.xml";
    //! Default system folder
    static constexpr const char* sDefaultSystemThemeFolder = "default";

    //! List of theme w/ name
    typedef HashMap<String, Path> ThemeList;

    //! Default constructor
    explicit ThemeManager(IGlobalVariableResolver& globalResolver);

    //! Destructor
    ~ThemeManager() override;

    //! Load initial theme
    void Initialize(WindowManager* window);

    /*!
     * @brief Get a list of available theme
     * @return Theme list
     */
    static ThemeList AvailableThemes();

    /*!
     * @brief Load system theme
     * @param system System
     */
    void LoadSystemTheme(const SystemData& system);

    /*
     * Getters
     */

    //! Main theme
    [[nodiscard]] const ThemeData& Main() const { return mMain; }

    //! Menu theme
    [[nodiscard]] const MenuThemeData& Menu() const { return mMenu; }

    //! System theme
    const ThemeData& System(const SystemData* system) { return CreateOrGetSystem(system); }

    /*!
     * @brief Register a new switchable implementation
     * @param switchable Switchable implementation
     */
    void Register(IThemeSwitchable& switchable) { mSwitchables.Add(&switchable); }

    /*!
     * @brief Unregister a switchable implementation. May fail silently if the implementation is unknown
     * @param switchable Switchable implementation
     */
    void Unregister(IThemeSwitchable& switchable) { mSwitchables.Remove(&switchable); }

    /*!
     * @brief Theme or theme configuration has changed, reload main, menu & all existing system themes
     * then, apply theme in all UIs
     * @param window Window manager
     * @param force True to force theme reload
     */
    void DoThemeChange(WindowManager* window, bool force = false);

    //! Get global variable resolver
    const IGlobalVariableResolver& GlobalResolver() const { return mGlobalResolver; }

  private:
    //! Global file cache
    ThemeFileCache mCache;

    //! Switchable implementation list
    Array<IThemeSwitchable*> mSwitchables;

    //! Main theme
    ThemeData mMain;
    //! Menu theme data
    MenuThemeData mMenu;
    //! System themes
    HashMap<const SystemData*, ThemeData*> mSystem;

    //! Current theme root path
    Path mRootPath;

    //! Wait bar
    GuiSyncWaiter* mWaiter;
    //! Wait bar reference time
    DateTime mWaitBarReference;

    //! Global variable resolver
    IGlobalVariableResolver& mGlobalResolver;

    /*!
     * @brief Create or get existing system theme
     * @param system System
     * @return ThemeData instance
     */
    ThemeData& CreateOrGetSystem(const SystemData* system);

    /*!
     * @brief Notify all IThemeSwitchable implementation that a new theme has been loaded
     * allowing them to refresh their themable elements
     * @param refreshOnly the theme did not change but options may have - Implementation must refresh element's properties
     */
    void NotifyThemeChanged(bool refreshOnly);

    /*!
     * @brief Return the currently selected theme root peth
     * If the theme is inconsistent with available themes, it is changed to match the first theme available
     */
    static Path GetThemeRootPath();

    /*!
     * @brief Load main theme & menu theme
     */
    void LoadMainTheme();

    /*
     * IThreadPoolWorkerInterface implementation
     */

    const SystemData* ThreadPoolRunJob(const SystemData*& feed) override;

    /*
     * IThemeSwitchTick implementation
     */

    /*!
     * @brief Called by IThemeSwitchable implementation every time they update something slow
     * This is used by the theme manager to update wait bars while switching theme
     */
    void ThemeSwitchTick() override;
};

