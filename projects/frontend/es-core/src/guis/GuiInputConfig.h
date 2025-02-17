#pragma once

#include <guis/Gui.h>
#include <components/NinePatchComponent.h>
#include <components/ComponentGrid.h>
#include <components/ComponentList.h>
#include <input/InputStack.h>
#include <utils/locale/LocaleHelper.h>
#include <input/IInputChange.h>

class TextComponent;

class GuiInputConfig : public Gui
                     , private IComponentListRowEventInterceptor
                     , private IInputChange
{
  public:
    GuiInputConfig(WindowManager& window, InputDevice* target, const std::function<void()>& doneCallback);
    ~GuiInputConfig() override;
    void onSizeChanged() override;
      bool CollectHelpItems(Help& help) override;

  private:
    //! Max axis
    static constexpr int sMaxAxis = 16;

    class FormInput
    {
      public:
        const String name;
        const String label;
        const Path icon;
        const bool skippable;
        const InputEvent::EventType preferredType;
        FormInput(const String& inName, const String& inLabel, const Path& inIcon, bool inSkippable, InputEvent::EventType inPreferredType)
          : name(inName),
            label(inLabel),
            icon(inIcon),
            skippable(inSkippable),
            preferredType(inPreferredType)
        {
        };
    };

    inline void setMapped() { setText(_("ALREADY TAKEN"), mMainColor); }
    inline void setSkipped() { setText(_("(skipped)"), mMainColor); }
    inline void setNotDefined() { setText("", mMainColor); }
    inline void setAssignedTo(InputEvent input) { setText(input.ToString().UpperCase(), mMainColor); }

    void initFormInputs();
    void addFormInput(const char* name, const String& label, const char* icon, bool skippable, InputEvent::EventType preferredType);

    void setHelpMessage();
    void setPress();
    void setText(const String& msg, unsigned int color);
    void setText(const String& msg, unsigned int color, int inputId);

    bool assign(InputEvent input);
    void unAssign();
    void restaurePreviousAssignment();
    bool isAssigned();

    void rowDone();

    bool EventReceived(int id, const InputCompactEvent& event) override;

    NinePatchComponent mBackground;
    ComponentGrid mGrid;

    std::vector< FormInput > mFormInputs;

    std::shared_ptr<TextComponent> mTitle;
    std::shared_ptr<TextComponent> mSubtitle1;
    std::shared_ptr<TextComponent> mSubtitle2;
    std::shared_ptr<ComponentList> mList;
    std::vector< std::shared_ptr<TextComponent> > mMappings;
    std::shared_ptr<ComponentGrid> mButtonGrid;

    InputDevice* mTargetDevice;

    std::vector<InputEvent> mEventList; //!< Recorded events
    int mActiveButtons;                 //!< Active (pressed) buttons
    int mActiveAxis;                    //!< Active (far enought from their original position) axis
    int mActiveHats;                    //!< Active (non zero) hats

    bool mNeutralPositionSet;           //!< Neutral position has been reached, we can start recording
    bool mCursorOnList;
    bool mFirstEventReceived;           //!< First event received flag
    unsigned int mMainColor;

    /*!
     * @brief Analyse and record raw event
     * @param raw raw event
     */
    void RecordRawInput(const InputEvent& raw);

    //! Check if the current pad is in neutral position
    [[nodiscard]] bool NeutralPosition() const { return (mActiveHats | mActiveButtons | mActiveAxis) == 0; }

    /*!
     * @brief Process all received events
     */
    void ProcessEvents();


    /*
     * IInputChange implementation
     */

    //! Refresh pad list
    void PadsAddedOrRemoved(bool removed) override { if (removed) Close(); };
};
