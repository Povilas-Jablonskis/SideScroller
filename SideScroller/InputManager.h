#ifndef _inputManagerH
#define _inputManagerH

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <iostream>
#include <algorithm>

#include "Text.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Text>> CurrentEditedKeyBinding;

	class InputManager
	{
		public:
			InputManager();
			inline bool isKeyBindingUsed(short key) const { return std::any_of(keyBindings.begin(), keyBindings.end(), [key](std::pair<std::string, int> element){return element.second == key; }); }
			inline std::vector<std::pair<std::string, int>>* getKeyBindings() { return &keyBindings; }
			void setKeyBinding(const std::string&, int);
			int getKeyBinding(const std::string&);
			inline CurrentEditedKeyBinding* getCurrentEditedKeyBinding() { return &currentEditedKeyBinding; }
			inline void setCurrentEditedKeyBinding(CurrentEditedKeyBinding value) { currentEditedKeyBinding = value; }
			bool resetCurrentEditedKeyBinding();
		private:
			CurrentEditedKeyBinding currentEditedKeyBinding;
			std::vector<std::pair<std::string, int>> keyBindings;
	};
}
#endif