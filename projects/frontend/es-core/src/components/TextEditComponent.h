#pragma once

#include <guis/IGuiArcadeVirtualKeyboardInterface.h>
#include "components/base/Component.h"
#include "components/NinePatchComponent.h"

class Font;

// Used to enter text.
class TextEditComponent : public Component, public IGuiArcadeVirtualKeyboardInterface
{
public:
	explicit TextEditComponent(WindowManager&window);
	
	void textInput(const char* text) override;
	bool ProcessInput(const InputCompactEvent& event) override;
	void Update(int deltaTime) override;
	void Render(const Transform4x4f& parentTrans) override;

	void onFocusGained() final;
	void onFocusLost() final;

	void onSizeChanged() override;

	void setValue(const String& val) override;
	String getValue() const override { return mText; }

	inline bool isEditing() const { return mEditing; };
	inline const std::shared_ptr<Font>& getFont() const { return mFont; }

	inline void setCursorChangedCallback(const std::function<void()>& callback) { mTextChangedCallback = callback; };

	void setCursor(size_t pos);

	bool CollectHelpItems(Help& help) override;

	void startEditing();
	void stopEditing();

  int getVerticalMargins() const;
  int getTextHeight() const;

private:
  static constexpr int TEXT_PADDING_HORIZ = 10;
  static constexpr int TEXT_PADDING_VERT = 4;

  static constexpr int CURSOR_REPEAT_START_DELAY = 500;
  static constexpr int CURSOR_REPEAT_SPEED = 28; // lower is faster

	void onTextChanged();
	void onCursorChanged();

	void updateCursorRepeat(int deltaTime);
	void moveCursor(int amt);

	bool isMultiline();
	Vector2f getTextAreaPos();
	Vector2f getTextAreaSize() const;

	String mText;
	bool mFocused;
	bool mEditing;
	int mCursor; // cursor position in characters

	std::function<void()> mTextChangedCallback;

	int mCursorRepeatTimer;
	int mCursorRepeatDir;
  int mTextWidth;
  int mTextHeight;

	Vector2f mScrollOffset;

	NinePatchComponent mBox;

	std::shared_ptr<Font> mFont;


    /*
     * GuiArcadeVirtualKeyboardInterface implementation
     */

    /*!
   * @brief Called when the edited text change.
   * Current text is available from the Text() method.
   */
    void ArcadeVirtualKeyboardTextChange(GuiArcadeVirtualKeyboard& vk, const String& text) override;

    /*!
     * @brief Called when the edited text is validated (Enter or Start)
     * Current text is available from the Text() method.
     */
    void ArcadeVirtualKeyboardValidated(GuiArcadeVirtualKeyboard& vk, const String& text) override;

    /*!
     * @brief Called when the edited text is cancelled.
     */
    void ArcadeVirtualKeyboardCanceled(GuiArcadeVirtualKeyboard& vk) override;
};
