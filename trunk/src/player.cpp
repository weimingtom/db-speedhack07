#include "player.hpp"
#include "level.hpp"
#include "playerbullet.hpp"
#include "util.hpp"
#include "resourcehandler.hpp"

#include <cmath>
#include <iostream>

Player::Player() :
	Entity(115, 20, 10, 20, true),
	mPlayerAni("player.bmp", 1),
	mPodAni("pod.bmp", 3)
{
	mShotSample = ResourceHandler::getInstance()->getSample("explo1.wav");
    reset();
}

Player::~Player()
{
}

void Player::setState(unsigned int state)
{
    mState = state;
}

void Player::reset()
{
    mX = 115;
    mY = 20;
    mFrameCounter = 0;
    mDX = 0;
    mDY = 0;
    mShotPressed = false;
    mShotReleased = true;
    mShotBurstCounter = 0;
    mNumPods = 10;
    mState = NEW;
    mImortalButtonPressed = false;
}

void Player::kill()
{
    if (mState == NORMAL)
    {
        mState = KILLED;
        mFrameCounter = 0;
    }
}

unsigned int Player::getState()
{
    return mState;
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

    if (mState == DEAD || mState == NEW || mState == IMORTAL)
    {
        if (mFrameCounter % 4 < 2)
        {
	        mPlayerAni.drawFrame(dest, 0, getX() - 3, getY() - scrolly - 3);
        }
    }
    else
    {
        mPlayerAni.drawFrame(dest, 0, getX() - 3, getY() - scrolly - 3);
    }
	//rect(dest, getX(), getY() - scrolly, getX() + getWidth() - 1, getY() + getHeight() - 1  - scrolly, makecol(255, 255, 255));

	for (int i = 0; i < mNumPods; i++)
	{
		if (getPodDepth(i) >= 0.0f)
		{
			int frame = getPodDepth(i) < 0.4f ? 1 : 0;
            
            if (mState == DEAD || mState == NEW || mState == IMORTAL)
            {
                if (mFrameCounter % 4 < 2)
                {
			        mPodAni.drawFrame(dest, frame, getCenterX() + getPodOffset(i) - 2, getY() - scrolly + 4);
		        }
            }
            else
            {
                mPodAni.drawFrame(dest, frame, getCenterX() + getPodOffset(i) - 2, getY() - scrolly + 4);
            }
        }
	}	
}

void Player::logic(Level* level)
{
    if (key[KEY_I] 
        && !mImortalButtonPressed 
        && mState == IMORTAL)
    {
        mState = NORMAL;
    }
    else if (key[KEY_I] 
        && !mImortalButtonPressed 
        && (mState == NEW || mState == NORMAL))
    {
        mState = IMORTAL;
    }

    mImortalButtonPressed = key[KEY_I];

    if (mState == NEW)
    {
        movementLogic(level);

        if (mFrameCounter > 100)
        {
            mState = NORMAL;
        }
    }
    else if (mState == NORMAL || mState == IMORTAL)
    {
        movementLogic(level);
    }
    else if (mState == KILLED)
    {
		int targetDY = 64 / level->getAirResistance();

        if (mDY < targetDY)
        {
	        mDY++;
        }
        else if (mDY > targetDY)
        {
	        mDY--;
        }

        mY += mDY / 8;

        if (mFrameCounter % 10 < 2)
        {
            level->spawnDebris(1, mX, mY, mW, mH);
            level->spawnExplosions(1, mX, mY, mW, mH);
        }

        if (mFrameCounter > 100)
        {
            mState = DEAD;
            level->spawnNewPlayer();
		} else if(mFrameCounter == 0)
		{
			level->addShakeAmount(600);
		}
    }

	mFrameCounter++;
}

void Player::movementLogic(Level* level)
{
	bool leftPressed = key[KEY_A] || key[KEY_LEFT];
	bool rightPressed = key[KEY_D] || key[KEY_RIGHT];

	mTargetX = level->getMouseX();
	mTargetY = level->getMouseY();

	int maxDX = 64 / level->getAirResistance();

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

	if (mX < 0) {
		mX = 0;
		if (mDX < 0) {
			mDX = 0;
		}
	}

	if (mX + mW >= 240) {
		mX = 240 - mW;
		if (mDX > 0) {
			mDX = 0;
		}
	}

    int targetDY;
    if (level->isBrakePressed())
    {
       targetDY = 8;
    }
    else if (level->isBurnPressed())
    {
		targetDY = (64 / level->getAirResistance()) * 2;
    }
    else
    {
	    targetDY = 64 / level->getAirResistance();
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

		if (angle < -1.4) {
			angle = -1.4;
		}

		if (angle > 1.4) {
			angle = 1.4;
		}

		if (mShotBurstCounter % SHOT_FRAME_DELAY == 0)
		{
			int x = getCenterX();
			int y = mY + mW;

			float dx = sin(angle) * 10.0f;
			float dy = cos(angle) * 10.0f + (mDY / 8);
			PlayerBullet *bullet = new PlayerBullet(x, y, dx, dy, 2, angle);
			level->addEntity(bullet);			
			play_sample(mShotSample, 50, 128, 1000, 0);
		}		

		for (int i = 0; i < mNumPods; i++)
		{
			if (i % SHOT_POD_DELAY == mFrameCounter % SHOT_POD_DELAY)
			{			
				int x = getCenterX() + getPodOffset(i);
				int y = mY + 8;
				
				float a = angle + getPodOffsetFloat(i) / 30.0f;
				a = getPodOffsetFloat(i) / 50.0f;
				float dx = sin(a) * 8.0f;
				float dy = cos(a) * 8.0f + (mDY / 8);
				PlayerBullet *bullet = new PlayerBullet(x, y, dx, dy, 1, a);
				level->addEntity(bullet);
			}
		}

		mShotBurstCounter--;		
	}
}

bool Player::isToBeDeleted()
{
    return false;
}

int Player::getSpeed()
{
	return mDY / 8;
}