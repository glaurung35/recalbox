//
// Created by digitalLumberjack on 20/06/2022.
//

#include <recalbox/RecalboxSystem.h>
#include "PiBoard.h"
#include "hardware/ISpecialGlobalActions.h"

bool PiBoard::OnRebootOrShutdown() {
  if (Path("/sys/class/leds/retroflagled/trigger").Exists())
  {
    RecalboxSystem::execute("echo 'heartbeat' > /sys/class/leds/retroflagled/trigger");
    return true;
  }
  return false;
}

bool PiBoard::ProcessSpecialInputs(InputCompactEvent& inputEvent, ISpecialGlobalAction* action)
{
  if ((Case::CurrentCase().Model() == Case::CaseModel::PiBoyXRS || Case::CurrentCase().Model() == Case::CaseModel::PiBoyDMG) &&
      inputEvent.RawEvent().Type() == InputEvent::EventType::Button && inputEvent.RawEvent().Id() == 10)
  /*if (inputEvent.Hotkey())*/
    if (action != nullptr)
    {
      if (inputEvent.RawEvent().Value() != 0) action->EnableOSDImage(Path(":/help/piboy_xrs_menu.png"), 0.f, 0.f, 1.f, 1.f, 0.8f, true);
      else action->DisableOSDImage();
      return true;
    }
  return false;
}
