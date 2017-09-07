#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Player.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			bool checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>, bool, glm::vec2 = glm::vec2(0.0f,0.0f));
			bool checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>, glm::vec2 = glm::vec2(0.0f, 0.0f));
			bool checkCollision(std::shared_ptr<Player>, std::shared_ptr<BaseGameObject>, bool, glm::vec2 = glm::vec2(0.0f, 0.0f));
			bool checkCollision(std::shared_ptr<Player>, std::shared_ptr<BaseGameObject>, glm::vec2 = glm::vec2(0.0f, 0.0f));
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList, bool checkX, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
			{
				if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return;
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					if (checkCollision(object, *it, checkX, offset))
						return;
				}
			}
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
			{
				if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return;
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					if (checkCollision(object, *it, offset))
						return;
				}
			}
		private:
			glm::vec2 checkCollision(glm::vec4, glm::vec4);
	};
}
#endif