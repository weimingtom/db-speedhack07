#include <iostream>
//#include <allegro.c>

#include "block.hpp"
#include "level.hpp"
#include "debris.hpp"
#include "util.hpp"


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
			//todo some explosions
			
			for (int i = 0; i < 3; i++)
			{
				Debris *d = new Debris(mX + rand() % mW,
									   mY + rand() % mH,
									   (frand() - 0.5f) * 5.0f,
									   (frand() - 0.2f) * 10.0f,
									   "debris.bmp", 2);

				level->addEntity(d);
			}

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
	} else
	{
		mAnimation->drawFrame(dest, mFrameCounter / 5, getX(), getY() - scrolly);
	}
}

bool Block::isToBeDeleted()
{
    return mToBeDeleted && !mIsHit;
}