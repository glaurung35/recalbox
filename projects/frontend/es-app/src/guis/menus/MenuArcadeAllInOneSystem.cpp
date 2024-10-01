#include <RecalboxConf.h>
#include <utils/locale/LocaleHelper.h>
#include <systems/SystemManager.h>
#include "MenuArcadeAllInOneSystem.h"
#include <guis/MenuMessages.h>

MenuArcadeAllInOneSystem::MenuArcadeAllInOneSystem(WindowManager& window, SystemManager& systemManager)
  : Menu(window, _("ARCADE VIRTUAL SYSTEM"))
  , mSystemManager(systemManager)
{
}

void MenuArcadeAllInOneSystem::BuildMenuItems()
{
  // Enable arcade
  AddSwitch(_("ENABLE ARCADE VIRTUAL SYSTEM"), RecalboxConf::Instance().GetCollectionArcade(), (int)Components::ArcadeOnOff, this, _(MENUMESSAGE_UI_ARCADE_AIO_VIRTUALSYSTEM_MSG));

  // Include neogeo?
  AddSwitch(_("INCLUDE NEO-GEO"), RecalboxConf::Instance().GetCollectionArcadeNeogeo(), (int)Components::IncludeNeogeo, this, _(MENUMESSAGE_UI_ARCADE_AIO_NEOGEO_MSG));

  // Hide original systems?
  AddSwitch(_("HIDE ORIGINAL SYSTEMS"), RecalboxConf::Instance().GetCollectionArcadeHideOriginals(), (int)Components::HideOriginals, this, _(MENUMESSAGE_UI_ARCADE_AIO_HIDEORIGINALS_MSG));
}

void MenuArcadeAllInOneSystem::MenuSwitchChanged(int id, bool& status)
{
  switch((Components)id)
  {
    case Components::ArcadeOnOff:
    {
      RecalboxConf::Instance().SetCollectionArcade(status).Save();
      mSystemManager.UpdateSystemsVisibility(mSystemManager.VirtualSystemByType(VirtualSystemType::Arcade),
                                             status ? SystemManager::Visibility::ShowAndSelect : SystemManager::Visibility::Hide);
      mSystemManager.ManageArcadeVirtualSystem();
      break;
    }
    case Components::IncludeNeogeo:
    {
      RecalboxConf::Instance().SetCollectionArcadeNeogeo(status).Save();
      mSystemManager.ManageArcadeVirtualSystem();
      break;
    }
    case Components::HideOriginals:
    {
      RecalboxConf::Instance().SetCollectionArcadeHideOriginals(status).Save();
      mSystemManager.ManageArcadeVirtualSystem();
      break;
    }
    default: break;
  }
}

