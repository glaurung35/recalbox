//
// Created by bkg2k on 11/08/24.
//
#pragma once

#include <components/MenuComponent.h>
#include <guis/menus/GuiMenuBase.h>
#include <components/SwitchComponent.h>
#include <components/SliderComponent.h>
#include <systems/SystemManager.h>

class GuiMenuSystemCategories : public GuiMenuBase
                              , private ISwitchComponent
                              , private IOptionListComponent<SystemDescriptor::SystemCategory>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    GuiMenuSystemCategories(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
      Enabled,
      Category,
      QuickChange,
    };

    //! System Manager
    SystemManager& mSystemManager;

    //! Get Sorting List
    std::vector<ListEntry<SystemDescriptor::SystemCategory>> GetCategoryEntries();

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool& status) override;

    /*
     * IOptionListComponent<SystemSorting> implementation
     */

    void OptionListComponentChanged(int id, int index, const SystemDescriptor::SystemCategory& value, bool quickChange) override;
};

