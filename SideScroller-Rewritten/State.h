#ifndef _stateH
#define _stateH

namespace Engine
{
	enum State
	{
		STATE_IDLE,
		STATE_JUMPING,
		STATE_WALKINGRIGHT,
		STATE_WALKINGLEFT,
		STATE_FALLING,
		STATE_CLIMBING
	};
}
#endif