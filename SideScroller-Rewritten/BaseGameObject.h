#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "RenderObject.h"
#include "Addon.h"
#include "State.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Entity;
	class BaseGameObject : public RenderObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(float, float, glm::vec2, glm::vec2, glm::vec4);
			virtual bool update(float, glm::vec2);
			void addAnimation(std::string, std::shared_ptr<Animation>);
			std::shared_ptr<Animation> getAnimationByIndex(std::string);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; }
			inline float getVelocity(int index) { return velocity[index]; }
			inline glm::vec2 getVelocity() { return velocity; }
			std::function<void()> onDeath;
			inline void setNeedsToBeDeleted(bool boolean) { needsToBeDeleted = boolean; }
			inline bool getNeedsToBeDeleted() const { return needsToBeDeleted; }
			inline State getFirstState() const { return firstState; }
			void setFirstState(State);
			inline State getSecondState() const { return secondState; }
			void setSecondState(State);
			inline void setIsDucking(bool boolean) { isDucking = boolean; }
			inline bool getIsDucking() const { return isDucking; }
		protected:
			bool isDucking;
			State firstState;
			State secondState;
			bool needsToBeDeleted;
			std::vector<std::pair<std::string, std::shared_ptr<Animation>>> animations;
			glm::vec2 velocity;
	};
}
#endif