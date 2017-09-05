#ifndef _animationH
#define _animationH

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <vec4.hpp>
#include <vector>
#include <SOIL.h>

namespace Engine
{
	class Animation
	{
		public:
			Animation(GLuint = 0, int = 0, int = 0);
			inline void setDelay(float _delay) { delay = _delay; }
			inline float getDelay() const { return delay; }
			inline void setLoopStatus(bool _status) { loop = _status; }
			inline bool getLoopStatus() const { return loop; }
			inline void addSprite(glm::vec4 sprite) { sprites.push_back(sprite); }
			inline std::vector<glm::vec4>* getAnimation() { return &sprites; }
			GLuint getSpriteSheetTexture() const { return spriteSheetTexture; }
			int getSpriteSheetSize(int) const;
			void loadFromFile(const std::string&);
		private:
			int spriteSheetWidth;
			int spriteSheetHeigth;
			GLuint spriteSheetTexture;
			std::vector<glm::vec4> sprites;
			bool loop;
			float delay;
	};
}
#endif