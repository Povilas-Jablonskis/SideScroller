#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <functional>
#include <memory>
#include <algorithm>

#include "RenderObject.h"

#include "glm/gtc/type_ptr.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	class UIElementBase : public RenderObject
	{
		public:
			UIElementBase(float, float, glm::vec2, glm::vec4, glm::vec2);
			virtual void update(float);
			virtual void fixPosition(UIElementBase* = nullptr);
			virtual bool checkIfCollides(glm::vec2);
			virtual void checkOnHover(glm::vec2);
			virtual void checkOnMouseClick(glm::vec2);
			virtual void checkOnMouseRelease();
			std::function<void()> onHoverEnterFunc;
			std::function<void()> onHoverExitFunc;
			std::function<void()> onMouseClickFunc;
			std::function<void()> onMouseReleaseFunc;
		protected:
			float originalWidth;
			float originalHeight;
			glm::vec2 positionPercents;
			bool gotMousedHovered;
			bool gotMousedClicked;
	};
}
#endif