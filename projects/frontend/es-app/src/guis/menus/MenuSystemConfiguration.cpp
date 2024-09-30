//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/MenuSystemConfiguration.h>
#include <systems/SystemManager.h>
#include <guis/MenuMessages.h>
#include <LibretroRatio.h>
#include "MenuTools.h"
#include "views/ViewController.h"

MenuSystemConfiguration::MenuSystemConfiguration(WindowManager& window, SystemData& system, SystemManager& systemManager, AdvancedMenuOptions options)
  : Menu(window, system.FullName())
  , mSystemManager(systemManager)
  , mSystem(system)
{
  // Default emulator/core
  String emulatorAndCore;
  String defaultEmulatorAndCore;
  if (hasFlag(options, AdvancedMenuOptions::Emulator))
    AddList(_("Emulator"), (int)Components::Emulator, this, GetEmulatorEntries(emulatorAndCore, defaultEmulatorAndCore),
            emulatorAndCore, defaultEmulatorAndCore, _(MENUMESSAGE_ADVANCED_EMU_EMU_HELP_MSG));

  // Screen ratio choice
  if (hasFlag(options, AdvancedMenuOptions::Ratio))
    AddList(_("GAME RATIO"), (int)Components::Ratio, this, GetRatioEntries(), RecalboxConf::Instance().GetSystemRatio(mSystem), String::Empty, _(MENUMESSAGE_GAME_RATIO_HELP_MSG));

  // smoothing
  if (hasFlag(options, AdvancedMenuOptions::Smooth))
    AddSwitch(_("SMOOTH GAMES"), RecalboxConf::Instance().GetSystemSmooth(system), (int)Components::Smooth, this, _(MENUMESSAGE_GAME_SMOOTH_HELP_MSG));

  // rewind
  if (hasFlag(options, AdvancedMenuOptions::Rewind))
    AddSwitch(_("REWIND"), RecalboxConf::Instance().GetSystemRewind(system), (int)Components::Rewind, this, _(MENUMESSAGE_GAME_REWIND_HELP_MSG));

  // autosave
  if (hasFlag(options, AdvancedMenuOptions::AutoSave))
    AddSwitch(_("AUTO SAVE/LOAD"), RecalboxConf::Instance().GetSystemAutoSave(system), (int)Components::AutoSave, this, _(MENUMESSAGE_GAME_AUTOSAVELOAD_HELP_MSG));

  // Shaders
  if (hasFlag(options, AdvancedMenuOptions::Shaders))
    AddList(_("SHADERS"), (int)Components::Shaders, this, GetShadersEntries(), RecalboxConf::Instance().GetSystemShaders(mSystem), String::Empty, _(MENUMESSAGE_GAME_SHADERS_HELP_MSG));

  // Shaders preset
  if (hasFlag(options, AdvancedMenuOptions::ShaderSet))
    AddList(_("SHADERS SET"), (int)Components::ShaderSet, this, GetShaderSetEntries(), RecalboxConf::Instance().GetSystemShaderSet(mSystem), String("none"), _(MENUMESSAGE_GAME_SHADERSET_HELP_MSG));
}

SelectorEntry<String>::List MenuSystemConfiguration::GetEmulatorEntries(String& emulatorAndCore, String& defaultEmulatorAndCore)
{
  SelectorEntry<String>::List list;

  String currentEmulator(RecalboxConf::Instance().GetSystemEmulator(mSystem));
  String currentCore    (RecalboxConf::Instance().GetSystemCore(mSystem));
  MenuTools::EmulatorAndCoreList eList =
    MenuTools::ListEmulatorAndCore(mSystem, mDefaultEmulator, mDefaultCore, currentEmulator, currentCore);
  if (!eList.empty())
    for (const MenuTools::EmulatorAndCore& emulator : eList)
    {
      list.push_back({ emulator.Displayable, emulator.Identifier });
      if (emulator.Selected) emulatorAndCore = emulator.Identifier;
      if (defaultEmulatorAndCore.empty()) defaultEmulatorAndCore = emulator.Identifier;
    }

  return list;
}

SelectorEntry<String>::List MenuSystemConfiguration::GetRatioEntries()
{
  SelectorEntry<String>::List list;
  for (const auto& ratio : LibretroRatio::GetRatio())
    list.push_back({ ratio.first, ratio.second });
  return list;
}

SelectorEntry<String>::List MenuSystemConfiguration::GetShadersEntries()
{
  SelectorEntry<String>::List list;
  MenuTools::ShaderList shaderList = MenuTools::ListShaders();
  list.push_back({ _("NONE"), String::Empty });
  for (const MenuTools::Shader& shader : shaderList)
    list.push_back({ shader.Displayable, shader.ShaderPath.ToString() });
  return list;
}

SelectorEntry<String>::List MenuSystemConfiguration::GetShaderSetEntries()
{
  SelectorEntry<String>::List list;
  list.push_back({ _("NONE"), "none" });
  list.push_back({ _("CRT CURVED"), "crtcurved" });
  list.push_back({ _("SCANLINES"), "scanlines" });
  list.push_back({ _("RETRO"), "retro" });
  return list;
}

void MenuSystemConfiguration::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::Emulator:
    {
      // Split emulator & core
      String emulator;
      String core;
      if (value.Extract(':', emulator, core, false))
      {
        // Set emulator/core
        if (emulator == mDefaultEmulator && core == mDefaultCore)
          RecalboxConf::Instance().SetSystemEmulator(mSystem, "")
                                  .SetSystemCore(mSystem, "").Save();
        else
          RecalboxConf::Instance().SetSystemEmulator(mSystem, emulator)
                                  .SetSystemCore(mSystem, core).Save();
        // Force refresh of gamelist
        ViewController::Instance().ForceGamelistRefresh(mSystem);
      }
      else { LOG(LogError) << "[SystemConfigurationGui] Error splitting emulator and core!"; }
      break;
    }
    case Components::Ratio: RecalboxConf::Instance().SetSystemRatio(mSystem, value).Save(); break;
    case Components::Shaders: RecalboxConf::Instance().SetSystemShaders(mSystem, value).Save(); break;
    case Components::ShaderSet: RecalboxConf::Instance().SetSystemShaderSet(mSystem, value).Save(); break;
    case Components::Smooth:
    case Components::Rewind:
    case Components::AutoSave: break;
  }
}

void MenuSystemConfiguration::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::Smooth: RecalboxConf::Instance().SetSystemSmooth(mSystem, status).Save(); break;
    case Components::Rewind: RecalboxConf::Instance().SetSystemRewind(mSystem, status).Save(); break;
    case Components::AutoSave: RecalboxConf::Instance().SetSystemAutoSave(mSystem, status).Save(); break;
    case Components::Emulator:
    case Components::Ratio:
    case Components::Shaders:
    case Components::ShaderSet: break;
  }
}
