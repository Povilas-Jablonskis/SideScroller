#include "Subject.h"
#include "BaseGameObject.h"

namespace Engine
{
	void Subject::addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void Subject::removeObserver(Observer* observer)
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void Subject::notify(ObserverEvent _event, BaseGameObject* subject)
	{
		for (auto observer : observers)
		{
			observer->onNotify(_event, subject);
		}
	}
}