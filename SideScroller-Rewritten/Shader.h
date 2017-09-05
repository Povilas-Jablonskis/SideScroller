#ifndef _shaderH
#define _shaderH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	class Shader
	{
		public:
			~Shader();
			Shader(const std::string&, const std::string&);
			void loadShader(const std::string&, const std::string&);
			std::string readShaderFile(const std::string&);
			inline GLuint getShader() const { return program; }
		private:
			GLuint program;
	};
}
#endif