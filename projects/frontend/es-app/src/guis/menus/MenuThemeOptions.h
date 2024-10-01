//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "guis/menus/base/ISelectorChanged.h"
#include "guis/menus/base/ISwitchChanged.h"

//! Theme name + path
struct ThemeSpec
{
  String Name;
  Path FolderPath;

  bool operator==(const ThemeSpec& right) const { return Name == right.Name && FolderPath == right.FolderPath; }
};

class MenuThemeOptions : public Menu
                          , private ISwitchChanged
                          , private ISingleSelectorChanged<String>
                          , private ISingleSelectorChanged<ThemeSpec>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuThemeOptions(WindowManager& window, const IGlobalVariableResolver& resolver);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    static constexpr int sRecalboxMinimumCompatibilityVersion = 0x902; // (Major << 8) + Minor

    //! Components
    enum class Components
    {
      Carousel,
      Transition,
      Theme,
      Region,
    };

    //! Theme spec list
    typedef std::vector<ThemeSpec> ThemeSpecList;

    //! Resolver
    const IGlobalVariableResolver& mResolver;

    //! Default theme
    ThemeSpec GetCurrentTheme();
    //! Default theme
    ThemeSpec GetDefaultTheme();
    //! Get Transition list
    SelectorEntry<String>::List GetTransitionEntries();
    //! Get Theme List
    SelectorEntry<ThemeSpec>::List GetThemeEntries();
    //! Get Region List
    static SelectorEntry<String>::List GetRegionEntries();

    /*!
     * @brief Check theme compatibility with current display & recalbox version
     * @param themePath Theme folder path
     * @param compatible Set o True if the theme is compatible when the method returns. False otherwise
     * @param switchTheme True to switch theme & notify the user of potential incompatibilities
     * @return Theme name (or folder of a name is not available) or empty if the theme has not been analyzed successfully
     */
    String CheckCompatibility(const Path& themePath, [[out]] bool& compatible, bool switchTheme);

    /*!
     * @brief Actually switch theme
     * @param themePath Theme path
     */
    void DoSwitchTheme(const Path& themePath);

    static String GetDisplayList(ThemeData::Compatibility display);

    static String GetResolutionList(ThemeData::Resolutions resolutions);

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * IOptionListComponent<ThemeSpec> implementation
     */

    void MenuSingleChanged(int id, int index, const ThemeSpec& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};
