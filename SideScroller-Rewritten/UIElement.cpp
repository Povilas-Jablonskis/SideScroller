#include "UIElement.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement(float _width, float _height, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<UIElement> _parent, glm::vec2 _positionPerc)
		: UIElementBase(_width, _height, _position, _color, _positionPerc), parentMenu(_parent)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::hideMain(bool hideEverything)
	{
		if (hideEverything)
		{
			changeColor(0.0f, 3);

			for (auto element : elements)
			{
				element->hideMain();
			}
		}

		for (auto text : texts)
		{
			text->changeColor(0.0f, 3);
		}
	}

	void UIElement::showMain(bool showEverything)
	{
		if (showEverything)
			changeColor(1.0f, 3);

		for (auto text : texts)
		{
			text->changeColor(1.0f, 3);
		}
	}

	void UIElement::onHoverEnterFuncDefaults()
	{

	}

	void UIElement::onHoverExitFuncDefaults()
	{

	}

	void UIElement::fixPosition(UIElementBase* _parent)
	{
		UIElementBase::fixPosition(_parent);

		for (auto text : texts)
		{
			text->fixPosition(this);
		}

		for (auto element : elements)
		{
			element->fixPosition(this);
		}
	}

	void UIElement::GetAllChildrenElements(std::vector<std::shared_ptr<UIElement>>* out)
	{
		auto tempList = getElements();

		for (std::vector<std::shared_ptr<UIElement>>::iterator it = tempList->begin(); it != tempList->end(); ++it)
		{
			out->push_back(*it);
			(*it)->GetAllChildrenElements(out);
		}
	}

	void UIElement::GetAllChildrenTexts(std::vector<std::shared_ptr<Text>>* out)
	{
		auto tempList = getElements();
		auto tempTextList = getTexts();

		out->insert(out->end(), tempTextList->begin(), tempTextList->end());

		for (std::vector<std::shared_ptr<UIElement>>::iterator it = tempList->begin(); it != tempList->end(); ++it)
			(*it)->GetAllChildrenTexts(out);
	}

	void UIElement::checkIfMouseHoverThis(glm::vec2 lastMousePosition)
	{
		UIElementBase::checkIfMouseHoverThis(lastMousePosition);

		for (auto text : texts)
		{
			text->checkIfMouseHoverThis(lastMousePosition);
		}

		for (auto element : elements)
		{
			element->checkIfMouseHoverThis(lastMousePosition);
		}
	}

	void UIElement::checkForMouseClickOnThis(bool leftMouseState, bool lastLeftMouseState, glm::vec2 lastMousePosition)
	{
		UIElementBase::checkForMouseClickOnThis(leftMouseState, lastLeftMouseState, lastMousePosition);

		for (auto text : texts)
		{
			text->checkForMouseClickOnThis(leftMouseState, lastLeftMouseState, lastMousePosition);
		}

		for (auto element : elements)
		{
			element->checkForMouseClickOnThis(leftMouseState, lastLeftMouseState, lastMousePosition);
		}
	}

	void UIElement::update(float dt)
	{
		UIElementBase::update(dt);

		for (auto text : texts)
		{
			text->update(dt);
		}

		for (auto element : elements)
		{
			element->update(dt);
		}
	}
}