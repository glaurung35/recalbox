#pragma once

#include "components/base/ThemableComponent.h"
#include <memory>
#include "resources/TextureResource.h"
#include "utils/gl/Colors.h"

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

    [[nodiscard]] Vector2f getTargetSize() const { return mTargetSize; }

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
      }
    }

    /*!
     * @brief Set top and bottom alpha for reflection
     * if both value are zero'ed, no reflection is applied
     * @param v Vector containing top alpha in x and bottom alpha in y
     */
    void SetReflection(const Vector2f& v) { SetReflection(v.x(), v.y()); }

    /*!
     * @brief Set box 4 corners color
     * @param color RGBA color
     */
    void SetColor(Colors::ColorRGBA color) { setColorShift(color); }

    /*!
     * @brief Set box top border color
     * @param color RGBA color
     */
    void SetTopColor(Colors::ColorRGBA color)
    {
      mTopLeftColor = color;
      mTopRightColor = color;
    }

    /*!
     * @brief Set box bottom border color
     * @param color RGBA color
     */
    void SetBottomColor(Colors::ColorRGBA color)
    {
      mBottomLeftColor = color;
      mBottomRightColor = color;
    }

    /*!
     * @brief Set box left border color
     * @param color RGBA color
     */
    void SetLeftColor(Colors::ColorRGBA color)
    {
      mTopLeftColor = color;
      mBottomLeftColor = color;
    }

    /*!
     * @brief Set box right border color
     * @param color RGBA color
     */
    void SetRightColor(Colors::ColorRGBA color)
    {
      mTopRightColor = color;
      mBottomRightColor = color;
    }

    /*!
     * @brief Set box top/left corner color
     * @param color RGBA color
     */
    void SetTopLeftColor(Colors::ColorRGBA color) { mTopLeftColor = color; }

    /*!
     * @brief Set box top/right corner color
     * @param color RGBA color
     */
    void SetTopRightColor(Colors::ColorRGBA color) { mTopRightColor = color; }

    /*!
     * @brief Set box bottom/left corner color
     * @param color RGBA color
     */
    void SetBottomLeftColor(Colors::ColorRGBA color) { mBottomLeftColor = color; }

    /*!
     * @brief Set box bottom/right corner color
     * @param color RGBA color
     */
    void SetBottomRightColor(Colors::ColorRGBA color) { mBottomRightColor = color; }

    /*!
     * @brief Get box top/left color
     * @return Box top/left color
     */
    [[nodiscard]] Colors::ColorRGBA TopLeftColor() const { return mTopLeftColor; }

    /*!
     * @brief Get box top/right color
     * @return Box top/right color
     */
    [[nodiscard]] Colors::ColorRGBA TopRightColor() const { return mTopRightColor; }

    /*!
     * @brief Get box bottom/left color
     * @return Box bottom/left color
     */
    [[nodiscard]] Colors::ColorRGBA BottomLeftColor() const { return mBottomLeftColor; }

    /*!
     * @brief Get box bottom/right color
     * @return Box bottom/right color
     */
    [[nodiscard]] Colors::ColorRGBA BottomRightColor() const { return mBottomRightColor; }

  private:
    Vector2f mTargetSize;

    Path mPath;
    bool mFlipX, mFlipY;

    // Calculates the correct mSize from our resizing information (set by setResize/setMaxSize).
    // Used internally whenever the resizing parameters or texture change.
    void resize();

    void fadeIn(bool textureLoaded);

    //! Color
    Colors::ColorRGBA mTopLeftColor;
    Colors::ColorRGBA mTopRightColor;
    Colors::ColorRGBA mBottomLeftColor;
    Colors::ColorRGBA mBottomRightColor;
    Colors::ColorRGBA mOriginColor;

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

