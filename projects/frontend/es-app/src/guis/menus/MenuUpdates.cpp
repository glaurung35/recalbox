//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/MenuUpdates.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>
#include <Upgrade.h>
#include <guis/GuiUpdateRecalbox.h>
#include "patreon/PatronInfo.h"

MenuUpdates::MenuUpdates(WindowManager& window)
  : Menu(window, _("UPDATES"))
{
  // Enable updates
  AddSwitch(_("CHECK UPDATES"), RecalboxConf::Instance().GetUpdatesEnabled(), (int)Components::Enable, this, _(MENUMESSAGE_UPDATE_CHECK_HELP_MSG));

  // Display available update version
  bool update = Upgrade::Instance().PendingUpdate();
  AddText(_("AVAILABLE UPDATE"), update ? _("YES") : _("NO"), _(MENUMESSAGE_UPDATE_VERSION_HELP_MSG));

  // Display available update changelog
  if (update)
  {
    AddSubMenu(_("UPDATE CHANGELOG"), (int) Components::Changelog, this, _(MENUMESSAGE_UPDATE_CHANGELOG_HELP_MSG));

    // Start update
    AddSubMenu(_("START UPDATE"), (int) Components::StartUpdate, this, _(MENUMESSAGE_START_UPDATE_HELP_MSG));
  }
  else if (RecalboxSystem::hasIpAdress(false))
  {
    // Start update
    AddSubMenu(_("CHECK FOR UPDATE NOW"), (int)Components::CheckUpdate, this, _(MENUMESSAGE_CHECK_UPDATE_HELP_MSG));
  }
  // Enable updates
  if (
    #ifdef BETA
      true ||
    #endif
      PatronInfo::Instance().IsPatron())
    AddList<RecalboxConf::UpdateType>(_("UPDATE TYPE"), (int)Components::UpdateType, this, GetUpdateTypeEntries(), RecalboxConf::Instance().GetUpdateType(), RecalboxConf::UpdateType::Stable, _(MENUMESSAGE_UPDATE_TYPE_HELP_MSG));
}

SelectorEntry<RecalboxConf::UpdateType>::List MenuUpdates::GetUpdateTypeEntries()
{
  SelectorEntry<RecalboxConf::UpdateType>::List list;

  list.push_back({ "stable", RecalboxConf::UpdateType::Stable });
  list.push_back({ "patron", RecalboxConf::UpdateType::Patron });
  list.push_back({ "alpha" , RecalboxConf::UpdateType::Alpha });
  list.push_back({ "jamma" , RecalboxConf::UpdateType::Jamma });

  return list;
}

void MenuUpdates::MenuSwitchChanged(int id, bool& status)
{
  if ((Components)id == Components::Enable)
    RecalboxConf::Instance().SetUpdatesEnabled(status).Save();
}

void MenuUpdates::SubMenuSelected(int id)
{
  Upgrade& upgrade = Upgrade::Instance();
  if ((Components) id == Components::Changelog)
  {
    String changelog = upgrade.NewReleaseNote();
    if (!changelog.empty())
    {
      const String& message = changelog;
      String updateVersion = upgrade.NewVersion();
      mWindow.displayScrollMessage(_("AN UPDATE IS AVAILABLE FOR YOUR RECALBOX"),
                                   _("NEW VERSION:") + ' ' + updateVersion + "\n" + _("UPDATE CHANGELOG:") + "\n" +
                                   message);
    }
    else
      mWindow.displayMessage(_("AN UPDATE IS AVAILABLE FOR YOUR RECALBOX"));
  }
  else if ((Components) id == Components::StartUpdate)
    mWindow.pushGui(new GuiUpdateRecalbox(mWindow, upgrade.TarUrl(), upgrade.ImageUrl(), upgrade.HashUrl(), upgrade.NewVersion()));
  else if ((Components) id == Components::CheckUpdate)
  {
    upgrade.DoManualCheck();
    mWindow.CloseAll();
  }
}

void MenuUpdates::MenuSingleChanged(int id, int index, const RecalboxConf::UpdateType& value)
{
  (void)index;
  if ((Components)id == Components::UpdateType)
    RecalboxConf::Instance().SetUpdateType(value);
}
