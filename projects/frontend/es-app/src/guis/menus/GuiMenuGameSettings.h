//
// Created by bkg2k on 06/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/GuiMenuBase.h>

// Forward declaration
class SystemManager;
template<class T> class OptionListComponent;
class SwitchComponent;

class GuiMenuGameSettings : public GuiMenuBase
                          , private IOptionListComponent<std::string>
                          , private ISwitchComponent
                          , private IGuiMenuBase
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuGameSettings(WindowManager& window, SystemManager& systemManager);

    ~GuiMenuGameSettings() override = default;

  private:
    enum class Components
    {
      Ratio,
      RecalboxOverlays,
      Smooth,
      Rewind,
      AutoSave,
      QuitTwice,
      IntegerScale,
      Shaders,
      Softpatching,
      ShaderSet,
      RetroAchivements,
      Netplay,
      SuperGameBoy
    };

    //! System manager
    SystemManager& mSystemManager;

    //! Smooth
    std::shared_ptr<SwitchComponent> mSmooth;

    //! Get Ratio List
    static std::vector<ListEntry<std::string>> GetRatioEntries();
    //! Get Shaders List
    static std::vector<ListEntry<std::string>> GetShadersEntries();
    //! Get Softpatching List
    static std::vector<ListEntry<std::string>> GetSoftpatchingEntries();
    //! Get shader sets List
    static std::vector<ListEntry<std::string>> GetShaderPresetsEntries();
    //! Get shader sets List
    static std::vector<ListEntry<std::string>> GetSuperGameBoyEntries();

    /*
     * IGuiMenuBase implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * IOptionListComponent<std::string> implementation
     */

    void OptionListComponentChanged(int id, int index, const std::string& value) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool status) override;
};



