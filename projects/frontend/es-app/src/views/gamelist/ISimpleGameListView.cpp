#include "views/gamelist/ISimpleGameListView.h"
#include <systems/SystemManager.h>
#include <guis/GuiControlHints.h>
#include <guis/menus/MenuNetPlayHostPasswords.h>
#include "guis/menus/MenuGamelistOptions.h"
#include "views/ViewController.h"
#include "RotationManager.h"
#include "views/MenuFilter.h"
#include <usernotifications/NotificationManager.h>

ISimpleGameListView::ISimpleGameListView(WindowManager& window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver, PictogramCaches& flagCache)
  : Gui(window)
  , mPictogramCaches(flagCache)
  , mResolver(resolver)
  , mSystem(system)
  , mSystemManager(systemManager)
  , mHeaderText(window)
  , mHeaderImage(window)
  , mBackground(window)
  , mThemeExtras(window)
  , mVerticalMove(false)
  , mListRefreshRequired(false)
{
  setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());

  mHeaderText.setText("Logo Text");
  mHeaderText.setSize(mSize.x(), 0);
  mHeaderText.setPosition(0, 0);
  mHeaderText.setHorizontalAlignment(TextAlignment::Center);
  mHeaderText.setDefaultZIndex(50);

  mHeaderImage.setResize(0, mSize.y() * 0.185f);
  mHeaderImage.setOrigin(0.5f, 0.0f);
  mHeaderImage.setPosition(mSize.x() / 2, 0);
  mHeaderImage.setDefaultZIndex(50);

  mBackground.setResize(mSize.x(), mSize.y());
  mBackground.setDefaultZIndex(0);

  addChild(&mHeaderText);
  addChild(&mBackground);
}

void ISimpleGameListView::updateInfoPanel()
{
    if (IsEmpty()) return;

    NotificationManager::Instance().Notify(*getCursor(), Notification::GamelistBrowsing);
    OnGameSelected();
}

void ISimpleGameListView::SwitchToTheme(const ThemeData& theme, bool refreshOnly, IThemeSwitchTick* interface)
{
  mBackground.DoApplyThemeElement(theme, getName(), "background", ThemePropertyCategory::All);
  mHeaderImage.DoApplyThemeElement(theme, getName(), "logo", ThemePropertyCategory::All);
  mHeaderText.DoApplyThemeElement(theme, getName(), "logoText", ThemePropertyCategory::All);

  // Remove old theme extras
  for (ThemeExtras::Extra& extra : mThemeExtras.Extras())
    removeChild(&extra.Component());
  // Add new theme extras
  mThemeExtras.AssignExtras(theme, getName(), theme.GetExtras(getName(), mWindow, nullptr), refreshOnly);
  for (ThemeExtras::Extra& extra : mThemeExtras.Extras())
    addChild(&extra.Component());

  if (mHeaderImage.hasImage())
  {
    removeChild(&mHeaderText);
    addChild(&mHeaderImage);
  }
  else
  {
    addChild(&mHeaderText);
    removeChild(&mHeaderImage);
  }
  (void)interface;
}

void ISimpleGameListView::onChanged(Change change)
{
  (void)change;

  // Refresh list
  if (RecalboxConf::Instance().AsBool(mSystem.Name() + ".flatfolder")) populateList(mSystem.MasterRoot());
  else refreshList();

  // And refresh game info
  updateInfoPanel();
}

bool ISimpleGameListView::ProcessInput(const InputCompactEvent& event)
{
  bool hideSystemView = RecalboxConf::Instance().GetStartupHideSystemView();
  FileData* cursor = getCursor();

  // RUN GAME or ENTER FOLDER
  if (event.ValidReleased())
  {
    clean();
    if (cursor->IsGame())
    {
      //Sound::getFromTheme(getTheme(), getName(), "launch")->play();
      launch(cursor);
    }
    else if (cursor->IsFolder())
    {
      FolderData* folder = (FolderData*)cursor;
      if (folder->HasChildren())
      {
        mCursorStack.push(folder);
        populateList(*folder);
        setCursorIndex(0);
      }
    }
    return true;
  }

  // BACK to PARENT FOLDER or PARENT SYSTEM
  if (event.CancelReleased())
  {
    clean();
    if (!mCursorStack.empty())
    {
      FolderData* selected = mCursorStack.top();

      // remove current folder from stack
      mCursorStack.pop();

      populateList(!mCursorStack.empty() ? *mCursorStack.top() : mSystem.MasterRoot());

      setCursor(selected);
      //Sound::getFromTheme(getTheme(), getName(), "back")->play();
    }
    else if (!hideSystemView)
    {
      if(mSystem.Rotatable())
      {
        RotationType rotation = RotationType::None;
        if(RotationManager::ShouldRotateTateExit(rotation))
        {
          mWindow.Rotate(rotation);
        }
      }
      onFocusLost();
      ViewController::Instance().goToSystemView(&mSystem);
    }
    return true;
  }

  // TOGGLE FAVORITES
  if (event.YReleased() && !cursor->TopAncestor().PreInstalled() && MenuFilter::ShouldEnableFeature(MenuFilter::Favorites))
  {
    if (cursor->IsGame())
    {
      ViewController::Instance().ToggleFavorite(cursor);
      UpdateHelpBar();
    }
    return true;
  }

  // Check vertical move
  if (event.AnyPrimaryUpPressed() || event.AnyPrimaryDownPressed()) mVerticalMove = true;
  if (event.AnyPrimaryUpReleased() || event.AnyPrimaryDownReleased()) mVerticalMove = false;

  // MOVE to NEXT GAMELIST
  if (event.AnyPrimaryRightPressed())
  {
    if (RecalboxConf::Instance().GetQuickSystemSelect() && !hideSystemView && !mVerticalMove)
    {
      clean();
      onFocusLost();
      ViewController::Instance().goToNextGameList();
    }
    return true;
  }

  // MOVE to PREVIOUS GAMELIST
  if (event.AnyPrimaryLeftPressed())
  {
    if (RecalboxConf::Instance().GetQuickSystemSelect() && !hideSystemView && !mVerticalMove)
    {
      clean();
      onFocusLost();
      ViewController::Instance().goToPrevGameList();
    }
    return true;
  }

  // JUMP TO NEXT LETTER
  if (event.L1Pressed())
  {
    JumpToNextLetter(!FileSorts::Instance().IsAscending((FileSorts::Sorts)RecalboxConf::Instance().GetSystemSort(mSystem)));
    return true;
  }

  // JUMP TO PREVIOUS LETTER
  if (event.R1Pressed())
  {
    JumpToNextLetter(FileSorts::Instance().IsAscending((FileSorts::Sorts)RecalboxConf::Instance().GetSystemSort(mSystem)));
    return true;
  }

  // JUMP TO -10
  if (event.L2Pressed() || (!RecalboxConf::Instance().GetQuickSystemSelect() && event.AnyPrimaryLeftReleased()))
  {
    auto index = getCursorIndex();
    if (index > 0)
      setCursorIndex(index > 10 ? index - 10 : 0);
    else
      setCursorIndex(getCursorIndexMax());
    return true;
  }

  // JUMP TO +10
  if (event.R2Pressed() || (!RecalboxConf::Instance().GetQuickSystemSelect() && event.AnyPrimaryRightReleased()))
  {
    auto index = getCursorIndex();
    auto max = getCursorIndexMax();
    if (index == max)
      setCursorIndex(0);
    else
      setCursorIndex(index > max - 10 ? max : index + 10);
    return true;
  }

  // NETPLAY
  if (event.XReleased() && MenuFilter::ShouldEnableFeature(MenuFilter::Netplay)
      && (getCursor()->System().Descriptor().HasNetPlayCores()))
  {
    if (RecalboxSystem::hasIpAdress(false))
    {
      clean();
      mWindow.pushGui(new MenuNetPlayHostPasswords(mWindow, *getCursor()));
    }
    else
      mWindow.displayMessage(_("There is no network available.\nPlease connect your recalbox and try again."));
    return true;
  }
  if (event.XReleased() && MenuFilter::ShouldEnableFeature(MenuFilter::P2K))
  {
    FileData* fd = getCursor();
    if (fd != nullptr)
      if (fd->HasP2K())
        mWindow.pushGui(new GuiControlHints(mWindow, fd->RomPath()));
    return true;
  }
  if (event.StartReleased() && MenuFilter::ShouldDisplayMenu(MenuFilter::Menu::GamelistOptions))
  {
    clean();
    mWindow.pushGui(new MenuGamelistOptions(mWindow, mSystem, mSystemManager, getArcadeInterface(), mResolver));
    return true;
  }

  bool result = Gui::ProcessInput(event);

  return result;
}

bool ISimpleGameListView::CollectHelpItems(Help& help)
{
  bool hideSystemView = RecalboxConf::Instance().GetStartupHideSystemView();

  help.Set(Help::Valid(), _("LAUNCH"));

  if (getCursor()->System().Descriptor().HasNetPlayCores() && MenuFilter::ShouldEnableFeature(MenuFilter::Netplay))
    help.Set(HelpType::X, _("NETPLAY"));
  else
  {
    if (HasCurrentGameP2K() && MenuFilter::ShouldEnableFeature(MenuFilter::P2K))
      help.Set(HelpType::X, _("P2K CONTROLS"));
  }
  FileData* fd = getCursor();
  if (!fd->TopAncestor().PreInstalled() && MenuFilter::ShouldEnableFeature(MenuFilter::Favorites))
    help.Set(HelpType::Y, IsFavoriteSystem() ? _("Remove from favorite") : _("Favorite"));

  if (!hideSystemView)
    help.Set(Help::Cancel(), _("BACK"));

  help.Set(HelpType::UpDown, _("CHOOSE"));

  if (RecalboxConf::Instance().GetQuickSystemSelect() && !hideSystemView)
    help.Set(HelpType::LeftRight, _("SYSTEM"));

  if (!RecalboxConf::Instance().GetQuickSystemSelect())
    help.Set(HelpType::LeftRight, _("PAGE UP/DOWN"));

  if (MenuFilter::ShouldDisplayMenu(MenuFilter::Main))
    help.Set(HelpType::Start, _("OPTIONS"));

  return true;
}

Array<String::Unicode> ISimpleGameListView::GetAvailableLetters()
{
  constexpr int UnicodeSize = 0x10000;
  FileData::List files = getFileDataList(); // All file
  Array<unsigned int> unicode;        // 1 bit per unicode char used
  unicode.ExpandTo(UnicodeSize / (sizeof(unsigned int) * 8));

  for (auto* file : files)
    if (file->IsGame())
    {
      // Tag every first characters from every game name
      String::Unicode wc = String::UpperUnicode(file->Name().ReadFirstUTF8());
      if (wc < UnicodeSize) // Ignore extended unicodes
        unicode((int)(wc >> 5)) |= 1 << (wc & 0x1F);
    }

  // Rebuild a self-sorted unicode list with all tagged characters
  int unicodeOffset = 0;
  Array<String::Unicode> result;
  for(unsigned int i : unicode)
  {
    if (i != 0)
      for (int bit = 0; bit < 32; ++bit)
        if (((i >> bit) & 1) != 0)
          result.Add(unicodeOffset + bit);
    unicodeOffset += 32;
  }

  return result;
}

void ISimpleGameListView::JumpToNextLetter(bool forward)
{
  int cursorIndex = getCursorIndex();
  UnicodeChar baseChar = String::UpperUnicode(getCursor()->Name().ReadFirstUTF8()); // Change to dynamic naming ASAP
  int max = getCursorIndexMax() + 1;
  int step = max + (forward ? 1 : -1);

  for(int i = cursorIndex; (i = (i + step) % max) != cursorIndex; )
    if (String::UpperUnicode(getDataAt(i)->Name().ReadFirstUTF8()) != baseChar) // Change to dynamic naming ASAP
    {
      if (!forward) // In backward move, go to the latest same letter
      {
        baseChar = String::UpperUnicode(getDataAt(i)->Name().ReadFirstUTF8());
        for(int j = i; (j = (j + step) % max) != cursorIndex; )
          if (String::UpperUnicode(getDataAt(j)->Name().ReadFirstUTF8()) == baseChar) i = j;
          else break;
      }
      setCursorIndex(i);
      break;
    }
}

void ISimpleGameListView::JumpToLetter(unsigned int unicode)
{
  for(int c = 0; c < (int)getCursorIndexMax(); ++c)
    if (getDataAt(c)->IsGame())
      if (String::UpperUnicode(getDataAt(c)->Name().ReadFirstUTF8()) == unicode) // Change to dynamic naming ASAP
      {
        setCursor(getDataAt(c));
        break;
      }
}

