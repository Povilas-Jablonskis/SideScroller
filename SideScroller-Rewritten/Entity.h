#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"
#include "Subject.h"

namespace Engine
{
	class Entity : public BaseGameObject, public Subject
	{
		public:
			Entity(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, glm::vec2);
			void setFirstState(State);
			void setSecondState(State);
			inline void setIsDucking(bool boolean) { isDucking = boolean; }
			inline bool getIsDucking() const { return isDucking; }
			inline void setCanClimb(bool boolean) { canClimb = boolean; }
			inline bool getCanClimb() const { return canClimb; }
		protected:
			bool canClimb;
			bool isDucking;
	};
}
#endif