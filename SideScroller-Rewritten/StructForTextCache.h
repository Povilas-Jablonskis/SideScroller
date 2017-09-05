#ifndef _structForTextCacheH
#define _structForTextCacheH

#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	struct StructForTextCache
	{
		GLuint  TextureID;
		GLfloat vertices[6][4];
	};
}
#endif