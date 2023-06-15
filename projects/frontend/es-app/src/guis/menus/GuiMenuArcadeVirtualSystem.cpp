#include <RecalboxConf.h>
#include <utils/locale/LocaleHelper.h>
#include <systems/SystemManager.h>
#include <guis/GuiMsgBox.h>
#include "GuiMenuArcadeVirtualSystem.h"
#include "components/SwitchComponent.h"
#include "systems/ArcadeVirtualSystems.h"

GuiMenuArcadeVirtualSystem::GuiMenuArcadeVirtualSystem(WindowManager& window, SystemManager& systemManager)
  : GuiMenuBase(window, _("ARCADE VIRTUAL SYSTEM"), nullptr)
  , mSystemManager(systemManager)
  , mOriginalManufacturerList(RecalboxConf::Instance().GetRawCollectionArcadeManufacturers())
  , mOriginalArcadeOnOff(RecalboxConf::Instance().GetCollectionArcade())
  , mOriginalIncludeNeogeo(RecalboxConf::Instance().GetCollectionArcadeNeogeo())
  , mOriginalHideOriginals(RecalboxConf::Instance().GetCollectionArcadeHide())
  , mOriginalPosition(RecalboxConf::Instance().GetCollectionArcadePosition())
{
  // Per manufacturer systems
  AddMultiList<String>(_("MANUFACTURER VIRTUAL SYSTEMS"), (int)Components::ArcadeManufacturerList, this, GetManufacturersEntries());

  // Enable arcade
  AddSwitch(_("ENABLE ARCADE VIRTUAL SYSTEM"), mOriginalArcadeOnOff, (int)Components::ArcadeOnOff, this);

  // Include neogeo?
  AddSwitch(_("INCLUDE NEO-GEO"), mOriginalIncludeNeogeo, (int)Components::IncludeNeogeo, this);

  // Hide original systems?
  AddSwitch(_("HIDE ORIGINAL SYSTEMS"), mOriginalHideOriginals, (int)Components::HideOriginals, this);

  // Position in list
  AddList<int>(_("POSITION"), (int)Components::Position, this, GetPositionEntries());
}

GuiMenuArcadeVirtualSystem::~GuiMenuArcadeVirtualSystem()
{
  const RecalboxConf& conf = RecalboxConf::Instance();
  if ((conf.GetCollectionArcade() != mOriginalArcadeOnOff) ||
      (conf.GetCollectionArcadeNeogeo() != mOriginalIncludeNeogeo) ||
      (conf.GetCollectionArcadeHide() != mOriginalHideOriginals) ||
      (conf.GetCollectionArcadePosition() != mOriginalPosition) ||
      (conf.GetRawCollectionArcadeManufacturers() != mOriginalManufacturerList))
    RequestRelaunch();
}

std::vector<GuiMenuBase::ListEntry<int>> GuiMenuArcadeVirtualSystem::GetPositionEntries()
{
  std::vector<GuiMenuBase::ListEntry<int>> list;

  const SystemManager::SystemList& systemList = mSystemManager.GetVisibleSystemList();
  for(int i = 0; i < (int)systemList.size(); ++i)
  {
    std::string after = systemList[i]->FullName();
    std::string before = ((i <= 0) ? systemList[systemList.size() - 1]->FullName() : systemList[i - 1]->FullName());
    std::string displayString = _("BETWEEN %1 AND %2");
    Strings::ReplaceAllIn(displayString, "%1", before);
    Strings::ReplaceAllIn(displayString, "%2", after);
    list.push_back({ displayString, i, i == mOriginalPosition });
  }

  return list;
}

void GuiMenuArcadeVirtualSystem::OptionListComponentChanged(int id, int index, const int& value)
{
  (void)index;
  if ((Components)id == Components::Position)
    RecalboxConf::Instance().SetCollectionArcadePosition(value).Save();
}

void GuiMenuArcadeVirtualSystem::OptionListMultiComponentChanged(int id, const String::List& value)
{
  if ((Components)id == Components::ArcadeManufacturerList)
    RecalboxConf::Instance().SetCollectionArcadeManufacturers(value).Save();
}

void GuiMenuArcadeVirtualSystem::SwitchComponentChanged(int id, bool status)
{
  switch((Components)id)
  {
    case Components::ArcadeOnOff: RecalboxConf::Instance().SetCollectionArcade(status).Save(); break;
    case Components::IncludeNeogeo: RecalboxConf::Instance().SetCollectionArcadeNeogeo(status).Save(); break;
    case Components::HideOriginals: RecalboxConf::Instance().SetCollectionArcadeHide(status).Save(); break;
    case Components::Position:
    case Components::ArcadeManufacturerList:
      break;
  }
}

std::vector<GuiMenuBase::ListEntry<String>> GuiMenuArcadeVirtualSystem::GetManufacturersEntries()
{
  std::vector<GuiMenuBase::ListEntry<String>> result;
  const RecalboxConf& conf = RecalboxConf::Instance();

  for(const String& rawIdentifier : ArcadeVirtualSystems::GetVirtualArcadeSystemList())
  {
    String identifier(rawIdentifier);
    identifier.Replace('/', '-');
    result.push_back({ ArcadeVirtualSystems::GetRealName(rawIdentifier), identifier, conf.IsInCollectionArcadeManufacturers(identifier) });
  }
  return result;
}

