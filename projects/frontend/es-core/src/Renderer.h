#pragma once

#include "platform_gl.h"
#include "resources/TextureResource.h"
#include <stack>
#include <SDL.h>
#include <utils/math/Vector2f.h>
#include <utils/math/Vector4i.h>
#include <utils/math/Vector2i.h>
#include <utils/math/Transform4x4f.h>
#include <utils/cplusplus/StaticLifeCycleControler.h>
#include <utils/gl/Rectangle.h>
#include <utils/gl/Vertex.h>
#include <utils/gl/Colors.h>
#include <utils/storage/Stack.h>
#include <hardware/RotationType.h>
#include <stdint.h>

// Forward declaration
class Component;
class Font;

class Renderer : public StaticLifeCycleControler<Renderer>
{
  private:
    //! New clipping rectangle stack
    Stack<Rectangle> mClippings;
    //! Clipping rectangle stack
    std::stack<Vector4i> mClippingStack;

    //! SDL Surface
    SDL_Window*   mSdlWindow;
    //! SDL GL context
    SDL_GLContext mSdlGLContext;

    //! Viewport size as ints
    Vector2i   mViewportSize;
    //! Virtual viewport size as floats
    Vector2f   mVirtualViewportSizeFloat;
    //! Virtual viewport size as ints
    Vector2i   mVirtualViewportSize;

    //! Horizontal and vertical scaling
    Vector2f   mScale;
    //! Viewport rotation
    RotationType mRotate;

    //! True if both surface and context have been initialized
    bool mViewPortInitialized;
    //! Initial cursor state
    bool mInitialCursorState;
    //! Windowed mode
    bool mWindowed;

    /*!
     * @brief Create SDL display surface
     * @return True if the surface has been created successfuly
     */
    bool CreateSdlSurface(int width, int height);

    /*!
     * @brief Destroy SDL displaysurface
     */
    void DestroySdlSurface();

    /*!
     * @brief Activate GL debug callback, only in dev/DEBUG mode
     */
    static void ActivateGLDebug();

    /*!
     * @brief Get resolution from configuration file
     * @param w Width to fill
     * @param h Height to fill
     */
    static void GetResolutionFromConfiguration(int& w, int& h);

    /*!
     * @brief Show useful information in logs to help debug remote issues
     */
    static void InformationLogs();

  public:
    //! Error status
    enum class Error
    {
        NoError,        //!< Everything is fine
        OutOfMemory,    //!< Out of memory (CPU)
        OutOfGPUMemory, //!< Out of memory (GPU)
        NoResource,     //!< File not found or unknown internal resource
    };

    static void ColorToByteArray(GLubyte* array, Colors::ColorARGB color)
    {
      #ifdef LITTLE_ENDIAN
      *(unsigned int*)array = __builtin_bswap32(color);
      #else
      *(unsigned int*)array = color;
      #endif
    }

    static GLuint ColorToGL(Colors::ColorARGB color)
    {
      #ifdef LITTLE_ENDIAN
      return __builtin_bswap32(color);
      #else
      return color;
      #endif
    }

    /*!
     * @brief Constructor
     */
    Renderer(int width, int height, bool windowed, RotationType rotation = RotationType::None);

    /*!
     * @brief Destructor
     */
    ~Renderer();

    /*!
     * @brief Initialize GL viewport
     * @param w Required Width (0 = display width)
     * @param h Required Height (0 = display height)
     * @return true if everything is working fine, false otherwise
     */
    bool Initialize(int w, int h);

    /*!
     * @brief Reinitialize video using previous parameters
     * @return true if everything is working fine, false otherwise
     */
    bool ReInitialize();

    /*!
     * @brief Reinitialize video using previous parameters
     * @return true if everything is working fine, false otherwise
     */
    bool Rotate(RotationType rotation);

    /*!
     * Finalize GL viewport
     */
    void Finalize();

    /*!
     * @brief Applmy the given matrix to openGL context
     * @param transform Matrix
     */
    static void SetMatrix(const Transform4x4f& transform);

    /*!
     * @brief Swap working and dipslayed buffers in double buffering context
     */
    void SwapBuffers();

    /*
     * Clipping
     */

    /*!
     * @brief Push a new clipping rectangle
     * @param pos Top/Left coordinates
     * @param dim Width/Height
     */
    void PushClippingRect(Vector2i pos, Vector2i dim);

    /*!
     * @brief Pop latest clipping rectangle
     */
    void PopClippingRect();

    /*!
     * @brief Push a new clipping rectangle
     * @param pos Top/Left coordinates
     * @param dim Width/Height
     */
    void Clip(const Rectangle& area);

    /*!
     * @brief Pop latest clipping rectangle
     */
    void Unclip();

    /*
     * Drawing
     */

    /*!
     * @brief Draw rectangle
     * @param x Left coordinate
     * @param y Top coordinate
     * @param w Width
     * @param h Height
     * @param color Color
     * @param blend_sfactor Source blending
     * @param blend_dfactor Destination blending
     */
    static void DrawRectangle(const Rectangle& area, Colors::ColorARGB color, GLenum blend_sfactor = GL_SRC_ALPHA, GLenum blend_dfactor = GL_ONE_MINUS_SRC_ALPHA);

    /*!
     * @brief Draw rectangle
     * @param x Left coordinate
     * @param y Top coordinate
     * @param w Width
     * @param h Height
     * @param color Color
     * @param blend_sfactor Source blending
     * @param blend_dfactor Destination blending
     */
    static void DrawRectangle(int x, int y, int w, int h, Colors::ColorARGB color, GLenum blend_sfactor = GL_SRC_ALPHA, GLenum blend_dfactor = GL_ONE_MINUS_SRC_ALPHA);

    /*!
     * @brief Draw rectangle
     * @param x Left coordinate
     * @param y Top coordinate
     * @param w Width
     * @param h Height
     * @param color Color
     * @param blend_sfactor Source blending
     * @param blend_dfactor Destination blending
     */
    static void DrawRectangle(float x, float y, float w, float h, Colors::ColorARGB color, GLenum blend_sfactor = GL_SRC_ALPHA, GLenum blend_dfactor = GL_ONE_MINUS_SRC_ALPHA);

    /*!
     * @brief Draw rectangle
     * @param x Left coordinate
     * @param y Top coordinate
     * @param w Width
     * @param h Height
     * @param topleftcolor Top Left Color
     * @param toprightcolor Top Right Color
     * @param bottomleftcolor Bottom Left Color
     * @param bottomrightcolor Bottom Right Color
     */
    static void DrawRectangle(int x, int y, int w, int h,
                              Colors::ColorARGB topleftcolor,
                              Colors::ColorARGB toprightcolor,
                              Colors::ColorARGB bottomrightcolor,
                              Colors::ColorARGB bottomleftcolor);

    /*!
     * @brief Draw rectangle
     * @param x Left coordinate
     * @param y Top coordinate
     * @param w Width
     * @param h Height
     * @param topleftcolor Top Left Color
     * @param toprightcolor Top Right Color
     * @param bottomleftcolor Bottom Left Color
     * @param bottomrightcolor Bottom Right Color
     */
    static void DrawRectangle(float x, float y, float w, float h,
                            Colors::ColorARGB topleftcolor,
                            Colors::ColorARGB toprightcolor,
                            Colors::ColorARGB bottomrightcolor,
                            Colors::ColorARGB bottomleftcolor) { DrawRectangle((int)x, (int)y, (int)w, (int)h, topleftcolor, toprightcolor, bottomrightcolor, bottomleftcolor); }

    /*!
     * @brief Draw polylines
     * @param vertices Vertice list
     * @param colors Color array
     * @param count Vertex count
     */
    static void DrawLines(const Vector2f vertices[], const Colors::ColorARGB colors[], int count);

    /*!
     * @brief Draw textured triangles
     * @param id GL texture id
     * @param vertices Vertice list
     * @param colors Color list
     * @param count Vertice count
     * @param tiled draw tiled texture
     */
    static void DrawTexturedTriangles(GLuint id, const Vertex vertices[], const GLubyte colors[], int count, bool tiled);

    /*!
     * @brief Draw textured triangles using a single color
     * @param id GL texture id
     * @param vertices Vertice list
     * @param color Color
     * @param count Vertice count
     * @param tiled draw tiled texture
     */
    static void DrawTexturedTriangles(GLuint id, const Vertex vertices[], Colors::ColorARGB color, int count, bool tiled);

    /*!
     * @brief Upload Alpha texture data to GPU memory
     * @param id GL Texture id
     * @param width Data width
     * @param height Data height
     * @param data Alpha bytes (1 byte = 1 pixel)
     * @return NoError or OutOfGPUMemory
     */
    static Error UploadAlpha(GLuint id, int width, int height, const void* data);

    /*!
     * @brief Upload RGBA texture data to GPU memory
     * @param id GL Texture id
     * @param width Data width
     * @param height Data height
     * @param data RGBA bytes (4 byte = 1 pixel)
     * @return NoError or OutOfGPUMemory
     */
    static Error UploadRGBA(GLuint id, int width, int height, const void* data);

    /*!
     * @brief Upload Alpha texture data to GPU memory
     * @param id GL Texture id
     * @param width Data width
     * @param height Data height
     * @param data Alpha bytes (1 byte = 1 pixel)
     * @return NoError or OutOfGPUMemory
     */
    static Error UploadAlphaPart(GLuint id, int x, int y, int width, int height, const void* data);

    /*!
     * @brief Upload RGBA texture data to GPU memory
     * @param id GL Texture id
     * @param width Data width
     * @param height Data height
     * @param data RGBA bytes (4 byte = 1 pixel)
     * @return NoError or OutOfGPUMemory
     */
    static Error UploadRGBAPart(GLuint id, int x, int y, int width, int height, const void* data);

    /*!
     * @brief Build a GL color array
     * @param array Array pointer
     * @param color Color
     * @param count Count x Color => array
     */
    static void BuildGLColorArray(GLubyte* array, Colors::ColorARGB color, int Count);

    /*!
     * @brief Create a new texture and return its GL Identifier
     * @return GL Texture identifier
     */
    static GLuint CreateGLTexture();

    /*!
     * @brief Destroy the texture associated to the given id
     * @param id GL Texture identifier
     */
    static void DestroyGLTexture(GLuint id);

    /*
     * High level GL
     */

    /*!
     * @brief Draw a texture using full coordinates, width/height, keeping ratio yes/no
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param keepratio True to keep ratio, false to stretch
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool keepratio);

    /*!
     * @brief Draw a texture using full coordinates & texture width/height
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     */
    static void DrawTexture(TextureResource& texture, int x, int y) { DrawTexture(texture, x, y, (int)texture.width(), (int)texture.height(), false); }

    /*!
     * @brief Draw a texture using full coordinates & width/height
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h) { DrawTexture(texture, x, y, w, h, false); }

    /*!
     * @brief Draw a texture using full coordinates, width/height, keeping ratio yes/no and a given blending alpha
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param keepratio True to keep ratio, false to stretch
     * @param color Blending alpha
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool keepratio, unsigned char alpha);

    /*!
     * @brief Draw a texture using full coordinates, texture width/height and a given blending alpha
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param color Blending alpha
     */
    static void DrawTexture(TextureResource& texture, int x, int y, unsigned char alpha) { DrawTexture(texture, x, y, (int)texture.width(), (int)texture.height(), false, alpha); }

    /*!
     * @brief Draw a texture using full coordinates, width/height and a given blending alpha
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param color Blending alpha
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, unsigned char alpha) { DrawTexture(texture, x, y, w, h, false, alpha); }

    /*!
     * @brief Draw a texture using full coordinates, width/height, keeping ratio yes/no and a given blending color
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param keepratio True to keep ratio, false to stretch
     * @param color Blending color
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool keepratio, Colors::ColorARGB color);

    /*!
     * @brief Draw a texture using full coordinates, width/height, keeping ratio yes/no and a given blending color
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param keepratio True to keep ratio, false to stretch
     * @param color Blending color
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool flipX, bool flipY, bool keepratio, Colors::ColorARGB color);

    /*!
     * @brief Draw a texture using full coordinates, texture width/height and a given blending color
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param color Blending color
     */
    static void DrawTexture(TextureResource& texture, int x, int y, Colors::ColorARGB color) { DrawTexture(texture, x, y, (int)texture.width(), (int)texture.height(), false, color); }

    /*!
     * @brief Draw a texture using full coordinates, width/height and a given blending color
     * @param texture Texture to draw
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param color Blending color
     */
    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, Colors::ColorARGB color)  { DrawTexture(texture, x, y, w, h, false, color); }

    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool keepratio,
                            Colors::ColorARGB topleftcolor,
                            Colors::ColorARGB toprightcolor,
                            Colors::ColorARGB bottomrightcolor,
                            Colors::ColorARGB bottomleftcolor);

    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h,
                            Colors::ColorARGB topleftcolor,
                            Colors::ColorARGB toprightcolor,
                            Colors::ColorARGB bottomrightcolor,
                            Colors::ColorARGB bottomleftcolor) { DrawTexture(texture, x, y, w, h, false, topleftcolor, toprightcolor, bottomrightcolor, bottomleftcolor); }

    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h, bool keepratio,
                            bool flipOnX, bool flipOnY,
                            Colors::ColorARGB topleftcolor,
                            Colors::ColorARGB toprightcolor,
                            Colors::ColorARGB bottomrightcolor,
                            Colors::ColorARGB bottomleftcolor,
                            float topAlphaReflection,
                            float bottomAlphaReflection);

    static void DrawTexture(TextureResource& texture, int x, int y, int w, int h,
                            bool flipOnX, bool flipOnY,
                            Colors::ColorARGB topleftcolor,
                            Colors::ColorARGB toprightcolor,
                            Colors::ColorARGB bottomrightcolor,
                            Colors::ColorARGB bottomleftcolor,
                            float topAlphaReflection,
                            float bottomAlphaReflection) { DrawTexture(texture, x, y, w, h, false, flipOnX, flipOnY, topleftcolor, toprightcolor, bottomrightcolor, bottomleftcolor, topAlphaReflection, bottomAlphaReflection); }

    /*
     * Accessors
     */

    //! Get display Width as integer
    [[nodiscard]] int DisplayWidthAsInt() const { return mVirtualViewportSize.x(); }
    //! Get display Height as integer
    [[nodiscard]] int DisplayHeightAsInt() const { return mVirtualViewportSize.y(); }
    //! Get display Width as float
    [[nodiscard]] float DisplayWidthAsFloat() const { return mVirtualViewportSizeFloat.x(); }
    //! Get display Height as float
    [[nodiscard]] float DisplayHeightAsFloat() const { return mVirtualViewportSizeFloat.y(); }

    //! Is small resolution?
    [[nodiscard]] bool Is240p() const { return IsRotatedSide() ? mVirtualViewportSize.y() <= 480 || mViewportSize.x() <= 320 : mVirtualViewportSize.x() <= 480 || mViewportSize.y() <= 320; }
    //! Is middle resolution?
    [[nodiscard]] bool Is480pOrLower() const { return mViewportSize.y() <= 576; }
    //! Return true window width (not scaled, not rotated)
    [[nodiscard]] int RealDisplayWidthAsInt() const { return mViewportSize.x(); }
    //! Return true window height
    [[nodiscard]] int RealDisplayHeightAsInt() const { return mViewportSize.y(); }
    //! Return true if the screen is rotated either left or right
    [[nodiscard]] bool IsRotatedSide() const { return mRotate == RotationType::Left || mRotate == RotationType::Right; }
    //! Return the screen rotation
    [[nodiscard]] RotationType Rotation() const { return mRotate; }
    //! Is resolution QVGA or less?
    [[nodiscard]] bool IsQVGA() const { return Renderer::Instance().RealDisplayHeightAsInt() <= 288; }
    //! Is Resolution between QVGA (excluded) & VGA (included)?
    [[nodiscard]] bool IsVGA() const { int h = Renderer::Instance().RealDisplayHeightAsInt(); return h > 288 && h <= 576; }
    //! Is Resolution between VGA (excluded) & HD (included)?
    [[nodiscard]] bool IsHD() const { int h = Renderer::Instance().RealDisplayHeightAsInt(); return h > 576 && h <= 920; }
    //! Is Resolution higher than HD?
    [[nodiscard]] bool IsFHD() const { return Renderer::Instance().RealDisplayHeightAsInt() > 920; }


    //! Check if the Renderer is properly initialized
    [[nodiscard]] bool Initialized() const { return mViewPortInitialized; }
};
