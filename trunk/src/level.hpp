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

	int getMouseX();
	int getMouseY();

    static const int SPACE_MOTIF = 0;
    static const int SKY_MOTIF = 1;
    static const int WATER_MOTIF = 2;
    static const int CAVE_MOTIF = 3;

protected:
	void updateScrolling();
    void checkCollision(std::list<Entity*>& list1, std::list<Entity*>& list2);

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
    int mMotif;
};

#endif