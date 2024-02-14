#include "Component.h"
#include "WindowManager.h"
#include "utils/Log.h"
#include "Renderer.h"
#include "animations/AnimationController.h"
#include "themes/ThemeData.h"
#include <themes/ThemeManager.h>

Component::Component(WindowManager& window)
  : mTransform(Transform4x4f::Identity()),
    mAnimationMap{ nullptr },
    mChildren(nullptr),
    mWindow(window),
    mParent(nullptr),
    mPosition(Vector3f::Zero()),
    mOrigin(Vector2f::Zero()),
    mRotationOrigin(0.5, 0.5),
    mSize(Vector2f::Zero()),
    mRotation(0.0f),
    mScale(1.0f),
    mDefaultZIndex(0),
    mZIndex(0),
    mOpacity(255),
    mIsProcessing(false),
    mThemeDisabled(false)
{
}

Component::~Component()
{
	cancelAllAnimations();

  if(mParent != nullptr)
    mParent->removeChild(this);

  for (int i = (int)getChildCount(); --i >= 0; )
    getChild(i)->setParent(nullptr);

  delete mChildren;
}

bool Component::ProcessInput(const InputCompactEvent& event)
{
  int count = (int)getChildCount();
  for (int i = 0; i < count; ++i)
    if(getChild(i)->ProcessInput(event))
      return true;

  return false;
}

void Component::updateSelf(int deltaTime)
{
	for (int i = MAX_ANIMATIONS; --i >= 0; )
		advanceAnimation(i, deltaTime);
}

void Component::updateChildren(int deltaTime) const
{
	for (unsigned int i = 0; i < getChildCount(); i++)
	{
    getChild(i)->Update(deltaTime);
	}
}

void Component::Update(int deltaTime)
{
	updateSelf(deltaTime);
	updateChildren(deltaTime);
}

void Component::Render(const Transform4x4f& parentTrans)
{
    if(mThemeDisabled)
    {
        return;
    }
	Transform4x4f trans = parentTrans * getTransform();
	renderChildren(trans);
}

void Component::renderChildren(const Transform4x4f& transform) const
{
	for (unsigned int i = 0; i < getChildCount(); i++)
	{
    getChild(i)->Render(transform);
	}
}

void Component::setNormalisedPosition(float x, float y, float z)
{
    Vector2f pos = denormalise(x, y);
    setPosition(pos.x(), pos.y(), z);
}

void Component::setPosition(float x, float y, float z)
{
  if (x != mPosition.x() ||
      y != mPosition.y() ||
      z != mPosition.z())
  {
    mPosition.Set(x, y, z);
    onPositionChanged();
    mThemeDisabled = false;
  }
}

void Component::setOrigin(float x, float y)
{
  if (x != mOrigin.x() || y != mOrigin.y())
  {
    mOrigin.Set(x, y);
    onOriginChanged();
    mThemeDisabled = false;
  }
}

void Component::setRotationOrigin(float x, float y)
{
  if (x != mRotationOrigin.x() || y != mRotationOrigin.y())
  {
    mRotationOrigin.Set(x, y);
  }
}

void Component::setSize(float w, float h)
{
  if (w != mSize.x() || h != mSize.y())
  {
    mSize.Set(w, h);
    onSizeChanged();
  }
}

Vector2f Component::getCenter() const
{
	return { mPosition.x() - (getSize().x() * mOrigin.x()) + getSize().x() / 2,
					 mPosition.y() - (getSize().y() * mOrigin.y()) + getSize().y() / 2 };
}

//Children stuff.
void Component::addChild(Component* cmp)
{
  if (mChildren == nullptr) mChildren = new std::vector<Component*>();
  mChildren->push_back(cmp);

	if(cmp->getParent() != nullptr)
		cmp->getParent()->removeChild(cmp);

	cmp->setParent(this);
}

void Component::removeChild(Component* cmp)
{
	if(cmp->getParent() == nullptr)
		return;

	if(cmp->getParent() != this) { LOG(LogError) << "[Component] Tried to remove child from incorrect parent!"; }

	cmp->setParent(nullptr);

	if (mChildren != nullptr)
    for (auto i = mChildren->begin(); i != mChildren->end(); i++)
      if (*i == cmp)
      {
        mChildren->erase(i);
        break;
      }
}

void Component::clearChildren()
{
  if (mChildren != nullptr)
    mChildren->clear();
}

void Component::sortChildren()
{
  if (mChildren != nullptr)
    std::stable_sort(mChildren->begin(), mChildren->end(),  [](Component* a, Component* b) { return b->getZIndex() > a->getZIndex(); });
}

unsigned int Component::getChildCount() const
{
  if (mChildren == nullptr) return 0;
	return mChildren->size();
}

Component* Component::getChild(unsigned int i) const
{
  if (mChildren == nullptr) return nullptr;
  return mChildren->at(i);
}

void Component::setOpacity(unsigned char opacity)
{
	mOpacity = opacity;
	if (mChildren != nullptr)
    for (auto *it : *mChildren)
    {
      it->setOpacity(opacity);
    }
}

const Transform4x4f& Component::getTransform()
{
	mTransform = Transform4x4f::Identity();
	mTransform.translate(mPosition);
	if (mScale != 1.0)
	{
		mTransform.scale(Vector3f(mScale));
	}
	if (mRotation != 0.0)
	{
		// Calculate offset as difference between origin and rotation origin
		float xOff = (mOrigin.x() - mRotationOrigin.x()) * mSize.x();
		float yOff = (mOrigin.y() - mRotationOrigin.y()) * mSize.y();

		// transform to offset point
		if (xOff != 0.0 || yOff != 0.0)
			mTransform.translate(Vector3f(xOff * -1, yOff * -1, 0.0f));

		// apply rotation transform
		mTransform.rotate(mRotation, Vector3f::UnitZ());

		// Tranform back to original point
		if (xOff != 0.0 || yOff != 0.0)
			mTransform.translate(Vector3f(xOff, yOff, 0.0f));
	}
	mTransform.translate(Vector3f(mOrigin.x() * mSize.x() * -1, mOrigin.y() * mSize.y() * -1, 0.0f));
	return mTransform;
}

void Component::textInput(const char* text)
{
  if (mChildren != nullptr)
    for (auto *iter : *mChildren)
    {
      iter->textInput(text);
    }
}

void Component::setAnimation(Animation* anim, int delay, const std::function<void()>& finishedCallback, bool reverse, unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);

	AnimationController* oldAnim = mAnimationMap[slot];
	mAnimationMap[slot] = new AnimationController(anim, delay, finishedCallback, reverse);

	delete oldAnim;
}

bool Component::stopAnimation(unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);
	if(mAnimationMap[slot] != nullptr)
	{
		delete mAnimationMap[slot];
		mAnimationMap[slot] = nullptr;
		return true;
	}
  return false;
}

bool Component::cancelAnimation(unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);
	if(mAnimationMap[slot] != nullptr)
	{
		mAnimationMap[slot]->removeFinishedCallback();
		delete mAnimationMap[slot];
		mAnimationMap[slot] = nullptr;
		return true;
	}
	return false;
}

bool Component::finishAnimation(unsigned char slot)
{
  assert(slot < MAX_ANIMATIONS);
  if (mAnimationMap[slot] != nullptr)
  {
    // skip to animation's end
    const bool done = mAnimationMap[slot]->update(
      mAnimationMap[slot]->getAnimation()->getDuration() - mAnimationMap[slot]->getTime());
    (void) done;
    assert(done);

    delete mAnimationMap[slot]; // will also call finishedCallback
    mAnimationMap[slot] = nullptr;
    return true;
  }
  return false;
}

bool Component::advanceAnimation(unsigned char slot, unsigned int time)
{
	assert(slot < MAX_ANIMATIONS);
	AnimationController* anim = mAnimationMap[slot];
	if(anim != nullptr)
	{
		bool done = anim->update((int)time);
		if(done)
		{
			mAnimationMap[slot] = nullptr;
			delete anim;
		}
		return true;
	}
  return false;
}

void Component::stopAllAnimations()
{
	for (unsigned char i = 0; i < MAX_ANIMATIONS; i++)
		stopAnimation(i);
}

void Component::cancelAllAnimations()
{
	for (unsigned char i = 0; i < MAX_ANIMATIONS; i++)
		cancelAnimation(i);
}

bool Component::isAnimationPlaying(unsigned char slot) const
{
	return mAnimationMap[slot] != nullptr;
}

bool Component::isAnimationReversed(unsigned char slot) const
{
	assert(mAnimationMap[slot] != nullptr);
	return mAnimationMap[slot]->isReversed();
}

int Component::getAnimationTime(unsigned char slot) const
{
	assert(mAnimationMap[slot] != nullptr);
	return mAnimationMap[slot]->getTime();
}

Vector2f Component::denormalise(float x, float y)
{
    Vector2f value(x, y);
    return denormalise(value);
}

Vector2f Component::denormalise(const Vector2f& value) const
{
    Vector2f scale = getParent() != nullptr ? getParent()->getSize() : Vector2f(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());
    return value * scale;
}

void Component::UpdateHelpBar()
{
  if(getParent() != nullptr)
  {
    getParent()->UpdateHelpBar();
    return;
  }

  Help helpItems;
  if (CollectHelpItems(helpItems))
  {
    mWindow.RefreshHelpBar(helpItems);
  }
}

void Component::onShow()
{
  for (unsigned int i = 0; i < getChildCount(); i++)
    getChild(i)->onShow();
}

void Component::onHide()
{
	for (unsigned int i = 0; i < getChildCount(); i++)
    getChild(i)->onHide();
}

