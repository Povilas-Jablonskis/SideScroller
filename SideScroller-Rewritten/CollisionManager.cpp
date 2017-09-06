#include "CollisionManager.h"

namespace Engine
{
	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider, bool checkX) // AABB - AABB collision
	{
		if (object->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		// Calculate half sizes.
		float halfWidthA = object->getSize(0) / 2.0f;
		float halfHeightA = object->getSize(1) / 2.0f;
		float halfWidthB = collider->getSize(0) / 2.0f;
		float halfHeightB = collider->getSize(1) / 2.0f;

		// Calculate centers.
		auto centerA = glm::vec2(object->getPosition(0) + halfWidthA, object->getPosition(1) + halfHeightA);
		auto centerB = glm::vec2(collider->getPosition(0) + halfWidthB, collider->getPosition(1) + halfHeightB);

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceX = centerA.x - centerB.x;
		float distanceY = centerA.y - centerB.y;
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
			return false;

		collider->onCollision(object.get());

		// Calculate and return intersection depths.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;

		if (checkX)
			object->setPosition(0, object->getPosition(0) + depthX);
		else
		{
			if (depthY < 0.0f)
				object->setSecondState(STATE_FALLING);
			else
				object->setSecondState(STATE_IDLE);

			object->setVelocity(1, 0.0f);
			object->setPosition(1, object->getPosition(1) + depthY);
		}
		return true;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider) // AABB - AABB collision
	{
		if (object->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		// Calculate half sizes.
		float halfWidthA = object->getSize(0) / 2.0f;
		float halfHeightA = object->getSize(1) / 2.0f;
		float halfWidthB = collider->getSize(0) / 2.0f;
		float halfHeightB = collider->getSize(1) / 2.0f;

		// Calculate centers.
		auto centerA = glm::vec2(object->getPosition(0) + halfWidthA, object->getPosition(1) + halfHeightA);
		auto centerB = glm::vec2(collider->getPosition(0) + halfWidthB, collider->getPosition(1) + halfHeightB);

		// Calculate current and minimum-non-intersecting distances between centers.
		float distanceX = centerA.x - centerB.x;
		float distanceY = centerA.y - centerB.y;
		float minDistanceX = halfWidthA + halfWidthB;
		float minDistanceY = halfHeightA + halfHeightB;

		// If we are not intersecting at all, return (0, 0).
		if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
			return false;

		collider->onCollision(object.get());

		// Calculate and return intersection depths.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
		return true;
	}
}