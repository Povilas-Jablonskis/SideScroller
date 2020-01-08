#ifndef _playerH
#define _playerH

#include "Entity.h"

namespace Engine
{
	class Player : public Entity
	{
		public:
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, glm::vec2);
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notify(EVENT_HEALTH_CHANGED, nullptr); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notify(EVENT_SCORE_CHANGED, nullptr); }
			inline glm::vec2 getCamera() const { return camera; }
			float getCamera(int) const;
			float getLastCamera(int) const;
			inline glm::vec2 getLastCamera() const { return lastCamera; }
		private:
			glm::vec2 lastCamera;
			glm::vec2 camera;
			void respawn();
			void restart();
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif