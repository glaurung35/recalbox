//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/GuiMenuBase.h>
#include "ResolutionAdapter.h"

// Forward declaration
class SystemManager;
class SwitchComponent;
template<class T> class OptionListComponent;

class GuiMenuPinballSettings : public GuiMenuBase
                          , private IOptionListComponent<String>
                          , private ISwitchComponent
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuPinballSettings(WindowManager& window);

  private:
    enum class Components
    {
      Cabinet,
      Trail,
      BackglassScreen,
      BackglassResolution,
      Profile,
    };

    //! Enable Cabinet mode
    std::shared_ptr<SwitchComponent> mPinballCabinet;
    //! Enable ball trail
    std::shared_ptr<SwitchComponent> mPinballTrail;

    /*
     * ISwitchComponent implementation
     */
    void SwitchComponentChanged(int id, bool& status) override;
    void OptionListComponentChanged(int id, int index, const String& value, bool quickChange) override;
    std::vector<ListEntry<String>> GetPinballBackglassResolutionEntries();
    std::vector<ListEntry<String>> GetPinballBackglassScreenEntries();
    std::vector<ListEntry<String>> GetPinballProfileEntries();
};



