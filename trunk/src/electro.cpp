#include "electro.hpp"

Electro::Electro(int x, int y) :
	Entity(x, y, 16, 16, true),
	mFrameCounter(0),
	mAnimation("electro.bmp")
{
}

void Electro::logic(Level *level)
{
	mFrameCounter++;
}

void Electro::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	if (isOn())
	{
		mAnimation.drawFrame(dest, mFrameCounter / 3, mX, mY - scrolly);
	}
}

bool Electro::isOn()
{
	return (mFrameCounter & 32) != 0;
}

bool Electro::isCollidable()
{
	return isOn();
}
