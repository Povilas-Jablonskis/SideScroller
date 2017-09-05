#include "SpriteSheetManager.h"

namespace Engine
{
	SpriteSheetManager::~SpriteSheetManager()
	{
		spriteSheets.clear();
	}

	void SpriteSheetManager::loadSpriteSheet(const std::string& name, std::shared_ptr<SpriteSheet> _spriteSheet)
	{
		for (auto spriteSheet : spriteSheets)
		{
			if (spriteSheet.first == name)
				return;
		}
		spriteSheets.push_back(std::pair<std::string, std::shared_ptr<SpriteSheet>>(name, _spriteSheet));
	}

	std::shared_ptr<SpriteSheet> SpriteSheetManager::getSpriteSheet(const std::string& name)
	{
		for (auto spriteSheet : spriteSheets)
		{
			if (spriteSheet.first == name)
				return spriteSheet.second;
		}
		return nullptr;
	}
}