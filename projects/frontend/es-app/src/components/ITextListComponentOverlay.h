//
// Created by bkg2k on 08/02/2020.
//
#pragma once

#include <utils/math/Vector2f.h>

template<typename T> class ITextListComponentOverlay
{
  public:
    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param parentTrans Parent transform matrice
     * @param position Top/Left of the item rectangle
     * @param size  Width/Height of the item rectangle
     * @param labelWidth Cell's label width in pixel
     * @param data Linked data
     */
    virtual void OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size, int labelWidth, const T& data, unsigned int& color) = 0;

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @param data Linked data
     * @param labelWidth Cell's label width in pixel
     * @return left offset
     */
    virtual float OverlayGetLeftOffset(const T& data, int labelWidth) = 0;

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @param data Linked data
     * @param labelWidth Cell's label width in pixel
     * @return right offset
     */
    virtual float OverlayGetRightOffset(const T& data, int labelWidth) = 0;
};
