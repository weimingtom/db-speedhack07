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
		int frame = mFrameCounter / 3;
		bool vflip = ((frame / 3) & 1) != 0;
		bool hflip = ((frame / 6) & 1) != 0;
		mAnimation.drawFrame(dest, frame, mX, mY - scrolly, hflip, vflip);
	}
}
bool Electro::killsPlayer()
{
	return isOn();
}

bool Electro::isOn()
{
	return (mFrameCounter & 32) != 0;
}

bool Electro::isCollidable()
{
	return isOn();
}
