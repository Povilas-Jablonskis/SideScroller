#ifndef _timerH
#define _timerH

namespace Engine
{
	class Timer
	{
		public:
			template <class T>
			static void windowsTimer(const T& callback, unsigned int timeInMs)
			{
				concurrency::task_completion_event<void> tce;
				auto call = new concurrency::call<int>(
					[callback, tce](int)
				{
					callback();
					tce.set();
				});

				auto timer = new concurrency::timer<int>(timeInMs, 0, call, false);
				concurrency::task<void> event_set(tce);
				event_set.then([timer, call]()
				{
					delete call;
					delete timer;
				});

				timer->start();
			}
	};
}
#endif