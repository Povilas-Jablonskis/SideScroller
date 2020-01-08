#include "EffectManager.h"
#include <fstream>

namespace Engine
{
	void EffectManager::loadEffect(std::string index, std::function<void(BaseGameObject*)> _effect)
	{
		for (auto effect : effects)
		{
			if (effect.first == index)
				return;
		}

		effects.push_back(std::pair<std::string, std::function<void(BaseGameObject*)>>(index, _effect));
	}

	void EffectManager::loadEffect(std::pair<std::string, std::function<void(BaseGameObject*)>> _effect)
	{
		for (auto effect : effects)
		{
			if (effect.first == _effect.first)
				return;
		}

		effects.push_back(_effect);
	}

	std::function<void(BaseGameObject*)> EffectManager::getEffect(std::string index)
	{
		for (auto effect : effects)
		{
			if (effect.first == index)
				return effect.second;
		}

		return nullptr;
	}

	std::function<void(BaseGameObject*)> EffectManager::getRandomEffect()
	{
		int randIndex = rand() % effects.size();
		return effects[randIndex].second;
	}
}