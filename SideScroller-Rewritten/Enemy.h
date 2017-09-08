#ifndef _enemyH
#define _enemyH

#include "Entity.h"

namespace Engine
{
	class Enemy : public Entity
	{
		public:
			Enemy(float, float, glm::vec2, glm::vec2, glm::vec4);
	};
}
#endif