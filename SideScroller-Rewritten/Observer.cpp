#include "Observer.h"
#include "BaseGameObject.h"

namespace Engine
{
	Observer::Observer()
	{
		onNotify = [](ObserverEvent _event, BaseGameObject* subject)
		{

		};
	};
}