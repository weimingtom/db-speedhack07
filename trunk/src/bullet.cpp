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
	//fill_circle(
	circlefill(dest, getX(), getY()-scrolly, 4, makecol(255, 200, 0));
	//mAnimation->drawFrame(dest, 0, getX(), getY() - scrolly);
}


bool Bullet::isToBeDeleted()
{
    return false;
}