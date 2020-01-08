#ifndef _observerH
#define _observerH

#include <functional>

#include "ObserverEvent.h"

namespace Engine
{
	class BaseGameObject;
	class Observer
	{
		public:
			Observer();
			std::function<void(ObserverEvent, BaseGameObject*)> onNotify;
	};
}
#endif