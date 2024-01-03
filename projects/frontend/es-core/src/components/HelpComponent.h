#pragma once

#include "components/base/ThemableComponent.h"
#include "ComponentGrid.h"
#include "themes/IThemeSwitchable.h"
#include "help/Help.h"
#include "IViewChanged.h"

class ComponentGrid;

class HelpComponent : public ThemableComponent
                    , public IThemeSwitchable
                    , public IViewChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Target window
     */
    explicit HelpComponent(WindowManager&window);

    //! Get help bar height
    [[nodiscard]] float Height() const { return mGrid.getSize().y() + 0.5f; }

    //! Get help bar font
    [[nodiscard]] std::shared_ptr<Font> TextFont() const { return mFont; }

    /*!
     * @brief Called once per frame. Override to implement your own drawings.
     * Call your base::Update() to ensure animation and childrens are updated properly
     * @param deltaTime Elapsed time from the previous frame, in millisecond
     */
    void Update(int deltaTime) final;

    /*!
     * @brief Called once per frame, after Update
     * Implement your own Render() to draw your own compponents or over-drawings
     * First, execute: 	Transform4x4f trans = (parentTrans * getTransform()).round();
     *                  Renderer::setMatrix(trans);
     * Then draw your components
     * Finally, call your base.Render(trans) to draw animations and childrens
     * @param parentTrans Transformation
     */
    void Render(const Transform4x4f& parent) final;

    void setOpacity(unsigned char opacity) final;

    /*!
     * @brief Refresh the help component with new items
     * @param newHelpItems New help items
     * @param force Force refresh, even if the new items match old ones
     */
    void Refresh(const Help& newHelpItems, bool force);

    /*
     * IThemeSwitchable implementation
     */

    /*!
     * @brief Implementation istruct the Theme switcher to use a specific system theme
     * Returning nullptr means the global theme will be used
     * @return SystemData or nullptr
     */
    [[nodiscard]] SystemData* SystemTheme() const override { return nullptr; }

    /*!
     * @brief Called when theme switch.
     * @param theme New Theme
     * @param refreshOnly True if we need to refresh components only
     */
    void SwitchToTheme(const ThemeData& theme, bool refreshOnly) override;

    /*
     * Themable implemantation
     */

    /*!
     * @brief Called when a theme element is applyed on the current component.
     * @param element Element instance
     * @param properties Properties to update
     */
    void OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType() const override { return ThemeElementType::HelpSystem; }

    /*!
     * @brief A view or a window visibility has changed
     * @param currentView Current view
     * @param hasWindowOver Has window over the current view?
     */
    void ViewChanged(ViewType currentView, bool hasWindowOver) override;

  private:
    static constexpr int sPauseTime = 5000;

    //! Scrolling state for very long help systems
    enum class Scrolling
    {
      Initialize,    //!< Initialization frame
      PauseLeft,     //!< Paused left for X seconds
      ScrollToRight, //!< Scrolling to the right
      PauseRight,    //!< Paused right for X seconds
      ScrollToLeft,  //!< Scrolling to the left
    };

    //! Internal image map
    static const HashMap<HelpType, Path>& IconPathMap();

    //! Previous Help object
    Help mHelp;

    //! Grid component, 4 cells per Help item
  	ComponentGrid mGrid;

  	//! Scrolling
  	Scrolling mScrolling;

  	//! Scrolling time accumulator
  	int mScrollingTimeAccumulator;
  	//! Scrolling length
  	int mScrollingLength;
  	//! Scrolling offset
  	int mScrollingOffset;

    //! Images for every single help type
    Path mImagesPath[(int)HelpType::__Count + 1];
    //! Text font
    std::shared_ptr<Font> mFont;
    //! Position of help items
    Vector2f mPosition;
    //! Icon color
    unsigned int mIconColor;
    //! Text color
    unsigned int mTextColor;
};
