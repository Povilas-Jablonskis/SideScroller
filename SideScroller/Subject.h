#ifndef _subjectH
#define _subjectH

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

#include "Observer.h"

namespace Engine
{
	class BaseGameObject;
	class Subject
	{
		public:
			void addObserver(Observer*);
			void removeObserver(Observer*);
			void notify(ObserverEvent, BaseGameObject*);
		private:
			std::vector<Observer*> observers;
	};
}
#endif