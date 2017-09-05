#ifndef _enemyH
#define _enemyH

#include "BaseGameObject.h"
#include "Subject.h"

namespace Engine
{
	class Enemy : public BaseGameObject, public Subject
	{
		public:
			Enemy(float, float, glm::vec2, glm::vec2, glm::vec4);
	};
}
#endif