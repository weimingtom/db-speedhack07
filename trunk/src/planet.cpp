#include "planet.hpp"
#include "level.hpp"

Planet::Planet() :
	Entity(false),
	mAnimation("planet.bmp", 1),
	mLevelLength(0)
{
}

void Planet::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	if (mLevelLength <= 0) {
		return;
	}

	int y = (mAnimation.getFrameHeight() * scrolly) / (mLevelLength - 240);

	if (y > mAnimation.getFrameHeight())
	{
		y = mAnimation.getFrameHeight();
	}

	mAnimation.drawFrame(dest, 0, 0, 240 - y);
}

void Planet::logic(Level *level)
{
	mLevelLength = level->getLevelLength();
}
