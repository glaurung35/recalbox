#include <games/GameFilesUtils.h>
#include "MenuGamelistGameDeleteOptions.h"
#include "MenuGamelistGameDeleteSelectionOptions.h"
#include "guis/MenuMessages.h"
#include "guis/GuiMsgBoxScroll.h"

MenuGamelistGameDeleteOptions::MenuGamelistGameDeleteOptions(WindowManager& window, ISimpleGameListView& view, FileData& game)
  : Menu(window, _("DELETE ALL FILES"), GetFooter(game))
  , mView(view)
  , mGame(game)
{
  if (!mGame.IsGame()) Close();
}

void MenuGamelistGameDeleteOptions::BuildMenuItems()
{
  // Delete
  mGameFiles = GameFilesUtils::GetGameSubFiles(mGame);
  String fileCount = _N("%i file", "%i files", (int) mGameFiles.size() + 1).Replace("%i", String((int) mGameFiles.size() + 1));
  AddText(_("GAME FILES (ROM | DISK IMAGE)"), fileCount, _(MENUMESSAGE_GAMEDELETION_GAMEFILES_HELP_MSG));

  mMediaFiles = GameFilesUtils::GetMediaFiles(mGame);
  if(!mMediaFiles.empty())
  {
    fileCount = _N("%i file", "%i files", (int) mMediaFiles.size()).Replace("%i",String((int) mMediaFiles.size()));
    AddText(_("MEDIA FILES"), fileCount, _(MENUMESSAGE_GAMEDELETION_MEDIAFILES_HELP_MSG));
  }

  mExtraFiles = GameFilesUtils::GetGameExtraFiles(mGame);
  if(!mExtraFiles.empty())
  {
    fileCount = _N("%i file", "%i files", (int) GameFilesUtils::GetGameExtraFiles(mGame).size()).Replace("%i", String((int) GameFilesUtils::GetGameExtraFiles(mGame).size()));
    AddText(_("CONFIGURATION AND PATCH FILES"), fileCount, _(MENUMESSAGE_GAMEDELETION_CONFIGURATION_HELP_MSG));
  }

  mSaveFiles = GameFilesUtils::GetGameSaveFiles(mGame);
  if(!mSaveFiles.empty())
  {
    fileCount = _N("%i file", "%i files", (int) mSaveFiles.size()).Replace("%i", String((int) mSaveFiles.size()));
    AddText(_("SAVE FILES"), fileCount, _(MENUMESSAGE_GAMEDELETION_SAVES_HELP_MSG));
  }

  AddSubMenu(_("ADVANCED DELETE"),  (int)Components::Advanced, this, _(MENUMESSAGE_GAMEDELETION_ADVANCED_HELP_MSG));

  // Do delete actually
  AddAction(_("DELETE ALL FILES"), _("DELETE"), (int)Components::DoDelete, true, this);
  AddAction(_("DON'T DELETE ANYTHING!"), _("CANCEL"), (int)Components::DoNotDelete, false, this);
}

String MenuGamelistGameDeleteOptions::GetFooter(FileData& game)
{
  String gameName(game.Name());
  gameName.Append(" (").Append(game.RomPath().Filename()).Append(')');
  return _("GAME %s").Replace("%s", gameName.ToUpperCaseUTF8());
}

void MenuGamelistGameDeleteOptions::DeleteAllFiles()
{
  GameFilesUtils::DeleteAllFiles(mGame);
  mView.removeEntry(&mGame);
  mWindow.CloseAll();
}

void MenuGamelistGameDeleteOptions::SubMenuSelected(int id)
{
  switch((Components)id)
  {
    case Components::Advanced:
      mWindow.pushGui(new MenuGamelistGameDeleteSelectionOptions(mWindow, mView, mGame));
    case Components::Delete:
    case Components::DoDelete:
    case Components::DoNotDelete:
    default: break;
  }
}

String MenuGamelistGameDeleteOptions::ComputeMessage()
{
  String message = _("Game").Append(": ").Append(mGame.Name()).Append('\n');
  message.Append(_("You are about to delete this files, confirm ?"));
  message.Append("\n\n");
  message.Append(mGame.RomPath().Filename()).Append('\n');

  for(const auto& path : mGameFiles)
    message.Append(Path(path).Filename()).Append('\n');

  if(!mExtraFiles.empty())
    for (const auto& path: mExtraFiles)
      message.Append(Path(path).Filename()).Append('\n');

  if(!mSaveFiles.empty())
    for (const auto& path: mSaveFiles)
      message.Append(Path(path).Filename()).Append('\n');

  if(!mMediaFiles.empty())
    for (const auto& path: mMediaFiles)
    {
      if (path.Contains("/media/images/"))
        message.Append(_("Image")).Append('\n');
      if (path.Contains("/media/thumbnails/"))
        message.Append(_("Thumbnail")).Append('\n');
      if (path.Contains("/media/videos/"))
        message.Append(_("Video")).Append('\n');
    }

  return message;
}

void MenuGamelistGameDeleteOptions::MenuActionTriggered(int id)
{
  if (id == (int)Components::DoDelete)
    mWindow.pushGui((new GuiMsgBoxScroll(mWindow, _("DELETE ALL FILES, CONFIRM?"), ComputeMessage(), _("YES"),
                                         [this]{ DeleteAllFiles(); }, _("NO"), {}, "", nullptr, TextAlignment::Left))->SetDefaultButton(1));
  if (id == (int)Components::DoNotDelete)
    Close();
}
