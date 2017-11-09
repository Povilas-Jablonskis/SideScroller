#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager() : currentEditedKeyBinding(CurrentEditedKeyBinding("", nullptr))
	{

	}

	bool InputManager::resetCurrentEditedKeyBinding()
	{
		auto currentKeyBinding = getCurrentEditedKeyBinding();

		if (currentKeyBinding->first.empty() || currentKeyBinding->second == nullptr)
			return false;

		currentKeyBinding->second->changeColor(glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
		currentKeyBinding->second.reset();
		return true;
	}

	void InputManager::setKeyBinding(const std::string& key, int value)
	{
		for (auto it = keyBindings.begin(); it != keyBindings.end(); ++it)
		{
			if (it->first == key)
			{
				it->second = value;
				return;
			}
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
}