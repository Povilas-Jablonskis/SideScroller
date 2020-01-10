#ifndef _rendererH
#define _rendererH

#include <memory>
#include <algorithm>

#include "Shader.h"
#include "Text.h"
#include "RenderObject.h"
#include "Font.h"

namespace Engine
{
	class Renderer
	{
		public:
			~Renderer();
			Renderer();
			void addShader(const std::string& name, std::shared_ptr<Shader> shader);
			inline GLuint getVAO() const { return vao; }
			GLuint getShaderProgram(const std::string&) const;
			inline GLuint getTextVBO() const { return textVBO; }
			inline GLuint getTextVAO() const { return textVAO; }
			template <typename T>
			void draw(const std::vector<std::shared_ptr<T>>& elements, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
			{
				auto program = getShaderProgram("shader");
				float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowHeight = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "color");
				int offsetLocation2 = glGetUniformLocation(program, "renderMode");
				int offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
				int offsetLocation6 = glGetUniformLocation(program, "projection");
				int offsetLocation7 = glGetUniformLocation(program, "model");
				glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, 0.0f, 1.0f);
				glBindVertexArray(vao);
				glUseProgram(program);
				for (auto element : elements)
				{
					if (element->getColor(3) == 0.0f) continue;

					auto animation = element->getAnimation();

					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(element->getPosition() - offset, 0.0f));

					model = glm::translate(model, glm::vec3(0.5f * element->getWidth(), 0.5f * element->getHeight(), 0.0f));
					model = glm::rotate(model, element->getRotationAngle(), element->getRotationAxis());
					model = glm::translate(model, glm::vec3(-0.5f * element->getWidth(), -0.5f * element->getHeight(), 0.0f));

					model = glm::scale(model, glm::vec3(element->getWidth(), element->getHeight(), 1.0f));

					glUniform4f(offsetLocation, element->getColor(0) / 255.0f, element->getColor(1) / 255.0f, element->getColor(2) / 255.0f, element->getColor(3));

					if (animation != nullptr)
					{
						auto sprites = animation->getAnimation();

						auto currentSprite = sprites.at(element->getCurrentFrame());
						glBindTexture(GL_TEXTURE_2D, animation->getSpriteSheetTexture());

						glUniform1f(offsetLocation2, 1.0f);
						auto spriteSheetSize = glm::vec2(animation->getSpriteSheetSize(0), animation->getSpriteSheetSize(1));
						glUniform4f(offsetLocation3, currentSprite.x / spriteSheetSize.x, currentSprite.y / spriteSheetSize.y, currentSprite.z / spriteSheetSize.x, currentSprite.w / spriteSheetSize.y);
					}
					else
						glUniform1f(offsetLocation2, 0.0f);

					glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				glUseProgram(0);
				glBindVertexArray(0);
			}
			template <typename T>
			void draw(std::shared_ptr<T> element, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
			{
				auto program = getShaderProgram("shader");
				float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowHeight = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "color");
				int offsetLocation2 = glGetUniformLocation(program, "renderMode");
				int offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
				int offsetLocation6 = glGetUniformLocation(program, "projection");
				int offsetLocation7 = glGetUniformLocation(program, "model");
				glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, 0.0f, 1.0f);
				glBindVertexArray(vao);
				glUseProgram(program);
				if (element->getColor(3) == 0.0f) return;

				auto animation = element->getAnimation();

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(element->getPosition() - offset, 0.0f));

				model = glm::translate(model, glm::vec3(0.5f * element->getWidth(), 0.5f * element->getHeight(), 0.0f));
				model = glm::rotate(model, element->getRotationAngle(), element->getRotationAxis());
				model = glm::translate(model, glm::vec3(-0.5f * element->getWidth(), -0.5f * element->getHeight(), 0.0f));

				model = glm::scale(model, glm::vec3(element->getWidth(), element->getHeight(), 1.0f));

				glUniform4f(offsetLocation, element->getColor(0) / 255.0f, element->getColor(1) / 255.0f, element->getColor(2) / 255.0f, element->getColor(3));

				if (animation != nullptr)
				{
					auto sprites = animation->getAnimation();

					auto currentSprite = sprites.at(element->getCurrentFrame());
					glBindTexture(GL_TEXTURE_2D, animation->getSpriteSheetTexture());

					glUniform1f(offsetLocation2, 1.0f);
					auto spriteSheetSize = glm::vec2(animation->getSpriteSheetSize(0), animation->getSpriteSheetSize(1));
					glUniform4f(offsetLocation3, currentSprite.x / spriteSheetSize.x, currentSprite.y / spriteSheetSize.y, currentSprite.z / spriteSheetSize.x, currentSprite.w / spriteSheetSize.y);
				}
				else
					glUniform1f(offsetLocation2, 0.0f);

				glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUseProgram(0);
				glBindVertexArray(0);
			}
			void draw(const std::vector<std::shared_ptr<Text>>&);
			void draw(std::shared_ptr<Text>);
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::vector<std::pair<std::string, std::shared_ptr<Shader>>> shaders;
	};
}
#endif