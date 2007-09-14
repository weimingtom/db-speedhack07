#ifndef DBSH07_PLAYER_HPP
#define DBSH07_PLAYER_HPP

#include "entity.hpp"

class Player: public Entity
{
public:
    Player();
    ~Player();
    void draw(BITMAP *dest, int scrolly, unsigned int layer);
    void logic(Level* level);
    bool isToBeDeleted();
    unsigned int getType() { return Entity::PLAYER_TYPE; }
	virtual bool drawInLayer(unsigned int layer);

	static const int AIR_RESISTANCE_LOW = 2;
	static const int AIR_RESISTANCE_MEDIUM = 3;
	static const int AIR_RESISTANCE_HIGH = 6;

private:
	int mDX, mDY;
	int mAirResistance;
	int mTargetX, mTargetY;
};

#endif