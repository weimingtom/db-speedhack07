#include <iostream>

#include "staticshooter.hpp"
#include "enemybullet.hpp"
#include "playerbullet.hpp"
#include "level.hpp"
#include "util.hpp"

StaticShooter::StaticShooter(int x, int y, FireDirection direction)
: Enemy(x, y, 16, 16, true),
  mIsToBeDeleted(false),
  mDirection(direction),
  mFrameCount(0),
  mHitCount(4)
{
    mAnimation = new Animation("turret.bmp");
}

StaticShooter::~StaticShooter()
{
    delete mAnimation;
}

void StaticShooter::logic(Level* level)
{
	mFrameCount++;
	if(mFrameCount%60 == 0)
	{
		//mIsTimeToFire = true;
		float xSpeed=0, ySpeed=0, direction=0;
		if(mDirection == RIGHT) { xSpeed = 1.5f; direction = M_PI/2.0f; }
		if(mDirection == LEFT) { xSpeed = -1.5f; direction = M_PI/2.0f; }
		if(mDirection == UP) { ySpeed = -1.5f; direction = 0.0f; }
		if(mDirection == DOWN) { ySpeed = 1.5f; direction = 0.0f; }

		level->addEntity(new EnemyBullet(getCenterX(), getCenterY(), xSpeed, ySpeed, 1, direction));
		//new PlayerBullet(
	}
}

void StaticShooter::handleCollision(Entity *other, Level *level)
{
	if(other->getType() == Entity::PLAYER_BULLET_TYPE)
	{
		int dx = getCenterX() - other->getCenterX();

		mHitCount--;
		mRenderAsHit = true;

		if(mHitCount <= 0)
		{
			mIsToBeDeleted = true;
			mCollidable = false; //do not collide while blinking

			level->spawnDebris(8, mX, mY, mW, mH);
		    level->spawnExplosions(10, mX, mY, mW, mH);
			level->addShakeAmount(100);
		}
	}


}

void StaticShooter::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	int frame = mRenderAsHit?2:(mFrameCount%20)<10?0:1;

	if(mDirection==UP||mDirection==DOWN) {
		mAnimation->drawRotatedFrame(dest, frame, getX(), getY() - scrolly, mDirection==UP?-64:64, false);
	}
	else
	{
		mAnimation->drawFrame(dest, frame, getX(), getY() - scrolly, mDirection==LEFT?true:false, false);
	}
	mRenderAsHit = false;
}

bool StaticShooter::isToBeDeleted()
{
    return mIsToBeDeleted && !mRenderAsHit;
}