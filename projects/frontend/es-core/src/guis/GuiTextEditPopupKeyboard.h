#include "GuiComponent.h"

#include "components/NinePatchComponent.h"
#include "components/ButtonComponent.h"
#include "components/ComponentGrid.h"
#include "components/TextEditComponent.h"
#include "components/TextComponent.h"

class GuiTextEditPopupKeyboard : public GuiComponent
{
public:
	GuiTextEditPopupKeyboard(Window* window, const std::string& title, const std::string& initValue,
		                       const std::function<void(const std::string&)>& okCallback, bool multiLine,
		                       const std::string& acceptBtnText);
  GuiTextEditPopupKeyboard(Window* window, const std::string& title, const std::string& initValue,
                           const std::function<void(const std::string&)>& okCallback, bool multiLine)
    : GuiTextEditPopupKeyboard(window, title, initValue, okCallback, multiLine, "OK")
  {
  }

	bool input(InputConfig* config, Input input) override;
	void update(int) override {}
	void onSizeChanged() override;
	std::vector<HelpPrompt> getHelpPrompts() override;

private:

    class KeyboardButton
    {
    public:
        std::shared_ptr<ButtonComponent> button;
		    const std::string key;
		    const std::string shiftedKey;
        KeyboardButton(const std::shared_ptr<ButtonComponent>& b, const std::string& k, const std::string& sk)
          : button(b),
            key(k),
            shiftedKey(sk)
        {
        };
    };

	void switchShift();
    std::shared_ptr<ButtonComponent> makeButton(const std::string& key, const std::string& shiftedKey);

    NinePatchComponent mBackground;
	ComponentGrid mGrid;

	std::vector< KeyboardButton > keyboardButtons;

	std::shared_ptr<TextComponent> mTitle;
	std::shared_ptr<TextEditComponent> mText;
	std::shared_ptr<ComponentGrid> mKeyboardGrid;
	std::shared_ptr<ComponentGrid> mButtonGrid;
	std::shared_ptr<ComponentGrid> mButtons;

	bool mMultiLine;
	bool mShift = false;
	std::shared_ptr<ButtonComponent> mShiftButton;
};

