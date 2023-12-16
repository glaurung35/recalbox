//
// Created by bkg2k on 15/12/23.
//
#pragma once

// Forward declaration
class SystemData;
class ThemeData;
class ThemeSwitcher;

class IThemeSwitchable
{
  public:
    //! Constructor
    IThemeSwitchable();

    //! Default destructor
    virtual ~IThemeSwitchable();

    /*!
     * @brief Implementation istruct the Theme switcher to use a specific system theme
     * Returning nullpre means the global theme will be used
     * @return SystemData or nullptr
     */
    [[nodiscard]] virtual SystemData* SystemTheme() const = 0;

    /*!
     * @brief Called when theme switch.
     * @param theme New Theme
     */
    virtual void SwitchToTheme(ThemeData& theme) = 0;
};
