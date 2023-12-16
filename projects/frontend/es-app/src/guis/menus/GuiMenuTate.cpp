#include <RecalboxConf.h>
#include <guis/GuiMsgBox.h>
#include "GuiMenuTate.h"
#include "components/SwitchComponent.h"
#include "guis/MenuMessages.h"
#include "recalbox/BootConf.h"
#include <views/ViewController.h>

GuiMenuTate::GuiMenuTate(WindowManager& window, SystemManager& systemManager)
  : GuiMenuBase(window, _("TATE SETTINGS"), nullptr)
  , mSystemManager(systemManager)
  , mOriginalTateEnabled(RecalboxConf::Instance().GetCollectionTate())
  , mOriginalGamesRotation(RotationUtils::FromUint(RecalboxConf::Instance().GetTateGameRotation()))
  , mOriginalSystemRotation(BootConf::Instance().GetRotation())
  , mOriginalTateOnly(RecalboxConf::Instance().GetTateOnly())
{
  // Enable virtual system
  AddSwitch(_("ENABLE TATE VIRTUAL SYSTEM"), mOriginalTateEnabled, (int)Components::TateEnabled, this, _(MENUMESSAGE_TATE_VIRTUALSYSTEM_MSG));

  class Filter : public IFilter
  {
    public:
      bool ApplyFilter(const FileData& file) override
      {
        return file.Metadata().Rotation() == RotationType::Left ||
               file.Metadata().Rotation() == RotationType::Right;
      }
  } filter;

  // Enable virtual system
  bool hasTateGames = false;
  for(const SystemData* system : systemManager.VisibleSystemList())
    if (system->MasterRoot().HasFilteredItemsRecursively(&filter))
    {
      hasTateGames = true;
      break;
    }
  if (hasTateGames)
    AddSwitch(_("DISPLAY ONLY TATE GAMES IN GAMELISTS"), mOriginalTateOnly, (int)Components::TateOnly, this, _(MENUMESSAGE_TATE_GAMELISTS_MSG));

  // Rotate games
  RotationCapability cap = Board::Instance().GetRotationCapabilities();

  if(cap.rotationAvailable)
  {
    if(cap.defaultRotationWhenTate != RotationType::None)
    {
      // We have default rotation for this board, so we allow only changing rotation to none
      bool isAuto = (! RecalboxConf::Instance().IsDefined(RecalboxConf::sTateGameRotation)) || mOriginalGamesRotation == cap.defaultRotationWhenTate;
      if(isAuto)
        mOriginalGamesRotation = cap.defaultRotationWhenTate;
      AddList<RotationType>(_("GAMES ROTATION"), (int)Components::TateGamesRotation, this,
                             std::vector<GuiMenuBase::ListEntry<RotationType>>(
                                 {
                                   { "AUTO", cap.defaultRotationWhenTate, isAuto },
                                   { "NONE", RotationType::None, !isAuto }
                                 }),
                             _(MENUMESSAGE_TATE_GAMESROTATION_MSG));
    }
    else
    {
      // Rotation
      AddList<RotationType>(_("GAMES ROTATION"), (int)Components::TateGamesRotation, this, GetRotationEntries(mOriginalGamesRotation), _(MENUMESSAGE_TATE_GAMESROTATION_MSG));
    }
    // Screen rotation
    if(cap.systemRotationAvailable)
      mSystemRotation = AddList<RotationType>(_("COMPLETE SYSTEM ROTATION"), (int)Components::TateCompleteSystemRotation, this, GetRotationEntries(mOriginalSystemRotation), _(MENUMESSAGE_TATE_SCREEN_ROTATION));
  }
}

GuiMenuTate::~GuiMenuTate()
{
  if (mSystemRotation && mSystemRotation->getSelected() != mOriginalSystemRotation)
    RequestRelaunch();
}

std::vector<GuiMenuBase::ListEntry<RotationType>> GuiMenuTate::GetRotationEntries(RotationType currentRotation)
{
  std::vector<GuiMenuBase::ListEntry<RotationType>> list;
  list.push_back({ _("NONE"), RotationType::None, currentRotation == RotationType::None });
  list.push_back({ _("LEFT"), RotationType::Left, currentRotation == RotationType::Left });
  list.push_back({ _("RIGHT"), RotationType::Right, currentRotation == RotationType::Right });
  list.push_back({ _("UPSIDEDOWN"), RotationType::Upsidedown, currentRotation == RotationType::Upsidedown });

  return list;
}

void GuiMenuTate::OptionListComponentChanged(int id, int index, const RotationType& value, bool quickChange)
{
  (void)quickChange;
  (void)index;
  if ((Components)id == Components::TateGamesRotation)
  {
    RecalboxConf::Instance().SetTateGameRotation((int)value).Save();
  }
  else if ((Components)id == Components::TateCompleteSystemRotation)
  {
    BootConf::Instance().SetRotation(value).Save();
  }
}

void GuiMenuTate::SwitchComponentChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::TateEnabled:
    {
      RecalboxConf::Instance().SetCollectionTate(status).Save();
      mSystemManager.UpdateVirtualSystemsVisibility(VirtualSystemType::Tate,
                                                    status ? SystemManager::Visibility::ShowAndSelect : SystemManager::Visibility::Hide);
      break;
    }
    case Components::TateOnly:
    {
      RecalboxConf::Instance().SetTateOnly(status);
      if (mSystemManager.UpdatedTopLevelFilter())
        RecalboxConf::Instance().Save();
      else
      {
        mWindow.displayMessage(_("There is no TATE game to show!No change recorded."));
        RecalboxConf::Instance().SetTateOnly(!status);
        status = !status;
      }
      break;
    }
    case Components::TateGamesRotation:
    case Components::TateCompleteSystemRotation:
      break;
  }
}

