//
// Created by bkg2k on 28/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "MenuGamelistGameOptions.h"
#include <guis/MenuMessages.h>
#include <guis/GuiScraperSingleGameRun.h>
#include "MenuTools.h"
#include <LibretroRatio.h>
#include "games/GameFilesUtils.h"
#include <guis/menus/base/ItemText.h>
#include <guis/menus/base/ItemSwitch.h>
#include <guis/menus/base/ItemEditable.h>
#include <guis/menus/base/ItemRating.h>
#include <views/ViewController.h>

MenuGamelistGameOptions::MenuGamelistGameOptions(WindowManager& window, ISimpleGameListView& view, SystemManager& systemManager, SystemData& system, FileData& game)
  : Menu(window, _("GAME OPTIONS"), GetFooter(game))
  , mView(view)
  , mSystemManager(systemManager)
  , mSystem(system)
  , mGame(game)
{
  // Run width
  if (mGame.IsGame())
  {
    String emulatorAndCore;
    mEmulator = AddList<String>(_("RUN WITH"), (int) Components::Emulator, this, GetEmulatorEntries(emulatorAndCore),
                                emulatorAndCore, emulatorAndCore, _(MENUMESSAGE_ADVANCED_EMU_EMU_HELP_MSG));
  }

  // Patch width
  if (mGame.IsGame() && !GameFilesUtils::GetSoftPatches(&mGame).empty())
    mPath = AddList<Path>(_("SOFTPATCHING"), (int)Components::Patch, this, GetPatchEntries(), mGame.Metadata().LastPatch(), Path("original"), _(MENUMESSAGE_GAME_SOFTPATCHING_HELP_MSG));

  // Ratio
  if (mGame.IsGame())
    mRatio = AddList<String>(_("Ratio"), (int)Components::Ratio, this, GetRatioEntries(), mGame.Metadata().Ratio(), "auto", _(MENUMESSAGE_GAME_RATIO_HELP_MSG));

  // Game name
  mName = AddEditable(_("Name"), mGame.Metadata().Name(), (int)Components::Name, this, _(MENUMESSAGE_GAME_NAME_HELP_MSG), false);

  // Rating
  if (mGame.IsGame())
    mRating = AddRating(_("Rating"), mGame.Metadata().Rating(), (int)Components::Rating, this, _(MENUMESSAGE_GAME_RATING_HELP_MSG));

  // Normalized genre
  if (mGame.IsGame())
    mGenre = AddList<GameGenres>(_("Genre"), (int)Components::Genre, this, GetGenreEntries(), mGame.Metadata().GenreId(), GameGenres::None, _(MENUMESSAGE_GAME_GENRE_HELP_MSG));

  // Description
  mDescription = AddEditable(_("Description"), mGame.Metadata().Description(), (int)Components::Description, this, _(MENUMESSAGE_GAME_DESCRIPTION_HELP_MSG), false);

  // Favorite
  if (mGame.IsGame())
    mFavorite = AddSwitch(_("Favorite"), mGame.Metadata().Favorite(), (int)Components::Favorite, this, _(MENUMESSAGE_GAME_FAVORITE_HELP_MSG));

  // Hidden
  mHidden = AddSwitch(_("Hidden"), mGame.Metadata().Hidden(), (int)Components::Hidden, this, _(MENUMESSAGE_GAME_HIDDEN_HELP_MSG));

  // Adult
  if (mGame.IsGame())
    mAdult = AddSwitch(_("Adult"), mGame.Metadata().Adult(), (int)Components::Adult, this, _(MENUMESSAGE_GAME_ADULT_HELP_MSG));
  // Adult
  if (mGame.IsGame())
    mRotation = AddSwitch(_("Rotation"), mGame.Metadata().Rotation() != RotationType::None, (int)Components::Rotation, this, _(MENUMESSAGE_GAME_ROTATION_HELP_MSG));

  // Scrape
  if (mGame.IsGame())
    AddAction(_("SCRAPE"), _("RUN"), (int)Components::Scrape, true, this, _(MENUMESSAGE_GAME_SCRAP_HELP_MSG));

  //  _N("%i GAME HIDDEN", "%i GAMES HIDDEN", data.Hidden)) .Replace("%i", String(data.Hidden)
  if (mGame.IsGame() )
  {
    int timePlayed = mGame.Metadata().TimePlayed();
    if (timePlayed >= 3600)
    {
      int hours = timePlayed / 3600;
      AddText(_("TIME PLAYED"), _N("%i HOUR", "%i HOURS", hours).Replace("%i", String(hours)));
    }
    else if (timePlayed >= 60)
    {
      int minutes = timePlayed / 60;
      AddText(_("TIME PLAYED"), _N("%i MINUTE", "%i MINUTES", minutes).Replace("%i", String(minutes)));
    }
  }
}

String MenuGamelistGameOptions::GetFooter(FileData& file)
{
  if (file.IsGame())
  {
    String gameName(file.Name());
    gameName.Append(" (").Append(file.RomPath().Filename()).Append(')');
    return _("GAME %s").Replace("%s", gameName.UpperCaseUTF8());
  }
  else if (file.IsFolder())
    return _("FOLDER %s").Replace("%s", file.Name().ToUpperCaseUTF8());
  return String::Empty;
}

MenuGamelistGameOptions::~MenuGamelistGameOptions()
{
  if(mGame.Metadata().IsDirty())
    mSystem.UpdateGamelistXml();
}

SelectorEntry<String>::List MenuGamelistGameOptions::GetRatioEntries()
{
  SelectorEntry<String>::List list;

  String currentRatio = mGame.Metadata().Ratio();
  for (const auto& ratio : LibretroRatio::GetRatio())
    list.push_back({ ratio.first, ratio.second });

  return list;
}

SelectorEntry<GameGenres>::List MenuGamelistGameOptions::GetGenreEntries()
{
  SelectorEntry<GameGenres>::List list;
  for(GameGenres genre : Genres::GetOrderedList())
    list.push_back({ Genres::GetFullName(genre), genre });

  return list;
}

SelectorEntry<String>::List MenuGamelistGameOptions::GetEmulatorEntries(String& emulatorAndCore)
{
  SelectorEntry<String>::List list;

  String currentEmulator(mGame.Metadata().Emulator());
  String currentCore    (mGame.Metadata().Core());
  MenuTools::EmulatorAndCoreList eList =
    MenuTools::ListGameEmulatorAndCore(mGame, mDefaultEmulator, mDefaultCore, currentEmulator, currentCore);
  for (const MenuTools::EmulatorAndCore& emulator : eList)
  {
    list.push_back({ emulator.Displayable, emulator.Identifier });
    if (emulator.Selected) emulatorAndCore = emulator.Identifier;
  }

  return list;
}

SelectorEntry<Path>::List MenuGamelistGameOptions::GetPatchEntries()
{
  SelectorEntry<Path>::List list;
  Path::PathList patches = GameFilesUtils::GetSoftPatches(&mGame);

  list.push_back({ _("original"), Path("original") });
  for(auto& path : patches)
  {
    String patchName = path.Directory() == mGame.RomPath().Directory() ? path.Filename() + " (auto)" : path.Filename();
    list.push_back({ patchName, path });
  }
  return list;
}

void MenuGamelistGameOptions::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  if ((Components)id == Components::Emulator)
  {
    mGame.Metadata().SetEmulator(String::Empty);
    mGame.Metadata().SetCore(String::Empty);
    // Split emulator & core
    String emulator;
    String core;
    if (value.Extract(':', emulator, core, false))
      if (emulator != mDefaultEmulator || core != mDefaultCore)
      {
        mGame.Metadata().SetEmulator(emulator);
        mGame.Metadata().SetCore(core);
      }
  }
  else if ((Components)id == Components::Ratio)
    mGame.Metadata().SetRatio(value);
}

void MenuGamelistGameOptions::MenuSingleChanged(int id, int index, const Path& value)
{
  (void)index;
  if ((Components)id == Components::Patch)
    mGame.Metadata().SetLastPatch(value);
}

void MenuGamelistGameOptions::MenuSingleChanged(int id, int index, const GameGenres& value)
{
  (void)index;
  if ((Components)id == Components::Genre)
    mGame.Metadata().SetGenreId(value);
}

void MenuGamelistGameOptions::MenuEditableChanged(int id, const String& text)
{
  if ((Components)id == Components::Name)
    mGame.Metadata().SetName(text);
  else if ((Components)id == Components::Description)
    mGame.Metadata().SetDescription(text);
}

void MenuGamelistGameOptions::MenuSwitchChanged(int id, bool& status)
{
  MetadataType updatedMetadata = MetadataType::None;
  switch((Components)id)
  {
    case Components::Favorite:
    {
      ViewController::Instance().ToggleFavorite(&mGame, true, status);
      updatedMetadata = MetadataType::Favorite;
      break;
    }
    case Components::Rotation:
    {
      mGame.Metadata().SetRotation(status ? RotationType::Left : RotationType::None);
      updatedMetadata = MetadataType::Rotation;
      break;
    }
    case Components::Hidden:
    {
      mGame.Metadata().SetHidden(status);
      updatedMetadata = MetadataType::Hidden;
      break;
    }
    case Components::Adult:
    {
      mGame.Metadata().SetAdult(status);
      updatedMetadata = MetadataType::Adult;
      break;
    }
    case Components::Name:
    case Components::Description:
    case Components::Rating:
    case Components::Genre:
    case Components::Scrape:
    case Components::Ratio:
    case Components::Emulator:
    case Components::Patch:
      break;
  }
  if (updatedMetadata != MetadataType::None)
    mSystemManager.UpdateSystemsOnGameChange(&mGame, updatedMetadata, false);
}

void MenuGamelistGameOptions::MenuRatingMoved(int id, float value)
{
  if ((Components)id == Components::Rating)
    mGame.Metadata().SetRating(value);
}

void MenuGamelistGameOptions::MenuActionTriggered(int id)
{
  if ((Components)id == Components::Scrape)
    mWindow.pushGui(new GuiScraperSingleGameRun(mWindow, mSystemManager, mGame, this));
}

void MenuGamelistGameOptions::ScrapingComplete(FileData& game, MetadataType changedMetadata)
{
  (void)changedMetadata;

  // Refresh menu
  if ((changedMetadata & MetadataType::Name) != 0) mName->SetText(game.Metadata().Name());
  if ((changedMetadata & MetadataType::Rating) != 0) mRating->SetValue(game.Metadata().Rating(), false);
  if ((changedMetadata & MetadataType::GenreId) != 0) mGenre->SetSelectedItemValue(game.Metadata().GenreId(), false);
  if ((changedMetadata & MetadataType::Synopsis) != 0) mDescription->SetText(game.Metadata().Description());
  if ((changedMetadata & MetadataType::Adult) != 0) mAdult->SetState(game.Metadata().Adult(), false);
  if ((changedMetadata & MetadataType::Rotation) != 0) mRotation->SetState(game.Metadata().Rotation() != RotationType::None, false);
}
