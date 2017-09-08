#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>
#include "Application.h"

namespace Engine
{
	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity)
	{
	}

	bool Player::update(float dt, glm::vec2 gravity)
	{
		if (needsToBeDeleted)
		{
			needsToBeDeleted = false;
			respawn();
			return false;
		}

		Entity::update(dt, gravity);
		return true;
	}

	void Player::respawn()
	{
		setHealth(getHealth()-1);
		if (getHealth() < 1)
			onDeath();
		setVelocity(startVelocity);
		setPosition(glm::vec2(0.0f, 34.0f));
	}

	void Player::restart()
	{
		setScore(0);
		setHealth(startHealth);
		setVelocity(startVelocity);
		setPosition(glm::vec2(0.0f, 34.0f));
	}
}