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

void BoxComponent::OnApplyThemeElement(const ThemeElement& element, ThemePropertiesType properties)
{
  if (hasFlag(properties, ThemePropertiesType::Color) && element.HasProperty("color"))
    setColor((Colors::ColorRGBA )element.AsInt("color"));
}
