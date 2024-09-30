//
// Created by bkg2k on 06/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuGameSettings : public Menu
                       , private ISingleSelectorChanged<String>
                       , private ISingleSelectorChanged<RecalboxConf::SoftPatching>
                       , private ISwitchChanged
                       , private ISubMenuSelected
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuGameSettings(WindowManager& window, SystemManager& systemManager);

    //! Build menu items
    void BuildMenuItems() final;

    //! Destructor
    ~MenuGameSettings() override = default;

  private:
    enum class Components
    {
      Ratio,
      RecalboxOverlays,
      Smooth,
      Rewind,
      AutoSave,
      ShowSaveStates,
      QuitTwice,
      IntegerScale,
      Shaders,
      Softpatching,
      ShaderSet,
      RetroAchivements,
      Netplay,
      SuperGameBoy,
      HDMode,
      WideScreenMode,
      VulkanDriver
    };

    //! System manager
    SystemManager& mSystemManager;

    //! Smooth
    ItemSwitch* mSmooth;
    //! Show save states
    ItemSwitch* mShowSaveStates;
    //! Auto-save
    ItemSwitch* mAutoSave;

    //! Get Ratio List
    SelectorEntry<String>::List GetRatioEntries();
    //! Get Shaders List
    static SelectorEntry<String>::List GetShadersEntries();
    //! Get Softpatching List
    static SelectorEntry<RecalboxConf::SoftPatching>::List GetSoftpatchingEntries();
    //! Get shader sets List
    static SelectorEntry<String>::List GetShaderPresetsEntries();
    //! Get shader sets List
    static SelectorEntry<String>::List GetSuperGameBoyEntries();

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const RecalboxConf::SoftPatching& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};



