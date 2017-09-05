#include "SpriteManager.h"

namespace Engine
{
	SpriteManager::~SpriteManager()
	{
		for (std::map<std::string, std::shared_ptr<Animation>>::iterator it = sprites.begin(); it != sprites.end(); ++it)
		{
			auto textureID = it->second->getSpriteSheetTexture();
			glDeleteTextures(1, &textureID);
		}
		sprites.clear();
	}

	void SpriteManager::loadSprite(const std::string& _path, const std::string& _name)
	{
		if (sprites.find(_name) != sprites.end()) return;

		auto tempSprite = std::make_shared<Animation>();
		tempSprite->loadFromFile(_path);
		sprites.insert(std::pair<std::string, std::shared_ptr<Animation>>(_name, std::move(tempSprite)));
	}

	std::shared_ptr<Animation> SpriteManager::getSprite(const std::string& name)
	{
		auto tempSprite = sprites.find(name);

		if (tempSprite != sprites.end())
			return tempSprite->second;

		return nullptr;
	}
}