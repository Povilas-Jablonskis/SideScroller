#include "Enemy.h"
#include "Player.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Engine
{
	Enemy::Enemy(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}
}