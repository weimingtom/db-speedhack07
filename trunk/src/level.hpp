#ifndef DBSH07_LEVEL_HPP
#define DBSH07_LEVEL_HPP

#include "entity.hpp"
#include <vector>

class Level
{
public:
    Level();
    ~Level();
    void draw(BITMAP* dest);
    void logic();

protected:
    std::vector<Entity*> mEntities;
    std::vector<Entity*> mHibernatingEntities;
    std::vector<Entity*> mEnemyEntities;
    std::vector<Entity*> mPlayerEntities;
};

#endif