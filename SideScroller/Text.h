#ifndef _textH
#define _textH

#include <ft2build.h>
#include "glm/gtc/type_ptr.hpp"
#include FT_FREETYPE_H
#include <functional>

#include "UIElementBase.h"
#include "Font.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			~Text();
			Text(const std::string&, glm::vec2, glm::vec4, std::shared_ptr<Font>, glm::vec2, bool = false);
			//Text(const char, glm::vec2, glm::vec4, std::shared_ptr<Font>, glm::vec2);
			void update(float);
			inline std::shared_ptr<Font> getFont() const { return font; }
			inline const std::vector<std::pair<GLuint, std::vector<GLfloat>>>& getCachedCharacters() { return cachedCharacters; }
			inline std::string getText() const { return text; }
			inline void setText(const std::string& newtext) { needUpdate = true; text = newtext; }
			//inline void setText(const char newtext) { needUpdate = true; text = newtext; }
			inline void setPosition(glm::vec2 _position) { needUpdate = true; position = _position; }
			inline void setPosition(int index, float _position) { needUpdate = true; position[index] = _position; }
			bool checkIfCollides(glm::vec2);
			void fixPosition(UIElementBase* = nullptr);
		private:
			bool needUpdate;
			std::string text;
			std::vector<std::pair<GLuint, std::vector<GLfloat>>> cachedCharacters;
			std::shared_ptr<Font> font;
	};
}
#endif