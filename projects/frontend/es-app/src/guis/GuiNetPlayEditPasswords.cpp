#include <RecalboxConf.h>
#include "guis/GuiNetPlayEditPasswords.h"

#include <guis/GuiArcadeVirtualKeyboard.h>
#include "components/SwitchComponent.h"
#include "utils/locale/LocaleHelper.h"

GuiNetPlayEditPasswords::GuiNetPlayEditPasswords(WindowManager& window)
  : Gui(window),
    mCurrentPasswordIndex(0),
    mMenu(window, _("PREDEFINED PASSWORDS"))
{
	addChild(&mMenu); // #TODO: Rewrite using MenuBase

  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();

  for(int i = 0; i < DefaultPasswords::sPasswordCount; i++)
  {
    String password = RecalboxConf::Instance().AsString("netplay.password." + String(i), DefaultPasswords::sDefaultPassword[i]);
    mPasswords[i] = std::make_shared<TextComponent>(mWindow, password, menuTheme.Text().font, menuTheme.Text().color);
    mMenu.addWithLabel(mPasswords[i], _("PASSWORD #%i").Replace("%i", String(i)),
                       "", false, true, [this, i]
                       {
                         mCurrentPasswordIndex = i;
                         Gui* vk = new GuiArcadeVirtualKeyboard(mWindow, _("PASSWORD #%i").Replace("%i", String(i)), mPasswords[i]->getValue(), this);
                         mWindow.pushGui(vk);
                       });
  }

	mMenu.addButton(_("OK"), "OK", [this]
	{
    for(int i = DefaultPasswords::sPasswordCount; --i >= 0; )
      RecalboxConf::Instance().SetString("netplay.password." + String(i), mPasswords[i]->getValue());
    RecalboxConf::Instance().Save();
    Close();
	});

	mMenu.setPosition((Renderer::Instance().DisplayWidthAsFloat() - mMenu.getSize().x()) / 2, (Renderer::Instance().DisplayHeightAsFloat() - mMenu.getSize().y()) / 2);
}

bool GuiNetPlayEditPasswords::ProcessInput(const InputCompactEvent& event)
{
  if (event.CancelReleased())
  {
    Close();
    return true;
  }

  return Component::ProcessInput(event);
}

bool GuiNetPlayEditPasswords::CollectHelpItems(Help& help)
{
  mMenu.CollectHelpItems(help);
	help.Set(Help::Cancel(), _("BACK"))
	    .Set(Help::Valid(), _("EDIT"));
	return true;
}

void GuiNetPlayEditPasswords::ArcadeVirtualKeyboardTextChange(GuiArcadeVirtualKeyboard&, const String&)
{
}

void GuiNetPlayEditPasswords::ArcadeVirtualKeyboardValidated(GuiArcadeVirtualKeyboard&, const String& text)
{
  mPasswords[mCurrentPasswordIndex]->setText(text);
  mMenu.onSizeChanged();
}

void GuiNetPlayEditPasswords::ArcadeVirtualKeyboardCanceled(GuiArcadeVirtualKeyboard&)
{
}
