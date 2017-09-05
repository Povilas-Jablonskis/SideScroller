#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"
#include "Subject.h"
#include "Addon.h"

namespace Engine
{
	class Entity : public BaseGameObject, public Subject
	{
		public:
			Entity(float, float, glm::vec2, glm::vec2, glm::vec4);
			inline std::vector<std::pair<std::string, std::shared_ptr<Addon>>>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			void addAddon(std::pair<std::string, std::shared_ptr<Addon>>);
			void removeAddon(std::string);
		private:
			std::vector<std::pair<std::string, std::shared_ptr<Addon>>> addons;
	};
}
#endif