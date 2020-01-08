#ifndef _climbableH
#define _climbableH

#include "BaseGameObject.h"

namespace Engine
{
	class Climbable : public BaseGameObject
	{
		public:
			Climbable(float, float, glm::vec2, glm::vec2, glm::vec4);
	};
}
#endif