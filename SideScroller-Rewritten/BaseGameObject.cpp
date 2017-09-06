#include "BaseGameObject.h"
#include "Player.h"
#include "Enemy.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), needsToBeDeleted(false), firstState(State::STATE_IDLE), secondState(State::STATE_IDLE), isDucking(false), canClimb(false)
	{
		onCollision = [](BaseGameObject* collider)
		{

		};

		onDeath = []()
		{

		};
	}

	BaseGameObject::~BaseGameObject()
	{
		animations.clear();
	}

	bool BaseGameObject::update(float dt, glm::vec2 gravity)
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

	void BaseGameObject::addAnimation(std::string index, std::shared_ptr<Animation> animation)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Animation>>>::iterator it = animations.begin(); it != animations.end(); it++)
		{
			if (it->first == index)
				return;
		}

		animations.push_back(std::pair<std::string, std::shared_ptr<Animation>>(index, animation));
	}

	std::shared_ptr<Animation> BaseGameObject::getAnimationByIndex(std::string index)
	{
		for (auto animation : animations)
		{
			if (animation.first == index)
				return animation.second;
		}
		return nullptr;
	}

	void BaseGameObject::setFirstState(State state)
	{
		auto lastFirstState = getFirstState();

		if (lastFirstState == state)
			return;

		firstState = state;

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
			else if (secondState == STATE_IDLE)
				applyAnimation(getAnimationByIndex("stand"));
		}
	}

	void BaseGameObject::setSecondState(State state)
	{
		auto lastSecondState = getSecondState();

		if (lastSecondState == state)
			return;

		secondState = state;

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
}