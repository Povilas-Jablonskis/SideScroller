#include "Addon.h"

namespace Engine
{
	Addon::Addon(float _width, float _height, glm::vec2 _positionOffset) :
		RenderObject(_width, _height, glm::vec2(0.0, 0.0), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)), positionOffset(_positionOffset), needsToBeDeleted(false)
	{

	}

	bool Addon::update(float dt, glm::vec2 playerPosition)
	{
		position = playerPosition + positionOffset;
		updateAnimation(dt);
		return getNeedsToBeDeleted();
	}
}