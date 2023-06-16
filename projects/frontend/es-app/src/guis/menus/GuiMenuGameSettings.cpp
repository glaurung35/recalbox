//
// Created by bkg2k on 06/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "GuiMenuGameSettings.h"
#include "GuiMenuTools.h"
#include "GuiMenuNetplay.h"
#include "GuiMenuRetroAchievements.h"
#include "guis/GuiMsgBox.h"
#include <components/OptionListComponent.h>
#include <systems/SystemManager.h>
#include <guis/MenuMessages.h>
#include <LibretroRatio.h>
#include <algorithm>

GuiMenuGameSettings::GuiMenuGameSettings(WindowManager& window, SystemManager& systemManager)
  : GuiMenuBase(window, _("GAMES SETTINGS"), this)
  , mSystemManager(systemManager)
{
  // Screen ratio choice
  bool isCrt = Board::Instance().CrtBoard().IsCrtAdapterAttached();
  if (! isCrt && RecalboxConf::Instance().GetMenuType() != RecalboxConf::Menu::Bartop)
    AddList<std::string>(_("GAME RATIO"), (int)Components::Ratio, this, GetRatioEntries(), _(MENUMESSAGE_GAME_RATIO_HELP_MSG));

  // RecalboxOverlays
  AddSwitch(_("RECALBOX OVERLAYS"), RecalboxConf::Instance().GetGlobalRecalboxOverlays(), (int)Components::RecalboxOverlays, this, _(MENUMESSAGE_GAME_OVERLAYS_HELP_MSG));

  // smoothing
  mSmooth = AddSwitch(_("SMOOTH GAMES"), RecalboxConf::Instance().GetGlobalSmooth(), (int)Components::Smooth, this, _(MENUMESSAGE_GAME_SMOOTH_HELP_MSG));

  // rewind
  AddSwitch(_("REWIND"), RecalboxConf::Instance().GetGlobalRewind(), (int)Components::Rewind, this,_(MENUMESSAGE_GAME_REWIND_HELP_MSG));

  // Softpatching
  AddList<std::string>(_("SOFTPATCHING"), (int)Components::Softpatching, this, GetSoftpatchingEntries(), _(MENUMESSAGE_GAME_SOFTPATCHING));

  // show savestates
  mShowSaveStates = AddSwitch(_("SHOW SAVE STATES ON START"), RecalboxConf::Instance().GetGlobalShowSaveStateBeforeRun(), (int)Components::ShowSaveStates, this, _(MENUMESSAGE_GAME_SHOW_SAVESTATES_HELP_MSG));

  // autosave
  mAutoSave = AddSwitch(_("AUTO SAVE/LOAD"), RecalboxConf::Instance().GetGlobalAutoSave(), (int)Components::AutoSave, this, _(MENUMESSAGE_GAME_AUTOSAVELOAD_HELP_MSG));

  // Press twice to quit
  AddSwitch(_("PRESS TWICE TO QUIT GAME"), RecalboxConf::Instance().GetGlobalQuitTwice(), (int)Components::QuitTwice, this, _(MENUMESSAGE_GAME_PRESS_TWICE_QUIT_HELP_MSG));

  // Integer Scale
  if(!isCrt)
    AddSwitch(_("INTEGER SCALE (PIXEL PERFECT)"), RecalboxConf::Instance().GetGlobalIntegerScale(), (int)Components::IntegerScale, this, _(MENUMESSAGE_GAME_INTEGER_SCALE_HELP_MSG));

  // Shaders preset
  if(!isCrt)
    AddList<std::string>(_("SHADERS SET"), (int)Components::ShaderSet, this, GetShaderPresetsEntries(), _(MENUMESSAGE_GAME_SHADERSET_HELP_MSG));

  // Shaders
  if(!isCrt)
    AddList<std::string>(_("ADVANCED SHADERS"), (int)Components::Shaders, this, GetShadersEntries(), _(MENUMESSAGE_GAME_SHADERS_HELP_MSG));

  // Super GameBoy
  AddList<std::string>(_("GAME BOY MODE"), (int)Components::SuperGameBoy, this, GetSuperGameBoyEntries(), _(MENUMESSAGE_GAME_SUPERGAMEBOY_HELP_MSG));

  // Retroachievements
  if (RecalboxConf::Instance().GetMenuType() != RecalboxConf::Menu::Bartop)
  {
    AddSubMenu(_("RETROACHIEVEMENTS SETTINGS"), (int)Components::RetroAchivements, _(MENUMESSAGE_RA_HELP_MSG));
    AddSubMenu(_("NETPLAY SETTINGS"), (int)Components::Netplay, _(MENUMESSAGE_NP_HELP_MSG));
  }
}

GuiMenuGameSettings::~GuiMenuGameSettings() = default;

std::vector<GuiMenuBase::ListEntry<std::string>> GuiMenuGameSettings::GetRatioEntries()
{
  std::vector<GuiMenuBase::ListEntry<std::string>> list;

  std::string currentRatio = RecalboxConf::Instance().GetGlobalRatio();
  for (const auto& ratio : LibretroRatio::GetRatio())
    list.push_back({ ratio.first, ratio.second, currentRatio == ratio.second });

  return list;
}

std::vector<GuiMenuBase::ListEntry<std::string>> GuiMenuGameSettings::GetShadersEntries()
{
  std::vector<GuiMenuBase::ListEntry<std::string>> list;

  GuiMenuTools::ShaderList shaderList = GuiMenuTools::ListShaders();
  String currentShader = RecalboxConf::Instance().GetGlobalShaders();
  list.push_back({ _("NONE"), "", currentShader.empty() });
  for (const GuiMenuTools::Shader& shader : shaderList)
    list.push_back({ shader.Displayable, shader.ShaderPath.ToString(), currentShader == shader.ShaderPath.ToString() });

  return list;
}

std::vector<GuiMenuBase::ListEntry<std::string>> GuiMenuGameSettings::GetSuperGameBoyEntries()
{
  std::vector<GuiMenuBase::ListEntry<std::string>> list;

  std::string currentOption = RecalboxConf::Instance().GetSuperGameBoy();
  list.push_back({ _("GAME BOY"), "gb", currentOption == "gb" });
  list.push_back({ _("SUPER GAME BOY"), "sgb", currentOption == "sgb" });
  list.push_back({ _("ASK AT LAUNCH"), "ask", currentOption == "ask" });
  return list;
}

std::vector<GuiMenuBase::ListEntry<std::string>> GuiMenuGameSettings::GetSoftpatchingEntries()
{
  std::vector<GuiMenuBase::ListEntry<std::string>> list;

  std::string currentOption = RecalboxConf::Instance().GetGlobalSoftpatching();

  if(currentOption != "auto" && currentOption != "select" && currentOption != "disable")
    currentOption = "auto";

  list.emplace_back( _("AUTO"), "auto", currentOption == "auto" );
  list.emplace_back( _("SELECT"), "select", currentOption == "select" );
  list.emplace_back( _("DISABLE"), "disable", currentOption == "disable");

  return list;
}

std::vector<GuiMenuBase::ListEntry<std::string>> GuiMenuGameSettings::GetShaderPresetsEntries()
{
  std::vector<GuiMenuBase::ListEntry<std::string>> list;

  std::string currentPreset = RecalboxConf::Instance().GetGlobalShaderSet();
  if (currentPreset != "scanlines" && currentPreset != "retro" && currentPreset != "crtcurved") currentPreset = "none";
  list.push_back({ _("NONE"), "none", currentPreset == "none" });
  list.push_back({ _("CRT CURVED"), "crtcurved", currentPreset == "crtcurved" });
  list.push_back({ _("SCANLINES"), "scanlines", currentPreset == "scanlines" });
  list.push_back({ _("RETRO"), "retro", currentPreset == "retro" });

  return list;
}

void GuiMenuGameSettings::SubMenuSelected(int id)
{
  if ((Components)id == Components::RetroAchivements) mWindow.pushGui(new GuiMenuRetroAchievements(mWindow));
  else if ((Components)id == Components::Netplay) mWindow.pushGui(new GuiMenuNetplay(mWindow, mSystemManager));
}

void GuiMenuGameSettings::ChangeShadersOptions(){
  mSmooth->setState(false);
}

void GuiMenuGameSettings::OptionListComponentChanged(int id, int index, const std::string& value)
{
  (void)index;
  if ((Components)id == Components::Ratio) RecalboxConf::Instance().SetGlobalRatio(value).Save();
  else if ((Components)id == Components::Softpatching)
    RecalboxConf::Instance().SetGlobalSoftpatching(value).Save();
  else if ((Components)id == Components::Shaders) RecalboxConf::Instance().SetGlobalShaders(value).Save();
  else if ((Components)id == Components::ShaderSet)
  {
    if(value != "none" && (RecalboxConf::Instance().GetGlobalSmooth()))
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("YOU JUST ACTIVATED THE SHADERS FOR ALL SYSTEMS. FOR A BETTER RENDERING, IT IS ADVISED TO DISABLE GAME SMOOTHING. DO YOU WANT TO CHANGE THIS OPTION AUTOMATICALLY?"),
                                    _("LATER"), nullptr,
                                    _("YES"), std::bind(&GuiMenuGameSettings::ChangeShadersOptions, this)));
    }
    RecalboxConf::Instance().SetGlobalShaderSet(value).Save();
  }
  else if ((Components)id == Components::SuperGameBoy)
  {
    RecalboxConf::Instance().SetSuperGameBoy(value).Save();
  }
}

void GuiMenuGameSettings::SwitchComponentChanged(int id, bool status)
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
        mAutoSave->setState(false);
        RecalboxConf::Instance().SetGlobalAutoSave(false).Save();
      }
      break;
    case Components::AutoSave:
      RecalboxConf::Instance().SetGlobalAutoSave(status).Save();
      if (status)
      {
        mShowSaveStates->setState(false);
        RecalboxConf::Instance().SetGlobalShowSaveStateBeforeRun(false).Save();
      }
      break;
    case Components::QuitTwice: RecalboxConf::Instance().SetGlobalQuitTwice(status).Save(); break;
    case Components::IntegerScale: RecalboxConf::Instance().SetGlobalIntegerScale(status).Save(); break;
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

