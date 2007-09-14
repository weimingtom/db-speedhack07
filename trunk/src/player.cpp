#include "player.hpp"
#include "level.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323
#endif

Player::Player() :
	Entity(115, 20, 10, 20, true),
	mFrameCounter(0),
	mDX(0),
	mDY(0),
	mAirResistance(AIR_RESISTANCE_LOW),
	mShotPressed(false),
	mShotReleased(true),
	mShotBurstCounter(0),
	mNumPods(2),
	mPlayerAni("player.bmp", 1),
	mPodAni("pod.bmp", 3)
{
}

Player::~Player()
{

}

int Player::getPodOffset(int i)
{
	return (int)(std::cos(mFrameCounter * 0.1f + i * M_PI * 2.0f / mNumPods) * 10.0f - 0.5f);
}

float Player::getPodDepth(int i)
{
	return std::sin(mFrameCounter * 0.1f + i * M_PI * 2.0f / mNumPods);
}

bool Player::drawInLayer(unsigned int layer)
{
	return layer == Entity::PLAYER_LAYER;
}

void Player::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	for (int i = 0; i < mNumPods; i++)
	{
		if (getPodDepth(i) < 0.0f)
		{
			int frame = getPodDepth(i) > -0.2f ? 1 : 2;
			mPodAni.drawFrame(dest, frame, getX() + getPodOffset(i) + 3, getY() - scrolly + 4);
		}
	}

	mPlayerAni.drawFrame(dest, 0, getX(), getY() - scrolly);
	//rect(dest, getX(), getY() - scrolly, getX() + getWidth() - 1, getY() + getHeight() - 1  - scrolly, makecol(255, 255, 255));

	for (int i = 0; i < mNumPods; i++)
	{
		if (getPodDepth(i) >= 0.0f)
		{
			int frame = getPodDepth(i) < 0.4f ? 1 : 0;
			mPodAni.drawFrame(dest, frame, getX() + getPodOffset(i) + 3, getY() - scrolly + 4);
		}
	}

	circle(dest, mTargetX, mTargetY - scrolly, 6, makecol(255, 255, 255));
	arc(dest, mTargetX, mTargetY - scrolly, itofix(scrolly * 3), itofix(scrolly * 3 + 100), 5, makecol(255, 0, 0));
	arc(dest, mTargetX, mTargetY - scrolly, itofix(scrolly * 3), itofix(scrolly * 3 + 100), 7, makecol(255, 0, 0));
}

void Player::logic(Level* level)
{
	bool leftPressed = key[KEY_A] || key[KEY_LEFT];
	bool rightPressed = key[KEY_D] || key[KEY_RIGHT];

	mTargetX = level->getMouseX();
	mTargetY = level->getMouseY();

	int maxDX = 64 / mAirResistance;

	if (leftPressed && !rightPressed && -mDX < maxDX)
	{
		if (mDX > -8)
		{
			mDX = -8;
		}
		else
		{
			mDX -= 2;
		}
	}
	else if (rightPressed && !leftPressed && mDX < maxDX)
	{
		if (mDX < 8)
		{
			mDX = 8;
		}
		else
		{
			mDX += 2;
		}
	}
	else 
	{
		if (mDX > 0)
		{
			mDX -= 4;
		}
		
		if (mDX < 0)
		{
			mDX += 4;
		}
	}

	if (mDX > maxDX) {
		mDX--;
	}

	if (mDX < -maxDX) {
		mDX++;
	}

	int targetDY = 64 / mAirResistance;

	if (mDY < targetDY)
	{
		mDY++;
	}
	else if (mDY > targetDY)
	{
		mDY--;
	}

	mX += mDX / 8;
	mY += mDY / 8;

	// Shooting
	if (level->isFirePressed())
	{
		if (mShotReleased)
		{
			mShotPressed = true;
			mShotReleased = false;
		}
	}
	else
	{
		mShotReleased = true;
	}
	
	if (mShotPressed && mShotBurstCounter <= 0)
	{
		mShotBurstCounter = 15;
		mShotPressed = false;
	}

	if (mShotBurstCounter > 0)
	{
		if (mShotBurstCounter % 2 == 1)
		{
			float dx = mTargetX - mX;
			float dy = mTargetY - mY;
			float l = std::sqrt(dx * dx + dy * dy);
			if (l > 0.0f)
			{
				dx /= l;
				dy /= l;
			}
			else
			{
				dx = 0.0f;
				dy = 1.0f;
			}
			dy += mDY / 8;

//			level->addEntity(shot);
		}

		mShotBurstCounter--;		
	}

	mFrameCounter++;
}

bool Player::isToBeDeleted()
{
    return false;
}