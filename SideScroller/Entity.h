#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"
#include "Subject.h"
#include "InputManager.h"

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
			void resetInput();
			void fixInput();
			void updateInput(std::shared_ptr<InputManager>);
			inline bool getKey(char key) const { return pressedkeys[key]; }
			inline bool getKey(int key) const { return pressedkeys[key]; }
			inline void setKey(char key, bool boolean) { pressedkeys[key] = boolean; }
			inline void setKey(int key, bool boolean) { pressedkeys[key] = boolean; }
		protected:
			static const int pressedKeyCount = 256;
			bool* pressedkeys;
			bool isDucking;
	};
}
#endif