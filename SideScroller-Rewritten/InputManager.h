#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"
#include "vec4.hpp"
#include <memory>
#include <iostream>
#include <algorithm>

#include "Text.h"

namespace Engine
{
	typedef std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>> CurrentEditedKeyBinding;

	class InputManager
	{
		public:
			InputManager();
			inline bool getLeftMouseState() const { return leftMouseClick; }
			inline bool getRightMouseState() const { return rightMouseClick; }
			inline void setLeftMouseState(bool boolean) { leftMouseClick = boolean; }
			inline void setRightMouseState(bool boolean) { rightMouseClick = boolean; }
			inline bool getLastLeftMouseState() const { return lastLeftMouseClick; }
			inline bool getLastRightMouseState() const { return lastRightMouseClick; }
			inline void setLastLeftMouseState(bool boolean) { lastLeftMouseClick = boolean; }
			inline void setLastRightMouseState(bool boolean) { lastRightMouseClick = boolean; }
			inline void setLastMousePosition(glm::vec2 position) { lastMousePosition = position; }
			inline glm::vec2 getLastMousePosition() const { return lastMousePosition; }
			inline bool isKeyBindingUsed(short key) const { return std::any_of(keyBindings.begin(), keyBindings.end(), [key](std::pair<std::string, int> element){return element.second == key; }); }
			void fixInput();
			inline std::vector<std::pair<std::string, int>>* getKeyBindings() { return &keyBindings; }
			void setKeyBinding(const std::string&, int);
			int getKeyBinding(const std::string&);
			inline CurrentEditedKeyBinding* getCurrentEditedKeyBinding() { return &currentEditedKeyBinding; }
			inline void setCurrentEditedKeyBinding(CurrentEditedKeyBinding value) { currentEditedKeyBinding = value; }
			bool resetCurrentEditedKeyBinding();
		private:
			CurrentEditedKeyBinding currentEditedKeyBinding;
			std::vector<std::pair<std::string, int>> keyBindings;
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
	};
}
#endif