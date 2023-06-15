#pragma once

#include <guis/menus/GuiMenuBase.h>
#include <systems/SystemData.h>
#include <components/MenuComponent.h>

template<typename T>
class OptionListComponent;
class SwitchComponent;

class GuiMenuArcadeVirtualSystem : public GuiMenuBase
                                 , private IOptionListComponent<int>
                                 , private IOptionListMultiComponent<String>
                                 , private ISwitchComponent
{
  public:
    //! Constructor
    GuiMenuArcadeVirtualSystem(WindowManager& window, SystemManager& systemManager);

    //! Destructor
    ~GuiMenuArcadeVirtualSystem() override;

  private:
    enum class Components
    {
      ArcadeManufacturerList,
      ArcadeOnOff,
      IncludeNeogeo,
      HideOriginals,
      Position,
    };

    //! SystemManager instance
    SystemManager& mSystemManager;

    //! Original manufacturer list
    String mOriginalManufacturerList;
    //! Original Arcade ON/OFF value
    bool mOriginalArcadeOnOff;
    //! Original Include neogeo value
    bool mOriginalIncludeNeogeo;
    //! Original Hide Original value
    bool mOriginalHideOriginals;
    //! Original position
    int  mOriginalPosition;


    //! Get position entries
    std::vector<GuiMenuBase::ListEntry<int>> GetPositionEntries();
    //! Get manufacturer/system entries
    static std::vector<GuiMenuBase::ListEntry<String>> GetManufacturersEntries();

    /*
     * IOptionListComponent<int> implementation
     */

    void OptionListComponentChanged(int id, int index, const int& value) override;

    /*
     * IOptionListComponent<String> implementation
     */

    void OptionListMultiComponentChanged(int id, const String::List& value) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool status) override;

};
