#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Player.h"
#include "CollisionInfo.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>& colliderList, glm::vec2 lastCamera = glm::vec2(0.0f, 0.0f), glm::vec2 camera = glm::vec2(0.0f, 0.0f))
			{
				if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return;
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					if (checkCollision(object, *it, true, lastCamera, camera))
						break;
				}
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					if (checkCollision(object, *it, false, lastCamera, camera))
						break;
				}
			}
		private:
			bool checkCollision(std::shared_ptr<Entity>, std::shared_ptr<BaseGameObject>, bool, glm::vec2, glm::vec2);
			bool addCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			bool removeCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			std::vector<std::pair<std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>>> collisions;
			CollisionInfo checkCollision(glm::vec4, glm::vec4);
	};
}
#endif