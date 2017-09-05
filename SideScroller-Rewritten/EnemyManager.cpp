#include "EnemyManager.h"
#include <fstream>

namespace Engine
{
	void EnemyManager::loadEnemiesFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, std::shared_ptr<EffectManager> effectManager)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("enemies.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Enemies");
		//Iterate over the brewerys
		for (rapidxml::xml_node<> * brewery_node = root_node->first_node("Enemy"); brewery_node; brewery_node = brewery_node->next_sibling("Enemy"))
		{
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			auto shootingEffect = brewery_node->first_attribute("shootingEffect")->value();
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto enemy = std::make_shared<Enemy>(32.0f, 32.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
			effectManager->getEffect(shootingEffect)(enemy.get());
			enemy->applyAnimation(sprite);

			for (rapidxml::xml_node<> * beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
			{
				for (rapidxml::xml_node<> * beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
				{
					enemy->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));
				}
				for (rapidxml::xml_node<> * beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
				{
					enemy->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node2->first_attribute("animationName")->value()));
				}
			}

			/*for (auto effect : effects)
			{
				if (effect.first == brewery_node->first_attribute("effect")->value())
					enemy->setEffect(effect.second);
			}*/

			enemies.push_back(std::pair<std::string, std::shared_ptr<Enemy>>(brewery_node->first_attribute("name")->value(), std::move(enemy)));
		}
	}

	std::shared_ptr<Enemy> EnemyManager::getEnemy(std::string index)
	{
		for (auto enemy : enemies)
		{
			if (enemy.first == index)
				return std::make_shared<Enemy>(*enemy.second);
		}

		return nullptr;
	}

	std::shared_ptr<Enemy> EnemyManager::getRandomEnemy()
	{
		int randIndex = rand() % enemies.size();
		return std::make_shared<Enemy>(*enemies[randIndex].second);
	}
}