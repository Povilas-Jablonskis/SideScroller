#ifndef _textureManagerH
#define _textureManagerH

#include <iostream>
#include <map>
#include <memory>
#include <vec2.hpp>

#include "Animation.h"

namespace Engine
{
	class SpriteManager
	{
		public:
			~SpriteManager();
			void loadSprite(const std::string&, const std::string&);
			std::shared_ptr<Animation> getSprite(const std::string&);
		private:
			std::map<std::string, std::shared_ptr<Animation>> sprites;
	};
}
#endif