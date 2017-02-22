#pragma once
#include "constdecl.h"
#include "sfwdraw.h"


class Instructions
{
	int font;

public:

	void init(int a_font);
	void draw();

	APP_State next();
};