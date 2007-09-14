#ifndef DBSH07_LEVEL_HPP
#define DBSH07_LEVEL_HPP

#include "entity.hpp"
#include <list>

class Level
{
public:
    Level(const std::string& filename);
    ~Level();
    void draw(BITMAP* dest);
    void logic();
    void load(const std::string& filename);

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