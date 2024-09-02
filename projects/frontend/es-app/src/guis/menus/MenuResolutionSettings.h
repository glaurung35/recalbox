//
// Created by bkg2k on 31/01/2022.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "ResolutionAdapter.h"

class SystemManager;

class MenuResolutionSettings : public Menu
                                , private ISingleSelectorChanged<String>
                                , private ISubMenuSelected
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuResolutionSettings(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      GlobalResolution,
      FrontendResolution,
      Emulators,
    };

    //! System manager reference
    SystemManager& mSystemManager;

    // Resolution Adapter
    ResolutionAdapter mResolutionAdapter;

    SelectorEntry<String>::List GetGlobalResolutionEntries();
    SelectorEntry<String>::List GetFrontEndResolutionEntries();

    /*
     * ISingleSelectorChanged<Overclocking> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};



