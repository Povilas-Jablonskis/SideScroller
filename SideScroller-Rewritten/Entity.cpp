#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShootsTimer(0.0f)
	{

	}

	std::shared_ptr<Addon> Entity::getAddon(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
				return it->second;
		}
		return nullptr;
	}

	void Entity::removeAddon(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
			{
				addons.erase(it);
				return;
			}
		}
	}

	void Entity::addAddon(std::pair<std::string, std::shared_ptr<Addon>> _addon)
	{
		for (auto addon : addons)
		{
			if (addon.first == _addon.first) return;
		}

		_addon.second->setPosition(getPosition() + _addon.second->getPositionOffset());
		addons.push_back(_addon);
	}
}