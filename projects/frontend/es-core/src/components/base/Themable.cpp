//
// Created by bkg2k on 14/12/23.
//

#include "Themable.h"
#include <components/base/Component.h>
#include <themes/ThemeElement.h>
#include <themes/ThemeData.h>
#include <Renderer.h>

void Themable::DoApplyThemeElement(const ThemeData& theme, const String& viewName, const String& elementName, ThemePropertyCategory properties)
{
  const ThemeElement* elem = theme.Element(viewName, elementName, GetThemeElementType());
  if (elem != nullptr)
  {
    OnApplyThemeElementBase(*elem, properties);
    OnApplyThemeElement(*elem, properties);
  }
}

void Themable::OnApplyThemeElementBase(const ThemeElement& element, ThemePropertyCategory properties)
{
  Vector2f scale = mComponent.getParent() != nullptr ? mComponent.getParent()->getSize()
                                                     : Vector2f(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());

  if (hasFlag(properties, ThemePropertyCategory::Position))
  {
    if (element.HasProperty(ThemePropertyName::Pos))
    {
      Vector2f denormalized = element.AsVector(ThemePropertyName::Pos) * scale;
      mComponent.setPosition(Vector3f(denormalized.x(), denormalized.y(), 0));
    }
    //else mComponent.setPosition(Vector3f(0, 0, 0));
  }

  if(hasFlag(properties, ThemePropertyCategory::Size))
    mComponent.setSize(element.HasProperty(ThemePropertyName::Size) ? element.AsVector(ThemePropertyName::Size) * scale : mComponent.getSize() /*Vector2f(0, 0)*/);

  // position + size also implies origin
  if (hasFlag(properties, ThemePropertyCategory::Origin) || hasFlags(properties, ThemePropertyCategory::Position, ThemePropertyCategory::Size))
    mComponent.setOrigin(element.HasProperty(ThemePropertyName::Origin) ? element.AsVector(ThemePropertyName::Origin) : Vector2f(0, 0));

  if (hasFlag(properties, ThemePropertyCategory::Rotation))
  {
    mComponent.setRotationDegrees(element.HasProperty(ThemePropertyName::Rotation) ? element.AsFloat(ThemePropertyName::Rotation) : 0.f);
    mComponent.setRotationOrigin(element.HasProperty(ThemePropertyName::RotationOrigin) ? element.AsVector(ThemePropertyName::RotationOrigin) : Vector2f(0, 0));
  }

  // Default ZIndex must always be set
  if (hasFlag(properties, ThemePropertyCategory::ZIndex) && element.HasProperty(ThemePropertyName::ZIndex)) mComponent.setZIndex(element.AsFloat(ThemePropertyName::ZIndex));
  else mComponent.setZIndex(mComponent.getDefaultZIndex());

  if (hasFlag(properties, ThemePropertyCategory::Position))
    mComponent.setThemeDisabled(element.HasProperty(ThemePropertyName::Disabled) && element.AsBool(ThemePropertyName::Disabled));
}