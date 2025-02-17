//
// Created by bkg2k on 02/01/2020.
//

#include <utils/locale/LocaleHelper.h>
#include <themes/MenuThemeData.h>
#include <components/TextComponent.h>
#include "components/MenuComponent.h" // for makeButtonGrid
#include "GuiBiosMd5.h"

GuiBiosMd5::GuiBiosMd5(WindowManager& window, const Bios& bios)
  : Gui(window),
    mBackground(window, Path(":/frame.png")),
    mGrid(window, Vector2i(3,4))
{
  BuildUI();

  bool match = (bios.BiosStatus() == Bios::Status::HashMatching);
  // Display our MD5 only if the file exists
  String theMd5 = bios.CurrentMD5();
  if (bios.BiosStatus() != Bios::Status::FileNotFound)
  {
    mList->add(_("Your bios' MD5"), true, sColorIndexNormal, sColorIndexGray, HorizontalAlignment::Left);
    mList->add(theMd5, true, match ? sColorIndexNormal : sColorIndexHalf, match ? sColorIndexGreen : sColorIndexRed, HorizontalAlignment::Right);
  }
  // Display regular list
  mList->add(_("Known MD5 List"), true, sColorIndexNormal, sColorIndexGray, HorizontalAlignment::Left);
  String::List list = bios.MD5List();
  for(const String& md5 : list)
  {
    if (!match)
      mList->add(md5, true, sColorIndexNormal, -1, HorizontalAlignment::Right);
    else
    {
      bool isMatching = (md5 == theMd5);
      mList->add(md5, true, isMatching ? sColorIndexNormal : sColorIndexHalf, isMatching ? sColorIndexGreen : -1, HorizontalAlignment::Right);
    }
  }
  mHeader->setValue(_("%i Known MD5").Replace("%i", String((int)list.size())));
}

GuiBiosMd5::GuiBiosMd5(WindowManager& window, const BiosList& biosList)
  : Gui(window),
    mBackground(window, Path(":/frame.png")),
    mGrid(window, Vector2i(3,4))
{
  BuildUI();

  int count = 0;
  for(int i = 0; i < biosList.BiosCount(); ++i)
  {
    const Bios& bios = biosList.BiosAt(i);
    count += bios.MD5Count();
    // Display regular list
    mList->add(bios.Filename().Append(" (").Append(bios.Cores()).Append(')'), true, sColorIndexNormal, sColorIndexGray, HorizontalAlignment::Left);
    String::List list = bios.MD5List();
    for (const String& md5 : list)
      mList->add(md5, true, sColorIndexNormal, -1, HorizontalAlignment::Right);
  }
  mHeader->setValue(_("%i Known MD5").Replace("%i", String(count)));
}

void GuiBiosMd5::BuildUI()
{
  // Window's background
  addChild(&mBackground);
  // Add grid as the main component
  addChild(&mGrid);

  // Get theme
  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();
  mBackground.setImagePath(menuTheme.Background().path);
  mBackground.setCenterColor(menuTheme.Background().color);
  mBackground.setEdgeColor(menuTheme.Background().color);

  // Title
  mTitle = std::make_shared<TextComponent>(mWindow, _("MD5 LIST"), menuTheme.Title().font, menuTheme.Title().color, TextAlignment::Center);
  mGrid.setEntry(mTitle, Vector2i(1, 0), false, false, Vector2i(1,1) );

  // Header
  mHeader = std::make_shared<TextComponent>(mWindow, "", menuTheme.Footer().font, menuTheme.Footer().color, TextAlignment::Center);
  mGrid.setEntry(mHeader, Vector2i(1, 1), false, false, Vector2i(1,1) );

  // List
  mList = std::make_shared<TextListComponent<bool>>(mWindow);
  mList->setFont(menuTheme.SmallText().font);
  mList->setAlignment(HorizontalAlignment::Right);
  mList->setSelectorColor(menuTheme.Text().selectorColor);
  mList->setSelectedColor(menuTheme.Text().selectedColor);
  mList->setColorAt(sColorIndexNormal, menuTheme.Text().color);
  mList->setColorAt(sColorIndexHalf, (menuTheme.Text().color & 0xFFFFFF00) | 0x80);
  mList->setColorAt(sColorIndexRed, 0xFF000022); // Red
  mList->setColorAt(sColorIndexGray, 0x00000022); // Yellow
  mList->setColorAt(sColorIndexGreen, 0x00FF0022); // Greeen
  mList->setHorizontalMargin(Renderer::Instance().DisplayWidthAsFloat() * 0.95f * 0.01f);
  mList->setSelectorHeight((float)menuTheme.SmallText().font->getSize() * 1.5f);
  mGrid.setEntry(mList, Vector2i(1,2), true, false, Vector2i(1, 1));

  // Buttons
  mButtonClose = std::make_shared<ButtonComponent>(mWindow, _("CLOSE"), _("CLOSE"), [this] { Close(); } );
  std::vector<std::shared_ptr<ButtonComponent>> buttons;
  buttons.push_back(mButtonClose);
  mButtonGrid = MenuComponent::MakeButtonGrid(mWindow, buttons);
  mGrid.setEntry(mButtonGrid, Vector2i(1, 3), true, false, Vector2i(1,1));

  // Set Window position/size
  float width = Math::max(menuTheme.SmallText().font->sizeText(String('0', 16)).x() * 2.5f, Renderer::Instance().DisplayWidthAsFloat() * 0.33f);
  setSize(width, Renderer::Instance().DisplayHeightAsFloat() * 0.849f);
  setPosition((Renderer::Instance().DisplayWidthAsFloat() - mSize.x()) / 2, (Renderer::Instance().DisplayHeightAsFloat() - mSize.y()) / 2);

  // Force component size to refresh
  onSizeChanged();
}

void GuiBiosMd5::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  // Set grid size
  float titlePercent = mTitle->getFont()->getLetterHeight() * 2.6f / mSize.y();
  float headerPercent = mHeader->getFont()->getLetterHeight() * 2.6f / mSize.y();
  float buttonPercent = (mButtonGrid->getSize().y() * 1.2f) / mSize.y();

  mGrid.setColWidthPerc(0, 0.02f, false);
  mGrid.setColWidthPerc(1, 0.96f, false);
  mGrid.setColWidthPerc(2, 0.02f, false);
  mGrid.setRowHeightPerc(0, titlePercent, false);
  mGrid.setRowHeightPerc(1, headerPercent, false);
  mGrid.setRowHeightPerc(2, 1.0f - (titlePercent + headerPercent + buttonPercent), false);
  mGrid.setRowHeightPerc(3, buttonPercent, false);
  mGrid.setSize(mSize);

  // Resize all components
  float marginPercent = 0.95f;
  mTitle->setSize(marginPercent * mGrid.getColWidth(1), mGrid.getRowHeight(0));
  mHeader->setSize(marginPercent * mGrid.getColWidth(1), mGrid.getRowHeight(1));
  mButtonGrid->setSize(marginPercent * mGrid.getColWidth(1), mGrid.getRowHeight(3));
  // List need to be anchored to the grid manually
  mList->setPosition(mGrid.getColWidth(0), mGrid.getRowHeight(0, 1));
  mList->setSize(mGrid.getColWidth(1), mGrid.getRowHeight(2));
}

bool GuiBiosMd5::CollectHelpItems(Help& help)
{
  mGrid.CollectHelpItems(help);
  help.Set(HelpType::UpDown, _("BROWSE"))
      .Set(Help::Cancel(), _("CLOSE"));
  return true;
}

bool GuiBiosMd5::ProcessInput(const InputCompactEvent& event)
{
  if (event.CancelReleased())
  {
    Close();
    return true;
  }

  return Gui::ProcessInput(event);
}
