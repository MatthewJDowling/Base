#include "Splash.h"
#include "sfwdraw.h"
#include <cstdio>
#include "sfwdraw.h"

using namespace sfw;
void Splash::init(int a_font)
{
	font = a_font;
}

void Splash::draw()
{

	char buffer[64];
	sprintf_s(buffer, "ENTER to play.");
	sfw::drawString(font, buffer, 100, 200, 20, 20);
	sprintf_s(buffer, "BackSpace for Controls");
	sfw::drawString(font, buffer, 100, 150, 20, 20);
	sprintf_s(buffer, "Zombie Attack");
	sfw::drawString(font, buffer, 320, 550, 20, 20);

}

APP_State Splash::next()
{
	if (getKey(KEY_ENTER))
	{
		return ENTER_GAME;
	}
	if (getKey(KEY_BACKSPACE))
	{
		return INSTRUCTION;
	}
	else
	{
		return SPLASH;
	}
}