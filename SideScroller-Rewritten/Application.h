#ifndef _applicationH
#define _applicationH

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

#include <irrKlang.h>

#include "Renderer.h"
#include "FontManager.h"
#include "GameState.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "CollisionManager.h"
#include "SpriteSheetManager.h"
#include "EnemyManager.h"
#include "EffectManager.h"

#include "UIElement.h"
#include "Player.h"
#include "Enemy.h"
#include "Observer.h"
#include "Timer.h"

namespace Engine
{
	class Application : public Observer
	{
		public:
			~Application();
			Application();
			std::string virtualKeyCodeToString(SHORT);
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }
			inline void addEnemyToList(std::shared_ptr<Enemy> enemy) { enemies.push_back(enemy); }
			inline void addObjectToList(std::shared_ptr<BaseGameObject> obj){ objects.push_back(obj); }

			void render();
			void keyboardInputUp(unsigned char, int, int);
			void resize(int, int);
			void keyboardInput(unsigned char, int, int);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);
		private:
			std::shared_ptr<UIElement> getUIElement(const std::string&);
			std::shared_ptr<UIElement> getPlayerUIElement(const std::string&);
			void erasePlayerUIElement(const std::string&);

			void initPlayerUI();
			void initSpriteSheets();
			void updatePlayerHealth();
			void updatePlayerScore();
			void initScene();
			void initGameUI();

			irrklang::ISoundEngine* soundEngine;

			std::shared_ptr<Player> player;
			std::shared_ptr<UIElementBase> background;
			std::shared_ptr<UIElement> currentMenu;

			static const glm::vec2 gravity;

			float t;
			float dt;
			float currentTime;
			float accumulator;

			std::vector<std::shared_ptr<BaseGameObject>> backgroundObjects;
			std::vector<std::shared_ptr<BaseGameObject>> objects;
			std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>> unlockableObjects;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::pair<std::string, std::shared_ptr<UIElement>>> ui;
			std::vector<std::pair<std::string, std::shared_ptr<UIElement>>> playerUI;

			std::shared_ptr<SpriteSheetManager> spriteSheetManager;
			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState gameState;
	};
}
#endif