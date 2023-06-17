#pragma once

#include <systems/SystemManager.h>
#include <guis/menus/GuiMenuBase.h>

template<typename T>
class OptionListComponent;
class SwitchComponent;

class GuiMenuArcadeAllInOneSystem : public GuiMenuBase
                                  , private IOptionListComponent<int>
                                  , private ISwitchComponent
{
  public:
    //! Constructor
    explicit GuiMenuArcadeAllInOneSystem(WindowManager& window, SystemManager& systemManager);

    //! Destructor
    ~GuiMenuArcadeAllInOneSystem() override;

  private:
    enum class Components
    {
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

    /*
     * IOptionListComponent<int> implementation
     */

    void OptionListComponentChanged(int id, int index, const int& value) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool status) override;

};
