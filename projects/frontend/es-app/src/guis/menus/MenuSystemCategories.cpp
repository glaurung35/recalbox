//
// Created by bkg2k on 11/08/24.
//

#include "MenuSystemCategories.h"

MenuSystemCategories::MenuSystemCategories(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("SYSTEM CATEGORIES"))
  , mSystemManager(systemManager)
{
  // Enable
  AddSwitch(_("ENABLE CATEGORY SELECTIONS"), mConfiguration.GetCategoryEnabled(), (int)Components::Enabled, this,
            _("Enable system filtering by category: console, computer, ..."));

  // Current filter
  AddList(_("CURRENT CATEGORY"), (int)Components::Category, this, GetCategoryEntries(), mConfiguration.GetCategorySelected(), SystemDescriptor::SystemCategory::All,
          _("Current category of displayed systems in the system view"));

  // Quick change
  AddSwitch(_("ENABLE QUICK CHANGE"), mConfiguration.GetCategoryQuickChange(), (int)Components::QuickChange, this,
            _("Enable direct category change instead of showing the catégory window"));
}

void MenuSystemCategories::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::Enabled: mConfiguration.SetCategoryEnabled(status).Save(); break;
    case Components::Category: break;
    case Components::QuickChange: mConfiguration.SetCategoryQuickChange(status).Save(); break;
  }
}

void MenuSystemCategories::MenuSingleChanged(int id, int index, const SystemDescriptor::SystemCategory& value)
{
  (void)index;
  if ((Components)id == Components::Category)
    mConfiguration.SetCategorySelected(value).Save();
}

SelectorEntry<SystemDescriptor::SystemCategory>::List MenuSystemCategories::GetCategoryEntries()
{
  SelectorEntry<SystemDescriptor::SystemCategory>::List result;
  HashSet<SystemDescriptor::SystemCategory> set = mSystemManager.GetAvailableCategories();
  if (set.empty()) return result; // empty
  // Not empty? add "all"
  result.push_back({ _("ALL"), SystemDescriptor::SystemCategory::All });
  if (set.contains(SystemDescriptor::SystemCategory::Arcade))   result.push_back({ _("ARCADE SYSTEMS"), SystemDescriptor::SystemCategory::Arcade });
  if (set.contains(SystemDescriptor::SystemCategory::Console))  result.push_back({ _("HOME CONSOLES"), SystemDescriptor::SystemCategory::Console });
  if (set.contains(SystemDescriptor::SystemCategory::Handheld)) result.push_back({ _("HANDHELD CONSOLES"), SystemDescriptor::SystemCategory::Handheld });
  if (set.contains(SystemDescriptor::SystemCategory::Computer)) result.push_back({ _("COMPUTERS"), SystemDescriptor::SystemCategory::Computer });
  if (set.contains(SystemDescriptor::SystemCategory::Fantasy))  result.push_back({ _("FANTASY CONSOLES"), SystemDescriptor::SystemCategory::Fantasy });
  if (set.contains(SystemDescriptor::SystemCategory::Engine))   result.push_back({ _("GAME ENGINES"), SystemDescriptor::SystemCategory::Engine });
  return result;
}