#include <iostream>

#include "mine.hpp"
#include "level.hpp"
#include "resourcehandler.hpp"

Mine::Mine(int x, int y, bool isMagnetic)
: Enemy(x, y, 16, 16, true),
  mToBeDeleted(false),
  mHitCount(10),
  mIsMagnetic(isMagnetic)
{
    mAnimation = new Animation("mine.bmp");
	mRenderAsHit = false;
	mY = getY();
	mX = getX();
	mDy = 0;
	mDx = 0;
	mFrameCount = 0;

	mExplosionSample = ResourceHandler::getInstance()->getSample("explo2.wav");
}

Mine::~Mine()
{
    delete mAnimation;
}

void Mine::logic(Level* level)
{
	if(mIsMagnetic) {
		int playerDiffX = level->getPlayer()->getCenterX() - getX();
		int playerDiffY = level->getPlayer()->getCenterY() - getY();
		
		mDx += playerDiffX/5000.0f;
		mDy += playerDiffY/5000.0f;
	}

	float targetDy = 0.4;
	float dyDy = targetDy - mDy;
	float dxDx = -mDx;
	

	mY += mDy;
	mX += mDx;
	mFrameCount++;

	mDy += dyDy/20.0f;
	mDx += dxDx/20.0f;

	setY((int)mY);
	setX((int)mX);

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
		//spawnDebris(level, 1, mX, mY, mW, mH);
		if(mHitCount <= 0)
		{
			mToBeDeleted = true;
			mCollidable = false; //do not collide while blinking
		}
	} 
    else if (other->getType() == Entity::PLAYER_TYPE)
    {
        level->getPlayer()->kill();

		mToBeDeleted = true;
		mCollidable = false;
    }
    else if(other->isCollidable())
	{
		mCollidable = false;
		mToBeDeleted = true;
	}
	if(!mCollidable)
	{
		spawnDebris(level, 8, mX, mY, mW, mH);
        spawnExplosions(level, 10, mX, mY, mW, mH);
		level->addShakeAmount(100);
		int pan = (getCenterX() * 256) / 240;
		if (pan < 0)
		{
			pan = 0;
		} 
		else if (pan > 255)
		{
			pan = 255;
		}

		play_sample(mExplosionSample, 150, pan, 750, 0);
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