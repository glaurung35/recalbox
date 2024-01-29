//
// Created by bkg2k on 21/01/24.
//
#pragma once

class Options
{
  public:
    /*!
     * @brief Constructor
     * @param argc Argument count
     * @param argv Arguments
     */
    Options(int argc, char* argv[]);

    //! Get debug state for logs
    [[nodiscard]] bool Debug() const { return mDebug; }

    //! Get state state for logs
    [[nodiscard]] bool Trace() const { return mTrace; }

    //! Requested width (0 = auto)
    [[nodiscard]] int Width() const { return mWidth; }

    //! Requested Height (0 = auto)
    [[nodiscard]] int Height() const { return mHeight; }

    //! Run in windowed mode (not fullscreen)
    [[nodiscard]] bool Windowed() const { return !mFullscreen; }

    //! Show FPS?
    [[nodiscard]] bool FPS() const { return mFPS; }

    //! Emulate RGB Dual?
    [[nodiscard]] bool EmulateRGBDual() const { return mEmulateRGBDual; }

    //! Emulate RGB Jamma
    [[nodiscard]] bool EmulateRGBJamma() const { return mEmulateRGBJamma; }

  private:
    int mWidth;
    int mHeight;
    bool mDebug;
    bool mTrace;
    bool mFullscreen;
    bool mEmulateRGBDual;
    bool mEmulateRGBJamma;
    bool mFPS;

    //! Parse and fill members
    void Parse(int argc, char* argv[]);
};
