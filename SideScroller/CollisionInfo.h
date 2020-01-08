#ifndef _collisionInfoH
#define _collisionInfoH

#include "CollisionSide.h"

#include <vec2.hpp>

namespace Engine
{
	struct CollisionInfo
	{
		glm::vec2 depth;
		CollisionSide side;
	};
}
#endif