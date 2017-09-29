#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		setCurrentEditedKeyBinding(CurrentEditedKeyBinding(keyBindings.end(), nullptr));
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

	void InputManager::fixInput()
	{
		if (!GetAsyncKeyState(VK_RBUTTON) && rightMouseClick)
		{
			lastRightMouseClick = true;
			rightMouseClick = false;
		}

		if (!GetAsyncKeyState(VK_LBUTTON) && leftMouseClick)
		{
			lastLeftMouseClick = true;
			leftMouseClick = false;
		}
	}
}