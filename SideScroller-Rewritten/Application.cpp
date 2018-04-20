#include "Application.h"
#include <ctime>
#include <fstream>

namespace Engine
{
	const glm::vec2 Application::gravity = glm::vec2(15.0f, -120.0f);

	Application::Application() 
		: inputManager(std::make_shared<InputManager>()), spriteSheetManager(std::make_shared<SpriteSheetManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), gameState(STATE_NOT_STARTED_YET), currentMainFont("")
	{
		soundEngine = irrklang::createIrrKlangDevice();

		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;
		t = 0.0f;

		srand((int)time(NULL));

		onNotify = [this](ObserverEvent _event, BaseGameObject* subject)
		{
			switch (_event)
			{
				case EVENT_SCORE_CHANGED:
				{
					updatePlayerScore();
					break;
				}
				case EVENT_HEALTH_CHANGED:
				{
					updatePlayerHealth();
					break;
				}
			}
		};

		renderer->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		renderer->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		loadConfig();
		initSpriteSheets();

		background = std::make_shared<UIElementBase>(2016.0f, 2016.0f, glm::vec2(0.0f, 0.0f), glm::vec4(208.0f, 244.0f, 247.0f, 1.0f), glm::vec2(0.0f, 0.0f));

		initGameUI();
		currentMenu = getUIElement("Main Menu");
	}

	Application::~Application()
	{
		enemies.clear();
		ui.clear();
		playerUI.clear();
	}

	void Application::initSpriteSheets()
	{
		auto healthSpriteSheet = std::make_shared<SpriteSheet>();
		auto spriteSheet = std::make_shared<SpriteSheet>();
		auto playerSpriteSheet = std::make_shared<SpriteSheet>();
		auto enemySpriteSheet = std::make_shared<SpriteSheet>();
		auto ropeAttachedSpriteSheet = std::make_shared<SpriteSheet>();
		auto ropeVerticalSpriteSheet = std::make_shared<SpriteSheet>();
		auto boxItemAltSpriteSheet = std::make_shared<SpriteSheet>();
		auto grassMidSpriteSheet = std::make_shared<SpriteSheet>();
		auto brickWallSpriteSheet = std::make_shared<SpriteSheet>(); 
		auto keyYellowSpriteSheet = std::make_shared<SpriteSheet>();
		auto lock_yellowSpriteSheet = std::make_shared<SpriteSheet>();
		auto signExitSpriteSheet = std::make_shared<SpriteSheet>();
		auto bridgeLogsSpriteSheet = std::make_shared<SpriteSheet>();
		auto liquidWaterTop_midSpriteSheet = std::make_shared<SpriteSheet>();
		auto hill_smallSpriteSheet = std::make_shared<SpriteSheet>();
		auto snailShellSpriteSheet = std::make_shared<SpriteSheet>();

		std::vector<glm::vec4> sprites;

		boxItemAltSpriteSheet->loadSpriteSheet("Tiles/boxItemAlt.png");
		ropeAttachedSpriteSheet->loadSpriteSheet("Tiles/ropeAttached.png");
		ropeVerticalSpriteSheet->loadSpriteSheet("Tiles/ropeVertical.png");
		grassMidSpriteSheet->loadSpriteSheet("Tiles/grassMid.png");
		brickWallSpriteSheet->loadSpriteSheet("Tiles/brickWall.png");
		keyYellowSpriteSheet->loadSpriteSheet("Items/keyYellow.png");
		lock_yellowSpriteSheet->loadSpriteSheet("Tiles/lock_yellow.png");
		signExitSpriteSheet->loadSpriteSheet("Tiles/signExit.png");
		bridgeLogsSpriteSheet->loadSpriteSheet("Tiles/bridgeLogs.png");
		liquidWaterTop_midSpriteSheet->loadSpriteSheet("Tiles/liquidWaterTop_mid.png");
		hill_smallSpriteSheet->loadSpriteSheet("Tiles/hill_small.png");
		snailShellSpriteSheet->loadSpriteSheet("Enemies/snailShell.png");

		enemySpriteSheet->loadSpriteSheet("Enemies/enemies_spritesheet.png");
		enemySpriteSheet->loadSpritesFromXml("Enemies/enemies_spritesheet.xml");

		sprites.clear();
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("slimeWalk1"));
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("slimeWalk2"));
		enemySpriteSheet->makeAnimation("slimeWalk", sprites);

		enemySpriteSheet->getAnimation("slimeWalk")->setLoopStatus(true);
		enemySpriteSheet->getAnimation("slimeWalk")->setDelay(0.1f);

		sprites.clear();
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("flyFly1"));
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("flyFly2"));
		enemySpriteSheet->makeAnimation("flyFly", sprites);

		enemySpriteSheet->getAnimation("flyFly")->setLoopStatus(true);
		enemySpriteSheet->getAnimation("flyFly")->setDelay(0.1f);

		sprites.clear();
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("snailWalk1"));
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("snailWalk2"));
		enemySpriteSheet->makeAnimation("snailWalk", sprites);

		enemySpriteSheet->getAnimation("snailWalk")->setLoopStatus(true);
		enemySpriteSheet->getAnimation("snailWalk")->setDelay(0.1f);

		playerSpriteSheet->loadSpriteSheet("Player/p1_spritesheet.png");
		playerSpriteSheet->loadSpritesFromXml("Player/p1_spritesheet.xml");

		sprites.clear();
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk01"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk02"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk03"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk04"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk05"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk06"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk07"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk08"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk09"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk10"));
		sprites.push_back(playerSpriteSheet->getSpriteAsVector("walk11"));
		playerSpriteSheet->makeAnimation("walk", sprites);

		playerSpriteSheet->getAnimation("walk")->setLoopStatus(true);
		playerSpriteSheet->getAnimation("walk")->setDelay(0.1f);

		spriteSheet->loadSpriteSheet("Tiles/box.png");

		healthSpriteSheet->loadSpriteSheet("GUI/healthBar.png");
		healthSpriteSheet->loadSpritesFromXml("GUI/healthBar.xml");

		spriteSheetManager->loadSpriteSheet("health", healthSpriteSheet);
		spriteSheetManager->loadSpriteSheet("box", spriteSheet);
		spriteSheetManager->loadSpriteSheet("player", playerSpriteSheet);
		spriteSheetManager->loadSpriteSheet("enemy", enemySpriteSheet);
		spriteSheetManager->loadSpriteSheet("ropeAttached", ropeAttachedSpriteSheet);
		spriteSheetManager->loadSpriteSheet("ropeVertical", ropeVerticalSpriteSheet);
		spriteSheetManager->loadSpriteSheet("boxItemAlt", boxItemAltSpriteSheet);
		spriteSheetManager->loadSpriteSheet("grassMid", grassMidSpriteSheet);
		spriteSheetManager->loadSpriteSheet("brickWall", brickWallSpriteSheet);
		spriteSheetManager->loadSpriteSheet("keyYellow", keyYellowSpriteSheet);
		spriteSheetManager->loadSpriteSheet("lock_yellow", lock_yellowSpriteSheet);
		spriteSheetManager->loadSpriteSheet("signExit", signExitSpriteSheet);
		spriteSheetManager->loadSpriteSheet("bridgeLogs", bridgeLogsSpriteSheet);
		spriteSheetManager->loadSpriteSheet("liquidWaterTop_mid", liquidWaterTop_midSpriteSheet);
		spriteSheetManager->loadSpriteSheet("hill_small", hill_smallSpriteSheet);
		spriteSheetManager->loadSpriteSheet("snailShell", snailShellSpriteSheet);
	}

	void Application::initPlayerUI()
	{
		playerUI.clear();

		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option = std::make_shared<Text>(std::to_string(player->getScore()), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), fontManager->getFont(currentMainFont), glm::vec2(90.0f, 93.0f), true);
		getPlayerUIElement("Score")->addText(std::move(option));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option2 = std::make_shared<UIElement>(162.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("health")->getSprite(std::to_string(player->getHealth())));
		getPlayerUIElement("Health")->addUIElement(std::move(option2));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option3 = std::make_shared<Text>("Level completed!", glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(40.0f, 55.0f), true);
		getPlayerUIElement("Level completed")->addText(std::move(option3));

		getPlayerUIElement("Score")->fixPosition();
		getPlayerUIElement("Health")->fixPosition();
		getPlayerUIElement("Level completed")->fixPosition();
	}

	void Application::updatePlayerHealth()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		erasePlayerUIElement("Health");
		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		//Health
		auto option2 = std::make_shared<UIElement>(162.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("health")->getSprite(std::to_string(player->getHealth())));
		getPlayerUIElement("Health")->addUIElement(std::move(option2));
		getPlayerUIElement("Health")->fixPosition();
	}

	void Application::updatePlayerScore()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		erasePlayerUIElement("Score");
		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		//Score
		auto option = std::make_shared<Text>(std::to_string(player->getScore()), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), fontManager->getFont(currentMainFont), glm::vec2(90.0f, 93.0f), true);
		getPlayerUIElement("Score")->addText(std::move(option));
		getPlayerUIElement("Score")->fixPosition();
	}

	void Application::initScene()
	{
		player = std::make_shared<Player>(32.0f, 32.0f, glm::vec2(0.0f, 64.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		player->addAnimation("walk", spriteSheetManager->getSpriteSheet("player")->getAnimation("walk"));
		player->addAnimation("stand", spriteSheetManager->getSpriteSheet("player")->getSprite("stand"));
		player->addAnimation("jump", spriteSheetManager->getSpriteSheet("player")->getSprite("jump"));
		player->addAnimation("duck", spriteSheetManager->getSpriteSheet("player")->getSprite("duck"));
		player->onDeath = [this]()
		{
			player->resetInput();
			setState(STATE_ENDED);
			currentMenu = getUIElement("Game Over");
			getUIElement("Game Over")->showMain(false);
		};
		player->addObserver(this);

		objects.clear();
		unlockableObjects.clear();

		for (int i = 32; i < 256; i += 32)
		{
			auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(192.0f, i), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
			object->applyAnimation(spriteSheetManager->getSpriteSheet("lock_yellow")->getSprite("wholeSpriteSheet"));
			unlockableObjects.push_back(std::pair<std::string, std::shared_ptr<BaseGameObject>>("keyYellow", std::move(object)));
		}

		auto object = std::make_shared<BaseGameObject>(24.0f, 53.0f, glm::vec2(0.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("hill_small")->getSprite("wholeSpriteSheet"));
		backgroundObjects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(0.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(32.0f, 128.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("brickWall")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(64.0f, 128.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("boxItemAlt")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(64.0f, 160.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("keyYellow")->getSprite("wholeSpriteSheet"));
		object->onCollisionEnter = [this, object](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);
			if (player == nullptr || object->getNeedsToBeDeleted()) return;
			object->setNeedsToBeDeleted(true);
			unlockableObjects.clear();
		};
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(96.0f, 128.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("brickWall")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(128.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<Climbable>(5.0f, 128.0f, glm::vec2(128.0f, 64.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->onCollisionEnter = [](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto entity = dynamic_cast<Entity*>(collider);

			if (entity != nullptr && entity->getSecondState() != STATE_CLIMBING && entity->getSecondState() != STATE_FLYING)
			{
				entity->setVelocity(1, 0.0f);
				entity->setSecondState(STATE_CLIMBING);
			}
		};
		object->onCollisionExit = [](BaseGameObject* collider)
		{
			auto entity = dynamic_cast<Entity*>(collider);

			if (entity != nullptr && entity->getSecondState() == STATE_CLIMBING)
				entity->setSecondState(STATE_FALLING);
		};
		object->applyAnimation(spriteSheetManager->getSpriteSheet("ropeVertical")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(256.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("signExit")->getSprite("wholeSpriteSheet"));
		object->onCollisionEnter = [this, object](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			if (object->getNeedsToBeDeleted()) return;
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr)
			{
				object->setNeedsToBeDeleted(true);
				Timer::windowsTimer([this]{ getPlayerUIElement("Level completed")->hideMain(); }, 2000);
				getPlayerUIElement("Level completed")->showMain(false);
			}
		};
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(352.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(384.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(512.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(576.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		addObjectToList(std::move(object));

		for (int i = 0; i < 2016; i += 32)
		{
			if (i > 32 && i < 128)
			{
				auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("bridgeLogs")->getSprite("wholeSpriteSheet"));
				addObjectToList(std::move(object));

				object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("liquidWaterTop_mid")->getSprite("wholeSpriteSheet"));
				addObjectToList(std::move(object));
			}
			else
			{
				auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("grassMid")->getSprite("wholeSpriteSheet"));
				addObjectToList(std::move(object));
			}
		}

		enemies.clear();

		auto enemy = std::make_shared<Enemy>(35.0f, 20.0f, glm::vec2(64.0f, 32.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		enemy->addAnimation("walk", spriteSheetManager->getSpriteSheet("enemy")->getAnimation("slimeWalk"));
		enemy->addAnimation("dead", spriteSheetManager->getSpriteSheet("enemy")->getSprite("slimeDead"));
		enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
		enemy->updateInput(inputManager);
		enemy->onDeath = [this, enemy]()
		{
			enemy->setSecondState(STATE_IDLE);
			enemy->setKey(inputManager->getKeyBinding("Jump"), true);
			enemy->setKey(inputManager->getKeyBinding("Move Left"), false);
			enemy->setKey(inputManager->getKeyBinding("Move Right"), false);
			enemy->updateInput(inputManager);
			enemy->setFirstState(STATE_DEAD);
		};
		enemy->onCollisionEnter = [this, enemy](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr )
			{
				if (collisionInfo.side == SIDE_TOP)
				{
					player->setScore(player->getScore() + 100);
					enemy->onDeath();
				}
				else
					player->setNeedsToBeDeleted(true);
			}
			else
			{
				if (collisionInfo.side == SIDE_LEFT)
				{
					enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
					enemy->setKey(inputManager->getKeyBinding("Move Left"), false);
					enemy->updateInput(inputManager);
				}
				else if (collisionInfo.side == SIDE_RIGHT)
				{
					enemy->setKey(inputManager->getKeyBinding("Move Right"), false);
					enemy->setKey(inputManager->getKeyBinding("Move Left"), true);
					enemy->updateInput(inputManager);
				}
			}
		};
		enemies.push_back(std::move(enemy));

		enemy = std::make_shared<Enemy>(35.0f, 20.0f, glm::vec2(320.0f, 300.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		enemy->addAnimation("walk", spriteSheetManager->getSpriteSheet("enemy")->getAnimation("flyFly"));
		enemy->addAnimation("dead", spriteSheetManager->getSpriteSheet("enemy")->getSprite("flyDead"));
		enemy->setSecondState(STATE_FLYING);
		enemy->setKey(inputManager->getKeyBinding("Move Left"), true);
		enemy->updateInput(inputManager);
		enemies.push_back(std::move(enemy));

		enemy = std::make_shared<Enemy>(54.0f, 31.0f, glm::vec2(352.0f, 65.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		enemy->addAnimation("walk", spriteSheetManager->getSpriteSheet("enemy")->getAnimation("snailWalk"));
		enemy->addAnimation("dead", spriteSheetManager->getSpriteSheet("enemy")->getSprite("snailShell_upsidedown"));
		enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
		enemy->updateInput(inputManager);
		enemy->onCollisionEnter = [this, enemy](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr)
			{
				if (collisionInfo.side == SIDE_TOP)
				{
					if (enemy->getSize(0) != 44.0f && enemy->getSize(1) != 30.0f)
					{
						player->setScore(player->getScore() + 100);
						enemy->setSize(0, 44.0f);
						enemy->setSize(1, 30.0f);
						enemy->addAnimation("stand", spriteSheetManager->getSpriteSheet("snailShell")->getSprite("wholeSpriteSheet"));
						enemy->changeAnimation("walk", spriteSheetManager->getSpriteSheet("snailShell")->getSprite("wholeSpriteSheet"));
						enemy->setKey(inputManager->getKeyBinding("Move Left"), false);
						enemy->setKey(inputManager->getKeyBinding("Move Right"), false);
						enemy->updateInput(inputManager);
					}
					else
					{
						if (!enemy->getKey(inputManager->getKeyBinding("Move Right")) && !enemy->getKey(inputManager->getKeyBinding("Move Left")))
							enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
						else
						{
							enemy->setKey(inputManager->getKeyBinding("Move Right"), false);
							enemy->setKey(inputManager->getKeyBinding("Move Left"), false);
						}
						enemy->updateInput(inputManager);
					}
				}
				else
					player->setNeedsToBeDeleted(true);
			}
			else
			{
				if (collisionInfo.side == SIDE_LEFT)
				{
					enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
					enemy->setKey(inputManager->getKeyBinding("Move Left"), false);
					enemy->updateInput(inputManager);
				}
				else if (collisionInfo.side == SIDE_RIGHT)
				{
					enemy->setKey(inputManager->getKeyBinding("Move Right"), false);
					enemy->setKey(inputManager->getKeyBinding("Move Left"), true);
					enemy->updateInput(inputManager);
				}
			}
		};
		enemies.push_back(std::move(enemy));

		initPlayerUI();

		accumulator = 0.0f;
		t = 0.0f;
	}

	void Application::initGameUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_INIT_WINDOW_WIDTH)), (float)(glutGet(GLUT_INIT_WINDOW_HEIGHT)));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), nullptr, glm::vec2(30.0f, 30.0f))));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		auto Options = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), getUIElement("Main Menu"), glm::vec2(0.0f, 0.0f));
		auto Controls = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), Options, glm::vec2(0.0f, 0.0f));

		//Main Menu
		auto options = std::make_shared<Text>("Start Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(48.0f, 60.0f));
		options->onMouseClickFunc = [this, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			initScene();
			setState(STATE_STARTED);
			currentMenu = nullptr;
			getUIElement("Main Menu")->hideMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("Options", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(48.0f, 55.0f));
		options->onMouseClickFunc = [this, Options, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentMenu = Options;
			getUIElement("Main Menu")->hideMain();
			Options->showMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(48.0f, 50.0f));
		options->onMouseClickFunc = [this, options]()
		{
			options->checkOnMouseRelease();

			currentMenu = nullptr;
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		getUIElement("Main Menu")->addText(std::move(options));
		getUIElement("Main Menu")->showMain(true);

		//Pause Menu
		options = std::make_shared<Text>("Go To Main Menu", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(45.0f, 60.0f));
		options->onMouseClickFunc = [this, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);
			currentMenu = getUIElement("Main Menu");
			setState(STATE_NOT_STARTED_YET);
			getUIElement("Pause Menu")->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		getUIElement("Pause Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(48.0f, 55.0f));
		options->onMouseClickFunc = [this, options]()
		{
			options->checkOnMouseRelease();

			currentMenu = nullptr;
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		getUIElement("Pause Menu")->addText(std::move(options));
		
		//Controls
		auto keybindings = inputManager->getKeyBindings();
		size_t i = 0;
		for (std::vector<std::pair<std::string, int>>::iterator it = keybindings->begin(); it != keybindings->end(); ++it)
		{
			options = std::make_shared<Text>(it->first + ": ", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(20.0f, 80.0f - (10 * i)), true);
			Controls->addText(std::move(options));
			options = std::make_shared<Text>(virtualKeyCodeToString(it->second), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(50.0f, 80.0f - (10 * i)), true);
			options->onMouseClickFunc = [this, options, it]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);
				inputManager->resetCurrentEditedKeyBinding();
				inputManager->setCurrentEditedKeyBinding(CurrentEditedKeyBinding(it->first, options));
				options->changeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			};
			Controls->addText(std::move(options));
			i++;
		}
		options = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(20.0f, 80.0f - (10 * i)));
		options->onMouseClickFunc = [this, Options, Controls, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = Options;
			Controls->hideMain();
			Options->showMain();
		};
		Controls->addText(std::move(options));

		//Options 
		options = std::make_shared<Text>("Controls", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(50.0f, 60.0f));
		options->onMouseClickFunc = [this, Options, Controls, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentMenu = Controls;
			Options->hideMain();
			Controls->showMain();
		};
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Sounds", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(50.0f, 55.0f));
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(50.0f, 50.0f));
		options->onMouseClickFunc = [this, Options, options]()
		{
			options->checkOnMouseRelease();

			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = getUIElement("Main Menu");
			Options->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		Options->addText(std::move(options));
		Options->addUIElement(std::move(Controls));
		getUIElement("Main Menu")->addUIElement(std::move(Options));

		//Game Over
		options = std::make_shared<Text>("Game Over", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont(currentMainFont), glm::vec2(50.0f, 55.0f), true);
		getUIElement("Game Over")->addText(std::move(options));

		getUIElement("Game Over")->hideMain();
		getUIElement("Pause Menu")->hideMain();
	}

	void Application::loadConfig()
	{
		std::ifstream fs("config.json");
		nlohmann::json j;
		fs >> j;

		for (size_t i = 0; i < j["keyBindings"].size(); i++)
		{
			auto key = j["keyBindings"][i]["key"].get<std::string>();
			auto value = std::stoul(j["keyBindings"][i]["value"].get<std::string>(), nullptr, 16);
			inputManager->setKeyBinding(key, value);
		}

		auto fontName = j["font"]["key"].get<std::string>();
		auto fontSize = j["font"]["value"].get<int>();
		auto stringBeforeSeparator = getStringBeforeSeparator(fontName, '.');

		currentMainFont = stringBeforeSeparator + std::to_string(fontSize);
		fontManager->loadFont(fontName, stringBeforeSeparator + std::to_string(fontSize), fontSize);
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto listOfAllElements = std::vector<std::shared_ptr<UIElement>>();
		auto listOfAllTexts = std::vector<std::shared_ptr<Text>>();

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
		float frameTime = (newTime - currentTime) / 1000.0f;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			if (getState() == STATE_STARTED)
			{
				for (std::vector<std::shared_ptr<Enemy>>::iterator it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt, gravity))
						it = enemies.erase(it);
					else
					{
						//Collision detection

						collisionManager->checkCollision(*it, objects);
						std::vector<std::shared_ptr<BaseGameObject>> tempVector;

						for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it2 = unlockableObjects.begin(); it2 != unlockableObjects.end(); ++it2)
							tempVector.push_back(it2->second);

						collisionManager->checkCollision(*it, tempVector);

						++it;
					}
				}

				for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = objects.begin(); it != objects.end();)
				{
					if ((*it)->update(dt, gravity))
						it = objects.erase(it);
					else
						++it;
				}

				for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it = unlockableObjects.begin(); it != unlockableObjects.end();)
				{
					if (it->second->update(dt, gravity))
						it = unlockableObjects.erase(it);
					else
						++it;
				}

				player->fixInput();
				player->update(dt, gravity);

				//Collision detection

				collisionManager->checkCollision(player, objects);

				std::vector<std::shared_ptr<BaseGameObject>> tempVector;

				for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it = unlockableObjects.begin(); it != unlockableObjects.end(); ++it)
					tempVector.push_back(it->second);

				collisionManager->checkCollision(player, tempVector);
				collisionManager->checkCollision(player, enemies);

				t += dt;
			}
			accumulator -= dt;
		}

		if (getState() == STATE_STARTED)
		{
			//Render background
			renderer->draw(background, player->getCamera());
			//Render backgroundObjects
			renderer->draw(backgroundObjects, player->getCamera());
			//Render objects
			renderer->draw(objects, player->getCamera());
			//Render unlockableObjects
			for (auto unlockableObject : unlockableObjects)
			{
				renderer->draw(unlockableObject.second, player->getCamera());
			}
			//Render player
			renderer->draw(player);
			//Render enemies
			renderer->draw(enemies, player->getCamera());
			//Render & Update player UI
			for (auto uiElement : playerUI)
			{
				uiElement.second->update(dt);
				listOfAllElements.push_back(uiElement.second);
				uiElement.second->GetAllChildrenElements(listOfAllElements);
				uiElement.second->GetAllChildrenTexts(listOfAllTexts);
			}

			renderer->draw(listOfAllElements);
			renderer->draw(listOfAllTexts);

			listOfAllElements.clear();
			listOfAllTexts.clear();
		}

		//Render & Update UI
		for (auto uiElement : ui)
		{
			uiElement.second->update(dt);
			listOfAllElements.push_back(uiElement.second);
			uiElement.second->GetAllChildrenElements(listOfAllElements);
			uiElement.second->GetAllChildrenTexts(listOfAllTexts);
		}

		renderer->draw(listOfAllElements);
		renderer->draw(listOfAllTexts);

		glutSwapBuffers();
	}

	void Application::resize(int width, int height)
	{
		//const float ar = (float) width / (float) height;

		for (auto uiElement : ui)
			uiElement.second->fixPosition();

		for (auto uiElement : playerUI)
			uiElement.second->fixPosition();

		#if _DEBUG
			glutReshapeWindow(width, height);
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
			return;
		#endif

		width = glutGet(GLUT_INIT_WINDOW_WIDTH);
		height = glutGet(GLUT_INIT_WINDOW_HEIGHT);

		glutReshapeWindow(width, height);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	}

	void Application::keyboardInputUp(unsigned char c, int x, int y)
	{
		if (player == nullptr) return;

		auto key = VkKeyScan(c);
		player->setKey(key, false);
		player->updateInput(inputManager);
	}

	void Application::keyboardInput(unsigned char c, int x, int y)
	{
		auto key = VkKeyScan(c);

		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		if (c >= 32 && c < 127 && !inputManager->isKeyBindingUsed(key) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			inputManager->setKeyBinding(currentKeyBinding->first, key);
			currentKeyBinding->second->setText(virtualKeyCodeToString(key));
			inputManager->resetCurrentEditedKeyBinding();
		}

		switch (c)
		{
			case 27:
			{
				if (getState() == STATE_ENDED)
				{
					currentMenu = getUIElement("Main Menu");
					getUIElement("Game Over")->hideMain();
					getUIElement("Main Menu")->showMain();
					setState(STATE_NOT_STARTED_YET);
				}
				else if (getState() == STATE_PAUSED)
				{
					currentMenu = nullptr;
					getUIElement("Pause Menu")->hideMain();
					setState(STATE_STARTED);
				}
				else if (getState() == STATE_STARTED)
				{
					currentMenu = getUIElement("Pause Menu");
					getUIElement("Pause Menu")->showMain(false);
					setState(STATE_PAUSED);
				}
				else if (getState() == STATE_NOT_STARTED_YET)
				{
					if (!inputManager->resetCurrentEditedKeyBinding() && currentMenu != nullptr && currentMenu->getParent() != nullptr)
					{
						soundEngine->play2D("Sounds/buttonselect/6.wav", GL_FALSE);
						currentMenu->hideMain();
						currentMenu = currentMenu->getParent();
						currentMenu->showMain();
					}
				}
				break;
			}
		}

		if (player == nullptr) return;

		player->setKey(key, true);
		player->updateInput(inputManager);
	}

	void Application::specialKeyInputUp(int key, int x, int y)
	{
		if (player == nullptr) return;

		switch (key)
		{
			case GLUT_KEY_UP:
			{
				if (player->getKey(VK_UP))
					player->setKey(VK_UP, false);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				if (player->getKey(VK_DOWN))
					player->setKey(VK_DOWN, false);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				if (player->getKey(VK_LEFT))
					player->setKey(VK_LEFT, false);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				if (player->getKey(VK_RIGHT))
					player->setKey(VK_RIGHT, false);
				break;
			}
		}

		player->updateInput(inputManager);
	}

	void Application::specialKeyInput(int key, int x, int y)
	{
		int c = 0;
		std::string charText = "";

		switch (key)
		{
			case GLUT_KEY_UP:
			{
				charText = virtualKeyCodeToString(VK_UP);
				c = VK_UP;
				if (player != nullptr && !player->getKey(VK_UP))
					player->setKey(VK_UP, true);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				charText = virtualKeyCodeToString(VK_DOWN);
				c = VK_DOWN;
				if (player != nullptr && !player->getKey(VK_DOWN))
					player->setKey(VK_DOWN, true);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				charText = virtualKeyCodeToString(VK_LEFT);
				c = VK_LEFT;
				if (player != nullptr && !player->getKey(VK_LEFT))
					player->setKey(VK_LEFT, true);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				charText = virtualKeyCodeToString(VK_RIGHT);
				c = VK_RIGHT;
				if (player != nullptr && !player->getKey(VK_RIGHT))
					player->setKey(VK_RIGHT, true);
				break;
			}
		}

		if (player != nullptr)
			player->updateInput(inputManager);

		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		if (c != 0 && !inputManager->isKeyBindingUsed(key) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			inputManager->setKeyBinding(currentKeyBinding->first, c);
			currentKeyBinding->second->setText(charText);
			inputManager->resetCurrentEditedKeyBinding();
		}
	}

	void Application::motionFunc(int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		for (auto uiElement : ui)
		{
			uiElement.second->checkOnHover(lastMousePosition);
		}
	}

	void Application::processMouseClick(int button, int state, int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			for (auto uiElement : ui)
			{
				uiElement.second->checkOnMouseClick(lastMousePosition);
			}
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			for (auto uiElement : ui)
			{
				uiElement.second->checkOnMouseRelease();
			}
		}
	}

	std::string Application::virtualKeyCodeToString(SHORT virtualKey)
	{
		UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		CHAR szName[128];
		int result = 0;
		switch (virtualKey)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= 0x100;
			default:
				result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}
		return szName;
	}

	std::shared_ptr<UIElement> Application::getUIElement(const std::string& index)
	{
		for (auto uiElement : ui)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	std::shared_ptr<UIElement> Application::getPlayerUIElement(const std::string& index)
	{
		for (auto uiElement : playerUI)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	void Application::erasePlayerUIElement(const std::string& index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<UIElement>>>::iterator it = playerUI.begin(); it != playerUI.end(); ++it)
		{
			if (it->first == index)
			{
				playerUI.erase(it);
				return;
			}
		}
	}

	std::string Application::getStringBeforeSeparator(std::string const& s, char separator)
	{
		std::string::size_type pos = s.find(separator);
		if (pos != std::string::npos)
		{
			return s.substr(0, pos);
		}
		else
		{
			return s;
		}
	}
}