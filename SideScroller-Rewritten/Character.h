#ifndef _characterH
#define _characterH

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <vec2.hpp>

namespace Engine
{
	struct Character 
	{
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};
}
#endif