#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"
#include "vec4.hpp"
#include <memory>
#include <iostream>

#include "Text.h"
#include "Player.h"

namespace Engine
{
	class InputManager
	{
		public:
			InputManager();
			void resetInput();
			void fixInput();
			void updatePlayerInput(Player*);
			void simulateInputForEntity(Entity*, int, bool);
			inline bool getLeftMouseState() const { return leftMouseClick; }
			inline bool getRightMouseState() const { return rightMouseClick; }
			inline void setLeftMouseState(bool boolean) { leftMouseClick = boolean; }
			inline void setRightMouseState(bool boolean) { rightMouseClick = boolean; }
			inline bool getLastLeftMouseState() const { return lastLeftMouseClick; }
			inline bool getLastRightMouseState() const { return lastRightMouseClick; }
			inline void setLastLeftMouseState(bool boolean) { lastLeftMouseClick = boolean; }
			inline void setLastRightMouseState(bool boolean) { lastRightMouseClick = boolean; }
			inline bool getKey(char key) const { return pressedkeys[key]; }
			inline bool getKey(int key) const { return pressedkeys[key]; }
			inline void setKey(char key, bool boolean) { lastpressedkeys[key] = pressedkeys[key]; pressedkeys[key] = boolean; }
			inline void setKey(int key, bool boolean) { lastpressedkeys[key] = pressedkeys[key]; pressedkeys[key] = boolean; }
			inline bool getLastKey(char key) const { return lastpressedkeys[key]; }
			inline bool getLastKey(int key) const { return lastpressedkeys[key]; }
			inline void setLastMousePosition(glm::vec2 position) { lastMousePosition = position; }
			inline glm::vec2 getLastMousePosition() const { return lastMousePosition; }
			inline std::vector<std::pair<std::string, int>>* getKeyBindings() { return &keyBindings; }
			void setKeyBinding(const std::string&, int);
			int getKeyBinding(const std::string&);
			inline std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>* getCurrentEditedKeyBinding() { return &currentEditedKeyBinding; }
			inline void setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>> value) { currentEditedKeyBinding = value; }
			bool resetCurrentEditedKeyBinding();
		private:
			const int pressedKeyCount = 256;
			std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>> currentEditedKeyBinding;
			std::vector<std::pair<std::string, int>> keyBindings;
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
			bool* pressedkeys;
			bool* lastpressedkeys;
	};
}
#endif