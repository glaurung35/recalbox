#include "guis/GuiTextEditPopup.h"
#include "components/MenuComponent.h"
#include "Locale.h"
#include "MenuThemeData.h"

GuiTextEditPopup::GuiTextEditPopup(Window* window, const std::string& title, const std::string& initValue,
				                           const std::function<void(const std::string&)>& okCallback, bool multiLine,
				                           const std::string& acceptBtnText)
	: GuiComponent(window),
	  mBackground(window, ":/frame.png"),
	  mGrid(window, Vector2i(1, 3)),
	  mMultiLine(multiLine)
{
	
	auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
	
	mBackground.setImagePath(menuTheme->menuBackground.path);
	mBackground.setCenterColor(menuTheme->menuBackground.color);
	mBackground.setEdgeColor(menuTheme->menuBackground.color);
	
	addChild(&mBackground);
	addChild(&mGrid);

	mTitle = std::make_shared<TextComponent>(mWindow, StringUtil::toUpper(title), menuTheme->menuTitle.font, menuTheme->menuTitle.color, TextAlignment::Center);

	mText = std::make_shared<TextEditComponent>(mWindow);
	mText->setValue(initValue);

	if(!multiLine)
		mText->setCursor(initValue.size());

	std::vector< std::shared_ptr<ButtonComponent> > buttons;
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, acceptBtnText, acceptBtnText, [this, okCallback] { okCallback(mText->getValue()); delete this; }));
	buttons.push_back(std::make_shared<ButtonComponent>(mWindow, _("CANCEL"), _("DISCARD CHANGES"), [this] { delete this; }));

	mButtonGrid = makeButtonGrid(mWindow, buttons);

	mGrid.setEntry(mTitle, Vector2i(0, 0), false, true);
	mGrid.setEntry(mText, Vector2i(0, 1), true, false, Vector2i(1, 1), Borders::Top | Borders::Bottom);
	mGrid.setEntry(mButtonGrid, Vector2i(0, 2), true, false);

	float textHeight = mText->getFont()->getHeight();
	if(multiLine)
		textHeight *= 6;
	mText->setSize(0, textHeight);

	setSize(Renderer::getDisplayWidthAsFloat() * 0.5f, mTitle->getFont()->getHeight() + textHeight + mButtonGrid->getSize().y() + 40);
	setPosition((Renderer::getDisplayWidthAsFloat() - mSize.x()) / 2, (Renderer::getDisplayHeightAsFloat() - mSize.y()) / 2);
}

void GuiTextEditPopup::onSizeChanged()
{
	mBackground.fitTo(mSize, Vector3f::Zero(), Vector2f(-32, -32));

	mText->setSize(mSize.x() - 40, mText->getSize().y());

	// update grid
	mGrid.setRowHeightPerc(0, mTitle->getFont()->getHeight() / mSize.y());
	mGrid.setRowHeightPerc(2, mButtonGrid->getSize().y() / mSize.y());
	mGrid.setSize(mSize);
}

bool GuiTextEditPopup::input(InputConfig* config, Input input)
{
	if(GuiComponent::input(config, input))
		return true;

	// pressing back when not text editing closes us
	if(config->isMappedTo("a", input) && (input.value != 0))
	{
		delete this;
		return true;
	}

	return false;
}

std::vector<HelpPrompt> GuiTextEditPopup::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mGrid.getHelpPrompts();
	prompts.push_back(HelpPrompt("a", _("BACK")));
	return prompts;
}
