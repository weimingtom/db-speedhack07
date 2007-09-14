#ifndef DBSH07_LEVEL_HPP
#define DBSH07_LEVEL_HPP

#include "entity.hpp"
#include "player.hpp"
#include <list>

class Level
{
public:
    Level(const std::string& filename);
    ~Level();
    void draw(BITMAP* dest);
    void logic();
    void load(const std::string& filename);
    void addEntity(Entity* entity);

protected:
    std::list<Entity*> mEntities;
    std::list<Entity*> mHibernatingEntities;
    std::list<Entity*> mEnemyEntities;
    std::list<Entity*> mEnemyBulletEntities;
    std::list<Entity*> mPlayerEntities;
    std::list<Entity*> mPlayerBulletEntities;
    int mGameScrollY;
    int mBackgroundScrollY;
    Player* mPlayer;
    Entity* mBackground;
};

#endif