#include <recalbox/RecalboxSystem.h>
#include "components/MenuComponent.h"
#include "components/ButtonComponent.h"
#include "utils/locale/LocaleHelper.h"
#include "themes/MenuThemeData.h"

#define BUTTON_GRID_VERT_PADDING Math::max(Renderer::Instance().DisplayHeightAsFloat() * 0.008f, 2.0f)
#define BUTTON_GRID_HORIZ_PADDING Math::max(Renderer::Instance().DisplayWidthAsFloat() * 0.01f, 3.0f)

#define TITLE_VERT_PADDING (Renderer::Instance().DisplayHeightAsFloat()*0.0637f)

#define TITLE_HEIGHT (mTitle->getFont()->getLetterHeight() + TITLE_VERT_PADDING)

MenuComponent::MenuComponent(WindowManager& window, const String& title, const std::shared_ptr<Font>& titleFont)
  : Component(window), mBackground(window), mGrid(window, Vector2i(1, 4)), mTimeAccumulator(0)
{
  (void) titleFont;

  addChild(&mBackground);
  addChild(&mGrid);

  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();

  mBackground.setImagePath(menuTheme.Background().path);
  mBackground.setCenterColor(menuTheme.Background().color);
  mBackground.setEdgeColor(menuTheme.Background().color);

  auto headerGrid = std::make_shared<ComponentGrid>(mWindow, Vector2i(5, 1));
  headerGrid->setColWidthPerc(0, 0.02);
  headerGrid->setColWidthPerc(4, 0.02);
  //headerGrid->SetColumnHighlight(true, 0, 5);

  if (RecalboxConf::Instance().GetClock())
  {
    mDateTime = std::make_shared<DateTimeComponent>(mWindow);
    mDateTime->setDisplayMode(DateTimeComponent::Display::RealTime);
    mDateTime->setHorizontalAlignment(TextAlignment::Right);
    mDateTime->setFont(menuTheme.Text().font);
    mDateTime->setColor(menuTheme.Text().color);
    headerGrid->setColWidthPerc(3, menuTheme.Text().font->sizeText("00:00:00").x() / MenuWidth());
    headerGrid->setEntry(mDateTime, Vector2i(3, 0), false);
  }

  // set up title
  mTitle = std::make_shared<TextScrollComponent>(mWindow);
  mTitle->setHorizontalAlignment(TextAlignment::Center);

  setTitle(title, menuTheme.Title().font);
  mTitle->setColor(menuTheme.Title().color);

  headerGrid->setEntry(mTitle, Vector2i(1, 0), false, true, Vector2i(RecalboxConf::Instance().GetClock() ? 2 : 3, 1));

  mGrid.setEntry(headerGrid, Vector2i(0, 0), false);

  // set up list which will never change (externally, anyway)
  mList = std::make_shared<ComponentList>(mWindow);
  mGrid.setEntry(mList, Vector2i(0, 1), true);

  updateGrid();
  updateSize();

  mGrid.resetCursor();
}

bool MenuComponent::ProcessInput(const InputCompactEvent& event)
{
  if (Component::ProcessInput(event)) return true;

  if (event.AnyDownPressed())
  {
    mGrid.setCursorTo(mList);
    mList->setCursorIndex(0);
    return true;
  }
  if (event.AnyUpPressed())
  {
    mList->setCursorIndex(mList->size() - 1);
    if (!mButtons.empty()) mGrid.moveCursor(Vector2i(0, 1));
    else mGrid.setCursorTo(mList);
    return true;
  }
  return false;
}


void MenuComponent::setTitle(const String& title, const std::shared_ptr<Font>& font)
{
  mTitle->setText(title.ToUpperCaseUTF8());
  mTitle->setFont(font);
}

float MenuComponent::getButtonGridHeight() const
{
  //const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  return ((mButtonGrid ? mButtonGrid->getSize().y() : 0 /*menuTheme.Text().font->getHeight()*/) +
          BUTTON_GRID_VERT_PADDING);
}

void MenuComponent::updateSize()
{
  float menuTheme = ThemeManager::Instance().Menu().Height();
  const float maxHeight = Renderer::Instance().DisplayHeightAsFloat() * menuTheme;
  float height = TITLE_HEIGHT + mList->getTotalRowHeight() + getButtonGridHeight() + getFooterHeight() + 2;
  if (height > maxHeight)
  {
    height = TITLE_HEIGHT + getButtonGridHeight() + getFooterHeight() + 2;
    int i = 0;
    while (i < mList->size())
    {
      float rowHeight = mList->getRowHeight(i);
      if (height + rowHeight < maxHeight)
        height += rowHeight;
      else
        break;
      i++;
    }
  }

  setSize(MenuWidth(), height);
}

void MenuComponent::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  // update grid row/col sizes
  mGrid.setRowHeightPerc(0, TITLE_HEIGHT / mSize.y());
  mGrid.setRowHeightPerc(2, getButtonGridHeight() / mSize.y());
  mGrid.setRowHeightPerc(3, getFooterHeight() / mSize.y());

  mGrid.setSize(mSize);
}

void MenuComponent::addButton(const String& name, const String& helpText, const std::function<void()>& callback)
{
  mButtons.push_back(std::make_shared<ButtonComponent>(mWindow, name.ToUpperCaseUTF8(), helpText, callback));
  updateGrid();
  updateSize();
}

void MenuComponent::updateGrid()
{
  Vector2i buttonGridCursor = Vector2i(0, 0);
  if (mButtonGrid)
  {
    buttonGridCursor = mButtonGrid->getCursor();
    mGrid.removeEntry(mButtonGrid);
  }
  if (mFooter) mGrid.removeEntry(mFooter);

  mButtonGrid.reset();

  if (!mButtons.empty())
  {
    mButtonGrid = MakeButtonGrid(mWindow, mButtons);
    mGrid.setEntry(mButtonGrid, Vector2i(0, 2), true, false);
    mButtonGrid->setCursor(buttonGridCursor);
  }
  if (mFooter) mGrid.setEntry(mFooter, Vector2i(0, 3), false, false);
}

bool MenuComponent::CollectHelpItems(Help& help)
{
  return mGrid.CollectHelpItems(help);
}

void MenuComponent::setFooter(const String& label)
{
  if (mFooter) mGrid.removeEntry(mFooter);
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  mFooter = std::make_shared<TextComponent>(mWindow, label, menuTheme.Footer().font, menuTheme.Footer().color);
  updateGrid();
  updateSize();
}

float MenuComponent::getFooterHeight() const
{
  //const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  return ((mFooter ? mFooter->getSize().y() : 0) + BUTTON_GRID_VERT_PADDING);
}

std::shared_ptr<ComponentGrid> MenuComponent::MakeButtonGrid(WindowManager& window, const std::vector<std::shared_ptr<ButtonComponent> >& buttons)
{
  std::shared_ptr<ComponentGrid> grid = std::make_shared<ComponentGrid>(window, Vector2i(buttons.size(), 2));

  float gridWidth = (float) BUTTON_GRID_HORIZ_PADDING * buttons.size(); // initialize to padding
  for (int i = 0; i < (int) buttons.size(); i++)
  {
    grid->setEntry(buttons[i], Vector2i(i, 0), true, false);
    gridWidth += buttons[i]->getSize().x();
  }
  for (int i = 0; i < (int) buttons.size(); i++)
    grid->setColWidthPerc(i, (buttons[i]->getSize().x() + BUTTON_GRID_HORIZ_PADDING) / gridWidth);

  grid->setSize(gridWidth, buttons[0]->getSize().y() + BUTTON_GRID_VERT_PADDING + 2);
  grid->setRowHeightPerc(1, 2 / grid->getSize().y()); // spacer row to deal with dropshadow to make buttons look centered

  return grid;
}

/**
 * Limitation: same number of button per line, same dimension per cell
 */

std::shared_ptr<ComponentGrid> MenuComponent::MakeMultiDimButtonGrid(WindowManager& window,
                                                                     const std::vector<std::vector<std::shared_ptr<ButtonComponent> > >& buttons,
                                                                     const float outerWidth, const float outerHeight)
{
  const int sizeX = (int) buttons[0].size();
  const int sizeY = (int) buttons.size();
  const float buttonHeight = buttons[0][0]->getSize().y();
  const float gridHeight = (buttonHeight + BUTTON_GRID_VERT_PADDING + 2) * (float) sizeY;

  float horizPadding = (float) BUTTON_GRID_HORIZ_PADDING;
  float gridWidth;
  float buttonWidth;

  do
  {
    gridWidth = outerWidth -
                horizPadding; // to get centered because size * (button size + BUTTON_GRID_VERT_PADDING) let a half BUTTON_GRID_VERT_PADDING left / right marge
    buttonWidth = (gridWidth / (float) sizeX) - horizPadding;
    horizPadding -= 2;
  } while ((buttonWidth < 100) && (horizPadding > 2));


  std::shared_ptr<ComponentGrid> grid = std::make_shared<ComponentGrid>(window, Vector2i(sizeX, sizeY));

  grid->setSize(gridWidth, gridHeight < outerHeight ? gridHeight : outerHeight);

  for (int x = 0; x < sizeX; x++)
    grid->setColWidthPerc(x, (float) 1 / (float) sizeX);

  for (int y = 0; y < sizeY; y++)
  {
    for (int x = 0; x < sizeX; x++)
    {
      const std::shared_ptr<ButtonComponent>& button = buttons[y][x];
      button->setSize(buttonWidth, buttonHeight);
      grid->setEntry(button, Vector2i(x, y), true, false);
    }
  }

  return grid;
}

std::shared_ptr<ImageComponent> MenuComponent::MakeArrow(WindowManager& window)
{
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  auto bracket = std::make_shared<ImageComponent>(window);
  bracket->setImage(menuTheme.Elements().arrow);
  bracket->setColorShift(menuTheme.Text().color);
  bracket->setResize(0, round(menuTheme.Text().font->getLetterHeight()));

  return bracket;
}

void MenuComponent::SetDefaultButton(int index)
{
  if ((unsigned int) index < (unsigned int) mButtons.size())
    mButtonGrid->setCursorTo(mButtons[index]);
}

void MenuComponent::UpdateMenuTheme(const MenuThemeData& theme)
{
  mBackground.setImagePath(theme.Background().path);
  mBackground.setCenterColor(theme.Background().color);
  mBackground.setEdgeColor(theme.Background().color);

  mTitle->setFont(theme.Title().font);
  mTitle->setColor(theme.Title().color);

  if (RecalboxConf::Instance().GetClock())
  {
    mDateTime->setHorizontalAlignment(TextAlignment::Right);
    mDateTime->setFont(theme.Text().font);
    mDateTime->setColor(theme.Text().color);
  }

  updateGrid();
  updateSize();
}
