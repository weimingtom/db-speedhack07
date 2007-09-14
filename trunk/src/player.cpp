#include "player.hpp"

Player::Player() :
	Entity(115, 0, 10, 20, true),
	mDX(0),
	mDY(0)
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
	if (key[KEY_A] || key[KEY_LEFT]) {
		mX--;
	}

	if (key[KEY_D] || key[KEY_RIGHT]) {
		mX++;
	}
}

bool Player::isToBeDeleted()
{
    return false;
}