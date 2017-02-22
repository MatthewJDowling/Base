#pragma once
#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"

class MenuState : public BaseState
{
	enum APP_State
	{
		ENTER_GAME,
		GAME,
		ENTER_SPLASH,
		SPLASH,
		INSTRUCTION,
		cats,
		dragons,
		catdragons,
		dragoncats
	};
};