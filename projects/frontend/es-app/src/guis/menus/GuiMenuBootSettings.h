//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/GuiMenuBase.h>

// Forward declaration
class SystemManager;
class SwitchComponent;
template<class T> class OptionListComponent;

class GuiMenuBootSettings : public GuiMenuBase
                          , private IOptionListComponent<std::string>
                          , private ISwitchComponent
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuBootSettings(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      KodiAtStartup,
      GamelistOnly,
      SelectedSystem,
      StartOnGamelist,
      HideSystemView,
    };

    //! System manager
    SystemManager& mSystemManager;

    //! Get system list
    std::vector<ListEntry<std::string>> GetSystemEntries();

    /*
     * IOptionListComponent<Overclocking> implementation
     */

    void OptionListComponentChanged(int id, int index, const std::string& value) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool status) override;
};




