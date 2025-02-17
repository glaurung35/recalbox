//
// Created by xizor on 30/03/18.
//

#include "GuiInfoPopupBase.h"
#include "Renderer.h"
#include <SDL_timer.h>
#include <themes/MenuThemeData.h>
#include <components/ScrollableContainer.h>
#include <themes/ThemeManager.h>

GuiInfoPopupBase::GuiInfoPopupBase(WindowManager& window, bool selfProcessed, int duration, PopupType icon, int gridWidth, int gridHeight, float widthRatio)
  : Gui(window)
  , mGrid(window, Vector2i(gridWidth, gridHeight))
  , mFrame(window, Path(":/frame.png"))
  , mFrameColor(0)
  , mType(icon)
  , mCorner(Corner::TopRight)
  , mTargetOffset(0)
  , mDuration(duration * 1000)
  , mMaxAlpha(0xFF)
  , mStartTime(0)
  , mWidthRatio(widthRatio)
  , mRunning(true)
  , mInitialized(false)
  , mSelfProcessed(selfProcessed)
{
}

void GuiInfoPopupBase::Initialize()
{
  if (mInitialized) return;
  mInitialized = true;

  float maxWidth = mWidthRatio * Renderer::Instance().DisplayWidthAsFloat() * (Renderer::Instance().Is480pOrLower() ? 0.6f : 0.2f);
  float maxHeight = Renderer::Instance().DisplayHeightAsFloat() * (Renderer::Instance().Is480pOrLower() ? 0.6f : 0.4f);

  const MenuThemeData& menuTheme = ThemeManager::Instance().Menu();

  mMaxAlpha = (int)menuTheme.Background().color & 0xFF;
  mFrameColor = menuTheme.Background().color;

  // add a padding to the box
  int paddingX = (int) (Renderer::Instance().DisplayWidthAsFloat() * 0.02f);
  int paddingY = (int) (Renderer::Instance().DisplayHeightAsFloat() * 0.02f);

  float msgHeight = AddComponents(mWindow, mGrid, maxWidth, maxHeight, paddingX, paddingY);
  mGrid.setSize(maxWidth + (float)paddingX, msgHeight + (float)paddingY);

  mCorner = Corner::TopRight;
  float posX = Renderer::Instance().DisplayWidthAsFloat() * (Board::Instance().CrtBoard().IsCrtAdapterAttached() ? 0.94f : 0.98f ) - mGrid.getSize().x();
  float posY = Renderer::Instance().DisplayHeightAsFloat();

  setPosition(posX, posY, 0);
  setSize(mGrid.getSize());

  mFrame.setImagePath(menuTheme.Background().path);
  mFrame.setCenterColor(mFrameColor);
  mFrame.setEdgeColor(mFrameColor);
  mFrame.fitTo(mGrid.getSize(), Vector3f::Zero(), Vector2f(-32, -32));
  addChild(&mFrame);

  // we only init the actual time when we first start to render
  mStartTime = 0;

  addChild(&mGrid);
}

void GuiInfoPopupBase::Render(const Transform4x4f& parentTrans)
{
  (void)parentTrans;

	// we use identity as we want to render on a specific window position, not on the view
	Transform4x4f trans = getTransform() * Transform4x4f::Identity();

  Renderer::SetMatrix(trans);
  renderChildren(trans);
}

void GuiInfoPopupBase::Update(int delta)
{
  updateChildren(delta);

  // Update position
  switch(mCorner)
  {
    case Corner::TopRight:
    case Corner::TopLeft:
    {
      float targetY = Renderer::Instance().DisplayHeightAsFloat() * (Board::Instance().CrtBoard().IsCrtAdapterAttached() ? 0.06f : 0.02f ) + (float)mTargetOffset;
      float diff = (mPosition.y() - targetY) * .85f;
      if (diff >= -2.0f && diff <= 2.0f) diff = 0;
      mPosition.y() = targetY + diff;
      break;
    }
    case Corner::BottomRight:
    case Corner::BottomLeft:
    {
      float targetY = Renderer::Instance().DisplayHeightAsFloat() * (Board::Instance().CrtBoard().IsCrtAdapterAttached() ? 0.94f : 0.98f) - mSize.y() - (float)mTargetOffset;
      float diff = (mPosition.y() - targetY) * .85f;
      if (diff >= -2.0f && diff <= 2.0f) diff = 0;
      mPosition.y() = targetY + diff;
      break;
    }
  }

  // Update fade and timer
	int curTime = (int)SDL_GetTicks();

	// we only init the actual time when we first start to render
	if(mStartTime == 0)
	{
		mStartTime = curTime;
	}

	int alpha = 0;

	// compute fade in effect
	if ((curTime - mStartTime > mDuration) || // we're past the popup duration, no need to render
	    (curTime < mStartTime))               // if SDL reset
	{
		mRunning = false;
		return;
	}
	if (curTime - mStartTime <= 500)
	{
		alpha = ((curTime - mStartTime)*255/500);
	}
	else if (curTime - mStartTime < mDuration - 500)
	{
		alpha = 255;
	}
	else
	{
		alpha = ((-(curTime - mStartTime - mDuration)*255)/500);
	}

	if (alpha > mMaxAlpha) alpha = mMaxAlpha;

	mGrid.setOpacity(alpha);

	// apply fade in effect to popup frame
	mFrame.setEdgeColor((mFrameColor & 0xffffff00) | alpha);
	mFrame.setCenterColor((mFrameColor & 0xffffff00) | alpha);
}

void GuiInfoPopupBase::SlideOffset(int size)
{
  // Update position
  switch(mCorner)
  {
    case Corner::TopRight:
    case Corner::TopLeft:
    {
      mTargetOffset -= size;
      break;
    }
    case Corner::BottomRight:
    case Corner::BottomLeft:
    {
      mTargetOffset += size;
      break;
    }
  }
}


