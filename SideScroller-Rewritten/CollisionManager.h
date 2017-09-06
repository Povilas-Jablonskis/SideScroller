#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			bool checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>, bool);
			bool checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList, bool checkX)
			{
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted()) continue;
					if (checkCollision(object, *it, checkX))
						return;
				}
			}
	};
}
#endif