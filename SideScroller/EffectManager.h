#ifndef _effectManagerH
#define _effectManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

#include "Entity.h"

namespace Engine
{
	class EffectManager
	{
		public:
			void loadEffect(std::string index, std::function<void(Entity*)> _effect)
			{
				for (auto effect : effects)
				{
					if (effect.first == index)
						return;
				}

				effects.push_back(std::pair<std::string, std::function<void(Entity*)>>(index, _effect));
			}
			void loadEffect(std::pair<std::string, std::function<void(Entity*)>> _effect)
			{
				for (auto effect : effects)
				{
					if (effect.first == _effect.first)
						return;
				}

				effects.push_back(_effect);
			}
			std::function<void(Entity*)> getEffect(std::string index)
			{
				for (auto effect : effects)
				{
					if (effect.first == index)
						return effect.second;
				}

				return nullptr;
			}
			std::function<void(Entity*)> getRandomEffect()
			{
				int randIndex = rand() % effects.size();
				return effects[randIndex].second;
			}
		private:
			std::vector<std::pair<std::string, std::function<void(Entity*)>>> effects;
	};
}
#endif