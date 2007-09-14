#ifndef DBSH07_BULLET_HPP
#define DBSH07_BULLET_HPP

#include "entity.hpp"

class Bullet: public Entity
{
public:
    Bullet(unsigned int type);
    unsigned int getType() { return mType; }
protected:
    unsigned int mType;
};

#endif