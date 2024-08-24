//
// Created by bkg2k on 26/08/24.
//
#pragma once

#include "resources/TextureResource.h"
#include "utils/gl/Rectangle.h"
#include "utils/gl/Vertex.h"
#include "utils/gl/Colors.h"

class NinePatchImage
{
  public:
    /*!
     * @brief Constructor
     * @param image Image resource
     */
    NinePatchImage(const std::shared_ptr<TextureResource>& image)
      : mImage(image)
      , mLastTarget(0, 0)
    {}

    int ChunkWidth() const { return mImage->realWidth() / 3; }

    int ChunkHeight() const { return mImage->realHeight() / 3; }

    /*!
     * @brief Draw the nine patch image
     * @param targetArea Target rectangle
     * @param color Color
     */
    void Render(const Rectangle& targetArea, Colors::ColorRGBA color);

  private:
    //! Image
    std::shared_ptr<TextureResource> mImage;
    //! Vertexes
    Vertex mVertices[Vertex::sVertexPerRectangle * 9];
    //! Last rectangle
    Rectangle mLastTarget;

    /*!
     * @brief Prepare drawing to the target area
     * @param targetArea Target area
     */
    void PrepareDrawingTo(const Rectangle& targetArea);
};
