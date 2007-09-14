#include <cmath>

#include "bullet.hpp"
#include "exception.hpp"

Bullet::Bullet(int x, int y, int w, int h, unsigned int type, float angle, float speed, const std::string& filename, int damage)
:Entity(x, y, w, h, true),
 mType(type)
{
    if (type != Entity::ENEMY_BULLET_TYPE 
        && type != Entity::PLAYER_BULLET_TYPE)
    {
        throw DBSH07_EXCEPTION("Illegal bullet type!");
    }
	mType = type;
	mAngle = angle;
	mSpeed = speed;
	mX = x;
	mY = y;
	//mFileName = fileName;
	mDamage = damage;
}

void Bullet::logic(Level* level)
{
	mX += std::sin(mAngle) * mSpeed;
	mY += std::cos(mAngle) * mSpeed;

	setX((int)mX);
	setY((int)mY);
}
