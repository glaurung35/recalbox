//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "ResolutionAdapter.h"

// Forward declaration
class SystemManager;
class SwitchComponent;
template<class T> class OptionListComponent;

class MenuKodiSettings : public Menu
                       , private ISingleSelectorChanged<String>
                       , private ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuKodiSettings(WindowManager& window);

  private:
    enum class Components
    {
      Enabled,
      RunAtStartup,
      ButtonX,
      KodiResolution
    };

    //! Resolution Adapter
    ResolutionAdapter mResolutionAdapter;

    SelectorEntry<String>::List GetKodiResolutionsEntries();

    /*
     * ISwitchComponent implementation
     */
    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * ISingleSelectorChanged implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;
};



