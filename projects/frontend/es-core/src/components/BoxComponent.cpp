//
// Created by bkg2k on 11/12/23.
//

#include "BoxComponent.h"
#include <themes/ThemeData.h>

void BoxComponent::Render(const Transform4x4f& parentTrans)
{
  Transform4x4f trans = parentTrans * getTransform();
  Renderer::SetMatrix(trans);

  Renderer::DrawRectangle(0.f, 0.f, mSize.x(), mSize.y(), mColor);
}

void
BoxComponent::applyTheme(const ThemeData& theme, const String& view, const String& element, ThemeProperties properties)
{
  const ThemeElement* elem = theme.getElement(view, element, "box");
  if (elem == nullptr) return;

  Vector2f scale = getParent() != nullptr ? getParent()->getSize() : Vector2f(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());

  if (hasFlag(properties, ThemeProperties::Position) && elem->HasProperty("pos"))
  {
    Vector2f denormalized = elem->AsVector("pos") * scale;
    setPosition(Vector3f(denormalized.x(), denormalized.y(), 0));
  }

  if (hasFlag(properties,ThemeProperties::Size))
    if (elem->HasProperty("size"))
      setSize(elem->AsVector("size") * scale);

  // position + size also implies origin
  if ((hasFlag(properties, ThemeProperties::Origin) || (hasFlags(properties, ThemeProperties::Position, ThemeProperties::Size))) && elem->HasProperty("origin"))
    setOrigin(elem->AsVector("origin"));

  if (hasFlag(properties, ThemeProperties::Color) && elem->HasProperty("color"))
    setColor((Colors::ColorRGBA )elem->AsInt("color"));

  if (hasFlag(properties, ThemeProperties::Rotation))
  {
    if (elem->HasProperty("rotation")) setRotationDegrees(elem->AsFloat("rotation"));
    if (elem->HasProperty("rotationOrigin")) setRotationOrigin(elem->AsVector("rotationOrigin"));
  }

  if (hasFlag(properties, ThemeProperties::ZIndex) && elem->HasProperty("zIndex")) setZIndex(elem->AsFloat("zIndex"));
  else setZIndex(getDefaultZIndex());

  if (hasFlag(properties, ThemeProperties::Position) && elem->HasProperty("disabled"))
    mThemeDisabled = elem->AsBool("disabled");
}
