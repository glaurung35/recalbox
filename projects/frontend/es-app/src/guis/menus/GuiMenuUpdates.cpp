//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <guis/menus/GuiMenuUpdates.h>
#include <components/SwitchComponent.h>
#include <components/OptionListComponent.h>
#include <utils/locale/LocaleHelper.h>
#include <guis/MenuMessages.h>
#include <Upgrade.h>
#include <guis/GuiUpdateRecalbox.h>

GuiMenuUpdates::GuiMenuUpdates(WindowManager& window)
  : GuiMenuBase(window, _("UPDATES"), this)
{
  // Enable updates
  mEnableUpdate = AddSwitch(_("CHECK UPDATES"), RecalboxConf::Instance().GetUpdatesEnabled(), (int)Components::Enable, this, _(MENUMESSAGE_UPDATE_CHECK_HELP_MSG));

  // Display available update version
  bool update = Upgrade::Instance().PendingUpdate();
  mAvailable = AddText(_("AVAILABLE UPDATE"), update ? _("YES") : _("NO"), _(MENUMESSAGE_UPDATE_VERSION_HELP_MSG));

  if (update)
  {
    // Display available update changelog
    AddSubMenu(_("UPDATE CHANGELOG"), (int)Components::Changelog, _(MENUMESSAGE_UPDATE_CHANGELOG_HELP_MSG));
    // Start update
    AddSubMenu(_("START UPDATE"), (int)Components::StartUpdate, _(MENUMESSAGE_START_UPDATE_HELP_MSG));
  }
  else if (RecalboxSystem::hasIpAdress(false))
  {
    // Start update
    AddSubMenu(_("CHECK FOR UPDATE NOW"), (int)Components::CheckUpdate, _(MENUMESSAGE_CHECK_UPDATE_HELP_MSG));
  }

  // Enable updates
  // mType = AddList(_("UPDATE TYPE"), (int)Components::UpdateType, this, GetUpdateTypeEntries(), _(MENUMESSAGE_UPDATE_TYPE_HELP_MSG));
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuUpdates::GetUpdateTypeEntries()
{
  std::vector<ListEntry<String>> list;

  String updatesType = RecalboxConf::Instance().GetUpdatesType();
  if (updatesType != "stable" && updatesType != "custom") updatesType = "stable";
  list.push_back({ "stable", "stable", updatesType == "stable" });
  list.push_back({ "custom", "custom", updatesType != "stable" });

  return list;
}

void GuiMenuUpdates::SwitchComponentChanged(int id, bool& status)
{
  if ((Components)id == Components::Enable)
    RecalboxConf::Instance().SetUpdatesEnabled(status).Save();
}

void GuiMenuUpdates::SubMenuSelected(int id)
{
  Upgrade& upgrade = Upgrade::Instance();
  if ((Components)id == Components::Changelog)
  {
    String changelog = upgrade.NewReleaseNote();
    if (!changelog.empty())
    {
      const String& message = changelog;
      String updateVersion = upgrade.NewVersion();
      mWindow.displayScrollMessage(_("AN UPDATE IS AVAILABLE FOR YOUR RECALBOX"),
                                   _("NEW VERSION:") + ' ' + updateVersion + "\n" +
                                   _("UPDATE CHANGELOG:") + "\n" + message);
    }
    else
      mWindow.displayMessage(_("AN UPDATE IS AVAILABLE FOR YOUR RECALBOX"));
  }
  else if ((Components)id == Components::StartUpdate)
  {
    mWindow.pushGui(new GuiUpdateRecalbox(mWindow, upgrade.TarUrl(), upgrade.ImageUrl(), upgrade.HashUrl(), upgrade.NewVersion()));
  }
  else if ((Components)id == Components::CheckUpdate)
  {
    upgrade.DoManualCheck();
    mWindow.CloseAll();
  }
}

void GuiMenuUpdates::OptionListComponentChanged(int id, int index, const String& value, bool quickChange)
{
  (void)quickChange;
  (void)index;
  if ((Components)id == Components::UpdateType)
    RecalboxConf::Instance().SetUpdatesType(value);
}
