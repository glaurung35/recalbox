#pragma once

#include "components/base/ThemableComponent.h"
#include <memory>
#include "resources/TextureResource.h"

class ImageComponent : public ThemableComponent
{
  public:
    ImageComponent(WindowManager& window, bool keepRatio, const Path& imagePath, bool forceLoad, bool dynamic);

    ImageComponent(WindowManager& window, bool forceLoad, bool dynamic)
      : ImageComponent(window, false, Path::Empty, forceLoad, dynamic)
    {
    }

    ImageComponent(WindowManager& window, bool keepRatio, const Path& imagePath, bool forceLoad)
      : ImageComponent(window, keepRatio, imagePath, forceLoad, true)
    {
    }

    ImageComponent(WindowManager& window, bool forceLoad)
      : ImageComponent(window, false, Path::Empty, forceLoad, true)
    {
    }

    explicit ImageComponent(WindowManager& window, bool keepRatio, const Path& imagePath)
      : ImageComponent(window, keepRatio, imagePath, false, true)
    {
    }

    explicit ImageComponent(WindowManager& window)
      : ImageComponent(window, false, Path::Empty, false, true)
    {
    }

    ~ImageComponent() override = default;

    //Loads the image at the given filepath. Will tile if tile is true (retrieves texture as tiling, creates vertices accordingly).
    void setImage(const Path& path, bool tile = false);

    [[nodiscard]] Path getImage() const { return mPath; };

    //Loads an image from memory.
    void setImage(const char* image, size_t length, bool tile = false);

    //Use an already existing texture.
    void setImage(const std::shared_ptr<TextureResource>& texture);

    [[nodiscard]] Path getImagePath() const { return mPath; };

    void onSizeChanged() override;

    void setOpacity(unsigned char opacity) override;

    [[nodiscard]] Vector2f getTargetSize() const { return mTargetSize; }

    [[nodiscard]] Vector2i getSourceImageSize() const { return getTextureSize(); }

    // Resize the image to fit this size. If one axis is zero, scale that axis to maintain aspect ratio.
    // If both are non-zero, potentially break the aspect ratio.  If both are zero, no resizing.
    // Can be set before or after an image is loaded.
    // setMaxSize() and setResize() are mutually exclusive.
    void setResize(float width, float height);

    inline void setResize(const Vector2f& size) { setResize(size.x(), size.y()); }

    void setNormalisedSize(float width, float height);

    //! Set keep ratio
    void setKeepRatio(bool keepRatio);

    // Multiply all pixels in the image by this color when rendering.
    void setColorShift(unsigned int color);

    void setColor(unsigned int color) override;

    inline void setOriginColor(unsigned int color) { mOriginColor = color; };

    inline unsigned int getOriginColor() override { return mOriginColor; };

    void setFlipX(bool flip); // Mirror on the X axis.
    void setFlipY(bool flip); // Mirror on the Y axis.

    // Returns the size of the current texture, or (0, 0) if none is loaded.  May be different than drawn size (use getSize() for that).
    [[nodiscard]] Vector2i getTextureSize() const;

    bool hasImage() { return mTexture != nullptr; }

    void Render(const Transform4x4f& parentTrans) override;

    /*!
     * @brief Apply theme element to this image
     * @param element Theme element
     * @param properties Properties to set
     */
    void OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties) override;

    /*!
     * @brief Return theme element type
     * @return Element type
     */
    [[nodiscard]] ThemeElementType GetThemeElementType() const override { return ThemeElementType::Image; }

    bool CollectHelpItems(Help& help) override;

    /*!
     * @brief Set component visibility
     * @param enabled True (default) to render the component, false to hide it
     */
    void SetVisible(bool visible) { mVisible = visible; }

    /*!
     * @brief Set component visibility
     * @param enabled True (default) to render the component, false to hide it
     */
    [[nodiscard]] bool Visible() const { return mVisible; }

    /*!
     * @brief Set top and bottom alpha for reflection
     * if both value are zero'ed, no reflection is applied
     * @param top Top alpha
     * @param bottom Bottom alpha
     */
    void SetReflection(float top, float bottom)
    {
      top = Math::clamp(top, 0.f, 1.f);
      bottom = Math::clamp(bottom, 0.f, 1.f);
      if (mTopAlpha != top || mBottomAlpha != bottom)
      {
        mTopAlpha = top;
        mBottomAlpha = bottom;
        updateColors();
      }
    }

    /*!
     * @brief Set top and bottom alpha for reflection
     * if both value are zero'ed, no reflection is applied
     * @param v Vector containing top alpha in x and bottom alpha in y
     */
    void SetReflection(const Vector2f& v) { SetReflection(v.x(), v.y()); }

  private:
    Vector2f mTargetSize;

    Path mPath;
    bool mFlipX, mFlipY;

    // Calculates the correct mSize from our resizing information (set by setResize/setMaxSize).
    // Used internally whenever the resizing parameters or texture change.
    void resize();

    struct Vertex
    {
      Vector2f pos;
      Vector2f tex;
    } mVertices[12];

    GLubyte mColors[12 * 4];

    void updateVertices();

    void updateColors();

    void fadeIn(bool textureLoaded);

    unsigned int mColorShift;
    unsigned int mOriginColor;

    std::shared_ptr<TextureResource> mTexture;
    unsigned char mFadeOpacity;

    float mTopAlpha; //!< Reflexion top alpha
    float mBottomAlpha; //!< Reflexion top alpha

    bool mColorNotSet;
    bool mFading;
    bool mForceLoad;
    bool mDynamic;
    bool mVisible;
    bool mKeepRatio;
};

