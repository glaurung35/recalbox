//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <components/MenuComponent.h>
#include <components/OptionListComponent.h>
#include <guis/menus/GuiMenuBase.h>
#include <components/SwitchComponent.h>

class GuiMenuThemeOptions : public GuiMenuBase
                          , private ISwitchComponent
                          , private IOptionListComponent<String>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuThemeOptions(WindowManager& window);

    //! Destructor
    ~GuiMenuThemeOptions() override;

  private:
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

    //! Theme
    std::shared_ptr<OptionListComponent<String>> mTheme;

    //! Transition original value
    String mOriginalTransition;
    //! Theme original value
    String mOriginalTheme;
    //! Timed change
    int mTimer;

    //! Get Transition list
    static std::vector<ListEntry<String>> GetTransitionEntries();
    //! Get Theme List
    std::vector<ListEntry<String>> GetThemeEntries();
    //! Get Region List
    static std::vector<ListEntry<String>> GetRegionEntries();

    /*
     * IOptionListComponent<String> implementation
     */

    void OptionListComponentChanged(int id, int index, const String& value, bool quickChange) override;

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
