//
// Created by digitalLumberjack on 20/0/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "hardware/crt/CRTScanlines.h"

// Forward declaration
class SystemManager;

class MenuCRT : public Menu
              , private ISingleSelectorChanged<CrtAdapterType>
              , private ISingleSelectorChanged<String>
              , private ISingleSelectorChanged<CrtScanlines>
              , private ISingleSelectorChanged<ICrtInterface::HorizontalFrequency>
              , private ISliderChanged
              , private ISwitchChanged
              , private IActionTriggered
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuCRT(WindowManager& window, SystemManager& systemManager, const String title);

    //! Default destructor
    ~MenuCRT() override;

  private:
    enum class Components
    {
      CRTDac,
      EsResolution,
      GameRegion,
      GameResolution,
      DemoIn240pOn31kHz,
      ScanlinesOn31kHz,
      Extended15kHzRange,
      UseV2,
      SuperRez,
      ReduceLatency,
      RunAhead,
      ForceJack,
      Calibration,
      HorizontalPalOffset,
      VerticalPalOffset,
      ForceHDMI,
      JammaPanelButtons,
      JammaNeogeoLayoutP1,
      JammaNeogeoLayoutP2,
      JammaStartBtn1Credit,
      JammaHKOnStart,
      JammaExitOnStart,
      JammaMonoBoost,
      JammaScreenType,
      Jamma4Players,
      JammaAutoFire,
      JammaButtonsBtn6Gnd,
      JammaDualJoysticks,
      ResetJamma,
      JammaSoundOutput,
      JammaSoundOnStart,
      JammaDebounceTime
    };

    //! System manager reference
    SystemManager& mSystemManager;
    //! Dac selection
    ItemSelector<CrtAdapterType>* mDac;
    CrtAdapterType mOriginalDac;
    //! Es resolution
    ItemSelector<String>* mEsResolution;
    String mOriginalEsResolution;
    //! Force jack audio
    bool mForceJack;
    bool mOriginalForceJack;
    //! Force HDMI video
    bool mForceHDMI;
    bool mOriginalForceHDMI;
    bool mOriginalFrontendIn240pOn31kHz;
    std::string mOriginalJammaNeogeoLayout;
    ICrtInterface::HorizontalFrequency mOriginalScreenType;
    ItemSelector<ICrtInterface::HorizontalFrequency>* mScreenTypeList;

    //! Get dacs
    static SelectorEntry<CrtAdapterType>::List GetDacEntries(bool onlyRgbDual);
    //! Get resolutions
    static SelectorEntry<String>::List GetEsResolutionEntries();
    //! Get super resolutions
    static SelectorEntry<String>::List GetSuperRezEntries();
    //! Get scanlines
    static SelectorEntry<CrtScanlines>::List GetScanlinesEntries();


    /*!
     * @brief Get 50hz display test
     * @return Texg
     */
    static String Get50hz();

    /*
     * IOptionListComponent<CrtAdapterType> implementation
     */

    void MenuSingleChanged(int id, int index, const CrtAdapterType& value) final;

    /*
     * IOptionListComponent<std::string> implementation
     */

    void MenuSingleChanged(int id, int index, const String & value) final;

    /*
     * IOptionListComponent<CrtScanlines> implementation
     */
    void MenuSingleChanged(int id, int index, const CrtScanlines& value) final;

    /*
     * IOptionListComponent<ICrtInterface::HorizontalFrequency> implementation
     */
    void MenuSingleChanged(int id, int index, const ICrtInterface::HorizontalFrequency& value) final;

    /*
     * ISwitchChanged implementation
     */
    void MenuSwitchChanged(int id, bool& status) final;

    /*
     * ISliderChanged implementation
     */
    void MenuSliderMoved(int id, float value) final;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) final;
};
