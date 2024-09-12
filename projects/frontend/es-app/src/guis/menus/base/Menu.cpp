//
// Created by bkg2k on 20/08/24.
//

#include <guis/menus/base/Menu.h>
#include <themes/ThemeManager.h>
#include "components/TextScrollComponent.h"
#include "ItemSubMenu.h"
#include "ItemSwitch.h"
#include "ItemSlider.h"
#include "ItemEditable.h"
#include "ItemText.h"
#include "ItemRating.h"
#include "ItemAction.h"
#include "ItemBar.h"
#include "EmulationStation.h"
#include "ItemHeader.h"
#include "MainRunner.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiInfoPopup.h"
#include "ItemSelector.h"
#include "animations/LambdaAnimation.h"

#include <patreon/PatronInfo.h>
#include <guis/menus/base/MenuColors.h>

Menu::Menu(WindowManager& window, const String& title, const String& footer, bool animated)
  : Gui(window)
  , mTheme(ThemeManager::Instance().Menu())
  , mConfiguration(RecalboxConf::Instance())
  , mCache(ThemeManager::Instance().MenuCache())
  , mBackground(window)
  , mGrid(window, Vector2i(1, 3))
  , mTitleFont(Font::get(FONT_SIZE_LARGE))
  , mTitle(title)
  , mFooter(footer)
  , mTextMargin(3 + Renderer::Instance().DisplayWidthAsInt() / 320)
  , mIconMargin(3 + Renderer::Instance().DisplayWidthAsInt() / 320)
  , mMenuInitialized(false)
  , mAnimated(animated)
  , m240p(Renderer::Instance().Is240p())
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
  mList->setFont(mTheme.Text().font);
  mList->setSelectedColor(0); // If defined, it's given priority on color shift
  mList->setSelectorColor(mTheme.Text().selectorColor);
<<<<<<< HEAD
  mList->setColorAt(MenuColors::sSelectableColor, mTheme.Text().color);                                               // Text color
  mList->setColorAt(MenuColors::sSelectableSelectedColor, mTheme.Text().selectedColor);                               // selected text coplor
  mList->setColorAt(MenuColors::sUnselectableColor, MenuColors::Alpha25Percent(mTheme.Text().color));                 // Grayed color
  mList->setColorAt(MenuColors::sUnselectableSelectedColor, MenuColors::Alpha25Percent(mTheme.Text().selectedColor)); // Grayed selected color
  mList->setColorAt(MenuColors::sHeaderColor, mTheme.Section().color);                                                // Header color
  mList->setColorAt(MenuColors::sHeaderSelectedColor, mTheme.Section().selectedColor);                                // Header selected color
=======
  mList->setColorAt(MenuColors::sSelectableColor, mTheme.Text().color);                   // Text color
  mList->setColorAt(MenuColors::sBackgroundColor, mTheme.Background().color);             // Unselected Background color
  mList->setColorAt(MenuColors::sUnselectableColor, MenuColors::Alpha25Percent(mTheme.Text().color)); // Grayed item
  mList->setColorAt(MenuColors::sHeaderBackgroundColor, 0x00000040);                      // Grayed item - TODO: make this color themable
  mList->setColorAt(MenuColors::sHeaderColor, mTheme.Section().color);                    // Header color
>>>>>>> 3482c942b2 (chore: gcc compatibility fixes)
  mList->setSelectorHeight(mList->EntryHeight());
  mList->setShiftSelectedTextColor(true);
  mList->setAutoAlternate(true);
  mList->setCursorChangedCallback([this](CursorState state) { if (state == CursorState::Stopped) mWindow.UpdateHelpSystem(); });
  mGrid.setEntry(mList, Vector2i(0, 1), true);

  mGrid.resetCursor();
}

Menu::~Menu()
{
  for(ItemBase* item : mItems)
    delete(item);
}

void Menu::Reboot()
{
  MainRunner::RequestQuit(MainRunner::ExitState::NormalReboot);
}

void Menu::RequestReboot()
{
  mWindow.pushGui(new GuiMsgBox(mWindow, _("THE SYSTEM WILL NOW REBOOT"), _("OK"), Reboot, _("LATER"), std::bind(Menu::RebootPending, &mWindow)));
}

void Menu::RebootPending(WindowManager* window)
{
  static bool pending = false;
  if (!pending)
  {
    window->InfoPopupAdd(new GuiInfoPopup(*window, _("A reboot is required to apply pending changes."), 10000, PopupType::Reboot));
    pending = true;
  }
}

void Menu::Relaunch()
{
  MainRunner::RequestQuit(MainRunner::ExitState::Relaunch, true);
}

void Menu::RequestRelaunch()
{
  mWindow.pushGui(new GuiMsgBox(mWindow, _("EmulationStation must relaunch to apply your changes."), _("OK"), Relaunch, _("LATER"), std::bind(Menu::RebootPending, &mWindow)));
}

bool Menu::ProcessInput(const InputCompactEvent& event)
{
  // Close?
  if (event.CancelReleased()) { Close(); return true; }
  // Close all?
  if (event.StartReleased()) { mWindow.CloseAll(); return true; }

  // Get selected item
  const ItemBase& item = *mList->getSelected();
  // Help
  if (event.YPressed())
  {
    if (item.IsSelectable() && item.HasHelpText())
    {
      mWindow.InfoPopupAddRegular(mList->getSelected()->HelpText(), RecalboxConf::Instance().GetPopupHelp(),
                                  PopupType::Help, false);
      return true;
    }
    if (item.IsSelectable() && item.HasHelpUnselectableText())
    {
      mWindow.InfoPopupAddRegular(mList->getSelected()->HelpUnselectableText(), RecalboxConf::Instance().GetPopupHelp(),
                                  PopupType::Help, false);
      return true;
    }
  }
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
  // Clear all
  for(ItemBase* item : mItems)
    delete item;
  mItems.Clear();
  mList->clear();

  // Rebuild
  bool opened = true;
  BuildMenuItems();
  for(int i = 0; i < mItems.Count(); ++i)
    if (IsDisplayable(i))
    {
      if (ItemBase* item = mItems[i]; item->ItemType() == ItemBase::Type::Header)
      {
        opened = item->AsHeader()->IsOpened();
        mList->add(String::Empty, item, item->IsSelectable() ? MenuColors::sSelectableColor : MenuColors::sHeaderColor, -1/*MenuColors::sHeaderBackgroundColor*/, HorizontalAlignment::Right);
      }
      else if (opened)
        mList->add(item->Label(), item, item->IsSelectable() ? MenuColors::sSelectableColor : MenuColors::sUnselectableColor, HorizontalAlignment::Left);
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

Rectangle Menu::GetMenuMaximumArea() const
{
  float screenWidth = Renderer::Instance().DisplayWidthAsFloat();
  float screenHeight = Renderer::Instance().DisplayHeightAsFloat();
  float maxWidth = screenWidth * 0.9f;
  float maxHeight = screenHeight * ThemeManager::Instance().Menu().Height();
  return Rectangle((screenWidth - maxWidth) / 2, (screenHeight - maxHeight) / 2, maxWidth, maxHeight);
}

float Menu::MenuWidth() const
{
  //return Math::min(Renderer::Instance().DisplayHeightAsFloat() * 1.2f,
  //                 Renderer::Instance().DisplayWidthAsFloat() * 0.85f);
  return Renderer::Instance().DisplayWidthAsFloat() * 0.85f;
}

float Menu::MenuHeight() const
{
  float menuTheme = ThemeManager::Instance().Menu().Height();
  const float maxHeight = Renderer::Instance().DisplayHeightAsFloat() * menuTheme;
  float baseHeight = TitleHeight() + FooterHeight() + 2;
  int maxrows = (int)(maxHeight - baseHeight) / (int)mList->EntryHeight();
  return baseHeight + (float)(Math::min((int)mList->size(), maxrows)) * mList->EntryHeight();
}

void Menu::SetMenuSize()
{
  setSize(MenuWidth(), MenuHeight());
  setPosition((Renderer::Instance().DisplayWidthAsFloat() - getWidth()) / 2.f,
              mAnimated ? Renderer::Instance().DisplayHeightAsFloat()
                        : (Renderer::Instance().DisplayHeightAsFloat() - getHeight()) / 2.f);

  if (mAnimated)
  {
    // Animation
    auto fadeFunc = [this](float t)
    {
      setOpacity((int) lerp<float>(0, 255, t));
      setPosition(getPosition().x(), lerp<float>(Renderer::Instance().DisplayHeightAsFloat(), (Renderer::Instance().DisplayHeightAsFloat() - mSize.y()) / 2, t));
    };

    setOpacity(0);
    setAnimation(new LambdaAnimation(fadeFunc, 200), 0);
  }

  mWindow.UpdateHelpSystem();
}

void Menu::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  // update grid row/col sizes
  mGrid.setRowHeightPerc(0, TitleHeight() / mSize.y());
  mGrid.setRowHeightPerc(2, FooterHeight() / mSize.y());

  mGrid.setSize(mSize);
}

float Menu::OverlayGetLeftOffset(ItemBase* const& data, int labelWidth)
{
  int w = 0;
  if (data->HasValidIcon())
    w = Math::roundi(mList->EntryHeight() * .8f) + mIconMargin;
  return w + data->OverlayLeftOffset(labelWidth + mTextMargin * 2) + mTextMargin;
}

void Menu::OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size,
                        int labelWidth, ItemBase* const& data, unsigned int& color)
{
  (void)parentTrans;
  (void)color;
  bool selected = data == mList->getSelected();
  Colors::ColorRGBA iconColor = selected ? mTheme.Text().selectedColor : mTheme.Text().color;
  iconColor = data->IsSelectable() ? iconColor : MenuColors::Alpha25Percent(iconColor);
  Rectangle area(position, size);
  int offset = 0;
  if (data->HasValidIcon())
  {
    int h = (int)(mList->EntryHeight() * 0.8f);
    int x = (int)position.x() + mIconMargin;
    int y = (int)(position.y() + ((size.y() - (float)h) / 2.f));
    std::shared_ptr<TextureResource>& texture = data->IconPath().IsEmpty() ? mCache.GetIcon(data->Icon(), 0, h) : mCache.GetFromPath(data->IconPath(), 0, h);
    // Draw
    Renderer::DrawTexture(*texture, x, y, h, h, true, iconColor);
    offset = mIconMargin * 2;
    area.MoveLeft(h + offset);
  }
  data->OverlayDraw(labelWidth + mTextMargin * 2 + offset, Rectangle(position.x() - offset, position.y(), size.x(), size.y()), iconColor, selected);
}

ItemHeader* Menu::AddHeader(const String& label)
{
  ItemHeader* item = new ItemHeader(*this, mList->IsUppercase() ? label.ToUpperCaseUTF8() : label, mTheme, this);
  mItems.Add(item);
  return item;
}

ItemSubMenu* Menu::AddSubMenu(const String& label, int id, ISubMenuSelected* interface, const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSubMenu* item = new ItemSubMenu(*this, label, mTheme, interface, id, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemSubMenu* Menu::AddSubMenu(const String& label, MenuThemeData::MenuIcons::Type icon, int id, ISubMenuSelected* interface,
                     const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSubMenu* item = new ItemSubMenu(*this, label, mTheme, interface, id, icon, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemSwitch* Menu::AddSwitch(const String& label, bool value, int id, ISwitchChanged* interface, const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSwitch* item = new ItemSwitch(*this, label, mTheme, interface, id, value, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemSwitch* Menu::AddSwitch(const MenuThemeData::MenuIcons::Type icon, const String& label, bool value, int id, ISwitchChanged* interface,
                    const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSwitch* item = new ItemSwitch(*this, label, mTheme, interface, id, icon, value, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemSwitch* Menu::AddSwitch(const Path& icon, const String& label, bool value, int id, ISwitchChanged* interface,
                    const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSwitch* item = new ItemSwitch(*this, label, mTheme, interface, id, icon, value, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemSlider* Menu::AddSlider(const String& label, float min, float max, float inc, float value, float defaultValue, const String& suffix, int id,
                    ISliderChanged* interface, const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemSlider* item = new ItemSlider(*this, label, mTheme, interface, id, min, max, inc, value, defaultValue, suffix, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemText* Menu::AddText(const String& label, const String& value, const String& help)
{
  ItemText* item = new ItemText(*this, label, mTheme, 0, value, 0, help);
  mItems.Add(item);
  return item;
}

ItemText* Menu::AddText(const String& label, const String& value, unsigned int color, const String& help)
{
  ItemText* item = new ItemText(*this, label, mTheme, 0, value, color, help);
  mItems.Add(item);
  return item;
}

ItemEditable* Menu::AddEditable(const String& edittitle, const String& label, const String& value, int id,
                                IEditableChanged* interface, const String& help, bool masked, bool unselectable,
                                const String& helpUnselectable)
{
  ItemEditable* item = new ItemEditable(mWindow, *this, edittitle, label, mTheme, interface, id, value, masked, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}


ItemEditable* Menu::AddEditable(const String& label, const String& value, int id, IEditableChanged* interface, const String& help,
                                bool masked, bool unselectable, const String& helpUnselectable)
{
  ItemEditable* item = new ItemEditable(mWindow, *this, String::Empty, label, mTheme, interface, id, value, masked, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemAction* Menu::AddAction(const String& label, const String& buttonWord, int id, bool positive, IActionTriggered* interface, const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemAction* item = new ItemAction(*this, label, buttonWord, mTheme, interface, id, positive, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemRating* Menu::AddRating(const String& label, float value, int id, IRatingChanged* interface, const String& help, bool unselectable, const String& helpUnselectable)
{
  ItemRating* item = new ItemRating(*this, label, mTheme, interface, id, value, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

ItemBar* Menu::AddBar(const String& label, const String& text, float ratio, const String& help, bool unselectable,
                           const String& helpUnselectable)
{
  ItemBar* item = new ItemBar(*this, label, mTheme, 0, text, ratio, 0, help, unselectable, helpUnselectable);
  mItems.Add(item);
  return item;
}

