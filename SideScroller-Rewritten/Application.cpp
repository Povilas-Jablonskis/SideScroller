#include "Application.h"
#include <ctime>

namespace Engine
{
	Application::Application() 
		: inputManager(std::make_shared<InputManager>()), spriteSheetManager(std::make_shared<SpriteSheetManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), gameState(STATE_NOT_STARTED_YET)
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

		inputManager->setKeyBinding("Attack", 0x5A);
		inputManager->setKeyBinding("Move Left", 0x41);
		inputManager->setKeyBinding("Move Right", 0x44);
		inputManager->setKeyBinding("Jump", VK_SPACE);
		inputManager->setKeyBinding("Duck", 0x53);
		inputManager->setKeyBinding("Climb", 0x57);

		renderer->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		renderer->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		fontManager->loadFont("kenvector_future.ttf", "kenvector_future");
		fontManager->loadFont("kenvector_future_thin.ttf", "kenvector_future_thin");

		initSpriteSheets();

		background = std::make_shared<UIElementBase>(2000.0f, 2000.0f, glm::vec2(0.0f, 0.0f), glm::vec4(208.0f, 244.0f, 247.0f, 1.0f), glm::vec2(0.0f, 0.0f));

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

		enemySpriteSheet->loadSpriteSheet("Enemies/enemies_spritesheet.png");
		enemySpriteSheet->loadSpritesFromXml("Enemies/enemies_spritesheet.xml");

		sprites.clear();
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("slimeWalk1"));
		sprites.push_back(enemySpriteSheet->getSpriteAsVector("slimeWalk2"));
		enemySpriteSheet->makeAnimation("slimeWalk", sprites);

		enemySpriteSheet->getAnimation("slimeWalk")->setLoopStatus(true);
		enemySpriteSheet->getAnimation("slimeWalk")->setDelay(0.1f);

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
	}

	void Application::initPlayerUI()
	{
		playerUI.clear();

		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Score")->addText(std::move(option));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option2 = std::make_shared<UIElement>(162.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("health")->getSprite(std::to_string(player->getHealth())));
		getPlayerUIElement("Health")->addUIElement(std::move(option2));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option3 = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		option3->setIsStatic(true);
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
		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Score")->addText(std::move(option));
		getPlayerUIElement("Score")->fixPosition();
	}

	void Application::initScene()
	{
		player = std::make_shared<Player>(32.0f, 32.0f, glm::vec2(500.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
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

		for (int i = 0; i < 2000; i += 32)
		{
			auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, -16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
			if (i > 32 && i < 128)
			{
				auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, -16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("bridgeLogs")->getSprite("wholeSpriteSheet"));
				objects.push_back(std::move(object));

				object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, -16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("liquidWaterTop_mid")->getSprite("wholeSpriteSheet"));
				objects.push_back(std::move(object));
			}
			else
			{
				auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(i, -16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
				object->applyAnimation(spriteSheetManager->getSpriteSheet("grassMid")->getSprite("wholeSpriteSheet"));
				objects.push_back(std::move(object));
			}
		}

		for (int i = 16; i < 256; i += 32)
		{
			auto object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(188.0f, i), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
			object->applyAnimation(spriteSheetManager->getSpriteSheet("lock_yellow")->getSprite("wholeSpriteSheet"));
			unlockableObjects.push_back(std::pair<std::string, std::shared_ptr<BaseGameObject>>("keyYellow", std::move(object)));
		}

		auto object = std::make_shared<BaseGameObject>(24.0f, 53.0f, glm::vec2(16.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("hill_small")->getSprite("wholeSpriteSheet"));
		backgroundObjects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(0.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(32.0f, 124.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("brickWall")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(64.0f, 124.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("boxItemAlt")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(64.0f, 156.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("keyYellow")->getSprite("wholeSpriteSheet"));
		object->onCollisionEnter = [this, object](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			object->setNeedsToBeDeleted(true);
			unlockableObjects.clear();
		};
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(96.0f, 124.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("brickWall")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(128.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(5.0f, 108.0f, glm::vec2(134.0f, 48.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->setClimable(true);
		object->onCollisionEnter = [](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr)
				player->setCanClimb(true);
		};
		object->onCollisionExit = [](BaseGameObject* collider)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr)
				player->setCanClimb(false);
		};
		object->applyAnimation(spriteSheetManager->getSpriteSheet("ropeVertical")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(252.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("signExit")->getSprite("wholeSpriteSheet"));
		object->onCollisionEnter = [this, object](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr)
			{
				object->setNeedsToBeDeleted(true);
				Timer::windowsTimer([this]{ getPlayerUIElement("Level completed")->hideMain(); }, 2000);
				getPlayerUIElement("Level completed")->showMain(false);
			}
		};
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(320.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		object = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(352.0f, 16.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		object->applyAnimation(spriteSheetManager->getSpriteSheet("box")->getSprite("wholeSpriteSheet"));
		objects.push_back(std::move(object));

		enemies.clear();

		auto enemy = std::make_shared<Enemy>(35.0f, 19.6f, glm::vec2(320.0f, 50.0f), glm::vec2(10.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		enemy->addAnimation("walk", spriteSheetManager->getSpriteSheet("enemy")->getAnimation("slimeWalk"));
		enemy->addAnimation("dead", spriteSheetManager->getSpriteSheet("enemy")->getSprite("slimeDead"));
		enemy->setKey(inputManager->getKeyBinding("Move Right"), true);
		enemy->updateInput(inputManager);
		enemy->onCollisionEnter = [this, enemy](BaseGameObject* collider, CollisionInfo collisionInfo)
		{
			auto player = dynamic_cast<Player*>(collider);

			if (player != nullptr )
			{
				if (collisionInfo.side == SIDE_TOP)
				{
					player->setScore(player->getScore() + 100);
					enemy->setVelocity(glm::vec2(0.0f, 50.0f));
					enemy->setFirstState(STATE_DEAD);
				}
				else
					player->setNeedsToBeDeleted(true);
			}
			else
			{
				if (collisionInfo.side == SIDE_LEFT || collisionInfo.side == SIDE_RIGHT)
					enemy->setVelocity(0, enemy->getVelocity(0) * -1.0f);
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
		auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			initScene();
			setState(STATE_STARTED);
			currentMenu = nullptr;
			getUIElement("Main Menu")->hideMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = Options;
			getUIElement("Main Menu")->hideMain();
			Options->showMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
		options->onMouseReleaseFunc = [this]()
		{
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
		options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

			currentMenu = getUIElement("Main Menu");
			setState(STATE_NOT_STARTED_YET);
			getUIElement("Pause Menu")->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		getUIElement("Pause Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this]()
		{
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
		for (std::vector<std::pair<std::string, int>>::iterator it = keybindings->begin(); it != keybindings->end(); it++)
		{
			options = std::make_shared<Text>(it->first + ": ", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 80.0f - (10 * i)));
			options->setIsStatic(true);
			Controls->addText(std::move(options));
			options = std::make_shared<Text>(virtualKeyCodeToString(it->second), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 80.0f - (10 * i)));
			options->onMouseClickFunc = [this, options, it]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

				inputManager->resetCurrentEditedKeyBinding();
				inputManager->setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>(it, options));
				options->setIsStatic(true);
				options->changeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			};
			Controls->addText(std::move(options));
			i++;
		}
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 80.0f - (10 * i)));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = Options;
			Controls->hideMain();
			Options->showMain();
		};
		Controls->addText(std::move(options));

		//Options
		options = std::make_shared<Text>("Controls", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentMenu = Controls;
			Options->hideMain();
			Controls->showMain();
		};
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Sounds", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = getUIElement("Main Menu");
			Options->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		Options->addText(std::move(options));
		Options->addUIElement(std::move(Controls));
		getUIElement("Main Menu")->addUIElement(std::move(Options));

		//Game Over
		options = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		options->setIsStatic(true);
		getUIElement("Game Over")->addText(std::move(options));

		getUIElement("Game Over")->hideMain();
		getUIElement("Pause Menu")->hideMain();
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
			inputManager->fixInput();
			if (getState() == STATE_STARTED)
			{
				for (std::vector<std::shared_ptr<Entity>>::iterator it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt, gravity))
						it = enemies.erase(it);
					else
					{
						//Collision detection

						collisionManager->checkCollision(*it, &objects);
						std::vector<std::shared_ptr<BaseGameObject>> tempVector;

						for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it2 = unlockableObjects.begin(); it2 != unlockableObjects.end(); ++it2)
							tempVector.push_back(it2->second);

						collisionManager->checkCollision(*it, &tempVector);

						it++;
					}
				}

				for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = objects.begin(); it != objects.end();)
				{
					if ((*it)->update(dt, gravity))
						it = objects.erase(it);
					else
						it++;
				}

				for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it = unlockableObjects.begin(); it != unlockableObjects.end();)
				{
					if (it->second->update(dt, gravity))
						it = unlockableObjects.erase(it);
					else
						it++;
				}

				player->fixInput();
				player->update(dt, gravity);

				//Collision detection

				collisionManager->checkCollision(player, &objects);

				std::vector<std::shared_ptr<BaseGameObject>> tempVector;

				for (std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>>::iterator it = unlockableObjects.begin(); it != unlockableObjects.end(); ++it)
					tempVector.push_back(it->second);

				collisionManager->checkCollision(player, &tempVector);
				collisionManager->checkCollision(player, &enemies);

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
				uiElement.second->GetAllChildrenElements(&listOfAllElements);
				uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
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
			uiElement.second->GetAllChildrenElements(&listOfAllElements);
			uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
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

		auto keyBindings = inputManager->getKeyBindings();
		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		if (c >= 32 && c < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [key](std::pair<std::string, int> element){return element.second == key; }) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentKeyBinding->first->second = key;
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

		auto keyBindings = inputManager->getKeyBindings();
		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		if (c != 0 && !std::any_of(keyBindings->begin(), keyBindings->end(), [c](std::pair<std::string, int> element){return element.second == c; }) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentKeyBinding->first->second = c;
			currentKeyBinding->second->setText(charText);
			inputManager->resetCurrentEditedKeyBinding();
		}

		if (player == nullptr) return;

		switch (key)
		{
			case GLUT_KEY_UP:
			{
				charText = virtualKeyCodeToString(VK_UP);
				c = VK_UP;
				if (!player->getKey(VK_UP))
					player->setKey(VK_UP, true);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				charText = virtualKeyCodeToString(VK_DOWN);
				c = VK_DOWN;
				if (!player->getKey(VK_DOWN))
					player->setKey(VK_DOWN, true);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				charText = virtualKeyCodeToString(VK_LEFT);
				c = VK_LEFT;
				if (!player->getKey(VK_LEFT))
					player->setKey(VK_LEFT, true);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				charText = virtualKeyCodeToString(VK_RIGHT);
				c = VK_RIGHT;
				if (!player->getKey(VK_RIGHT))
					player->setKey(VK_RIGHT, true);
				break;
			}
		}
		player->updateInput(inputManager);
	}

	void Application::motionFunc(int x, int y)
	{
		inputManager->setLastMousePosition(glm::vec2(x, y));

		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		for (auto uiElement : ui)
		{
			uiElement.second->checkIfMouseHoverThis(lastMousePosition);
		}
	}

	void Application::processMouseClick(int button, int state, int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (button == GLUT_LEFT_BUTTON)
		{
			inputManager->setLeftMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastLeftMouseState(state == GLUT_DOWN ? false : true);

			for (auto uiElement : ui)
			{
				uiElement.second->checkForMouseClickOnThis(inputManager->getLeftMouseState(), inputManager->getLastLeftMouseState(), lastMousePosition);
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			inputManager->setRightMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastRightMouseState(state == GLUT_DOWN ? false : true);
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

	std::shared_ptr<UIElement> Application::getUIElement(std::string index)
	{
		for (auto uiElement : ui)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	std::shared_ptr<UIElement> Application::getPlayerUIElement(std::string index)
	{
		for (auto uiElement : playerUI)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	void Application::erasePlayerUIElement(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<UIElement>>>::iterator it = playerUI.begin(); it != playerUI.end(); it++)
		{
			if (it->first == index)
			{
				playerUI.erase(it);
				return;
			}
		}
	}

	float Application::randomFloat(float min, float max)
	{
		// this  function assumes max > min, you may want 
		// more robust error checking for a non-debug build
		assert(max > min);
		float random = ((float)rand()) / (float)RAND_MAX;

		// generate (in your case) a float between 0 and (4.5-.78)
		// then add .78, giving you a float between .78 and 4.5
		float range = max - min;
		return (random*range) + min;
	}
}