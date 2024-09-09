#include <games/GameFilesUtils.h>
#include "MenuSoftpatchingLauncher.h"
#include "MenuTools.h"
#include "games/FileData.h"
#include <views/ViewController.h>
#include "components/ButtonComponent.h"
#include "utils/locale/LocaleHelper.h"

MenuSoftpatchingLauncher::MenuSoftpatchingLauncher(WindowManager& window,
                                                   FileData& game,
                                                   std::vector<Path>&& patches,
                                                   int lastChoice,
                                                   ISoftPatchingNotifier* notifier)
  : Menu(window, _("SOFTPATCHING"), GetFooter(game))
  , mNotifier(notifier)
  , mGame(game)
  , mPatches(patches)
  , mPreviousChoice(lastChoice)
{
}

void MenuSoftpatchingLauncher::BuildMenuItems()
{
  // Original button
  AddAction(_("original"), _("RUN"), (int)Components::Original, true, this, _("Run the original, unpatched game"));
  // Original button
  AddAction(_("patched"), _("RUN"), (int)Components::Original, true, this, _("Run the game, patched with the selected patch"));

  // Patch list
  mPatchList = AddList<Path>(_("select a patch"), (int) Components::Patch, this,GetPatchesEntries(), mGame.Metadata().LastPatch(), Path::Empty);

  SetSelectedItem(mPreviousChoice);
}

String MenuSoftpatchingLauncher::GetFooter(FileData& game)
{
  String gameName(game.Name());
  gameName.Append(" (").Append(game.RomPath().Filename()).Append(')');
  return _("GAME %s").Replace("%s", gameName.UpperCaseUTF8());
}

SelectorEntry<Path>::List MenuSoftpatchingLauncher::GetPatchesEntries()
{
  SelectorEntry<Path>::List list;
  for(auto& path : mPatches)
  {
    String patchName = path.Directory() == mGame.RomPath().Directory() ? path.Filename() + " (auto)" : path.Filename();
    list.push_back({ patchName, path });
  }
  return list;
}

void MenuSoftpatchingLauncher::MenuSingleChanged(int id, int index, const Path& value)
{
  (void)index;
  if ((Components)id == Components::Patch)
    mGame.Metadata().SetLastPatch(value);
}

void MenuSoftpatchingLauncher::MenuActionTriggered(int id)
{
  switch((Components)id)
  {
    case Components::Original:
    {
      Close();
      mGame.Metadata().SetLastPatch(Path("original"));
      if (mNotifier != nullptr)
        mNotifier->SoftPathingDisabled();
      break;
    }
    case Components::Patched:
    {
      Close();
      if (mNotifier != nullptr)
        mNotifier->SoftPatchingSelected(mPatchList->SelectedValue());
      break;
    }
    case Components::Patch:
    default: break;
  }
}














