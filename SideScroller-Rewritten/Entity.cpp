#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), isDucking(false), canClimb(false)
	{

	}

	bool Entity::update(float dt, glm::vec2 gravity)
	{
		if (getSecondState() != STATE_CLIMBING)
		{
			velocity += gravity * dt;

			if (getCanClimb())
			{
				setVelocity(1, 0.0f);
				setSecondState(STATE_CLIMBING);
			}
			else if (velocity.y <= 0.0f)
				setSecondState(STATE_FALLING);
		}
		else
		{
			if (!getCanClimb())
				setSecondState(STATE_FALLING);
		}

		updateAnimation(dt);
		return getNeedsToBeDeleted();
	}

	void Entity::setFirstState(State state)
	{
		auto lastFirstState = getFirstState();

		if (lastFirstState == state || lastFirstState == STATE_DEAD) return;

		firstState = state;

		if (state != STATE_DEAD && getIsDucking())
		{
			applyAnimation(getAnimationByIndex("duck"));
			return;
		}

		if (state == STATE_DEAD)
			applyAnimation(getAnimationByIndex("dead"));
		if (lastFirstState == STATE_IDLE && (state == STATE_WALKINGLEFT || state == STATE_WALKINGRIGHT))
			applyAnimation(getAnimationByIndex("walk"));
		if ((lastFirstState == STATE_WALKINGLEFT || lastFirstState == STATE_WALKINGRIGHT) && state == STATE_IDLE)
		{
			auto secondState = getSecondState();
			if (secondState == STATE_FALLING || secondState == STATE_JUMPING || secondState == STATE_CLIMBING)
				applyAnimation(getAnimationByIndex("jump"));
			else if (secondState == STATE_IDLE)
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

		if (state != STATE_DEAD && getIsDucking())
		{
			applyAnimation(getAnimationByIndex("duck"));
			return;
		}

		else if ((state == STATE_FALLING || state == STATE_JUMPING || state == STATE_CLIMBING) && getFirstState() == STATE_IDLE)
			applyAnimation(getAnimationByIndex("jump"));
		else if (state == STATE_IDLE && getFirstState() == STATE_IDLE)
			applyAnimation(getAnimationByIndex("stand"));
	}
}