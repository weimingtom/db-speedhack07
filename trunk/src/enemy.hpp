#ifndef DBSH07_ENEMY_HPP
#define DBSH07_ENEMY_HPP

#include "entity.hpp"

class Enemy: public Entity
{
public:
    Enemy();
    unsigned int getEntityType() { return Entity::ENEMY_TYPE; }
};

#endif