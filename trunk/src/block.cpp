#include <iostream>

#include "block.hpp"
#include "level.hpp"



Block::Block(int x, int y, int width, int height, const std::string& filename, int hitCount)
: Entity(x, y, width, height, true),
  mToBeDeleted(false),
  mHitCount(hitCount),
  mFrameCounter(0)
{
    mAnimation = new Animation(filename);
	mIsHit = false;
}

Block::~Block()
{
    delete mAnimation;
}

void Block::logic(Level* level)
{
	if((getY() - level->getScrollY()) + getHeight() < 0)
	{
		mToBeDeleted = true;
	}

    mFrameCounter++;
}

void Block::handleCollision(Entity *other, Level *level)
{
    if (other->getType() == Entity::PLAYER_TYPE)
    {
        level->getPlayer()->kill();

        // Only vulnerable block should not be deleted
        // and spawn debri.
        if (mHitCount - 1 >= 0)
        {
            spawnDebris(level, 3, mX, mY, mW, mH);
		    mToBeDeleted = true;
        }

        return;
    }

    // Invulnerable block should not be effected by other entities
    // then the player
    if (mHitCount - 1 < 0)
    {
        return;
    }

	if(other->getType() == Entity::PLAYER_BULLET_TYPE)
	{
		mHitCount--;
		mIsHit = true;
		if(mHitCount <= 0)
		{
			spawnDebris(level, 3, mX, mY, mW, mH);
			mToBeDeleted = true;
		}
	}
}

void Block::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	if(mIsHit)
	{
		int x2 = getX() + getWidth();
		int y2 = getY()-scrolly + getHeight();
		rectfill(dest, getX(), getY()-scrolly, x2, y2, makecol(255, 255, 255));
		mIsHit = false;
	} 
	else
	{
		mAnimation->drawFrame(dest, mFrameCounter / 5, getX(), getY() - scrolly);
	}
}

bool Block::isToBeDeleted()
{
    return mToBeDeleted && !mIsHit;
}