#include "RenderObject.h"


namespace Engine
{
	RenderObject::RenderObject(float _width, float _height, glm::vec2 _position, glm::vec4 _color)
		: width(_width), height(_height), position(_position), color(_color), animation(nullptr), animComplete(false), animTimer(0.0f), currentFrame(0), rotationAngle(0.0f), rotationAxis(glm::vec3(0.0, 0.0, 1.0))
	{
		
	}

	void RenderObject::updateAnimation(float dt)
	{
		if (animation == nullptr) return;

		auto sprites = animation->getAnimation();

		if (sprites.size() > 1)
		{
			animTimer += dt;
			if (animTimer > animation->getDelay())
			{
				animTimer = 0;
				currentFrame++;
				if (currentFrame >= sprites.size())
				{
					if (animation->getLoopStatus())
						currentFrame = 0;
					else
					{
						currentFrame = (sprites.size() - 1);
						animComplete = true;
					}
				}
			}
		}
	}

	void RenderObject::applyAnimation(std::shared_ptr<Animation> _animation)
	{
		if (_animation == nullptr || _animation == animation) return;

		animation = _animation;
		animTimer = 0.0;
		animComplete = false;
		currentFrame = 0;
	}		

	float RenderObject::getSize(int index) const
	{
		switch (index)
		{
			case 0:
				return width;
			case 1:
				return height;
			default:
				return NULL;
		}
	}

	void RenderObject::setSize(int index, float value)
	{
		switch (index)
		{
			case 0:
			{
				width = value;
				break;
			}
			case 1:
			{
				height = value;
				break;
			}
		}
	}
}