#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <vector>
#include <string>
#include "platform.h"
#include "platform_gl.h"
#include "utils/math/Vectors.h"

class GuiComponent;
class Font;

//The Renderer provides several higher-level functions for drawing (rectangles, text, etc.).
//Renderer_draw_gl.cpp has most of the higher-level functions and wrappers.
//Renderer_init_*.cpp has platform-specific renderer initialziation/deinitialziation code.  (e.g. the Raspberry Pi sets up dispmanx/OpenGL ES)
namespace Renderer
{
	bool init(int w, int h);
	void deinit();

	int getScreenWidth();
	int getScreenHeight();

	void buildGLColorArray(GLubyte* ptr, unsigned int color, unsigned int vertCount);

	//graphics commands
	void swapBuffers();

	void pushClipRect(Vector2i pos, Vector2i dim);
	void popClipRect();

	void setMatrix(const Transform4x4f& transform);

	void drawRect(int x, int y, int w, int h, unsigned int color, GLenum blend_sfactor = GL_SRC_ALPHA, GLenum blend_dfactor = GL_ONE_MINUS_SRC_ALPHA);
	void drawRect(float x, float y, float w, float h, unsigned int color, GLenum blend_sfactor = GL_SRC_ALPHA, GLenum blend_dfactor = GL_ONE_MINUS_SRC_ALPHA);
}

#endif
