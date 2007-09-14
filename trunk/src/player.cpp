#include "player.hpp"

Player::Player() :
	Entity(115, 20, 10, 20, true),
	mDX(0),
	mDY(0),
	mAirResistance(AIR_RESISTANCE_LOW)
{
}

Player::~Player()
{

}

bool Player::drawInLayer(unsigned int layer)
{
	return layer == Entity::PLAYER_LAYER;
}

void Player::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	rectfill(dest, getX(), getY() - scrolly, getX() + getWidth() - 1, getY() + getHeight() - 1 - scrolly, makecol(128, 128, 128));
	rect(dest, getX(), getY() - scrolly, getX() + getWidth() - 1, getY() + getHeight() - 1  - scrolly, makecol(255, 255, 255));
}

void Player::logic(Level* level)
{
	bool leftPressed = key[KEY_A] || key[KEY_LEFT];
	bool rightPressed = key[KEY_D] || key[KEY_RIGHT];

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
}

bool Player::isToBeDeleted()
{
    return false;
}