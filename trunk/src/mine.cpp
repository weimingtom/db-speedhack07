#include <iostream>

#include "mine.hpp"
#include "level.hpp"

Mine::Mine(int x, int y, bool isMagnetic)
: Enemy(x, y, 16, 16, true),
  mToBeDeleted(false),
  mHitCount(10)
{
    mAnimation = new Animation("mine.bmp");
	mRenderAsHit = false;
	mY = getY();
	mX = getX();
	mDy = 0;
	mDx = 0;
	mFrameCount = 0;
}

Mine::~Mine()
{
    delete mAnimation;
}

void Mine::logic(Level* level)
{
	float targetDy = 0.4;
	float dyDy = targetDy - mDy;
	float dxDx = -mDx;
	
	setY((int)mY);
	setX((int)mX);
	mY += mDy;
	mX += mDx;
	mFrameCount++;

	mDy += dyDy/20.0f;
	mDx += dxDx/20.0f;

	if((getY() - level->getScrollY()) + getHeight() < 0)
	{
		mToBeDeleted = true;
	}
}

void Mine::handleCollision(Entity *other, Level *level)
{
	
	if(other->getType() == Entity::PLAYER_BULLET_TYPE)
	{
		int dx = getCenterX() - other->getCenterX();
		//int dy = getCenterY() - other->getCenterY();

		mHitCount--;
		mRenderAsHit = true;
		mDy += 1.0;
		//std::cout << "dx: " << dx << std::endl;
		mDx += dx / 20.0f;

		if(mHitCount <= 0)
		{
			//todo some explosions
			mToBeDeleted = true;
			mCollidable = false; //do not collide while blinking
		}
	} else if(other->isCollidable())
	{
		mToBeDeleted = true;
	}

}

void Mine::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	mAnimation->drawFrame(dest, mRenderAsHit?2:(mFrameCount%10)<5?0:1, getX(), getY() - scrolly);
	mRenderAsHit = false;
}

bool Mine::isToBeDeleted()
{
    return mToBeDeleted && !mRenderAsHit;
}