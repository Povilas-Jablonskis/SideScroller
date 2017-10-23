#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>
#include "Application.h"

namespace Engine
{
	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity), camera(glm::vec2(0.0f, 0.0f)), lastCamera(glm::vec2(0.0f, 0.0f))
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

		//if (getFirstState() == STATE_WALKINGLEFT)
		//	velocity.x -= gravity.x * dt;
		//else if (getFirstState() == STATE_WALKINGRIGHT)
		//	velocity.x += gravity.x * dt;

		Entity::update(dt, gravity);

		lastCamera = camera;

		if (getPosition(0) < glutGet(GLUT_INIT_WINDOW_WIDTH) / 2)
			camera.x = 0;
		else if (getPosition(0) > 2016.0f - glutGet(GLUT_INIT_WINDOW_WIDTH) / 2)
			camera.x = 2016.0f - glutGet(GLUT_INIT_WINDOW_WIDTH);
		else
			camera.x = getPosition(0) - glutGet(GLUT_INIT_WINDOW_WIDTH) / 2;

		if (getPosition(1) < glutGet(GLUT_INIT_WINDOW_HEIGHT) / 2)
			camera.y = 0;
		else if (getPosition(1) > 2016.0f - glutGet(GLUT_INIT_WINDOW_HEIGHT) / 2)
			camera.y = 2016.0f - glutGet(GLUT_INIT_WINDOW_HEIGHT);
		else
			camera.y = getPosition(1) - glutGet(GLUT_INIT_WINDOW_HEIGHT) / 2;

		return true;
	}

	void Player::respawn()
	{
		resetInput();
		firstState = STATE_IDLE;
		secondState = STATE_IDLE;
		setHealth(getHealth() - 1);
		if (getHealth() < 1)
			onDeath();
		setVelocity(startVelocity);
		position = glm::vec2(0.0f, 64.0f);
		lastPosition = glm::vec2(0.0f, 64.0f);
	}

	void Player::restart()
	{
		setScore(0);
		setHealth(startHealth);
		setVelocity(startVelocity);
		position = glm::vec2(0.0f, 64.0f);
		lastPosition = glm::vec2(0.0f, 64.0f);
	}

	float Player::getCamera(int index) const
	{
		switch (index)
		{
			case 0:
				return camera.x;
			case 1:
				return camera.y;
			default:
				return NULL;
		}
	}

	float Player::getLastCamera(int index) const
	{
		switch (index)
		{
			case 0:
				return lastCamera.x;
			case 1:
				return lastCamera.y;
			default:
				return NULL;
		}
	}
}