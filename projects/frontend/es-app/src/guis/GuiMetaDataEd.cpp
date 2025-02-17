#include "guis/GuiMetaDataEd.h"
#include "guis/GuiMsgBox.h"
#include <components/SwitchComponent.h>
#include <LibretroRatio.h>

#include "components/RatingComponent.h"
#include "guis/GuiTextEditPopup.h"
#include "guis/GuiTextEditPopupKeyboard.h"
#include "components/OptionListComponent.h"

#include "games/MetadataFieldDescriptor.h"
#include "GuiScraperSingleGameRun.h"
#include "utils/cplusplus/StaticLifeCycleControler.h"
#include <views/ViewController.h>

#define TITLE_VERT_PADDING (Renderer::Instance().DisplayHeightAsFloat()*0.0637f)

GuiMetaDataEd::GuiMetaDataEd(WindowManager& window,
                             SystemManager& systemManager,
                             FileData& game,
                             ISimpleGameListView* gamelistview,
                             IMetaDataAction* actions,
                             bool main)
  : Gui(window),
    mGame(game),
    mSystemManager(systemManager),
    mBackground(window, Path(":/frame.png")),
    mGrid(window, Vector2i(1, 3)),
    mMetaData(game.Metadata()),
    mGameListView(gamelistview),
    mActions(actions)
{
  addChild(&mBackground);
  addChild(&mGrid);

  mHeaderGrid = std::make_shared<ComponentGrid>(mWindow, Vector2i(1, 5));

  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();

  mBackground.setImagePath(menuTheme.Background().path);
  mBackground.setCenterColor(menuTheme.Background().color);
  mBackground.setEdgeColor(menuTheme.Background().color);

  mTitle = std::make_shared<TextComponent>(mWindow, _("EDIT METADATA"), menuTheme.Title().font, menuTheme.Title().color,
                                           TextAlignment::Center);
  mSubtitle = std::make_shared<TextComponent>(mWindow, game.RomPath().Filename().UpperCase(),
                                              menuTheme.Footer().font, menuTheme.Footer().color, TextAlignment::Center);
  float y = 0;
  y += mTitle->getFont()->getHeight() + mSubtitle->getFont()->getHeight();
  mHeaderGrid->setEntry(mTitle, Vector2i(0, 1), false, true);
  mHeaderGrid->setEntry(mSubtitle, Vector2i(0, 3), false, true);

  mGrid.setEntry(mHeaderGrid, Vector2i(0, 0), false, true);

  mList = std::make_shared<ComponentList>(mWindow);
  mGrid.setEntry(mList, Vector2i(0, 1), true, true);

  auto emu_choice = std::make_shared<OptionListComponent<String>>(mWindow, _("Emulator"), false, FONT_SIZE_MEDIUM);

  // Get list
  int fieldCount = 0;
  const MetadataFieldDescriptor *fields = mMetaData.GetMetadataFieldDescriptors(fieldCount);

  // populate list
  for (; --fieldCount >= 0;)
  {
    const MetadataFieldDescriptor &field = fields[0];
    if (field.EditType() == MetadataFieldDescriptor::EditableType::None) continue;
    fields++;

    std::shared_ptr<Component> ed;

    // don't add statistics
    if (field.IsStatistic())
    {
      continue; // TODO: Still usefull (== EditableType::None?)
    }

    // filter on main or secondary entries depending on the requested type
    if (field.IsMain() != main)
    {
      continue;
    }

    // create ed and add it (and any related components) to mMenu
    // ed's value will be set below
    ComponentListRow row;
    auto lbl = std::make_shared<TextComponent>(mWindow, field.DisplayName().ToUpperCaseUTF8(), menuTheme.Text().font, menuTheme.Text().color);

    row.addElement(lbl, true); // label
    y += lbl->getFont()->getHeight();

    switch (field.EditType())
    {
      case MetadataFieldDescriptor::EditableType::Rating:
      {
        ed = std::make_shared<RatingComponent>(window, menuTheme.Text().color);

        ed->setSize(0, lbl->getSize().y() * 0.8f);
        row.addElement(ed, false, true);

        auto spacer = std::make_shared<Component>(mWindow);
        spacer->setSize(Renderer::Instance().DisplayWidthAsFloat() * 0.0025f, 0);
        row.addElement(spacer, false);

        // pass input to the actual RatingComponent instead of the spacer
        //row.input_handler = std::bind(&Component::ProcessInput, ed.get(), std::placeholders::_1/*, std::placeholders::_2*/);

        break;
      }
      case MetadataFieldDescriptor::EditableType::Date:
      {
        ed = std::make_shared<DateTimeComponent>(mWindow);
        row.addElement(ed, false);

        auto spacer = std::make_shared<Component>(mWindow);
        spacer->setSize(Renderer::Instance().DisplayWidthAsFloat() * 0.0025f, 0);
        row.addElement(spacer, false);

        // pass input to the actual DateTimeComponent instead of the spacer
        //row.input_handler = std::bind(&Component::ProcessInput, ed.get(), std::placeholders::_1/*, std::placeholders::_2*/);

        break;
      }
      case MetadataFieldDescriptor::EditableType::Switch:
      {
        auto switchComp = std::make_shared<SwitchComponent>(mWindow);
        GetValueMethodType method = field.GetValueMethod();
        switchComp->setState((mMetaData.*method)() == "true");
        ed = switchComp;
        row.addElement(ed, false);
        break;
      }
      case MetadataFieldDescriptor::EditableType::ListMulti:
      case MetadataFieldDescriptor::EditableType::List:
        if (field.Key() == "emulator")
        {
          String currentEmulator = mMetaData.Emulator();
          String currentCore = mMetaData.Core();
          String defaultEmulator;
          String defaultCore;
          if (!EmulatorManager::GetDefaultEmulator(mGame.System(), defaultEmulator, defaultCore))
            continue;
          if (currentEmulator.empty()) currentEmulator = defaultEmulator;
          if (currentCore.empty()) currentCore = defaultCore;

          row.addElement(emu_choice, false);

          for (const String& emulatorName : EmulatorManager::GetEmulators(mGame.System()))
          {
            for (const String& coreName : EmulatorManager::GetCores(mGame.System(), emulatorName))
            {
              String displayName = emulatorName;
              if (displayName != coreName) displayName.Append(' ').Append(coreName);
              if (defaultCore == coreName && defaultEmulator == emulatorName)
                displayName.Append(" (").Append(_("DEFAULT")).Append(')');

              String emulatorAndCore = emulatorName;
              emulatorAndCore.Append(':').Append(coreName);
              emu_choice->add(displayName, emulatorAndCore, emulatorName == currentEmulator && coreName == currentCore, emulatorAndCore);
            }
          }

          ed = emu_choice;
        }
        else if (field.Key() == "core")
        {
          continue;
        }
        else if (field.Key() == "ratio")
        {
          auto ratio_choice = std::make_shared<OptionListComponent<String> >(mWindow, "ratio", false,
                                                                                  FONT_SIZE_MEDIUM);
          row.addElement(ratio_choice, false);
          const std::map<String, String>& ratioMap = LibretroRatio::GetRatio();
          if (mMetaData.Ratio().empty())
          {
            mMetaData.SetRatio("auto");
          }
          for (const auto& ratio : ratioMap)
          {
            ratio_choice->add(ratio.first, ratio.second, mMetaData.Ratio() == ratio.second, ratio.second);
          }
          ed = ratio_choice;
        }
        else if (field.Key() == "genreid")
        {
          auto genre_choice = std::make_shared<OptionListComponent<String> >(mWindow, "genre", false,
                                                                                  FONT_SIZE_MEDIUM);
          row.addElement(genre_choice, false);
          Genres::GenreMap map = Genres::GetShortNameMap();
          for(const auto& genre : Genres::GetOrderedList())
          {
            String genreString((int)genre);
            genre_choice->add((Genres::IsSubGenre(genre) ? "    " : "") + Genres::GetFullName(genre),genreString, mMetaData.GenreId() == genre, genreString);
          }
          ed = genre_choice;
        }
        break;
      case MetadataFieldDescriptor::EditableType::Text:
      {
        // MD_STRING
        ed = std::make_shared<TextComponent>(window, "", menuTheme.Text().font, menuTheme.Text().color, TextAlignment::Right);
        row.addElement(ed, true);

        auto spacer = std::make_shared<Component>(mWindow);
        spacer->setSize(Renderer::Instance().DisplayWidthAsFloat() * 0.005f, 0);
        row.addElement(spacer, false);

        auto bracket = std::make_shared<ImageComponent>(mWindow);

        bracket->setImage(menuTheme.Elements().FromType(MenuThemeData::IconElement::Type::Arrow));
        bracket->setColorShift(menuTheme.Text().color);
        bracket->setResize(Vector2f(0, lbl->getFont()->getLetterHeight()));
        row.addElement(bracket, false);

        bool multiLine = field.Type() == MetadataFieldDescriptor::DataType::Text;
        const String& title = field.DisplayPrompt();
        auto updateVal = [ed](const String &newVal)
        {
          ed->setValue(newVal);
        }; // ok callback (apply new value to ed)
        row.makeAcceptInputHandler([this, title, ed, updateVal, multiLine]
                                   {
                                     if (!multiLine)
                                     {
                                       mWindow.pushGui(new GuiTextEditPopupKeyboard(mWindow, title, ed->getValue(), updateVal, multiLine));
                                     }
                                     else
                                     {
                                       mWindow.pushGui(new GuiTextEditPopup(mWindow, title, ed->getValue(), updateVal, multiLine));
                                     }
                                   });
        break;
      }
      case MetadataFieldDescriptor::EditableType::None:
      default: break;
    }

    assert(ed);
    mList->addRow(row);
    GetValueMethodType method = field.GetValueMethod();
    ed->setValue((mMetaData.*method)());
    mEditors.push_back(ed);
    mMetaDataEditable.push_back(&field);
  }

  std::vector<std::shared_ptr<ButtonComponent> > buttons;

  if (main)
  {
    // append a shortcut to the secondary menu
    ComponentListRow row;
    auto lbl = std::make_shared<TextComponent>(mWindow, _("MORE DETAILS"), menuTheme.Text().font, menuTheme.Text().color);
    row.addElement(lbl, true);
    y += lbl->getFont()->getHeight();

    auto bracket = std::make_shared<ImageComponent>(mWindow);
    bracket->setImage(menuTheme.Elements().FromType(MenuThemeData::IconElement::Type::Arrow));
    bracket->setColorShift(menuTheme.Text().color);

    bracket->setResize(Vector2f(0, lbl->getFont()->getLetterHeight()));
    row.addElement(bracket, false);

    row.makeAcceptInputHandler([this]
                               {
                                 // call the same Gui with "main" set to "false"
                                 mWindow.pushGui(new GuiMetaDataEd(mWindow, mSystemManager, mGame, nullptr, nullptr, false));
                               });

    mList->addRow(row);
  }


  if (main)
  {
    buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("SCRAPE"), _("SCRAPE"), std::bind(&GuiMetaDataEd::fetch, this)));
  }

  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("SAVE"), _("SAVE"), [this]
  {
    save();
    Close();
  }));

  buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("CANCEL"), _("CANCEL"), [this] { Close(); }));

  if (main && mActions != nullptr && !mGame.System().IsVirtual())
  {
    auto deleteFileAndSelf = [this]
    {
      mActions->Delete(mGame);
      Close();
    };
    auto deleteBtnFunc = [this, deleteFileAndSelf]
    {
      mWindow.pushGui(new GuiMsgBox(mWindow, _("THIS WILL DELETE A FILE!\nARE YOU SURE?"), _("YES"), deleteFileAndSelf, _("NO"), nullptr));
    };
    buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("DELETE"), _("DELETE"), deleteBtnFunc));
  }


  mButtons = MenuComponent::MakeButtonGrid(mWindow, buttons);
  mGrid.setEntry(mButtons, Vector2i(0, 2), true, false);

  mGrid.setUnhandledInputCallback([this](const InputCompactEvent& event) -> bool
                                  {
                                    if (event.AnyDownPressed())
                                    {
                                      mGrid.setCursorTo(mList);
                                      mList->setCursorIndex(0);
                                      return true;
                                    }
                                    if (event.AnyUpPressed())
                                    {
                                      mList->setCursorIndex(mList->size() - 1);
                                      mGrid.moveCursor(Vector2i(0, 1));
                                      return true;
                                    }
                                    return false;
                                  });

  // resize + center
  y /= Renderer::Instance().DisplayHeightAsFloat();
  setSize(Renderer::Instance().DisplayWidthAsFloat() * 0.95f, Renderer::Instance().DisplayHeightAsFloat() * (y + 0.15f));
  setPosition((Renderer::Instance().DisplayWidthAsFloat() - mSize.x()) / 2, (Renderer::Instance().DisplayHeightAsFloat() - mSize.y()) / 2);
}

void GuiMetaDataEd::onSizeChanged()
{
  mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

  mGrid.setSize(mSize);

  const float titleHeight = mTitle->getFont()->getLetterHeight();
  const float subtitleHeight = mSubtitle->getFont()->getLetterHeight();
  const float titleSubtitleSpacing = mSize.y() * 0.03f;

  mGrid.setRowHeightPerc(0, (titleHeight + titleSubtitleSpacing + subtitleHeight + TITLE_VERT_PADDING) / mSize.y());
  mGrid.setRowHeightPerc(2, mButtons->getSize().y() / mSize.y());

  mHeaderGrid->setRowHeightPerc(1, titleHeight / mHeaderGrid->getSize().y());
  mHeaderGrid->setRowHeightPerc(2, titleSubtitleSpacing / mHeaderGrid->getSize().y());
  mHeaderGrid->setRowHeightPerc(3, subtitleHeight / mHeaderGrid->getSize().y());
}

void GuiMetaDataEd::save()
{
  MetadataType typesSaved = MetadataType::None;
  for (int i = 0; i < (int)mEditors.size(); i++)
  {
    if (mMetaDataEditable[i]->Type() != MetadataFieldDescriptor::DataType::List)
    {
      if ((mMetaData.*(mMetaDataEditable[i]->GetValueMethod()))() != mEditors[i]->getValue())
      {
        (mMetaData.*(mMetaDataEditable[i]->SetValueMethod()))(mEditors[i]->getValue());
        typesSaved |= mMetaDataEditable[i]->MetaType();
      }
    }
    else
    {
      std::shared_ptr<Component> ed = mEditors[i];
      std::shared_ptr<OptionListComponent<String>> list = std::static_pointer_cast<OptionListComponent<String>>(ed);

      if (mMetaDataEditable[i]->Key() == "emulator")
      {
        // Split emulator & core
        String::List split = list->getSelected().Split(':');
        if (split.size() == 2)
        {
          String defaultEmulator;
          String defaultCore;
          EmulatorManager::GetDefaultEmulator(mGame.System(), defaultEmulator, defaultCore);
          if (split[0] == defaultEmulator && split[1] == defaultCore)
          {
            mMetaData.SetEmulator("");
            mMetaData.SetCore("");
          }
          else
          {
            mMetaData.SetEmulator(split[0]);
            mMetaData.SetCore(split[1]);
          }
          typesSaved |= MetadataType::Emulator;
          typesSaved |= MetadataType::Core;
        }
        else { LOG(LogError) << "[MetadataEditor] Error splitting emulator and core!"; }
      }
      else if ((mMetaData.*(mMetaDataEditable[i]->GetValueMethod()))() != list->getSelected())
      {
        (mMetaData.*(mMetaDataEditable[i]->SetValueMethod()))(list->getSelected());
        typesSaved |= mMetaDataEditable[i]->MetaType();
      }
    }
  }

  mSystemManager.UpdateSystemsOnGameChange(&mGame, typesSaved, false);
  if (mActions != nullptr)
    mActions->Modified(mGameListView, mGame);
}

void GuiMetaDataEd::fetch()
{
  save();
  mWindow.pushGui(new GuiScraperSingleGameRun(mWindow, mSystemManager, mGame, this));
}

void GuiMetaDataEd::ScrapingComplete(FileData& game, MetadataType changedMetadata)
{
  (void)game;
  for (int i = 0; i < (int)mEditors.size(); i++)
  {
    GetValueMethodType method = mMetaDataEditable[i]->GetValueMethod();
    mEditors[i]->setValue((mMetaData.*method)());
  }
  mSystemManager.UpdateSystemsOnGameChange(&mGame, changedMetadata, false);
}

void GuiMetaDataEd::close(bool closeAllWindows)
{
  // find out if the user made any changes
  bool dirty = false;
  for (int i = 0; i < (int)mEditors.size(); i++)
  {
    // because mMetada do not contains default system emulator & core
    if(mMetaDataEditable[i]->Key() == "emulator")
    {
      String editorEmulatorCore = mEditors[i]->getValue();

      String metaEmulator = mMetaData.Emulator();
      String metaCore = mMetaData.Core();
      String metaEmulatorCore = metaEmulator;
      metaEmulatorCore.Append(':').Append(metaCore);

      String defaultEmulator;
      String defaultCore;
      EmulatorManager::GetDefaultEmulator(mGame.System(), defaultEmulator, defaultCore);
      String defaulEmulatorAndCore = defaultEmulator;
      defaulEmulatorAndCore.Append(':').Append(defaultCore);

      if(editorEmulatorCore != metaEmulatorCore && editorEmulatorCore != defaulEmulatorAndCore)
      {
        dirty = true;
        break;
      }
      continue;
    }

    GetValueMethodType method = mMetaDataEditable[i]->GetValueMethod();
    if ((mMetaData.*method)() != mEditors[i]->getValue())
    {
      dirty = true;
      break;
    }
  }

  std::function<void()> closeFunc;
  if (!closeAllWindows)
  {
    closeFunc = [this] { Close(); };
  }
  else
  {
    closeFunc = [this]
    {
      mWindow.CloseAll();
    };
  }


  if (dirty)
  {
    // changes were made, ask if the user wants to save them
    mWindow.pushGui(new GuiMsgBox(mWindow,
                                   _("SAVE CHANGES?"),
                                   _("YES"), [this, closeFunc]
                                   {
                                     save();
                                     closeFunc();
                                   },
                                   _("NO"), closeFunc
                                  ));
  }
  else
  {
    closeFunc();
  }
}

bool GuiMetaDataEd::ProcessInput(const InputCompactEvent& event)
{
  if (Component::ProcessInput(event))
  {
    return true;
  }

  if (event.CancelReleased() || event.StartReleased())
  {
    close(event.StartReleased());
    return true;
  }

  return false;
}

bool GuiMetaDataEd::CollectHelpItems(Help& help)
{
  mGrid.CollectHelpItems(help);
  help.Set(Help::Cancel(), _("BACK"))
      .Set(HelpType::Start, _("CLOSE"));
  return true;
}
