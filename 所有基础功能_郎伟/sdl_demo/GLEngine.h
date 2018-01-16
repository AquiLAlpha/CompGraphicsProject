#ifndef GL_ENGINE_H
#define GL_ENGINE_H

#ifdef WIN32
#include <windows.h>
#endif

#include<gl/GL.h>
#include<GL/GLU.h>

#include "Texture.h"
#include "Light.h"

#define iGLEngine GLEngine::getEngine()

class GLEngine
{
public:
	GLEngine();
	~GLEngine();

	static GLvoid Uninitialize(GLvoid);
	static GLEngine *getEngine(GLvoid);

	GLvoid Initialize(GLint width, GLint height);

	GLvoid buildTextureFont(GLvoid);
	GLvoid drawText(GLint x, GLint y, const char*in_text, ...);

	GLuint getTextWidth(const char*text);
	GLuint getTextHeight(const char*text);

private:
	GLuint fontBase;

	Texture	*fontTexture;
};


#endif
