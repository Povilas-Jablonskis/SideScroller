#include "CollisionManager.h"

namespace Engine
{
	CollisionInfo CollisionManager::checkCollision(glm::vec4 object, glm::vec4 collider) // AABB - AABB collision
	{
		// Calculate half sizes.
		float halfWidthA = object.z / 2.0f;
		float halfHeightA = object.w / 2.0f;
		float halfWidthB = collider.z / 2.0f;
		float halfHeightB = collider.w / 2.0f;

		// Calculate centers.
		auto centerA = glm::vec2(object.x + halfWidthA, object.y + halfHeightA);
		auto centerB = glm::vec2(collider.x + halfWidthB, collider.y + halfHeightB);

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceX = centerA.x - centerB.x;
		float distanceY = centerA.y - centerB.y;
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		{
			return
			{
				glm::vec2(0.0f,0.0f),
				SIDE_NONE
			};
		}

		// Calculate and return intersection depths.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

		auto side = SIDE_NONE;

		if (collider.y <= object.y - halfHeightA) // top
			side = SIDE_TOP;
		else if (collider.y >= object.y + halfHeightA) // bottom
			side = SIDE_BOTTOM;
		else if (collider.x < object.x) //left
			side = SIDE_LEFT;
		else if (collider.x > object.x) //right
			side = SIDE_RIGHT;

		return
		{
			glm::vec2(depthX, depthY),
			side
		};
	}

	bool CollisionManager::checkCollision(std::shared_ptr<Entity> object, std::shared_ptr<BaseGameObject> collider, bool checkX, CollisionInfo collisionInfo) // AABB - AABB collision
	{
		if (object->getNeedsToBeDeleted() || object->getFirstState() == STATE_DEAD) return false;
		if (collider->getNeedsToBeDeleted() || collider->getFirstState() == STATE_DEAD) return false;

		if (collisionInfo.depth == glm::vec2(0.0f, 0.0f))
		{
			if (removeCollision(object, collider))
			{
				collider->onCollisionExit(object.get());
				object->onCollisionExit(collider.get());
			}
			return false;
		}

		if (addCollision(object, collider))
		{
			collider->onCollisionEnter(object.get(), collisionInfo);
			object->onCollisionEnter(collider.get(), collisionInfo);
		}

		bool isClimbable = (dynamic_cast<Climbable*>(collider.get()) == nullptr) ? false : true;

		if (object->getSecondState() == STATE_CLIMBING && isClimbable)
			return true;

		if (checkX)
			object->setPosition(0, object->getPosition(0) + collisionInfo.depth.x);
		else
		{
			if (object->getSecondState() != STATE_CLIMBING)
			{
				if (collisionInfo.depth.y < 0.0f)
					object->setSecondState(STATE_FALLING);
				else
					object->setSecondState(STATE_IDLE);
				object->setVelocity(1, 0.0f);
			}

			object->setPosition(1, object->getPosition(1) + collisionInfo.depth.y);
		}
		return true;
	}

	bool CollisionManager::addCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider)
	{
		for (auto collision : collisions)
		{
			if (collision.first == object && collision.second == collider)
				return false;
		}

		collisions.push_back(std::pair<std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>>(object, collider));
		return true;
	}

	bool CollisionManager::removeCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider)
	{
		for (auto it = collisions.begin(); it != collisions.end(); ++it)
		{
			if (it->first == object && it->second == collider)
			{
				collisions.erase(it);
				return true;
			}
		}

		return false;
	}
}