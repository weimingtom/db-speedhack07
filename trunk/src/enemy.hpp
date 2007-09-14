#ifndef DBSH07_ENEMY_HPP
#define DBSH07_ENEMY_HPP

#include "entity.hpp"

class Enemy: public Entity
{
public:
    Enemy(int x, int y, int width, int height);
    unsigned int getType() { return Entity::ENEMY_TYPE; }
};

#endif