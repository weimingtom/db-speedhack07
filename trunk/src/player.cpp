#include "player.hpp"
#include "level.hpp"
#include "playerbullet.hpp"
#include "util.hpp"

#include <cmath>
#include <iostream>

Player::Player() :
	Entity(115, 20, 10, 20, true),
	mFrameCounter(0),
	mDX(0),
	mDY(0),
	mAirResistance(AIR_RESISTANCE_LOW),
	mShotPressed(false),
	mShotReleased(true),
	mShotBurstCounter(0),
	mNumPods(8),
	mPlayerAni("player.bmp", 1),
	mPodAni("pod.bmp", 3)
{
}

Player::~Player()
{

}

int Player::getPodOffset(int i)
{
	return (int)std::floor(getPodOffsetFloat(i) - 0.5f);
}

float Player::getPodOffsetFloat(int i)
{
	return std::cos(mFrameCounter * 0.3f / (mNumPods + 1) + (i * M_PI * 2.0f) / mNumPods) * 10.0f;
}

float Player::getPodDepth(int i)
{
	return std::sin(mFrameCounter * 0.3f / (mNumPods + 1) + i * M_PI * 2.0f / mNumPods);
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
			mPodAni.drawFrame(dest, frame, getCenterX() + getPodOffset(i) - 2, getY() - scrolly + 4);			
		}
	}

	mPlayerAni.drawFrame(dest, 0, getX() - 3, getY() - scrolly - 3);
	//rect(dest, getX(), getY() - scrolly, getX() + getWidth() - 1, getY() + getHeight() - 1  - scrolly, makecol(255, 255, 255));

	for (int i = 0; i < mNumPods; i++)
	{
		if (getPodDepth(i) >= 0.0f)
		{
			int frame = getPodDepth(i) < 0.4f ? 1 : 0;
			mPodAni.drawFrame(dest, frame, getCenterX() + getPodOffset(i) - 2, getY() - scrolly + 4);
		}
	}	
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

    int targetDY;
    if (level->isBrakePressed())
    {
       targetDY = 8;
    }
    else if (level->isBurnPressed())
    {
         targetDY = (64 / mAirResistance) * 2;
    }
    else
    {
	    targetDY = 64 / mAirResistance;
    }

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
		mShotBurstCounter = SHOT_BURST_LENGTH;
		mShotPressed = false;
	}

	if (mShotBurstCounter > 0)
	{
		float angle = std::atan2((float)(mTargetX - getCenterX()), (float)(mTargetY - getCenterY()) + 0.01f);		

		if (angle < -1.2) {
			angle = -1.2;
		}

		if (angle > 1.2) {
			angle = 1.2;
		}

		if (mShotBurstCounter % SHOT_FRAME_DELAY == 0)
		{
			int x = getCenterX();
			int y = mY + mW;

			float dx = sin(angle) * 10.0f;
			float dy = cos(angle) * 10.0f + (mDY / 8);
			PlayerBullet *bullet = new PlayerBullet(x, y, dx, dy, 2, angle);
			level->addEntity(bullet);
		}

		for (int i = 0; i < mNumPods; i++)
		{
			if (i % SHOT_POD_DELAY == mFrameCounter % SHOT_POD_DELAY)
			{			
				int x = getCenterX() + getPodOffset(i);
				int y = mY + 8;
				
				float a = angle + getPodOffsetFloat(i) / 30.0f;
				float dx = sin(a) * 8.0f;
				float dy = cos(a) * 8.0f + (mDY / 8);
				PlayerBullet *bullet = new PlayerBullet(x, y, dx, dy, 1, a);
				level->addEntity(bullet);
			}
		}

		mShotBurstCounter--;		
	}

	mFrameCounter++;
}

bool Player::isToBeDeleted()
{
    return false;
}

int Player::getSpeed()
{
	return mDY / 8;
}