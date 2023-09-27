//
// Created by bkg2k on 26/09/23.
//

#include "InputDeviceOSD.h"
#include "input/InputManager.h"

InputDeviceOSD::InputDeviceOSD(WindowManager& window)
  : Gui(window)
  , mMapper(InputManager::Instance().Mapper())
  , mFont(Font::get(Renderer::Instance().DisplayHeightAsInt() / 32))
  , mPadGlyph()
  , mAlpha { sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha }
  , mPadChar(0)
  , mPadCount(0)
  , mLastTimeBatteryRefreshed(0)
{
  UpdatePadIcon();
  setPosition(0, 0);
  setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());
}

bool InputDeviceOSD::ProcessInput(const InputCompactEvent& event)
{
  // Pad alpha
  if (mActive)
    if (event.Device().IsPad())
      if (int padIndex = InputManager::Instance().Mapper().PadIndexFromDeviceIdentifier(event.RawEvent().Device()); padIndex >= 0)
        mAlpha[padIndex] = sMaxAlpha;
  return false;
}

void InputDeviceOSD::Update(int deltaTime)
{
  mActive = RecalboxConf::Instance().GetPadOSD() || mForcedActive;
  if (!mActive) return;
  // Set alphas
  mPadCount = InputManager::Instance().Mapper().ConnectedPadCount();
  for(int i = Input::sMaxInputDevices; --i >= 0; )
    mAlpha[i] = Math::clampi(mAlpha[i] -= deltaTime / 4, i < mPadCount ? sMinAlpha : 0, sMaxAlpha);
}

void InputDeviceOSD::Render(const Transform4x4f& parentTrans)
{
  (void)parentTrans;
  static int flashing = 0;

  // Active?
  if (!mActive) return;

  InputManager& inputManager = InputManager::Instance();
  int step = Renderer::Instance().DisplayHeightAsInt() / 32;
  int x = (int)(Renderer::Instance().DisplayWidthAsFloat() / 160.f);
  int w = (int)mPadGlyph.advance.x() + x / 2;
  int bh = (int)mPadGlyph.bearing.y();
  for(int i = mPadCount; --i >= 0; )
  {
    int y = i * (step + 1) + Renderer::Instance().DisplayHeightAsInt() / 80;
    mFont->renderCharacter(mPadChar, (float)x, (float)y, 1.f, 1.f, sColor | mAlpha[i]);
    InputDevice& device = inputManager.GetDeviceConfigurationFromIndex(mMapper.PadAt(i).mIndex);
    if (device.HasBatteryLevel() && (device.BatteryLevel() > 15 || ((flashing >> 3) & 3) != 0))
    {
      float hr = (float)bh / (float)step;
      mFont->renderCharacter(device.BatteryLevelIcon(), (float)(x + w), (float)y, hr, hr, sColor | mAlpha[i]);
    }
  }
  flashing++;

}

void InputDeviceOSD::UpdatePadIcon()
{
  mPadChar = 0xF25E;
  switch(RecalboxConf::Instance().GetPadOSDType())
  {
    case RecalboxConf::PadOSDType::MD: mPadChar = 0xF26C; break;
    case RecalboxConf::PadOSDType::XBox: mPadChar = 0xF2F0; break;
    case RecalboxConf::PadOSDType::PSX: mPadChar = 0xF2C8; break;
    case RecalboxConf::PadOSDType::N64: mPadChar = 0xF260; break;
    case RecalboxConf::PadOSDType::DC: mPadChar = 0xF26E; break;
    case RecalboxConf::PadOSDType::Snes:
    default: break;
  }
  mPadGlyph = mFont->Character(mPadChar);
}

