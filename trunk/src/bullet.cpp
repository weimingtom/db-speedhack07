#include "bullet.hpp"
#include "exception.hpp"

Bullet::Bullet(unsigned int type)
:Entity(true),
 mType(type)
{
    if (type != Entity::ENEMY_BULLET_TYPE 
        && type != Entity::PLAYER_BULLET_TYPE)
    {
        throw DBSH07_EXCEPTION("Illegal bullet type!");
    }

}