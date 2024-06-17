//
// Created by bkg2k on 20/08/24.
//

#include <guis/menus/base/Menu.h>
#include <themes/ThemeManager.h>
#include "components/DateTimeComponent.h"
#include "components/TextScrollComponent.h"
#include "ItemSubMenu.h"
#include "ItemSwitch.h"
#include "EmulationStation.h"
#include "ItemHeader.h"
#include <patreon/PatronInfo.h>

Menu::Menu(WindowManager& window, const String& title, const String& footer)
  : Gui(window)
  , mTheme(ThemeManager::Instance().Menu())
  , mCache(ThemeManager::Instance().MenuCache())
  , mBackground(window)
  , mGrid(window, Vector2i(1, 3))
  , mTitleFont(Font::get(FONT_SIZE_LARGE))
  , mTitle(title)
  , mFooter(footer)
  , mTextMargin(2 + Renderer::Instance().DisplayWidthAsInt() / 320)
  , mIconMargin(2 + Renderer::Instance().DisplayWidthAsInt() / 320)
  , mMenuInitialized(false)
{
  addChild(&mBackground);
  addChild(&mGrid);

  mBackground.setImagePath(mTheme.Background().path);
  mBackground.setCenterColor(mTheme.Background().color);
  mBackground.setEdgeColor(mTheme.Background().color);

  std::shared_ptr<ComponentGrid> headerGrid = std::make_shared<ComponentGrid>(mWindow, Vector2i(5, 1));
  headerGrid->setColWidthPerc(0, 0.02);
  headerGrid->setColWidthPerc(4, 0.02);

  if (RecalboxConf::Instance().GetClock())
  {
    std::shared_ptr<DateTimeComponent> dateTime = std::make_shared<DateTimeComponent>(mWindow);
    dateTime->setDisplayMode(DateTimeComponent::Display::RealTime);
    dateTime->setHorizontalAlignment(TextAlignment::Right);
    dateTime->setFont(mTheme.Text().font);
    dateTime->setColor(mTheme.Text().color);
    headerGrid->setColWidthPerc(3, mTheme.Text().font->sizeText("00:00:00").x() / MenuWidth());
    headerGrid->setEntry(dateTime, Vector2i(3, 0), false);
  }

  // set up title

  std::shared_ptr<TextScrollComponent> scrolltitle = std::make_shared<TextScrollComponent>(mWindow);
  scrolltitle->setHorizontalAlignment(TextAlignment::Center);
  scrolltitle->setText(title.ToUpperCaseUTF8());
  scrolltitle->setFont(mTitleFont);
  scrolltitle->setColor(mTheme.Title().color);

  headerGrid->setEntry(scrolltitle, Vector2i(1, 0), false, true, Vector2i(RecalboxConf::Instance().GetClock() ? 2 : 3, 1));
  mGrid.setEntry(headerGrid, Vector2i(0, 0), false);

  //  set up footer
  if (mFooter.empty()) mFooter = String("RECALBOX VERSION ").Append(PROGRAM_VERSION_STRING).Append(PatronInfo::Instance().IsPatron() ? " - PATRON FLAVOR": "").UpperCase();
  std::shared_ptr<TextScrollComponent> footerscroll = std::make_shared<TextScrollComponent>(mWindow);
  footerscroll->setHorizontalAlignment(TextAlignment::Center);
  footerscroll->setText(mFooter);
  footerscroll->setFont(mTheme.Footer().font);
  footerscroll->setColor(mTheme.Footer().color);
  mGrid.setEntry(footerscroll, Vector2i(0, 2), false);

  // set up list which will never change (externally, anyway)
  mList = std::make_shared<TextListComponent<ItemBase*>>(mWindow);
  mList->SetOverlayInterface(this);
  mList->setUppercase(true);
  mList->setFont(mTheme.Text().font);
  mList->setSelectedColor(mTheme.Text().selectedColor);
  mList->setSelectorColor(mTheme.Text().selectorColor);
  mList->setColor(sSelectableColor, mTheme.Text().color);                   // Text color
  mList->setColor(sBackgroundColor, mTheme.Background().color);             // Unselected Background color
  mList->setColor(sUnselectableColor, Alpha25Percent(mTheme.Text().color)); // Grayed item
  mList->setColor(sHeaderBackgroundColor, 0x00000040);                      // Grayed item - TODO: make this color themable
  mList->setSelectorHeight(mList->EntryHeight());
  mList->setShiftSelectedTextColor(true);
  mList->setAutoAlternate(true);
  mGrid.setEntry(mList, Vector2i(0, 1), true);

  mGrid.resetCursor();
}

bool Menu::ProcessInput(const InputCompactEvent& event)
{
  // Close?
  if (event.CancelReleased()) { Close(); return true; }
  // Close all?
  if (event.StartReleased()) { mWindow.CloseAll(); return true; }
  // Then to the currently selected menu entry
  if (mList->getSelected()->IsSelectable())
    if (mList->getSelected()->ProcessInput(event)) return true;

  // Give event to parent
  return Gui::ProcessInput(event);
}

bool Menu::IsDisplayable(int index) const
{
  const ItemBase& first = *mItems[index];
  // Normal item
  if (!first.IsHeader())
    return first.IsDisplayable();
  // Header
  while(++index < mItems.Count())
  {
    const ItemBase& next = *mItems[index];
    if (next.IsHeader()) break;
    else if (next.IsDisplayable()) return true;
  }
  return false;
}

void Menu::BuildMenu()
{
  mList->clear();
  bool opened = true;
  for(int i = 0; i < mItems.Count(); ++i)
    if (IsDisplayable(i))
    {
      if (ItemBase* item = mItems[i]; item->ItemType() == ItemBase::Type::Header)
      {
        opened = item->AsHeader()->IsOpened();
        mList->add(String(opened ? "▼ " : "◀ ").Append(item->Label()), item, item->IsSelectable() ? sSelectableColor : sUnselectableColor, sHeaderBackgroundColor, HorizontalAlignment::Right);
      }
      else if (opened)
        mList->add(item->Label(), item, item->IsSelectable() ? sSelectableColor : sUnselectableColor, HorizontalAlignment::Left);
    }
}

float Menu::TitleHeight() const
{
  float padding = (Renderer::Instance().DisplayHeightAsFloat() * 0.0637f);
  return mTitleFont->getLetterHeight() + padding;
}

float Menu::FooterHeight() const
{
  return mTheme.Footer().font->getHeight();
}

float Menu::MenuWidth() const
{
  return Math::min(Renderer::Instance().DisplayHeightAsFloat() * 1.2f,
                   Renderer::Instance().DisplayWidthAsFloat() * 0.90f);
}

float Menu::MenuHeight() const
{
  float menuTheme = ThemeManager::Instance().Menu().Height();
  const float maxHeight = Renderer::Instance().DisplayHeightAsFloat() * menuTheme;
  float baseHeight = TitleHeight() + FooterHeight() + 2;
  int rows = (int)(maxHeight - baseHeight) / (int)mList->EntryHeight();
  return baseHeight + (float)rows * mList->EntryHeight();
}

void Menu::SetMenuSize()
{
  setSize(MenuWidth(), MenuHeight());
  setPosition((Renderer::Instance().DisplayWidthAsFloat() - getWidth()) / 2.f,
              (Renderer::Instance().DisplayHeightAsFloat() - getHeight()) / 2.f);
}

void Menu::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  // update grid row/col sizes
  mGrid.setRowHeightPerc(0, TitleHeight() / mSize.y());
  mGrid.setRowHeightPerc(2, FooterHeight() / mSize.y());

  mGrid.setSize(mSize);
}

float Menu::OverlayGetLeftOffset(ItemBase* const& data)
{
  int w = 0;
  if (data->HasValidIcon())
    w = Math::roundi(mList->EntryHeight() * .8f) + mIconMargin;
  return w + mList->getSelected()->OverlayLeftOffset(*this, *data) + mTextMargin;
}

void Menu::OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size,
                        ItemBase* const& data, unsigned int& color)
{
  (void)parentTrans;
  (void)color;
  Colors::ColorRGBA iconColor = data == mList->getSelected() ? mTheme.Background().color : mTheme.Text().color;
  iconColor = data->IsSelectable() ? iconColor : Alpha25Percent(iconColor);
  if (data->HasValidIcon())
  {
    int h = (int)(mList->EntryHeight() * 0.8f);
    int x = (int)position.x() + mIconMargin;
    int y = (int)(position.y() + ((size.y() - (float)h) / 2.f));
    std::shared_ptr<TextureResource>& texture = mCache.GetIcon(data->Icon());
    // Draw
    Renderer::DrawTexture(*texture, x, y, h, h, true, iconColor);
    // Next renderer
    mList->getSelected()->OverlayDraw(*this, Rectangle(position.x() + h + mIconMargin, position.y(), size.x() - (h + mIconMargin), size.y()), *data, iconColor);
  }
  else mList->getSelected()->OverlayDraw(*this, Rectangle(position, size), *data, iconColor);
}

int Menu::AddHeader(const String& label)
{
  mItems.Add(new ItemHeader(label, mTheme, this));
  return mItems.Count() - 1;
}

int Menu::AddSubMenu(const String& label, int id, ISubMenuSelected* interface, const String& help, bool unselectable)
{
  mItems.Add(new ItemSubMenu(label, mTheme, interface, id, help, unselectable));
  return mItems.Count() - 1;
}

int Menu::AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface,
                     const String& help, bool unselectable)
{
  mItems.Add(new ItemSubMenu(label, mTheme, interface, id, icon, help, unselectable));
  return mItems.Count() - 1;
}

int Menu::AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable)
{
  mItems.Add(new ItemSwitch(label, mTheme, interface, id, value, help, unselectable));
  return mItems.Count() - 1;
}

int Menu::AddSwitch(const MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface,
                    const String& help, bool unselectable)
{
  mItems.Add(new ItemSwitch(label, mTheme, interface, id, icon, value, help, unselectable));
  return mItems.Count() - 1;
}

