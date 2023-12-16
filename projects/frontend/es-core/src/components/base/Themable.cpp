//
// Created by bkg2k on 14/12/23.
//

#include "Themable.h"
#include <components/base/Component.h>
#include <themes/ThemeElement.h>
#include <themes/ThemeData.h>
#include <Renderer.h>

void Themable::DoApplyThemeElement(const ThemeData& theme, const String& viewName, const String& elementName, ThemePropertiesType properties)
{
  const ThemeElement* elem = theme.getElement(viewName, elementName, ThemeElementType());
  if (elem != nullptr)
  {
    OnApplyThemeElementBase(*elem, properties);
    OnApplyThemeElement(*elem, properties);
  }
}

void Themable::OnApplyThemeElementBase(const ThemeElement& element, ThemePropertiesType properties)
{
  Vector2f scale = mComponent.getParent() != nullptr ? mComponent.getParent()->getSize()
                                                     : Vector2f(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());

  if (hasFlag(properties, ThemePropertiesType::Position) && element.HasProperty("pos"))
  {
    Vector2f denormalized = element.AsVector("pos") * scale;
    mComponent.setPosition(Vector3f(denormalized.x(), denormalized.y(), 0));
  }

  if(hasFlag(properties, ThemePropertiesType::Size) && element.HasProperty("size"))
    mComponent.setSize(element.AsVector("size") * scale);

  // position + size also implies origin
  if ((hasFlag(properties, ThemePropertiesType::Origin) || hasFlags(properties, ThemePropertiesType::Position, ThemePropertiesType::Size)) && element.HasProperty("origin"))
    mComponent.setOrigin(element.AsVector("origin"));

  if (hasFlag(properties, ThemePropertiesType::Rotation))
  {
    if(element.HasProperty("rotation")) mComponent.setRotationDegrees(element.AsFloat("rotation"));
    if(element.HasProperty("rotationOrigin")) mComponent.setRotationOrigin(element.AsVector("rotationOrigin"));
  }

  if (hasFlag(properties, ThemePropertiesType::ZIndex) && element.HasProperty("zIndex")) mComponent.setZIndex(element.AsFloat("zIndex"));
  else mComponent.setZIndex(mComponent.getDefaultZIndex());

  if (hasFlag(properties, ThemePropertiesType::Position) && element.HasProperty("disabled"))
    mComponent.setThemeDisabled(element.AsBool("disabled"));
}