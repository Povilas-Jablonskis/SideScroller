#ifndef _collisionInfoH
#define _collisionInfoH

#include "CollisionSides.h"

#include <vec2.hpp>

namespace Engine
{
	struct CollisionInfo
	{
		glm::vec2 depth;
		CollisionSides side;
	};
}
#endif