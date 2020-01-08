#include "Shader.h"
#include <fstream>
#include <string>


namespace Engine
{
	Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
	{
		loadShader(vertex_path, fragment_path);
	}

	Shader::~Shader()
	{
		glDeleteProgram(program);
	}

	std::string Shader::readShaderFile(const std::string& path)
	{
		std::string content;
		std::ifstream fs(path);
		char buffer;

		if (fs.is_open())
		{
			while (fs.get(buffer))
				content.push_back(std::move(buffer));
			fs.close();
		}
		else
		{
			#if _DEBUG
				std::cout << "ERROR::Shader: File " << path.c_str() << " not found" << std::endl;
				getchar();
			#endif
			exit(0);
		}
		return content;
	}

	void Shader::loadShader(const std::string& vertex_path, const std::string& fragment_path)
	{
		std::string vertShaderStr = readShaderFile(vertex_path);
		std::string fragShaderStr = readShaderFile(fragment_path);
		const char *vertexShaderSource = vertShaderStr.c_str();
		const char *fragmentShaderSource = fragShaderStr.c_str();
		GLint isCompiled = 0;

		// Vertex shader
		GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLogs(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLogs[0]);

			#if _DEBUG
				std::cout << "ERROR::Shader: Errors in vertex shader ( " << vertex_path << " ) :" << std::endl;
				for (auto errorLog : errorLogs)
					std::cout << errorLog;
			#endif
		}

		// Fragment shader
		GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLogs(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLogs[0]);

			#if _DEBUG
				std::cout << "ERROR::Shader: Errors in fragment shader ( " << fragment_path << " ) :" << std::endl;
				for (auto errorLog : errorLogs)
					std::cout << errorLog;
			#endif
		}

		if (isCompiled == GL_FALSE)
		{
			#if _DEBUG
				getchar();
			#endif
			exit(0);
		}

		// Link shaders
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}