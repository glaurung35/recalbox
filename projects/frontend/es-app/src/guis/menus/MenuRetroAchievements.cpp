//
// Created by bkg2k on 03/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/MenuRetroAchievements.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>

MenuRetroAchievements::MenuRetroAchievements(WindowManager& window)
  : Menu(window, _("RETROACHIEVEMENTS SETTINGS"))
{
  // Retroachievement on/off
  AddSwitch(_("RETROACHIEVEMENTS"), RecalboxConf::Instance().GetRetroAchievementOnOff(), (int)Components::Enabled, this, _(MENUMESSAGE_RA_ONOFF_HELP_MSG));

  // Hardcore mode on/off
  AddSwitch(_("HARDCORE MODE"), RecalboxConf::Instance().GetRetroAchievementHardcore(), (int)Components::Hardcore, this, _(MENUMESSAGE_RA_HARDCORE_HELP_MSG));

  // Login
  AddEditable(_("USERNAME"), RecalboxConf::Instance().GetRetroAchievementLogin(), (int)Components::Login, this, _(MENUMESSAGE_RA_USERNAME_MSG), false);

  // Password
  AddEditable(_("PASSWORD"), RecalboxConf::Instance().GetRetroAchievementPassword(), (int)Components::Password, this, _(MENUMESSAGE_RA_PASSWORD_MSG), true);
}

void MenuRetroAchievements::MenuEditableChanged(int id, const String& text)
{
  if ((Components)id == Components::Login) RecalboxConf::Instance().SetRetroAchievementLogin(text).Save();
  else if ((Components)id == Components::Password) RecalboxConf::Instance().SetRetroAchievementPassword(text).Save();
}

void MenuRetroAchievements::MenuSwitchChanged(int id, bool& status)
{
  if ((Components)id == Components::Enabled) RecalboxConf::Instance().SetRetroAchievementOnOff(status).Save();
  else if ((Components)id == Components::Hardcore) RecalboxConf::Instance().SetRetroAchievementHardcore(status).Save();
}
