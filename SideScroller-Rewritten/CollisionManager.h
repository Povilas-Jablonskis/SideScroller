#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Player.h"
#include "CollisionInfo.h"
#include "Climbable.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			template <typename T2>
			void checkCollision(std::shared_ptr<Player> object, std::vector<std::shared_ptr<T2>>& colliderList)
			{
				if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return;

				CollisionInfo collisionInfo;

				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					collisionInfo = checkCollision(glm::vec4(object->getPosition(0), object->getLastPosition(1), object->getSize(0), object->getSize(1)), glm::vec4((*it)->getPosition() - glm::vec2(object->getCamera(0), object->getLastCamera(1)), (*it)->getSize(0), (*it)->getSize(1)));
					if (checkCollision(object, *it, true, collisionInfo))
						break;
				}
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					collisionInfo = checkCollision(glm::vec4(object->getLastPosition(0), object->getPosition(1), object->getSize(0), object->getSize(1)), glm::vec4((*it)->getPosition() - glm::vec2(object->getLastCamera(0), object->getCamera(1)), (*it)->getSize(0), (*it)->getSize(1)));
					if (checkCollision(object, *it, false, collisionInfo))
						break;
				}

			}
			template <typename T2>
			void checkCollision(std::shared_ptr<Entity> object, std::vector<std::shared_ptr<T2>>& colliderList)
			{
				if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return;

				CollisionInfo collisionInfo;

				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					collisionInfo = checkCollision(glm::vec4(object->getPosition(0), object->getLastPosition(1), object->getSize(0), object->getSize(1)), glm::vec4((*it)->getPosition(), (*it)->getSize(0), (*it)->getSize(1)));
					if (checkCollision(object, *it, true, collisionInfo))
						break;
				}
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList.begin(); it != colliderList.end(); ++it)
				{
					if ((*it)->getNeedsToBeDeleted() || (*it)->getFirstState() == STATE_DEAD) continue;
					collisionInfo = checkCollision(glm::vec4(object->getLastPosition(0), object->getPosition(1), object->getSize(0), object->getSize(1)), glm::vec4((*it)->getPosition(), (*it)->getSize(0), (*it)->getSize(1)));
					if (checkCollision(object, *it, false, collisionInfo))
						break;
				}
			}
		private:
			bool checkCollision(std::shared_ptr<Entity>, std::shared_ptr<BaseGameObject>, bool, CollisionInfo);
			bool addCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			bool removeCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			std::vector<std::pair<std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>>> collisions;
			CollisionInfo checkCollision(glm::vec4, glm::vec4);
	};
}
#endif