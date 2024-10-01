//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISelectorChanged.h>

// Forward declaration
class SystemManager;
class SystemData;

class MenuSystemConfiguration : public Menu
                              , private ISingleSelectorChanged<String>
                              , private ISwitchChanged
{
  public:
    enum class AdvancedMenuOptions : unsigned char
    {
      Emulator  = (1 << 0), //!< Can choose emulator & core
      Ratio     = (1 << 1), //!< Can choose emulator ratio
      Smooth    = (1 << 2), //!< Can choose bilinear filtering
      Rewind    = (1 << 3), //!< Can choose rewind
      AutoSave  = (1 << 4), //!< Can choose autosave/autoload
      Shaders   = (1 << 5), //!< Can choose individual shader
      ShaderSet = (1 << 6), //!< Can choose shader set
      All       = 0xFF
    };

    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuSystemConfiguration(WindowManager& window, SystemData& system, SystemManager& systemManager, AdvancedMenuOptions options = AdvancedMenuOptions::All);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      Emulator,
      Ratio,
      Smooth,
      Rewind,
      AutoSave,
      Shaders,
      ShaderSet,
    };

    //! System manager
    SystemManager& mSystemManager;
    //! Target system
    SystemData& mSystem;
    //! Default emulator
    String mDefaultEmulator;
    //! Default core
    String mDefaultCore;

    //! Get Emulator List
    SelectorEntry<String>::List GetEmulatorEntries(String& emulatorAndCore, String& defaultEmulatorAndCore);
    //! Get Ratio List
    SelectorEntry<String>::List GetRatioEntries();
    //! Get Shaders List
    SelectorEntry<String>::List GetShadersEntries();
    //! Get ShaderSet List
    SelectorEntry<String>::List GetShaderSetEntries();

    //! Menu options
    AdvancedMenuOptions mMenuOptions;

    /*
     * ISingleSelectorChanged<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};

DEFINE_BITFLAG_ENUM(MenuSystemConfiguration::AdvancedMenuOptions, unsigned char)

