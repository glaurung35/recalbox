//
// Created by digitalLumberjack on 20/0/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuCRT.h"
#include "guis/GuiMsgBox.h"
#include "systems/arcade/ArcadeVirtualSystems.h"
#include "views/ViewController.h"
#include <CrtConf.h>
#include <guis/MenuMessages.h>
#include <hardware/crt/CrtAdapterDetector.h>
#include <recalbox/RecalboxSystem.h>
#include <utils/locale/LocaleHelper.h>

MenuCRT::MenuCRT(WindowManager& window, SystemManager& systemManager, const String title)
  : Menu(window, title)
  , mSystemManager(systemManager)
{
}

void MenuCRT::BuildMenuItems()
{
  bool isRGBDual = Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBDual;
  bool isRGBJamma = Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma || Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJammaV2;
  bool is31kHz = Board::Instance().CrtBoard().GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHz31;
  bool supports120Hz = is31kHz && Board::Instance().CrtBoard().Has120HzSupport();
  bool supportsInterlaced = Board::Instance().CrtBoard().HasInterlacedSupport();
  bool multisync = Board::Instance().CrtBoard().MultiSyncEnabled();

  // If we run on Recalbox RGB Dual, we ignore the recalbox.conf configuration
  mOriginalDac = isRGBDual ? CrtAdapterType::RGBDual : CrtConf::Instance().GetSystemCRT();
  mOriginalResolution = Board::Instance().CrtBoard().GetHorizontalFrequency();

  // Selected Dac
  mDac = AddList<CrtAdapterType>(_("CRT ADAPTER"), (int)Components::CRTDac, this, GetDacEntries(isRGBDual),
                                 isRGBDual ? CrtAdapterType::RGBDual : CrtConf::Instance().GetSystemCRT(),
                                 CrtAdapterType::None, _(MENUMESSAGE_ADVANCED_CRT_DAC_HELP_MSG));

  // Resolution
  mOriginalEsResolution = is31kHz ? CrtConf::Instance().GetSystemCRT31kHzResolution() : CrtConf::Instance().GetSystemCRTResolution();
  String selectedResolution;
  mEsResolution = AddList<String>(_("MENU RESOLUTION"), (int)Components::EsResolution, this, GetEsResolutionEntries(is31kHz && !multisync, supports120Hz, multisync, supportsInterlaced, selectedResolution),
                                  selectedResolution, selectedResolution, _(MENUMESSAGE_ADVANCED_CRT_ES_RESOLUTION_HELP_MSG));

  // Horizontal output frequency
  if(isRGBJamma)
  {
    mScreenTypeList = AddList<ICrtInterface::HorizontalFrequency>(
      _("SCREEN TYPE"), (int)Components::JammaScreenType, this,
      SelectorEntry<ICrtInterface::HorizontalFrequency>::List(
        {
          {"15kHz", ICrtInterface::HorizontalFrequency::KHz15 },
          {"31kHz", ICrtInterface::HorizontalFrequency::KHz31 },
          {"MultiSync", ICrtInterface::HorizontalFrequency::KHzMulti },
        }), Board::Instance().CrtBoard().GetHorizontalFrequency(), ICrtInterface::HorizontalFrequency::KHz15,
      _(MENUMESSAGE_ADVANCED_CRT_JAMMA_SCREEN_TYPE));
  }
  else
  {
    if (Board::Instance().CrtBoard().Has31KhzSupport()) AddText(_("SCREEN TYPE"), GetHorizontalFrequency());
  }

  // Force 50HZ
  if (Board::Instance().CrtBoard().HasForced50hzSupport()) AddText(_("FORCE 50HZ"), Get50hz());

  // Force HDMI
  mOriginalForceHDMI = CrtConf::Instance().GetSystemCRTForceHDMI();
  mForceHDMI = mOriginalForceHDMI;
  if(isRGBDual && Board::Instance().GetBoardType() != BoardType::Pi5)
    AddSwitch(_("PRIORITY TO HDMI"), mOriginalForceHDMI, (int)Components::ForceHDMI, this, _(MENUMESSAGE_ADVANCED_CRT_FORCE_HDMI_HELP_MSG));

  // Game Region selection
  if(!isRGBJamma)
    AddSwitch(_("SELECT GAME REFRESH RATE AT LAUNCH"), CrtConf::Instance().GetSystemCRTGameRegionSelect(), (int)Components::GameRegion, this, _(MENUMESSAGE_ADVANCED_CRT_GAME_REGION_HELP_MSG));

  // Game Resolution selection
  AddSwitch(_("SELECT GAME RESOLUTION AT LAUNCH"), CrtConf::Instance().GetSystemCRTGameResolutionSelect(), (int)Components::GameResolution, this, _(MENUMESSAGE_ADVANCED_CRT_GAME_RESOLUTION_HELP_MSG));

  if(is31kHz)
  {
    // Demo Game Resolution on 31khz
    if (supports120Hz)
      AddSwitch(_("RUN DEMOS IN 240P@120"), CrtConf::Instance().GetSystemCRTRunDemoIn240pOn31kHz(),
                (int) Components::DemoIn240pOn31kHz, this,
                _(MENUMESSAGE_ADVANCED_CRT_DEMO_RESOLUTION_ON_31KHZ_HELP_MSG));
  }
  if(multisync || is31kHz)
  {
    // Scanlines on 31kHz resolution
    AddList<CrtScanlines>(_("SCANLINES FOR 240P GAMES IN 480"), (int)Components::ScanlinesOn31kHz, this, GetScanlinesEntries(), CrtConf::Instance().GetSystemCRTScanlines31kHz(), CrtScanlines::None, _(MENUMESSAGE_ADVANCED_CRT_SCANLINES_ON_31KHZ_HELP_MSG));
  }

  // Zero Lag
  AddSwitch(_("REDUCED LATENCY (EXPERIMENTAL)"), RecalboxConf::Instance().GetGlobalReduceLatency(), (int)Components::ReduceLatency, this, _(MENUMESSAGE_ADVANCED_CRT_RUN_AHEAD_HELP_MSG));
  AddSwitch(_("RUN AHEAD (EXPERIMENTAL)"), RecalboxConf::Instance().GetGlobalRunAhead(), (int)Components::RunAhead, this, _(MENUMESSAGE_ADVANCED_CRT_RUN_AHEAD_HELP_MSG));

  /*#if false//defined(BETA) || defined(DEBUG)
    // ConfiggenV2
    AddSwitch(_("USE V2 (BETA)"), CrtConf::Instance().GetSystemCRTUseV2(), (int)Components::UseV2, this, _(MENUMESSAGE_ADVANCED_CRT_V2));

    // Extended range
    AddSwitch(_("V2 - 15KHZ EXTENDED RANGE"), CrtConf::Instance().GetSystemCRTExtended15KhzRange(), (int)Components::Extended15kHzRange, this,  _(MENUMESSAGE_ADVANCED_CRT_EXTENDED));

    // Superrez multiplier
    AddList<String>(_("V2 - SUPERREZ MULTIPLIER"), (int)Components::SuperRez, this, GetSuperRezEntries(), CrtConf::Instance().GetSystemCRTSuperrez(), "x6", _(MENUMESSAGE_ADVANCED_CRT_SUPERREZ));
  #endif*/

  // Force Jack
  mOriginalForceJack = CrtConf::Instance().GetSystemCRTForceJack();
  mForceJack = mOriginalForceJack;
  if(isRGBDual)
    AddSwitch(_("FORCE SOUND ON JACK"), mOriginalForceJack, (int)Components::ForceJack, this, _(MENUMESSAGE_ADVANCED_CRT_FORCE_JACK_HELP_MSG));

  // If we run on Recalbox RGB Dual, we ignore the recalbox.conf configuration
  if(isRGBJamma)
  {
    AddList<String>(_("SOUND"), (int)Components::JammaSoundOutput, this,
                    SelectorEntry<String>::List(
                      {
                        { "JACK/MONO", "0" },
                        { "JACK/PINS", "1" }
                      }), CrtConf::Instance().GetSystemCRTJammaAmpDisable() ? "1" : "0", "0",
                    _(MENUMESSAGE_ADVANCED_CRT_JAMMA_SOUND_OUTPUT));

    AddList<String>(_("MONO AMP BOOST"), (int)Components::JammaMonoBoost, this,
                    SelectorEntry<String>::List(
                      {
                        { "default", "0" },
                        { "+6dB"   , "1" },
                        { "+12dB"  , "2" },
                        { "+16dB"  , "3" }
                      }), CrtConf::Instance().GetSystemCRTJammaMonoAmpBoost(), "0",
                    _(MENUMESSAGE_ADVANCED_CRT_JAMMA_MONO_AMP_BOOST));

    AddList<String>(_("PANEL TYPE"), (int)Components::JammaPanelButtons, this,
                    SelectorEntry<String>::List(
                      {
                        { "2 buttons", "2" },
                        { "3 buttons", "3" },
                        { "4 buttons", "4" },
                        { "5 buttons", "5" },
                        { "6 buttons", "6" },
                      }), CrtConf::Instance().GetSystemCRTJammaPanelButtons(), "2",
                    _(MENUMESSAGE_ADVANCED_CRT_JAMMA_PANEL_HELP_MSG));

    AddList<String>(_("NEOGEO/PGM LAYOUT P1"), (int)Components::JammaNeogeoLayoutP1, this,
                    SelectorEntry<String>::List(
                      {
                        { "Default", "neodefault" },
                        { "Line", "line" },
                        { "Square", "square" }
                      }), CrtConf::Instance().GetSystemCRTJammaNeogeoLayoutP1(), "neodefault",
                    _(MENUMESSAGE_ADVANCED_CRT_JAMMA_NEOGEO_LAYOUT));

    AddList<String>(_("NEOGEO/PGM LAYOUT P2"), (int)Components::JammaNeogeoLayoutP2, this,
                    SelectorEntry<String>::List(
                      {
                        { "Default", "neodefault" },
                        { "Line", "line" },
                        { "Square", "square" }
                      }), CrtConf::Instance().GetSystemCRTJammaNeogeoLayoutP2(), "neodefault",
                    _(MENUMESSAGE_ADVANCED_CRT_JAMMA_NEOGEO_LAYOUT));
    AddSwitch(_("4 PLAYERS MODE"), CrtConf::Instance().GetSystemCRTJamma4Players(),
              (int)Components::Jamma4Players, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_4PLAYERS));
    /*AddSlider(_("DEBOUNCE TIME (MS)"), 10, 60, 5, CrtConf::Instance().GetSystemCRTJammaDebounceTime(),"ms",
              (int)Components::JammaDebounceTime, this);*/
    AddSwitch(_("START+BTN1 = CREDIT"), CrtConf::Instance().GetSystemCRTJammaStartBtn1Credit(),
              (int)Components::JammaStartBtn1Credit, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_CREDIT));
    AddSwitch(_("START+BTN = HK+BTN"), CrtConf::Instance().GetSystemCRTJammaHKOnStart(),
              (int)Components::JammaHKOnStart, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_HK));
    AddSwitch(_("START+UP/DOWN = VOLUME"), CrtConf::Instance().GetSystemCRTJammaSoundOnStart(),
              (int)Components::JammaSoundOnStart, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_HK));
    AddSwitch(_("START 3SEC = EXIT"), CrtConf::Instance().GetSystemCRTJammaExitOnStart(),
              (int)Components::JammaExitOnStart, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_EXIT));
    AddSwitch(_("START+BTN 5SEC = AUTO FIRE"), CrtConf::Instance().GetSystemCRTJammaAutoFire(),
              (int)Components::JammaAutoFire, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_AUTOFIRE));
    AddSwitch(_("DUAL JOYSTICKS"), CrtConf::Instance().GetSystemCRTJammaDualJoysticks(),
              (int)Components::JammaDualJoysticks, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_DUAL_JOYSTICKS));
    AddSwitch(_("PIN E/27 AS GND"), CrtConf::Instance().GetSystemCRTJammaButtonsOnJamma() != "6",
              (int)Components::JammaButtonsBtn6Gnd, this,_(MENUMESSAGE_ADVANCED_CRT_JAMMA_BTN6GND));
    AddAction(_("RESET JAMMA CONFIGURATION"), _("RESET"), (int)Components::ResetJamma, true, this, _(MENUMESSAGE_ADVANCED_CRT_JAMMA_HELP_MSG));
  }

  // Screen Adjustments
  AddAction(Renderer::Instance().IsRotatedSide() ? _("SCREEN CALIBRATION (ONLY IN YOKO MODE)") : _("SCREEN CALIBRATION (BETA)"),
            _("CALIBRATE"), (int)Components::Calibration, true, this, _(MENUMESSAGE_ADVANCED_CRT_CALIBRATION));
}

MenuCRT::~MenuCRT()
{
  // Reboot?
  if (mOriginalDac != mDac->SelectedValue() || mOriginalEsResolution != mEsResolution->SelectedValue() ||
      mOriginalForceJack != mForceJack || mOriginalForceHDMI != mForceHDMI || mOriginalResolution != Board::Instance().CrtBoard().GetHorizontalFrequency())
    RequestReboot();
}

String MenuCRT::Get50hz()
{
  String result = Board::Instance().CrtBoard().MustForce50Hz() ? _("ON") : _("OFF");
  result.Append(' ').Append(_("(Hardware managed)"));
  return result;
}

String MenuCRT::GetHorizontalFrequency()
{
  String result = "15khz";
  switch(Board::Instance().CrtBoard().GetHorizontalFrequency())
  {
    case ICrtInterface::HorizontalFrequency::KHz31: result = "31khz"; break;
    case ICrtInterface::HorizontalFrequency::KHzMulti: result = "MultiSync"; break;
    case ICrtInterface::HorizontalFrequency::KHz15:
    default: break;
  }

  return result;
}

SelectorEntry<CrtAdapterType>::List MenuCRT::GetDacEntries(bool onlyRgbDual)
{
  SelectorEntry<CrtAdapterType>::List list;

  if(onlyRgbDual)
  {
    list.push_back( { "Recalbox RGB Dual", CrtAdapterType::RGBDual, true } );
    return list;
  }

  CrtAdapterType selectedDac = CrtConf::Instance().GetSystemCRT();

  static struct
  {
    const char* Name;
    CrtAdapterType Type;
  }
  Adapters[] =
  {
    { "Recalbox RGB Dual", CrtAdapterType::RGBDual },
    { "Recalbox RGB Jamma", CrtAdapterType::RGBJamma },
    { "Recalbox RGB Jamma Proto" , CrtAdapterType::RGBJammaV2 },
    { "VGA666", CrtAdapterType::Vga666 },
    { "RGBPi", CrtAdapterType::RGBPi },
    { "Pi2SCART", CrtAdapterType::Pi2Scart },
  };

  // Always push none
  list.push_back( { _("NONE"), CrtAdapterType::None, selectedDac == CrtAdapterType::None } );
  // Push all adapters or only one if it is automatically detected
  const ICrtInterface& crt = Board::Instance().CrtBoard();
  for(const auto& item : Adapters)
  {
    if (!crt.HasBeenAutomaticallyDetected() || crt.GetCrtAdapter() == item.Type)
      list.push_back( { item.Name, item.Type, selectedDac == item.Type } );
  }

  return list;
}

SelectorEntry<String>::List MenuCRT::GetEsResolutionEntries(bool only31kHz, bool supports120Hz, bool multisync, bool interlaced, String& selectedValue)
{
  SelectorEntry<String>::List list;

  if(only31kHz)
  {
    bool is480 = CrtConf::Instance().GetSystemCRT31kHzResolution() == "480";
    list.push_back({ "480p", "480", is480 });
    if(supports120Hz)
      list.push_back({ "240p@120Hz", "240", !is480 });
    selectedValue = is480 ? "480" : "240";
    return list;
  } else {
    bool rdef = CrtConf::Instance().GetSystemCRTResolution() == "240";
    list.push_back({ "240p", "240", rdef });
    selectedValue = rdef ? "240" : "480";
    if(multisync)
    {
      list.push_back({ "480p", "480", !rdef });
    } else if (interlaced) {
      list.push_back({ "480i", "480", !rdef });
    }
  }

  return list;
}

SelectorEntry<String>::List MenuCRT::GetSuperRezEntries()
{
  SelectorEntry<String>::List list;
  list.push_back({ "X6 (DEFAULT)", "x6" });
  list.push_back({ "ORIGINAL", "original" });
  list.push_back({ "X2", "x2" });
  list.push_back({ "x8", "x8" });
  return list;
}

SelectorEntry<CrtScanlines>::List MenuCRT::GetScanlinesEntries()
{
  SelectorEntry<CrtScanlines>::List list;
  list.push_back({ "NONE", CrtScanlines::None });
  list.push_back({ "LIGHT", CrtScanlines::Light });
  list.push_back({ "MEDIUM", CrtScanlines::Medium });
  list.push_back({ "HEAVY", CrtScanlines::Heavy });
  return list;
}

void MenuCRT::MenuSingleChanged(int id, int index, const CrtAdapterType& value)
{
  (void)index;
  if ((Components)id == Components::CRTDac)
  {
    CrtAdapterType oldValue = Board::Instance().CrtBoard().GetCrtAdapter();
    if (value == CrtAdapterType::None)
    {
      if (oldValue != CrtAdapterType::None)
      {
        CrtConf::Instance().SetSystemCRT(CrtAdapterType::None).Save();
        RecalboxConf::Instance().SetEmulationstationVideoMode("default").Save();
        mEsResolution->SetSelectedItemValue("default", false);
      }
    }
    CrtConf::Instance().SetSystemCRT(value).Save();
  }
}

void MenuCRT::MenuSingleChanged(int id, int index, const CrtScanlines& value)
{
  (void)index;
  if ((Components)id == Components::ScanlinesOn31kHz)
  {
    CrtConf::Instance().SetSystemCRTScanlines31kHz(value).Save();
  }
}

void MenuCRT::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  if ((Components)id == Components::EsResolution)
  {
    if(Board::Instance().CrtBoard().MultiSyncEnabled()){
      CrtConf::Instance().SetSystemCRTResolution(value).Save();
    }
    else if(Board::Instance().CrtBoard().GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHz31)
    {
      CrtConf::Instance().SetSystemCRT31kHzResolution(value).Save();
    } else {
      CrtConf::Instance().SetSystemCRTResolution(value).Save();
    }
  }
  else if ((Components)id == Components::SuperRez)
  {
    CrtConf::Instance().SetSystemCRTSuperrez(value).Save();
  }
  else if ((Components)id == Components::JammaPanelButtons)
  {
      CrtConf::Instance().SetSystemCRTJammaPanelButtons(value).Save();
  }
  else if ((Components)id == Components::JammaNeogeoLayoutP1)
  {
    CrtConf::Instance().SetSystemCRTJammaNeogeoLayoutP1(value).Save();
  }
  else if ((Components)id == Components::JammaNeogeoLayoutP2)
  {
    CrtConf::Instance().SetSystemCRTJammaNeogeoLayoutP2(value).Save();
  }
  else if ((Components)id == Components::JammaMonoBoost)
  {
    CrtConf::Instance().SetSystemCRTJammaMonoAmpBoost(value).Save();
  }
  else if ((Components)id == Components::JammaSoundOutput)
  {
    CrtConf::Instance().SetSystemCRTJammaAmpDisable(value == "1").Save();
  }
}

void MenuCRT::MenuSingleChanged(int id, int index, const ICrtInterface::HorizontalFrequency &value)
{
  (void)index;
  if ((Components)id == Components::JammaScreenType)
  {
    if(value == ICrtInterface::HorizontalFrequency::KHz15 && Board::Instance().CrtBoard().GetHorizontalFrequency() != ICrtInterface::HorizontalFrequency::KHz15)
    {
      mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("Are you sure you want to switch the display mode to 15kHz?"),
                                    _("CANCEL"), [this] { mScreenTypeList->SetSelectedItemValue(Board::Instance().CrtBoard().GetHorizontalFrequency(), false); },
                                    _("YES"), []{ CrtConf::Instance().SetSystemCRTScreen31kHz(false);
                                                      CrtConf::Instance().SetSystemCRTScreenMultiSync(false).Save(); }));
    }
    else if(value == ICrtInterface::HorizontalFrequency::KHz31  && Board::Instance().CrtBoard().GetHorizontalFrequency() != ICrtInterface::HorizontalFrequency::KHz31)
    {
      mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("Are you sure you want to switch the display mode to 31kHz? Your display must support the 31kHz (480p) mode."),
                                    _("CANCEL"), [this] { mScreenTypeList->SetSelectedItemValue(Board::Instance().CrtBoard().GetHorizontalFrequency(), false); },
                                    _("YES"), []{ CrtConf::Instance().SetSystemCRTScreen31kHz(true);
                                      CrtConf::Instance().SetSystemCRTScreenMultiSync(false).Save(); }));
    }
    else if(value == ICrtInterface::HorizontalFrequency::KHzMulti && Board::Instance().CrtBoard().GetHorizontalFrequency() != ICrtInterface::HorizontalFrequency::KHzMulti)
    {
      mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("Are you sure you want to switch the display mode to MultiSync? Your chassis must support automatic switching between 15kHz and 31kHz modes."),
                                    _("CANCEL"), [this] { mScreenTypeList->SetSelectedItemValue(Board::Instance().CrtBoard().GetHorizontalFrequency(), false); },
                                    _("YES"), []{ CrtConf::Instance().SetSystemCRTScreen31kHz(false);
                                      CrtConf::Instance().SetSystemCRTScreenMultiSync(true).Save(); }));
    }
  }
}

void MenuCRT::MenuSwitchChanged(int id, bool& status)
{
  if ((Components)id == Components::GameRegion)
    CrtConf::Instance().SetSystemCRTGameRegionSelect(status).Save();
  if ((Components)id == Components::GameResolution)
    CrtConf::Instance().SetSystemCRTGameResolutionSelect(status).Save();
  if ((Components)id == Components::DemoIn240pOn31kHz)
    CrtConf::Instance().SetSystemCRTRunDemoIn240pOn31kHz(status).Save();
  if ((Components)id == Components::ReduceLatency)
    RecalboxConf::Instance().SetGlobalReduceLatency(status).Save();
  if ((Components)id == Components::RunAhead)
    RecalboxConf::Instance().SetGlobalRunAhead(status).Save();
  if ((Components)id == Components::UseV2)
    CrtConf::Instance().SetSystemCRTUseV2(status).Save();
  if ((Components)id == Components::Extended15kHzRange)
    CrtConf::Instance().SetSystemCRTExtended15KhzRange(status).Save();
  if ((Components)id == Components::JammaExitOnStart)
    CrtConf::Instance().SetSystemCRTJammaExitOnStart(status).Save();
  if ((Components)id == Components::JammaHKOnStart)
    CrtConf::Instance().SetSystemCRTJammaHKOnStart(status).Save();
  if ((Components)id == Components::JammaStartBtn1Credit)
    CrtConf::Instance().SetSystemCRTJammaStartBtn1Credit(status).Save();
  if ((Components)id == Components::Jamma4Players)
  {
    CrtConf::Instance().SetSystemCRTJamma4Players(status).Save();
    auto setStatus = [this, status]{
      RecalboxConf::Instance().SetShowOnly3PlusPlayers(status);
      if (mSystemManager.UpdatedTopLevelFilter()) RecalboxConf::Instance().Save();
      else RecalboxConf::Instance().SetShowOnly3PlusPlayers(!status);
    };
    if(status && !RecalboxConf::Instance().GetShowOnly3PlusPlayers())
    {
      mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("Do you want to enable the 3+ player filter for all the games ?"),
                                    _("NO"), [] {},
                                    _("YES"), setStatus));
    }
    if(!status && RecalboxConf::Instance().GetShowOnly3PlusPlayers())
    {
      mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("Do you want to disable the 3+ player filter for all the games ?"),
                                    _("NO"), [] {},
                                    _("YES"), setStatus));
    }
  }
  if ((Components)id == Components::JammaAutoFire)
    CrtConf::Instance().SetSystemCRTJammaAutoFire(status).Save();
  if ((Components)id == Components::JammaButtonsBtn6Gnd)
    CrtConf::Instance().SetSystemCRTJammaButtonsOnJamma(status ? "5" : "6").Save();
  if ((Components)id == Components::JammaDualJoysticks)
    CrtConf::Instance().SetSystemCRTJammaDualJoysticks(status).Save();
  if ((Components)id == Components::JammaSoundOnStart)
    CrtConf::Instance().SetSystemCRTJammaSoundOnStart(status).Save();
  if ((Components)id == Components::ForceJack)
  {
    mForceJack = status;
    CrtConf::Instance().SetSystemCRTForceJack(status).Save();
  }
  if ((Components)id == Components::ForceHDMI)
  {
    mForceHDMI = status;
    CrtConf::Instance().SetSystemCRTForceHDMI(status).Save();
  }
}

void MenuCRT::MenuSliderMoved(int id, float value)
{
  if ((Components)id == Components::JammaDebounceTime)
  {
    CrtConf::Instance().SetSystemCRTJammaDebounceTime(value).Save();
  }
}

void MenuCRT::MenuActionTriggered(int id)
{
  if ((Components)id == Components::Calibration)
  {
    if (Renderer::Instance().IsRotatedSide())
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("Screen calibration only available in YOKO mode."), _("Ok"), [] {}));
      return;
    }
    if (Board::Instance().CrtBoard().GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHz31)
    {
      if (Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma)
      {
        ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz31_no_120);
        mWindow.CloseAll();
      }
      else
      {
        ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz31);
        mWindow.CloseAll();
      }
    }
    else if (Board::Instance().CrtBoard().MustForce50Hz())
    {
      ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_50Hz);
      mWindow.CloseAll();
    }
    else if (Board::Instance().CrtBoard().MultiSyncEnabled())
    {
      ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_60Hz_plus_kHz31);
      mWindow.CloseAll();
    }
    else if (Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma)
    {
      ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_60Hz);
      mWindow.CloseAll();
    }
    else
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("You will now calibrate different resolutions for your TV. Select the refresh rate according to what your TV supports.\nDuring the calibration, press B to validate, and A to cancel."),
                                    _("60Hz & 50Hz"), [this] {ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_60plus50Hz);
          mWindow.CloseAll(); },
                                    _("60Hz Only"), [this] {ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_60Hz);
          mWindow.CloseAll(); },
                                    _("50Hz Only"), [this] {ViewController::Instance().goToCrtView(CrtCalibrationView::CalibrationType::kHz15_50Hz);
          mWindow.CloseAll();},
                                    TextAlignment::Center));
    }
  }
  else if ((Components)id == Components::ResetJamma)
  {
    mWindow.pushGui(new GuiMsgBox(mWindow, _("Are you sure you want to reset JAMMA configuration?"),
                                  _("YES"), [this] {
        // recalbox.conf
        RecalboxConf::Instance().ResetWithFallback();
        // Set jamma config to default
        RecalboxConf::Instance().SetGlobalRewind(false);
        RecalboxConf::Instance().SetGlobalSmooth(false);
        RecalboxConf::Instance().SetQuickSystemSelect(false);
        RecalboxConf::Instance().SetAutoPairOnBoot(false);
        RecalboxConf::Instance().SetThemeFolder("recalbox-240p");
        RecalboxConf::Instance().SetThemeIconSet("recalbox-240p", "4-jamma");
        RecalboxConf::Instance().SetThemeGamelistView("recalbox-240p", "3-240p-large-names");
        RecalboxConf::Instance().SetGlobalHidePreinstalled(true);
        RecalboxConf::Instance().SetAutoPairOnBoot(false);
        RecalboxConf::Instance().SetShowGameClipClippingItem(false);
        RecalboxConf::Instance().SetShowGameClipHelpItems(false);
        RecalboxConf::Instance().SetGlobalDemoInfoScreen(0);

        std::vector<String> manufacturers;
        for(const String& rawIdentifier : ArcadeVirtualSystems::GetVirtualArcadeSystemList())
        {
          String identifier(SystemManager::sArcadeManufacturerPrefix);
          identifier.Append(rawIdentifier).Replace('\\', '-');
          manufacturers.push_back(identifier);
        }
        RecalboxConf::Instance().SetCollectionArcadeManufacturers(manufacturers);
        RecalboxConf::Instance().SetGlobalHidePreinstalled(true);
        RecalboxConf::Instance().SetKodiEnabled(false);
        RecalboxConf::Instance().SetSplashEnabled(false);
        RecalboxConf::Instance().Save();

        // recalbox-crt-options.cfg
        CrtConf::Instance().ResetWithFallback();
        CrtConf::Instance().SetSystemCRT(CrtAdapterType::RGBJamma);
        CrtConf::Instance().Save();
        // REBOOT
        RequestReboot();
      },
      _("NO"), [] { }));
  }
}
