//
// Created by bkg2k on 26/10/2020.
//
#pragma once

#include <guis/Gui.h>
#include <components/BusyComponent.h>
#include <WindowManager.h>

/*!
 * @brief Syncnronized operations waiter
 */
class GuiSyncWaiter : public Gui
{
  public:
    explicit GuiSyncWaiter(WindowManager& window, const String& message)
      : Gui(window)
      , mBusyAnim(window)
    {
      // Configure animation
      mBusyAnim.setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());
      mBusyAnim.setText(message);
    }

    void Show()
    {
      mWindow.pushGui(this);
      mWindow.RenderAll(false);
    }

    void Hide()
    {
      if (mWindow.RemoveGui(this))
        mWindow.RenderAll(false);
    }

  protected:
    /*!
     * @brief Update components
     * @param deltaTime delta ms from the previous frame
     */
    void Update(int deltaTime) override
    {
      (void)deltaTime;
      // This waiter is not intended to be executes in the main loop.
      Hide();
    }

    /*!
     * @brief Draw
     * @param parentTrans Parent transformation
     */
    void Render(const Transform4x4f& parentTrans) override
    {
      Transform4x4f trans = parentTrans * getTransform();
      Renderer::SetMatrix(trans);

      Renderer::DrawRectangle(0.f, (Renderer::Instance().DisplayHeightAsFloat() - mBusyAnim.RealHeight() * 1.6f) / 2.0f,
                         Renderer::Instance().DisplayWidthAsFloat(), mBusyAnim.RealHeight() * 1.6f, 0x00000080);

      mBusyAnim.Render(trans);
    }

    /*!
     * @brief This window is an overlay and does not hide underlying windows
     * @return Always true
     */
    [[nodiscard]] bool IsOverlay() const override { return true; }

  private:
    //! Busy animation
    BusyComponent mBusyAnim;
};

