#ifndef _spriteSheetManagerH
#define _spriteSheetManagerH

#include "SpriteSheet.h"

namespace Engine
{
	class SpriteSheetManager
	{
		public:
			~SpriteSheetManager();
			void loadSpriteSheet(const std::string&, std::shared_ptr<SpriteSheet>);
			std::shared_ptr<SpriteSheet> getSpriteSheet(const std::string&);
		private:
			std::vector<std::pair<std::string, std::shared_ptr<SpriteSheet>>> spriteSheets;
	};
}
#endif