//
// Created by gugue_u on 04/01/2023.
//

#include <components/ButtonComponent.h>
#include <components/MenuComponent.h>
#include <guis/GuiTextEditPopupKeyboard.h>
#include <views/ViewController.h>
#include "GuiSaveStates.h"
#include <games/GameFilesUtils.h>
#include <components/TextListComponent.h>

GuiSaveStates::GuiSaveStates(WindowManager& window, SystemManager& systemManager, FileData& game, const std::function<void(const std::string& slot)>& func, bool fromMenu)
  : Gui(window)
  , mSystemManager(systemManager)
  , mBackground(window, Path(":/frame.png"))
  , mGrid(window, Vector2i(5, 6))
  , mList(nullptr)
  , mGame(game)
  , mIsLibretro(mSystemManager.Emulators().GetGameEmulator(mGame).IsLibretro())
  , mFromMenu(fromMenu)
  , mCurrentState(Path(""))
  , mSort(Sort::Descending)
  , mFunc(func)
{
  addChild(&mBackground);
  addChild(&mGrid);

  std::shared_ptr<MenuTheme> mMenuTheme = MenuThemeData::getInstance()->getCurrentTheme();

  mBackground.setImagePath(mMenuTheme->menuBackground.path);
  mBackground.setCenterColor(mMenuTheme->menuBackground.color);
  mBackground.setEdgeColor(mMenuTheme->menuBackground.color);

  // Title
  std::shared_ptr<TextComponent> title = std::make_shared<TextComponent>(mWindow, _("SAVE STATES"), mMenuTheme->menuText.font, mMenuTheme->menuText.color, TextAlignment::Center);
  mGrid.setEntry(title, Vector2i(1, 1), false, true, Vector2i(3, 1));
  mTitleHeight = (title->getFont()->getLetterHeight() + Renderer::Instance().DisplayHeightAsFloat()*0.0437f);

  // Game name
  std::shared_ptr<TextComponent> gameName = std::make_shared<TextComponent>(mWindow, mGame.System().Descriptor().IconPrefix().Append(mGame.Name()), mMenuTheme->menuText.font, mMenuTheme->menuText.color, TextAlignment::Center);
  gameName->setUppercase(true);
  mGrid.setEntry(gameName, Vector2i(1, 2), false, true, Vector2i(3, 2));

  // Slot list
  mList = std::make_shared<TextListComponent<SaveState*>>(mWindow);
  //mList->applyTheme(ThemeData::getCurrent(), "basic", "gamelist", ThemeProperties::All);
  mList->setFont(mMenuTheme->menuText.font);
  mList->setSelectorHeight(mList->EntryHeight());
  mList->setHorizontalMargin(Renderer::Instance().DisplayWidthAsFloat() * 0.005f);
  mList->setAlignment(HorizontalAlignment::Left);
  mList->setSelectorColor(mMenuTheme->menuText.selectorColor);
  mList->setSelectedColor(mMenuTheme->menuText.selectedColor);
  mList->setColor(0, mMenuTheme->menuText.color);
  mList->setUppercase(true);
  mList->setCursorChangedCallback([this](CursorState) { updateInformations(); });
  mGrid.setEntry(mList, Vector2i(1, 4), true, true, Vector2i(1, 1));

  // Thumbnail
  mThumbnail = std::make_shared<ImageComponent>(mWindow);
  mThumbnail->setThemeDisabled(false);
  mGrid.setEntry(mThumbnail, Vector2i(3, 4), false, true, Vector2i(1, 1));

  // Set window properties
  const float height = Renderer::Instance().DisplayHeightAsFloat() * 0.75f;
  const float width = Renderer::Instance().DisplayWidthAsFloat() * (Renderer::Instance().Is240p() ? 0.95f : 0.85f);
  setSize(width, height);
  setPosition((Renderer::Instance().DisplayWidthAsFloat() - mSize.x()) / 2,
              (Renderer::Instance().DisplayHeightAsFloat() - mSize.y()) / 2);

  PopulateGrid();
}

void GuiSaveStates::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  // update grid row/col sizes
  mGrid.setColWidthPerc(0, 0.02f);
  mGrid.setColWidthPerc(1, Renderer::Instance().Is240p() ? 0.55 : 0.4f);
  mGrid.setColWidthPerc(2, 0.02f);
  mGrid.setColWidthPerc(3, 0.f); // Auto size
  mGrid.setColWidthPerc(4, 0.02f);

  mGrid.setRowHeightPerc(0, 0.02f);
  mGrid.setRowHeightPerc(1, mTitleHeight / mSize.y());
  mGrid.setRowHeightPerc(2, mTitleHeight / mSize.y());
  mGrid.setRowHeightPerc(3, 0.02f);
  mGrid.setRowHeightPerc(4, 0.f); // Auto size
  mGrid.setRowHeightPerc(5, 0.04f);

  mGrid.setSize(mSize);
}

bool GuiSaveStates::ProcessInput(const class InputCompactEvent & event)
{
  if (event.CancelPressed())
  {
    Close();
    return true;
  }
  else if (event.YPressed())
  {
    launch(Strings::Empty);
    return true;
  }
  else if (event.ValidPressed() && mIsLibretro && mCurrentState.GetPath().Extension() != ".auto" && mList->size() != 0)
  {
    if (mCurrentState.GetSlotNumber() == -2)
    {
      launch(Strings::Empty);
      return true;
    }

    std::string slot = Strings::ToString(mCurrentState.GetSlotNumber());
    launch(slot);

    return true;
  }
  else if (event.SelectPressed())
  {
    if(mSort == Sort::Ascending) mSort = Sort::Descending;
    else if(mSort == Sort::Descending) mSort = Sort::Ascending;
    PopulateGrid();
  }
  else if (event.XPressed())
  {
    std::string message = _("Game").append(": ").append(mGame.Name()).append("\n");
    message.append(_("You are about to delete this state, confirm ?"));
    message.append("\n\n");
    message.append(mCurrentState.GetPath().Filename());

    auto* msg = new GuiMsgBoxScroll(mWindow, _("DELETE STATE, CONFIRM?"), message,
                                   _("YES"), [this]
                                   {
                                     Delete();
                                     PopulateGrid();
                                   },
                                   _("BACK"), {}, "", nullptr, TextAlignment::Left);

    msg->SetDefaultButton(1);

    mWindow.pushGui(msg);
    return true;
  }

  return Component::ProcessInput(event);
}

bool GuiSaveStates::getHelpPrompts(Help& help)
{
  help.Clear();
  std::string sort = mSort == Sort::Descending ? "ASC" : "DESC";

  help.Set(HelpType::UpDown, _("CHOOSE"))
      .Set(Help::Cancel(), _("BACK"))
      .Set(HelpType::Select, _("CHANGE ORDER") + " " + sort);

  if (!mList->isEmpty())
  {
    if (mIsLibretro && mCurrentState.GetPath().Extension() != ".auto")
      help.Set(Help::Valid(), _("LAUNCH GAME FROM STATE"));
    help.Set(HelpType::X, _("DELETE STATE SLOT"));
  }
  return true;
}

void GuiSaveStates::Update(int deltaTime)
{
  Component::Update(deltaTime);
}

void GuiSaveStates::Render(const Transform4x4f& parentTrans)
{
  Transform4x4f trans = parentTrans * getTransform();

  renderChildren(trans);

  Renderer::SetMatrix(trans);
  Renderer::DrawRectangle(mGrid.getPosition().x() + mGrid.getColWidth(0),
                          mGrid.getPosition().y() + mGrid.getRowHeight(0, 3),
                          mGrid.getColWidth(1), mGrid.getRowHeight(4), 0x00000018);
  Renderer::DrawRectangle(mGrid.getPosition().x() + mGrid.getColWidth(0, 1),
                          mGrid.getPosition().y() + mGrid.getRowHeight(0, 3),
                          mGrid.getColWidth(2, 3), mGrid.getRowHeight(4), 0xFFFFFF18);
}

void GuiSaveStates::PopulateGrid()
{
  mSaveStates = GameFilesUtils::GetGameSaveStateFiles(mGame);
  std::sort(mSaveStates.begin(), mSaveStates.end(), mSort == Sort::Ascending ? GuiSaveStates::AscendingComparer : GuiSaveStates::DescendingComparer);

  int oldPosition = mList->isEmpty() ? -1 : mList->getCursorIndex();
  mList->clear();

  mList->add(_("none"), nullptr, 0);

  int index = 0;
  for (auto& state : mSaveStates)
  {
    String text = _("SLOT");
    if (state.GetIsAuto()) text.Append(' ').Append(_("AUTO"));
    else text.Append('#').Append(state.GetSlotNumber()).Append(" - ").Append(state.GetDateTime().ToStringFormat("%YYYY/%MM/%dd %HH:%mm:%ss"));
    mList->add(text, &mSaveStates[index++], 0);
  }
  mList->setCursorIndex(oldPosition >= 0 ? oldPosition : (mList->size() > 1 ? 1 : 0));

  updateInformations();
}

//called when changing cursor in mList to populate MD
void GuiSaveStates::updateInformations()
{
  mThumbnail->setImage(Path::Empty);
  if (mList->isEmpty()) return;

  mThumbnail->setMaxSize(mGrid.getColWidth(3), mGrid.getRowHeight(4));
  mThumbnail->setOrigin(0.5f, 0.5f);
  mThumbnail->setPosition(mGrid.getPosition().x() + mGrid.getColWidth(0, 2) + mGrid.getColWidth(3) / 2.f,
                          mGrid.getPosition().y() + mGrid.getRowHeight(0, 3) + mGrid.getRowHeight(4) / 2.f);

  int index = mList->getCursorIndex();
  mCurrentState = index == 0 ? SaveState(Path::Empty) : mSaveStates[index - 1];
  mThumbnail->setImage(index == 0 ? Path(":/no_image.png") : mCurrentState.GetThrumbnail());

  updateHelpPrompts();
}

void GuiSaveStates::launch(const String& slot)
{
  if (mFromMenu)
  {
    Vector3f target(Renderer::Instance().DisplayWidthAsFloat() / 2.0f,
                      Renderer::Instance().DisplayHeightAsFloat() / 2.0f, 0);
    GameLinkedData gameLinkedData = GameLinkedData();
    gameLinkedData.ConfigurableSaveState().SetSlotNumber(slot);
    ViewController::Instance().Launch(&mGame, gameLinkedData, target);
  }
  else
  {
    mFunc(slot);
    Close();
  }
}

void GuiSaveStates::Delete()
{
  mCurrentState.GetPath().Delete();
  mCurrentState.GetThrumbnail().Delete();
  updateHelpPrompts();
  { LOG(LogDebug) << "[SAVESTATE] " << mCurrentState.GetPath().Filename() << " slot has been deleted"; }
}

bool GuiSaveStates::AscendingComparer(const SaveState& first, const SaveState& second)
{
  if (first.GetIsAuto())  return true;
  if (second.GetIsAuto()) return false;
  return first.GetSlotNumber() < second.GetSlotNumber();
}

bool GuiSaveStates::DescendingComparer(const SaveState& first, const SaveState& second)
{
  if(first.GetIsAuto())  return true;
  if(second.GetIsAuto()) return false;
  return first.GetSlotNumber() > second.GetSlotNumber();
}

//non compatibl libretro core pi4
//libretro-atari800
//libretro-easyrpg
//libretro-gw
//libretro-lowresnx
//libretro-lutro
//libretro-mame2010
//libretro-minivmac
//libretro-px68k
//libretro-retro8
//libretro-virtualjaguar
