//
// Created by bkg2k on 26/09/23.
//

#include "OSDManager.h"
#include "input/InputManager.h"

OSDManager::OSDManager(WindowManager& window)
  : Gui(window)
  , mMapper(InputManager::Instance().Mapper())
  , mFont(Font::get(Renderer::Instance().DisplayHeightAsInt() / 32))
  , mFPSFont(Font::get(FONT_SIZE_SMALL))
  , mPadGlyph()
  , mFrameStart { 0 }
  , mFrameTimingComputations { 0 }
  , mFrameTimingTotal { 0 }
  , mAlpha { sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha, sMinAlpha }
  , mPadChar(0)
  , mTimingIndex(0)
  , mRecordedTimings(0)
  , mPadCount(0)
  , mActive(false)
  , mForcedActive(false)
{
  memset(mFrameStart, 0, sizeof(mFrameStart));
  memset(mFrameTimingComputations, 0, sizeof(mFrameTimingComputations));
  memset(mFrameTimingTotal, 0, sizeof(mFrameTimingTotal));

  Vector2f size = mFPSFont->sizeText(" 00.0 Fps (00.0%) ");
  mFPSArea = Rectangle(Renderer::Instance().DisplayWidthAsFloat() - size.x() - 2, 2, size.x(), size.y());

  UpdatePadIcon();
  setPosition(0, 0);
  setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());
}

bool OSDManager::ProcessInput(const InputCompactEvent& event)
{
  // Pad alpha
  if (mActive)
    if (event.Device().IsPad())
      if (int padIndex = InputManager::Instance().Mapper().PadIndexFromDeviceIdentifier(event.RawEvent().Device()); padIndex >= 0)
        mAlpha[padIndex] = sMaxAlpha;
  return false;
}

void OSDManager::Update(int deltaTime)
{
  mActive = RecalboxConf::Instance().GetPadOSD() || mForcedActive;
  if (!mActive) return;
  // Set alphas
  mPadCount = InputManager::Instance().Mapper().ConnectedPadCount();
  for(int i = Input::sMaxInputDevices; --i >= 0; )
    mAlpha[i] = Math::clampi(mAlpha[i] -= deltaTime / 4, i < mPadCount ? sMinAlpha : 0, sMaxAlpha);
}

void OSDManager::Render(const Transform4x4f& parentTrans)
{
  Renderer::SetMatrix(parentTrans);

  RenderPads();
  RenderFPS();
}

void OSDManager::UpdatePadIcon()
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

void OSDManager::RecordStopFrame()
{
  if (mTimingIndex == 0)
    if (mFrameStart[mTimingIndex] == 0) return;
  mFrameTimingComputations[mTimingIndex] = (int)SDL_GetTicks() - mFrameStart[mTimingIndex];
  mTimingIndex = (mTimingIndex + 1) & sMaxFrameTimingMask;
  if (++mRecordedTimings >= sMaxFrameTiming) mRecordedTimings = sMaxFrameTiming - 1;
}

void OSDManager::RecordStartFrame()
{
  mFrameStart[mTimingIndex] = (int)SDL_GetTicks();
  if (mRecordedTimings != 0)
  {
    int previous = (mTimingIndex - 1) & sMaxFrameTimingMask;
    mFrameTimingTotal[previous] = mFrameStart[mTimingIndex] - mFrameStart[previous];
  }
}

float OSDManager::CalculateFPS()
{
  float fps = 0;
  for(int i = mRecordedTimings; --i >= 0; )
    fps += (float)mFrameTimingTotal[(mTimingIndex - (i + 1)) & sMaxFrameTimingMask];
  fps /= (float)mRecordedTimings;
  return 1000.f / fps;
}

float OSDManager::CalculateFramePercentage()
{
  float percent = 0;
  for(int i = mRecordedTimings; --i >= 0; )
  {
    int index = (mTimingIndex - (i + 1)) & sMaxFrameTimingMask;
    percent += (float)mFrameTimingComputations[index] /  (float)mFrameTimingTotal[index];
  }
  percent /= (float)mRecordedTimings;
  return percent * 100.f;
}

void OSDManager::RenderFPS()
{
  if (RecalboxConf::Instance().GetGlobalShowFPS())
  {
    float fps = CalculateFPS();
    float percent = CalculateFramePercentage();
    String s = (_F(" {0} Fps ({1}%) ") / _FOV(Frac, 1) / fps / percent).ToString();
    Renderer::DrawRectangle(mFPSArea, 0x000000C0);
    TextCache* text = mFPSFont->buildTextCache(s, mFPSArea.Left(), mFPSArea.Top(), 0xFFFFFFFF);
    mFPSFont->renderTextCache(text);
    delete text;
  }
}

void OSDManager::RenderPads()
{
  // Active?
  if (mActive)
  {
    static int flashing = 0;
    InputManager& inputManager = InputManager::Instance();
    int step = Renderer::Instance().DisplayHeightAsInt() / 32;
    int x = (int) (Renderer::Instance().DisplayWidthAsFloat() / 160.f);
    int w = (int) mPadGlyph.advance.x() + x / 2;
    int bh = (int) mPadGlyph.bearing.y();
    for (int i = mPadCount; --i >= 0;)
    {
      int y = i * (step + 1) + Renderer::Instance().DisplayHeightAsInt() / 80;
      mFont->renderCharacter(mPadChar, (float) x, (float) y, 1.f, 1.f, sColor | mAlpha[i]);
      InputDevice& device = inputManager.GetDeviceConfigurationFromIndex(mMapper.PadAt(i).mIndex);
      if (device.HasBatteryLevel() && (device.BatteryLevel() > 15 || ((flashing >> 3) & 3) != 0))
      {
        float hr = (float) bh / (float) step;
        mFont->renderCharacter(device.BatteryLevelIcon(), (float) (x + w), (float) y, hr, hr, sColor | mAlpha[i]);
      }
    }
    flashing++;
  }
}

