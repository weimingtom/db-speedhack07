#ifndef DBSH07_LEVEL_HPP
#define DBSH07_LEVEL_HPP

#include "entity.hpp"
#include <list>

class Level
{
public:
    Level();
    ~Level();
    void draw(BITMAP* dest);
    void logic();

protected:
    std::list<Entity*> mEntities;
    std::list<Entity*> mHibernatingEntities;
    std::list<Entity*> mEnemyEntities;
    std::list<Entity*> mEnemyBulletsEntities;
    std::list<Entity*> mPlayerEntities;
    std::list<Entity*> mPlayerBulletsEntities;
    int mScrollY;
};

#endif