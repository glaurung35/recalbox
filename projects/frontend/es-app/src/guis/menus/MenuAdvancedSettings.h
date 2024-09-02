//
// Created by bkg2k on 11/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "ResolutionAdapter.h"
#include "hardware/Case.h"

// Forward declaration
class SystemManager;
class SystemData;

//! O/C file & description
struct Overclocking
{
  String File;
  String Description;
  bool Hazardous;
  int Frequency;

  bool operator == (const Overclocking& r) const
  {
    return (File == r.File) &&
           (Description == r.Description) &&
           (Hazardous == r.Hazardous) &&
           (Frequency == r.Frequency);
  }
};

class MenuAdvancedSettings : public Menu
                           , private ISingleSelectorChanged<Overclocking>
                           , private ISwitchChanged
                           , private ISubMenuSelected
                           , private ISingleSelectorChanged<String>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuAdvancedSettings(WindowManager& window, SystemManager& systemManager);

    //! Destructor
    ~MenuAdvancedSettings() override;

  private:
    enum class Components
    {
      DebugLogs,
      OverclockList,
      BootSubMenu,
      VirtualSubMenu,
      ResolutionSubMenu,
      AdvancedSubMenu,
      KodiSubMenu,
      PinballSubMenu,
      Cases,
      SecuritySubMenu,
      Overscan,
      ShowFPS,
      CrtSubMenu,
      Manager,
      FactoryReset,
      EepromUpdate
    };

    static constexpr const char* sOverclockBaseFolder = "/recalbox/system/configs/overclocking";
    static constexpr const char* sOverclockFile = "/boot/recalbox-oc-config.txt";

    //! Overclocking list
    typedef std::vector<Overclocking> OverclockList;

    //! System Manager
    SystemManager& mSystemManager;

    // Resolution Adapter
    ResolutionAdapter mResolutionAdapter;

    //! Default overclock value
    Overclocking mDefaultOverclock;
    //! Original overclock value
    String mOriginalOverclock;
    //! Last overclock hazardous?
    bool mLastHazardous;
    //! Is there at least a valid O/C?
    bool mValidOverclock;

    //! Overclock
    ItemSelector<Overclocking>* mOverclock;

    //! Get O/C list for the current board
    static OverclockList AvailableOverclocks();

    //! Get O/C List
    SelectorEntry<Overclocking>::List GetOverclockEntries();

    //! Get current O/C
    Overclocking GetCurrentOC();

    //! Reset overclock
    void ResetOverclock();

    //! Get supported cases List
    static SelectorEntry<String>::List GetCasesEntries();

    //! Reset Factory requested Level 1
    void ResetFactory();

    //! Reset Factory requested Level 2
    static void ResetFactoryReally(WindowManager* This);

    //! Do Reset Factory
    static void DoResetFactory();

    /*
     * IOptionListComponent<Overclocking> implementation
     */

    void MenuSingleChanged(int id, int index, const Overclocking& value) override;

    //! Case component management
    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    void EepromUpdate();

    String ExtractVersion(String cmdResult, String updateType);
};



