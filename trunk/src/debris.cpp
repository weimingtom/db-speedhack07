#include <allegro.h>

#include "debris.hpp"
#include "level.hpp"

Debris::Debris(int x, int y, float dx, float dy, const std::string& file, int animSpeed, int autoRemove) :
	Entity(false),
	mAnimation(file),
	mAnimSpeed(animSpeed),
	mAutoRemove(false),
	mToBeDeleted(false)
{
	mX = (float)x;
	mY = (float)y;
	mDX = dx;
	mDY = dy;
}

void Debris::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
}

void Debris::logic(Level *level)
{
}

bool Debris::isToBeDeleted()
{
	return mToBeDeleted;
}