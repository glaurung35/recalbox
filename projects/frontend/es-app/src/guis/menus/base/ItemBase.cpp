//
// Created by bkg2k on 21/08/24.
//

#include <guis/menus/base/ItemBase.h>
#include <guis/menus/base/IItemDataProvider.h>

bool ItemBase::CalibrateLeftAndRightParts(int leftWidth, int& rightDynamicWidth, int rightStaticWidth)
{
  // Adjust width
  int half = mDataProvider.ItemWidth() / 2;
  if (rightDynamicWidth + rightStaticWidth + leftWidth > mDataProvider.ItemWidth())
  {
    // Reduce right option to make room for the label
    if (leftWidth < half) { rightDynamicWidth = half - rightStaticWidth; return true; }
    // Keep option space and let the label scroll by itself
    else if (rightDynamicWidth + rightStaticWidth < half) return false;
    // Both are bigger than half the space. Reduce the right part to the right half and let the label scroll in the left half
    else { rightDynamicWidth = half - rightStaticWidth; return true; }
  }
  else
  {
    // Clamp dynamic part to half the total item width
    if (rightDynamicWidth + rightStaticWidth > half) { rightDynamicWidth = half - rightStaticWidth; return true; }
  }
  return false;
}