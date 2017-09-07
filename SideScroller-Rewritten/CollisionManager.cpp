#include "CollisionManager.h"

namespace Engine
{
	glm::vec2 CollisionManager::checkCollision(glm::vec4 object, glm::vec4 collider) // AABB - AABB collision
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
			return glm::vec2(0.0f, 0.0f);

		// Calculate and return intersection depths.
		float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
		float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
		return glm::vec2(depthX, depthY);
	}

	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider, bool checkX, glm::vec2 offset) // AABB - AABB collision
	{
		if (object->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		auto depth = checkCollision(glm::vec4(object->getPosition() + offset, object->getSize(0), object->getSize(1)), glm::vec4(collider->getPosition() - offset, collider->getSize(0), collider->getSize(1)));

		if (depth == glm::vec2(0.0f, 0.0f))
			return false;

		collider->onCollision(object.get(), depth);
		object->onCollision(collider.get(), depth);

		if (checkX)
			object->setPosition(0, object->getPosition(0) + depth.x);
		else
		{
			if (depth.y < 0.0f)
				object->setSecondState(STATE_FALLING);
			else
				object->setSecondState(STATE_IDLE);

			object->setVelocity(1, 0.0f);
			object->setPosition(1, object->getPosition(1) + depth.y);
		}
		return true;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<Player> player, std::shared_ptr<BaseGameObject> collider, bool checkX, glm::vec2 offset) // AABB - AABB collision
	{
		if (player->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		auto depth = checkCollision(glm::vec4(player->getPosition(), player->getSize(0), player->getSize(1)), glm::vec4(collider->getPosition() - offset, collider->getSize(0), collider->getSize(1)));

		if (depth == glm::vec2(0.0f, 0.0f))
			return false;

		collider->onCollision(player.get(), depth);
		player->onCollision(collider.get(), depth);

		if (checkX)
			player->setPosition(0, player->getPosition(0) + depth.x);
		else
		{
			if (depth.y < 0.0f)
				player->setSecondState(STATE_FALLING);
			else
				player->setSecondState(STATE_IDLE);

			player->setVelocity(1, 0.0f);
			player->setPosition(1, player->getPosition(1) + depth.y);
		}
		return true;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<Player> player, std::shared_ptr<BaseGameObject> collider, glm::vec2 offset) // AABB - AABB collision
	{
		if (player->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		auto depth = checkCollision(glm::vec4(player->getPosition(), player->getSize(0), player->getSize(1)), glm::vec4(collider->getPosition() - offset, collider->getSize(0), collider->getSize(1)));

		if (depth == glm::vec2(0.0f, 0.0f))
			return false;

		collider->onCollision(player.get(), depth);
		player->onCollision(collider.get(), depth);
		return true;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider, glm::vec2 offset) // AABB - AABB collision
	{
		if (object->getNeedsToBeDeleted() || collider->getNeedsToBeDeleted()) return false;

		auto depth = checkCollision(glm::vec4(object->getPosition() + offset, object->getSize(0), object->getSize(1)), glm::vec4(collider->getPosition() - offset, collider->getSize(0), collider->getSize(1)));

		if (depth == glm::vec2(0.0f, 0.0f))
			return false;

		collider->onCollision(object.get(), depth);
		object->onCollision(collider.get(), depth);
		return true;
	}
}