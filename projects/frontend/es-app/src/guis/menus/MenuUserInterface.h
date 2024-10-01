//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <systems/SystemManager.h>
#include "guis/menus/base/Menu.h"

class MenuUserInterface : public Menu
                           , private ISubMenuSelected
                           , private ISliderChanged
                           , private ISwitchChanged
                           , private ISingleSelectorChanged<SystemSorting>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuUserInterface(WindowManager& window, SystemManager& systemManager, const IGlobalVariableResolver& resolver);

    //! Build menu items
    void BuildMenuItems() final;

    /*!
     * @brief Called once per frame. Override to implement your own drawings.
     * Call your base::Update() to ensure animation and childrens are updated properly
     * @param deltaTime Elapsed time from the previous frame, in millisecond
     */
    void Update(int deltaTime) override;

  private:
    enum class Components
    {
      Brightness,
      ScreenSaver,
      Clock,
      SwapValidateAndCancel,
      Help,
      Popups,
      QuickSelect,
      Theme,
      ThemeConfig,
      SystemSort,
      UpdateGamelist,
      Filters,
      DisplayByFileName,
      HideSystems,
    };

    //! Global variable resolver
    const IGlobalVariableResolver& mResolver;
    //! System Manager
    SystemManager& mSystemManager;

    //! Brightness component index, or -1 if not created
    ItemSlider* mBrightness;
    //! Original Sorting type
    SystemSorting mOriginalSort;

    //! Get Sorting List
    std::vector<SelectorEntry<SystemSorting>> GetSortingEntries();

    /*!
     * @brief Reload gamelists
     */
    void ReloadGamelists();

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * ISliderChanged implementation
     */

    void MenuSliderMoved(int id, float value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * ISingleSelectorChanged<SystemSorting> implementation
     */

    void MenuSingleChanged(int id, int index, const SystemSorting& value) override;
};

