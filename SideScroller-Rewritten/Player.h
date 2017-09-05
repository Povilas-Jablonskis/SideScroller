#ifndef _playerH
#define _playerH

#include "BaseGameObject.h"
#include "Subject.h"

namespace Engine
{
	class Player : public BaseGameObject, public Subject
	{
		public:
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, glm::vec2);
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notify(ObserverEvent::HEALTHCHANGED, nullptr); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notify(ObserverEvent::SCORECHANGED, nullptr); }
		private:
			void respawn();
			void restart();
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif