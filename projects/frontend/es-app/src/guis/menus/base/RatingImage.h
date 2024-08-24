//
// Created by bkg2k on 30/08/24.
//
#pragma once

#include "resources/TextureResource.h"
#include "utils/gl/Vertex.h"
#include "utils/gl/Colors.h"

class RatingImage
{
  public:
    /*!
     * @brief Constructor
     * @param image Image resource
     */
    RatingImage(int height)
      : mFilledStar(TextureResource::get(Path(":/star_filled.svg"), true, true, true))
      , mUnfilledStar(TextureResource::get(Path(":/star_unfilled.svg"), true, true, true))
      , mLastX(0)
      , mLastY(0)
      , mLastValue(0)
    {
      mStarHeight = height;
      mStarLineWidth = height * sStarCount;
      mFilledStar->rasterizeAt(height, height);
      PrepareDrawingTo(0, 0, 0);
    }

    //! Width
    int Width() const { return mStarLineWidth; }
    //! Height
    int Height() const { return mStarHeight; }

    /*!
     * @brief Draw the nine patch image
     * @param targetArea Target rectangle
     * @param value current rating value between 0.f and 1.f
     * @param color Color
     */
    void Render(float x, float y, float value, Colors::ColorRGBA color);

  private:
    //! Start line count
    static constexpr int sStarCount = 5;
    //! Filled star
    std::shared_ptr<TextureResource> mFilledStar;
    //! Unfiles star
    std::shared_ptr<TextureResource> mUnfilledStar;
    //! Vertexes
    Vertex mVertices[Vertex::sVertexPerRectangle * 2];
    //! Last x coordinate
    float mLastX;
    //! Last Y coordinate
    float mLastY;
    //! Last value
    float mLastValue;
    //! Star height
    int mStarHeight;
    //! Stars width
    int mStarLineWidth;

    /*!
     * @brief Prepare drawing to the target area
     * @param targetArea Target area
     * @param value current rating value between 0.f and 1.f
     */
    void PrepareDrawingTo(float x, float y, float value);
};

