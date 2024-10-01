//
// Created by bkg2k on 06/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuGameSettings.h"
#include "MenuNetplay.h"
#include "MenuRetroAchievements.h"
#include "MenuTools.h"
#include "guis/GuiMsgBox.h"
#include "views/MenuFilter.h"
#include <LibretroRatio.h>
#include <guis/MenuMessages.h>
#include <systems/SystemManager.h>
#include <guis/menus/base/ItemSwitch.h>

MenuGameSettings::MenuGameSettings(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("GAMES SETTINGS"))
  , mSystemManager(systemManager)
{
}

void MenuGameSettings::BuildMenuItems()
{
  // Screen ratio choice
  bool isCrt = Board::Instance().CrtBoard().IsCrtAdapterAttached();
  bool isJamma = Board::Instance().CrtBoard().GetCrtAdapter() == CrtAdapterType::RGBJamma;
  if (! isCrt && RecalboxConf::Instance().GetMenuType() != RecalboxConf::Menu::Bartop)
    AddList<String>(_("GAME RATIO"), (int)Components::Ratio, this, GetRatioEntries(), RecalboxConf::Instance().GetGlobalRatio(), "auto", _(MENUMESSAGE_GAME_RATIO_HELP_MSG));

  // RecalboxOverlays
  AddSwitch(_("RECALBOX OVERLAYS"), RecalboxConf::Instance().GetGlobalRecalboxOverlays(), (int)Components::RecalboxOverlays, this, _(MENUMESSAGE_GAME_OVERLAYS_HELP_MSG));

  // smoothing
  mSmooth = AddSwitch(_("SMOOTH GAMES"), RecalboxConf::Instance().GetGlobalSmooth(), (int)Components::Smooth, this, _(MENUMESSAGE_GAME_SMOOTH_HELP_MSG));

  // rewind
  AddSwitch(_("REWIND"), RecalboxConf::Instance().GetGlobalRewind(), (int)Components::Rewind, this,_(MENUMESSAGE_GAME_REWIND_HELP_MSG));

  // Softpatching
  AddList<RecalboxConf::SoftPatching>(_("SOFTPATCHING"), (int)Components::Softpatching, this, GetSoftpatchingEntries(), RecalboxConf::Instance().GetGlobalSoftpatching(), RecalboxConf::SoftPatching::Disable, _(MENUMESSAGE_GAME_SOFTPATCHING));

  // show savestates
  mShowSaveStates = AddSwitch(_("SHOW SAVE STATES ON START"), RecalboxConf::Instance().GetGlobalShowSaveStateBeforeRun(), (int)Components::ShowSaveStates, this, _(MENUMESSAGE_GAME_SHOW_SAVESTATES_HELP_MSG));

  // autosave
  mAutoSave = AddSwitch(_("AUTO SAVE/LOAD"), RecalboxConf::Instance().GetGlobalAutoSave(), (int)Components::AutoSave, this, _(MENUMESSAGE_GAME_AUTOSAVELOAD_HELP_MSG));

  // Press twice to quit
  AddSwitch(_("PRESS TWICE TO QUIT GAME"), RecalboxConf::Instance().GetGlobalQuitTwice(), (int)Components::QuitTwice, this, _(MENUMESSAGE_GAME_PRESS_TWICE_QUIT_HELP_MSG));

  if(!isJamma)
  {
    // Integer Scale
    std::string entry = isCrt ? "HORIZONTAL " : "";
    entry.append("INTEGER SCALE").append(isCrt ? "" : " (PIXEL PERFECT)");
    AddSwitch(entry, RecalboxConf::Instance().GetGlobalIntegerScale(), (int)Components::IntegerScale, this, _(MENUMESSAGE_GAME_INTEGER_SCALE_HELP_MSG));
  }

  if(!isCrt)
  {
    // Shaders preset
    AddList<String>(_("SHADERS SET"), (int)Components::ShaderSet, this, GetShaderPresetsEntries(), RecalboxConf::Instance().GetGlobalShaderSet(), "none",_(MENUMESSAGE_GAME_SHADERSET_HELP_MSG));

    // Shaders
    AddList<String>(_("ADVANCED SHADERS"), (int)Components::Shaders, this, GetShadersEntries(), RecalboxConf::Instance().GetGlobalShaders(), "", _(MENUMESSAGE_GAME_SHADERS_HELP_MSG));

    // HD mode
    if(MenuFilter::ShouldDisplayMenuEntry(MenuFilter::HDMode))
      AddSwitch(_("HD MODE"), RecalboxConf::Instance().GetGlobalHDMode(), (int)Components::HDMode, this, _(MENUMESSAGE_GAME_HD_MODE_HELP_MSG));

    // Widescreen mode
    if(MenuFilter::ShouldDisplayMenuEntry(MenuFilter::Widescreen))
      AddSwitch(_("WIDESCREEN (16/9)"), RecalboxConf::Instance().GetGlobalWidescreenMode(), (int)Components::WideScreenMode, this, _(MENUMESSAGE_GAME_WIDESCREEN_MODE_HELP_MSG));

    if(Board::Instance().HasVulkanSupport())
      AddSwitch(_("ENABLE VULKAN DRIVER"), RecalboxConf::Instance().GetGlobalVulkanDriver(), (int)Components::VulkanDriver, this, _(MENUMESSAGE_GAME_VULKAN_DRIVER_HELP_MSG));
  }

  // Super GameBoy
  AddList<String>(_("GAME BOY MODE"), (int)Components::SuperGameBoy, this, GetSuperGameBoyEntries(), RecalboxConf::Instance().GetSuperGameBoy(), "gb", _(MENUMESSAGE_GAME_SUPERGAMEBOY_HELP_MSG));

  // Retroachievements
  if (RecalboxConf::Instance().GetMenuType() != RecalboxConf::Menu::Bartop)
  {
    AddSubMenu(_("RETROACHIEVEMENTS SETTINGS"), (int)Components::RetroAchivements, this, _(MENUMESSAGE_RA_HELP_MSG));
    AddSubMenu(_("NETPLAY SETTINGS"), (int)Components::Netplay, this, _(MENUMESSAGE_NP_HELP_MSG));
  }
}

SelectorEntry<String>::List MenuGameSettings::GetRatioEntries()
{
  SelectorEntry<String>::List list;
  for (const auto& ratio : LibretroRatio::GetRatio())
    list.push_back({ ratio.first, ratio.second });

  return list;
}

SelectorEntry<String>::List MenuGameSettings::GetShadersEntries()
{
  SelectorEntry<String>::List list;
  MenuTools::ShaderList shaderList = MenuTools::ListShaders();
  list.push_back({ _("NONE"), "" });
  for (const MenuTools::Shader& shader : shaderList)
    list.push_back({ shader.Displayable, shader.ShaderPath.ToString() });
  return list;
}

SelectorEntry<String>::List MenuGameSettings::GetSuperGameBoyEntries()
{
  return SelectorEntry<String>::List
  {
    { _("GAME BOY"), "gb" },
    { _("SUPER GAME BOY"), "sgb" },
    { _("ASK AT LAUNCH"), "ask" },
  };
}

SelectorEntry<RecalboxConf::SoftPatching>::List MenuGameSettings::GetSoftpatchingEntries()
{
  return SelectorEntry<RecalboxConf::SoftPatching>::List
  {
    { _("AUTO"), RecalboxConf::SoftPatching::Auto },
    { _("LAUNCH LAST"), RecalboxConf::SoftPatching::LaunchLast },
    { _("SELECT"), RecalboxConf::SoftPatching::Select },
    { _("DISABLE"), RecalboxConf::SoftPatching::Disable },
  };
}

SelectorEntry<String>::List MenuGameSettings::GetShaderPresetsEntries()
{
  return SelectorEntry<String>::List
  {
    { _("NONE"), "none" },
    { _("CRT CURVED"), "crtcurved" },
    { _("SCANLINES"), "scanlines" },
    { _("RETRO"), "retro" },
  };
}

void MenuGameSettings::SubMenuSelected(int id)
{
  if ((Components)id == Components::RetroAchivements) mWindow.pushGui(new MenuRetroAchievements(mWindow));
  else if ((Components)id == Components::Netplay) mWindow.pushGui(new MenuNetplay(mWindow, mSystemManager));
}


void MenuGameSettings::MenuSingleChanged(int id, int index, const RecalboxConf::SoftPatching& value)
{
  (void)index;
  if ((Components)id == Components::Softpatching)
    RecalboxConf::Instance().SetGlobalSoftpatching(value).Save();
}

void MenuGameSettings::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::Ratio: RecalboxConf::Instance().SetGlobalRatio(value).Save(); break;
    case Components::Shaders:  RecalboxConf::Instance().SetGlobalShaders(value).Save(); break;
    case Components::ShaderSet:
    {
      if (value != "none" && (RecalboxConf::Instance().GetGlobalSmooth()))
        mWindow.pushGui(new GuiMsgBox(mWindow,
                                      _("YOU JUST ACTIVATED THE SHADERS FOR ALL SYSTEMS. FOR A BETTER RENDERING, IT IS ADVISED TO DISABLE GAME SMOOTHING. DO YOU WANT TO CHANGE THIS OPTION AUTOMATICALLY?"),
                                      _("LATER"), nullptr, _("YES"), [this]
                                      { mSmooth->SetState(false, false); }));
      RecalboxConf::Instance().SetGlobalShaderSet(value).Save();
      break;
    }
    case Components::SuperGameBoy: RecalboxConf::Instance().SetSuperGameBoy(value).Save(); break;
    case Components::RecalboxOverlays:
    case Components::Smooth:
    case Components::Rewind:
    case Components::AutoSave:
    case Components::ShowSaveStates:
    case Components::QuitTwice:
    case Components::IntegerScale:
    case Components::Softpatching:
    case Components::RetroAchivements:
    case Components::Netplay:
    case Components::HDMode:
    case Components::WideScreenMode:
    case Components::VulkanDriver:
    default: break;
  }
}

void MenuGameSettings::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::Smooth: RecalboxConf::Instance().SetGlobalSmooth(status).Save(); break;
    case Components::RecalboxOverlays: RecalboxConf::Instance().SetGlobalRecalboxOverlays(status).Save(); break;
    case Components::Rewind: RecalboxConf::Instance().SetGlobalRewind(status).Save(); break;
    case Components::ShowSaveStates:
      RecalboxConf::Instance().SetGlobalShowSaveStateBeforeRun(status).Save();
      if (status)
      {
        mAutoSave->SetState(false, false);
        RecalboxConf::Instance().SetGlobalAutoSave(false).Save();
      }
      break;
    case Components::AutoSave:
      RecalboxConf::Instance().SetGlobalAutoSave(status).Save();
      if (status)
      {
        mShowSaveStates->SetState(false, false);
        RecalboxConf::Instance().SetGlobalShowSaveStateBeforeRun(false).Save();
      }
      break;
    case Components::QuitTwice: RecalboxConf::Instance().SetGlobalQuitTwice(status).Save(); break;
    case Components::IntegerScale: RecalboxConf::Instance().SetGlobalIntegerScale(status).Save(); break;
    case Components::HDMode: {
      if(status)
        mWindow.pushGui(new GuiMsgBox(mWindow,
                                    _("RECALBOX WILL AUTOMATICALLY SELECT EMULATORS TO RUN ON HD-COMPATIBLE SYSTEMS. SOME GAMES MAY NOT BE 100% COMPATIBLE, SO DON'T FORGET TO DISABLE THIS OPTION IF EMULATION OR PERFORMANCE ISSUES APPEAR."),
                                    _("OK"), nullptr));
      RecalboxConf::Instance().SetGlobalHDMode(status).Save();
      break;
    }
    case Components::WideScreenMode: {
      if(status)
        mWindow.pushGui(new GuiMsgBox(mWindow,
                                      _("RECALBOX WILL AUTOMATICALLY SELECT EMULATORS TO RUN ON WIDESCREEN-COMPATIBLE SYSTEMS. SOME GAMES MAY NOT BE 100% COMPATIBLE, SO DON'T FORGET TO DISABLE THIS OPTION IF EMULATION OR PERFORMANCE ISSUES APPEAR."),
                                      _("OK"), nullptr));
      RecalboxConf::Instance().SetGlobalWidescreenMode(status).Save(); break;
    }
    case Components::VulkanDriver: RecalboxConf::Instance().SetGlobalVulkanDriver(status).Save(); break;
    case Components::Ratio:
    case Components::Softpatching:
    case Components::Shaders:
    case Components::ShaderSet:
    case Components::RetroAchivements:
    case Components::Netplay:
    case Components::SuperGameBoy:
      break;
  }
}

