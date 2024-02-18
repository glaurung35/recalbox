//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <components/MenuComponent.h>
#include <components/OptionListComponent.h>
#include <guis/menus/GuiMenuBase.h>
#include <components/SwitchComponent.h>

//! Theme name + path
struct ThemeSpec { String Name; Path FolderPath; };

class GuiMenuThemeOptions : public GuiMenuBase
                          , private ISwitchComponent
                          , private IOptionListComponent<String>
                          , private IOptionListComponent<ThemeSpec>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuThemeOptions(WindowManager& window, const IGlobalVariableResolver& resolver);

    //! Destructor
    ~GuiMenuThemeOptions() override;

  private:
    static constexpr int sRecalboxMinimumCompatibilityVersion = 0x902; // (Major << 8) + Minor
    //! Apply change timer
    static constexpr int sApplyChangeTimer = 750;

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

    //! Theme
    std::shared_ptr<OptionListComponent<ThemeSpec>> mTheme;

    //! Last theme path (in use with timer)
    Path mLastThemePath;

    //! Transition original value
    String mOriginalTransition;
    //! Theme original value
    String mOriginalTheme;
    //! Timed change
    int mTimer;

    //! Get Transition list
    static std::vector<ListEntry<String>> GetTransitionEntries();
    //! Get Theme List
    std::vector<ListEntry<ThemeSpec>> GetThemeEntries();
    //! Get Region List
    static std::vector<ListEntry<String>> GetRegionEntries();

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

    void OptionListComponentChanged(int id, int index, const String& value, bool quickChange) override;

    /*
     * IOptionListComponent<ThemeSpec> implementation
     */

    void OptionListComponentChanged(int id, int index, const ThemeSpec& value, bool quickChange) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool& status) override;

    /*
     * Component overrides
     */

    void Update(int elapsed) override;

    bool ProcessInput(const InputCompactEvent& event) override;
};
