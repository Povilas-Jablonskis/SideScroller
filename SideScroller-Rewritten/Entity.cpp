#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), isDucking(false), canClimb(false)
	{
		pressedkeys = new bool[pressedKeyCount];
		resetInput();
	}

	void Entity::resetInput()
	{
		for (int i = 0; i < pressedKeyCount; i++)
		{
			setKey(i, false);
		}
	}

	void Entity::fixInput()
	{
		for (int i = 0; i < pressedKeyCount; i++)
		{
			if (!GetAsyncKeyState(i) && getKey(i))
				setKey(i, false);
		}
	}

	bool Entity::update(float dt, glm::vec2 gravity)
	{
		if ((getSecondState() != STATE_FLYING && getSecondState() != STATE_CLIMBING) || (getSecondState() == STATE_FLYING && getFirstState() == STATE_DEAD))
			velocity.y += gravity.y * dt;

		if (getSecondState() == STATE_IDLE || getSecondState() == STATE_JUMPING || getSecondState() == STATE_FALLING)
		{
			if (getCanClimb())
			{
				setVelocity(1, 0.0f);
				setSecondState(STATE_CLIMBING);
			}
			else if (velocity.y <= 0.0f)
				setSecondState(STATE_FALLING);
		}
		else if (getSecondState() == STATE_CLIMBING)
		{
			if (!getCanClimb())
				setSecondState(STATE_FALLING);
		}

		lastPosition = position;

		if (getIsDucking())
			position.x += (getVelocity(0) / 2.0f) * dt;
		else
			position.x += getVelocity(0) * dt;

		if (getSecondState() == STATE_CLIMBING && getIsDucking())
			position.y += (getVelocity(1) / 2.0f) * dt;
		else
			position.y += getVelocity(1) * dt;

		updateAnimation(dt);
		return getNeedsToBeDeleted();
	}

	void Entity::setFirstState(State state)
	{
		auto lastFirstState = getFirstState();

		if (lastFirstState == state || lastFirstState == STATE_DEAD) return;

		firstState = state;

		if (state == STATE_DEAD)
		{
			applyAnimation(getAnimationByIndex("dead"));
			return;
		}

		if (getIsDucking())
		{
			applyAnimation(getAnimationByIndex("duck"));
			return;
		}

		if (lastFirstState == STATE_IDLE && (state == STATE_WALKINGLEFT || state == STATE_WALKINGRIGHT))
			applyAnimation(getAnimationByIndex("walk"));
		if ((lastFirstState == STATE_WALKINGLEFT || lastFirstState == STATE_WALKINGRIGHT) && state == STATE_IDLE)
		{
			auto secondState = getSecondState();
			if (secondState == STATE_FALLING || secondState == STATE_JUMPING || secondState == STATE_CLIMBING)
				applyAnimation(getAnimationByIndex("jump"));
			else if (secondState == STATE_IDLE || secondState == STATE_FLYING)
				applyAnimation(getAnimationByIndex("stand"));
		}
	}

	void Entity::setSecondState(State state)
	{
		auto lastSecondState = getSecondState();

		if (lastSecondState == state || getFirstState() == STATE_DEAD) return;

		secondState = state;

		if (state == STATE_CLIMBING)
		{
			setIsDucking(false);
			applyAnimation(getAnimationByIndex("jump"));
			return;
		}

		if (getIsDucking())
		{
			applyAnimation(getAnimationByIndex("duck"));
			return;
		}

		if ((state == STATE_FALLING || state == STATE_JUMPING || state == STATE_CLIMBING) && getFirstState() == STATE_IDLE)
			applyAnimation(getAnimationByIndex("jump"));
		else if (state == STATE_IDLE && getFirstState() == STATE_IDLE)
			applyAnimation(getAnimationByIndex("stand"));
	}

	void Entity::updateInput(std::shared_ptr<InputManager> inputManager)
	{
		if (getSecondState() == STATE_IDLE)
		{
			if (getKey(inputManager->getKeyBinding("Jump")))
			{
				setSecondState(STATE_JUMPING);
				setVelocity(1, 100.0f);
			}
		}

		auto firstState = getFirstState();

		if (getSecondState() == STATE_CLIMBING)
		{
			if (getKey(inputManager->getKeyBinding("Jump")))
				setVelocity(1, 20.0f);
			else if (getKey(inputManager->getKeyBinding("Duck")))
				setVelocity(1, -20.0f);
			else
				setVelocity(1, 0.0f);
		}
		else if (getSecondState() != STATE_FLYING)
		{
			if (!getIsDucking() && getKey(inputManager->getKeyBinding("Duck")))
			{
				applyAnimation(getAnimationByIndex("duck"));
				setIsDucking(true);
			}
			else if (getIsDucking() && !getKey(inputManager->getKeyBinding("Duck")))
			{
				if (getFirstState() == STATE_WALKINGLEFT || getFirstState() == STATE_WALKINGRIGHT)
					applyAnimation(getAnimationByIndex("walk"));
				else if (getFirstState() == STATE_IDLE)
				{
					if (getSecondState() == STATE_FALLING || getSecondState() == STATE_JUMPING || getSecondState() == STATE_CLIMBING)
						applyAnimation(getAnimationByIndex("jump"));
					else if (getSecondState() == STATE_IDLE)
						applyAnimation(getAnimationByIndex("stand"));
				}
				setIsDucking(false);
			}
		}
		else if (getSecondState() == STATE_FLYING)
		{
			if (getKey(inputManager->getKeyBinding("Jump")))
				setVelocity(1, 20.0f);
			else if (getKey(inputManager->getKeyBinding("Duck")))
				setVelocity(1, -20.0f);
			else
				setVelocity(1, 0.0f);
		}

		switch (firstState)
		{
			case STATE_IDLE:
			{
				if (getKey(inputManager->getKeyBinding("Move Left")))
				{
					setFirstState(STATE_WALKINGLEFT);
					setVelocity(0, -20.0f);
				}
				else if (getKey(inputManager->getKeyBinding("Move Right")))
				{
					setFirstState(STATE_WALKINGRIGHT);
					setVelocity(0, 20.0f);
				}
				break;
			}
			case STATE_WALKINGRIGHT:
			{
				if (!getKey(inputManager->getKeyBinding("Move Right")))
				{
					if (getKey(inputManager->getKeyBinding("Move Left")))
					{
						setFirstState(STATE_WALKINGLEFT);
						setVelocity(0, -20.0f);
					}
					else
					{
						setFirstState(STATE_IDLE);
						setVelocity(0, 0.0f);
					}
				}
				break;
			}
			case STATE_WALKINGLEFT:
			{
				if (!getKey(inputManager->getKeyBinding("Move Left")))
				{
					if (getKey(inputManager->getKeyBinding("Move Right")))
					{
						setFirstState(STATE_WALKINGRIGHT);
						setVelocity(0, 20.0f);
					}
					else
					{
						setFirstState(STATE_IDLE);
						setVelocity(0, 0.0f);
					}
				}
				break;
			}
		}
	}
}