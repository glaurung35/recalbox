//
// Created by xizor on 30/03/18.
//
#pragma once

#include "guis/Gui.h"
#include "components/NinePatchComponent.h"
#include "components/ComponentGrid.h"
#include "Window.h"
#include "utils/Log.h"
#include "utils/math/Vectors.h"

class GuiInfoPopup : public Gui, public Window::InfoPopup
{
  public:
    GuiInfoPopup(Window& window, const std::string& message, int duration, int icon = 0);
    ~GuiInfoPopup() override = default;
    void render(const Transform4x4f& parentTrans) override;
    inline void stop() { running = false; };

  private:
    int mDuration;
    int maxAlpha;
    unsigned int mFrameColor;
    bool updateState();
    int mStartTime;
    ComponentGrid mGrid;
    NinePatchComponent mFrame;
    bool running;
    std::shared_ptr<TextComponent> mMsgText;
    std::shared_ptr<TextComponent> mMsgIcon;
};