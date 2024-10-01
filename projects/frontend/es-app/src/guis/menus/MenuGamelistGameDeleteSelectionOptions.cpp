#include <guis/menus/base/ItemSelector.h>
#include <games/GameFilesUtils.h>
#include "MenuGamelistGameDeleteSelectionOptions.h"
#include "guis/GuiMsgBoxScroll.h"
#include <views/ViewController.h>
#include <guis/GuiMsgBox.h>

MenuGamelistGameDeleteSelectionOptions::MenuGamelistGameDeleteSelectionOptions(WindowManager& window, ISimpleGameListView& view, FileData& game)
  : Menu(window, _("SELECT FILES TO DELETE"), GetFooter(game))
  , mView(view)
  , mGame(game)
{
  if (!mGame.IsGame()) Close();
}

void MenuGamelistGameDeleteSelectionOptions::BuildMenuItems()
{
  // Delete
  mGameFiles = AddMultiList<Path>(_("GAME FILES (ROM | DISK IMAGE)"), (int) Components::Path, nullptr, GetGameFileEntries());
  mMedias = AddMultiList<Path>(_("MEDIA FILES"), (int) Components::Path, nullptr, GetMediaEntries());
  mExtras = AddMultiList<Path>(_("CONFIGURATION AND PATCH FILES"), (int) Components::Path, nullptr, GetExtraEntries());
  mSaves = AddMultiList<Path>(_("SAVE FILES"), (int) Components::Path, nullptr, GetSaveEntries());

  AddAction(_("DELETE SELECTED FILES"), _("DELETE"), (int)Components::Delete, true, this);
  AddAction(_("DON'T DELETE ANYTHING!"), _("BACK"), (int)Components::Cancel, false, this);
}

String MenuGamelistGameDeleteSelectionOptions::GetFooter(FileData& game)
{
  String gameName(game.Name());
  gameName.Append(" (").Append(game.RomPath().Filename()).Append(')');
  return _("GAME %s").Replace("%s", gameName.UpperCaseUTF8());
}

SelectorEntry<Path>::List MenuGamelistGameDeleteSelectionOptions::GetGameFileEntries()
{
  SelectorEntry<Path>::List list;
  list.push_back({ mGame.RomPath().Filename(),  mGame.RomPath(), false });
  for (const auto& file : GameFilesUtils::GetGameSubFiles(mGame))
  {
    Path path = Path(file);
    list.push_back({ path.Filename(), path, false });
  }
  return list;
}

SelectorEntry<Path>::List MenuGamelistGameDeleteSelectionOptions::GetMediaEntries()
{
  SelectorEntry<Path>::List list;
  if (mGame.Metadata().Image().Exists())     list.push_back({ _("Image"), mGame.Metadata().Image(), false });
  if (mGame.Metadata().Thumbnail().Exists()) list.push_back({ _("Thumbnail"), mGame.Metadata().Thumbnail(), false });
  if (mGame.Metadata().Video().Exists())     list.push_back({ _("Video"), mGame.Metadata().Video(), false });
  return list;
}

SelectorEntry<Path>::List MenuGamelistGameDeleteSelectionOptions::GetExtraEntries()
{
  SelectorEntry<Path>::List list;
  for (const auto& patch : GameFilesUtils::GetGameExtraFiles(mGame))
  {
    Path path = Path(patch);
      list.push_back({ path.Filename(), path, false });
  }
  return list;
}

SelectorEntry<Path>::List MenuGamelistGameDeleteSelectionOptions::GetSaveEntries()
{
  SelectorEntry<Path>::List list;
  //std::vector<ListEntry<Path>> mediaList;
  for (const auto& patch : GameFilesUtils::GetGameSaveFiles(mGame))
  {
    Path path = Path(patch);
    list.push_back({ path.Filename(), path, false });
  }
  return list;
}

void MenuGamelistGameDeleteSelectionOptions::DeleteSelectedFiles()
{
  HashSet<String> list;
  HashSet<String> mediaList;

  bool mainGameDeleted = false;

  for(const Path& path : mGameFiles->AllSelectedValues())
  {
    if(path == mGame.RomPath())
    {
      mainGameDeleted = true;
    }
    list.insert(path.ToString());
  }

  for(const Path& path : mExtras->AllSelectedValues())
  {
    list.insert(path.ToString());
  }
  for(const Path& path : mSaves->AllSelectedValues())
  {
    list.insert(path.ToString());
  }

  for(const Path& path : mMedias->AllSelectedValues())
  {
    mediaList.insert(path.ToString());
  }

  GameFilesUtils::DeleteSelectedFiles(mGame, list, mediaList);
  if(mainGameDeleted)
  {
    mView.removeEntry(&mGame);
  }
  mWindow.CloseAll();
}

String MenuGamelistGameDeleteSelectionOptions::ComputeMessage()
{
  String message = _("Game").Append(": ").Append(mGame.Name()).Append("\n\n");

  if(mGameFiles->SelectedCount() != 0)
  {
    message.Append(_("GAME FILES (ROM | DISK IMAGE)").Append('\n'));
    for (const Path& path: mGameFiles->AllSelectedValues())
      message.Append(path.Filename()).Append('\n');
  }
  if(mExtras->SelectedCount() != 0)
  {
    message.Append('\n').Append(_("CONFIGURATION AND PATCH FILES")).Append('\n');
    for (const Path& path: mExtras->AllSelectedValues())
      message.Append(path.Filename()).Append('\n');
  }
  if(mSaves->SelectedCount() != 0)
  {
    message.Append('\n').Append(_("SAVE FILES")).Append('\n');
    for (const Path& path: mSaves->AllSelectedValues())
      message.Append(path.Filename()).Append('\n');
  }
  if(mMedias->SelectedCount())
  {
    message.Append('\n').Append(_("MEDIA FILES")).Append('\n');
    for (const Path& path: mMedias->AllSelectedValues())
      message.Append(path.Filename()).Append('\n');
  }

  return message;
}

void MenuGamelistGameDeleteSelectionOptions::MenuActionTriggered(int id)
{
  switch((Components)id)
  {
    case Components::Delete:
    {
      if (mGameFiles->SelectedCount() == 0 && mSaves->SelectedCount()== 0 && mExtras->SelectedCount() == 0 && mMedias->SelectedCount() == 0)
        mWindow.pushGui((new GuiMsgBox(mWindow, _("No file selected,\nyou must at least choose one."), _("BACK"), TextAlignment::Left)));
      else
        mWindow.pushGui((new GuiMsgBoxScroll(mWindow, _("DELETE SELECTED FILES, CONFIRM?"), ComputeMessage(), _("YES"), [this] { DeleteSelectedFiles(); }, _("BACK"), {}, "", nullptr, TextAlignment::Left))->SetDefaultButton(1));
      break;
    }
    case Components::Cancel: Close(); break;
    case Components::Path:
    default: break;
  }
}


