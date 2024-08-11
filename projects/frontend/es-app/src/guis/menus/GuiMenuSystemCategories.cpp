//
// Created by bkg2k on 11/08/24.
//

#include "GuiMenuSystemCategories.h"

GuiMenuSystemCategories::GuiMenuSystemCategories(WindowManager& window, SystemManager& systemManager)
  : GuiMenuBase(window, _("SYSTEM CATEGORIES"), nullptr)
  , mSystemManager(systemManager)
{
  // Enable
  AddSwitch(_("ENABLE CATEGORY SELECTIONS"), mConfiguration.GetCategoryEnabled(), (int)Components::Enabled, this,
            _("Enable system filtering by category: console, computer, ..."));

  // Current filter
  AddList(_("CURRENT CATEGORY"), (int)Components::Category, this, GetCategoryEntries(),
          _("Current category of displayed systems in the system view"));

  // Quick change
  AddSwitch(_("ENABLE QUICK CHANGE"), mConfiguration.GetCategoryQuickChange(), (int)Components::QuickChange, this,
            _("Enable direct category change instead of showing the catégory window"));
}

void GuiMenuSystemCategories::SwitchComponentChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::Enabled: mConfiguration.SetCategoryEnabled(status).Save(); break;
    case Components::Category: break;
    case Components::QuickChange: mConfiguration.SetCategoryQuickChange(status).Save(); break;
  }
}

void GuiMenuSystemCategories::OptionListComponentChanged(int id, int index, const SystemDescriptor::SystemCategory& value,
                                                    bool quickChange)
{
  (void)index;
  (void)quickChange;
  if ((Components)id == Components::Category)
    mConfiguration.SetCategorySelected(value).Save();
}

std::vector<GuiMenuSystemCategories::ListEntry<SystemDescriptor::SystemCategory>> GuiMenuSystemCategories::GetCategoryEntries()
{
  std::vector<GuiMenuSystemCategories::ListEntry<SystemDescriptor::SystemCategory>> result;
  HashSet<SystemDescriptor::SystemCategory> set = mSystemManager.GetVisibleCategories();
  if (set.empty()) return result; // empty
  // Not empty? add "all"
  SystemDescriptor::SystemCategory current = mConfiguration.GetCategorySelected();
  result.push_back({ _("ALL"), SystemDescriptor::SystemCategory::All, current == SystemDescriptor::SystemCategory::All} );
  if (set.contains(SystemDescriptor::SystemCategory::Arcade))
    result.push_back({ _("ARCADE SYSTEMS"), SystemDescriptor::SystemCategory::Arcade, current == SystemDescriptor::SystemCategory::Arcade} );
  if (set.contains(SystemDescriptor::SystemCategory::Console))
    result.push_back({ _("HOME CONSOLES"), SystemDescriptor::SystemCategory::Console, current == SystemDescriptor::SystemCategory::Console} );
  if (set.contains(SystemDescriptor::SystemCategory::Handheld))
    result.push_back({ _("HANDHELD CONSOLES"), SystemDescriptor::SystemCategory::Handheld, current == SystemDescriptor::SystemCategory::Handheld} );
  if (set.contains(SystemDescriptor::SystemCategory::Computer))
    result.push_back({ _("COMPUTERS"), SystemDescriptor::SystemCategory::Computer, current == SystemDescriptor::SystemCategory::Computer} );
  if (set.contains(SystemDescriptor::SystemCategory::Fantasy))
    result.push_back({ _("FANTASY CONSOLES"), SystemDescriptor::SystemCategory::Fantasy, current == SystemDescriptor::SystemCategory::Fantasy} );
  if (set.contains(SystemDescriptor::SystemCategory::Engine))
    result.push_back({ _("GAME ENGINES"), SystemDescriptor::SystemCategory::Engine, current == SystemDescriptor::SystemCategory::Engine} );
  return result;
}