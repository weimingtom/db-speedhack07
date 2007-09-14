#include <cmath>
#include <allegro.h>

#include "bullet.hpp"
#include "exception.hpp"


Bullet::Bullet(int x, int y, int w, int h, unsigned int type, float dx, float dy, const std::string& filename, int damage)
:Entity(x, y, w, h, true),
 mType(type)
{
    if (type != Entity::ENEMY_BULLET_TYPE 
        && type != Entity::PLAYER_BULLET_TYPE)
    {
        throw DBSH07_EXCEPTION("Illegal bullet type!");
    }
	mType = type;
	mDx = dx;
	mDy = dy;
	mX = x;
	mY = y;
	//mFileName = fileName;
	mDamage = damage;
}

void Bullet::logic(Level* level)
{
	mX += mDx; //std::sin(mAngle) * mSpeed;
	mY += mDy; //std::cos(mAngle) * mSpeed;

	setX((int)mX);
	setY((int)mY);
}

void Bullet::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	circlefill(dest, getX(), getY()-scrolly, 1, makecol(255, 200, 0));
	int y = getY() - scrolly;
	mIsToBeDeleted = getX() < 0 || getX() > 240 || y < 0 || y > 240;
}


bool Bullet::isToBeDeleted()
{
    return mIsToBeDeleted;
}