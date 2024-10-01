//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuBootSettings : public Menu
                          , private ISingleSelectorChanged<String>
                          , private ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuBootSettings(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      KodiAtStartup,
      GamelistOnly,
      SelectedSystem,
      StartOnGamelist,
      HideSystemView,
      SplashEnabled,
      BootOnGame,
    };

    //! System manager
    SystemManager& mSystemManager;

    //! Get system list
    SelectorEntry<String>::List GetSystemEntries();

    /*
     * IOptionListComponent<Overclocking> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};




