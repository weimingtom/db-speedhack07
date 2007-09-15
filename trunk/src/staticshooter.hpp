#ifndef DBSH07_STATICSHOOTER_HPP
#define DBSH07_STATICSHOOTER_HPP

#include "enemy.hpp"
#include "animation.hpp"

class StaticShooter: public Enemy
{
public:
	enum FireDirection
    {
		UP,
		DOWN,
        LEFT,
		RIGHT
    };

    StaticShooter(int x, int y, FireDirection direction);
    ~StaticShooter();
    void logic(Level* level);
    void draw(BITMAP *dest, int scrolly, unsigned int layer);
    bool drawInLayer(unsigned int layer) { return layer == Entity::ENEMY_LAYER; }
    bool isToBeDeleted();
	void handleCollision(Entity *other, Level *level);
	unsigned int getType() { return Entity::ENEMY_TYPE; }



protected:
    Animation* mAnimation;
	int mFrameCount;
	int mHitCount;
	bool mIsToBeDeleted;
	bool mRenderAsHit;
	FireDirection mDirection;
	//bool mIsTimeToFire;
	
};

#endif