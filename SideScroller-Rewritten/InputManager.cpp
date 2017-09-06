#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		pressedkeys = new bool[pressedKeyCount];
		lastpressedkeys = new bool[pressedKeyCount];
		resetInput();
		setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>(keyBindings.end(), nullptr));
	}

	void InputManager::resetInput()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		for (int i = 0; i < pressedKeyCount; i++)
		{
			setKey(i, false);
		}
	}

	bool InputManager::resetCurrentEditedKeyBinding()
	{
		auto currentKeyBinding = getCurrentEditedKeyBinding();

		if (currentKeyBinding->second == nullptr)
			return false;

		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == currentKeyBinding->first->first)
			{
				currentKeyBinding->second->setIsStatic(false);
				currentKeyBinding->second->changeColor(glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
				currentKeyBinding->second.reset();
				return true;
			}
		}

		return false;
	}

	void InputManager::fixInput()
	{
		for (int i = 0; i < pressedKeyCount; i++)
		{
			if (!GetAsyncKeyState(i) && getKey(i))
				setKey(i, false);
		}
	}

	void InputManager::setKeyBinding(const std::string& key, int value)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return;
		}

		keyBindings.push_back(std::pair<std::string, int>(key, value));
	}

	int InputManager::getKeyBinding(const std::string& key)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return keyBinding.second;
		}
		
		return -1;
	}

	void InputManager::updatePlayerInput(Player* player, float dt)
	{
		if (player == nullptr) return;

		if (player->getSecondState() == STATE_IDLE)
		{
			if (getKey(getKeyBinding("Jump")))
			{
				player->setSecondState(STATE_JUMPING);
				player->setVelocity(1, 80.0f);
			}
		}

		if (player->getSecondState() == STATE_CLIMBING && !getKey(getKeyBinding("Climb")))
		{
			player->setSecondState(STATE_IDLE);
			player->setVelocity(1, 0.0f);
		}
		else if (player->getSecondState() != STATE_CLIMBING && getKey(getKeyBinding("Climb")))
		{
			player->setSecondState(STATE_CLIMBING);
			player->setVelocity(1, 20.0f);
		}

		auto firstState = player->getFirstState();

		if (!player->getIsDucking() && getKey(getKeyBinding("Duck")))
		{
			player->applyAnimation(player->getAnimationByIndex("duck"));
			player->setIsDucking(true);
		}
		else if (player->getIsDucking() && !getKey(getKeyBinding("Duck")))
		{
			player->applyAnimation(player->getAnimationByIndex("stand"));
			player->setIsDucking(false);
		}

		switch (firstState)
		{
			case STATE_IDLE:
			{
				if (getKey(getKeyBinding("Move Left")))
				{
					player->setFirstState(STATE_WALKINGLEFT);
					player->setVelocity(0, -20.0f);
				}
				else if (getKey(getKeyBinding("Move Right")))
				{
					player->setFirstState(STATE_WALKINGRIGHT);
					player->setVelocity(0, 20.0f);
				}
				break;
			}
			case STATE_WALKINGRIGHT:
			{
				if (!getKey(getKeyBinding("Move Right")))
				{
					if (getKey(getKeyBinding("Move Left")))
					{
						player->setFirstState(STATE_WALKINGLEFT);
						player->setVelocity(0, -20.0f);
					}
					else
					{
						player->setFirstState(STATE_IDLE);
						player->setVelocity(0, 0.0f);
					}
				}
				break;
			}
			case STATE_WALKINGLEFT:
			{
				if (!getKey(getKeyBinding("Move Left")))
				{
					if (getKey(getKeyBinding("Move Right")))
					{
						player->setFirstState(STATE_WALKINGRIGHT);
						player->setVelocity(0, 20.0f);
					}
					else
					{
						player->setFirstState(STATE_IDLE);
						player->setVelocity(0, 0.0f);
					}
				}
				break;
			}
		}
	}
}