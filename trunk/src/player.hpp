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
private:
	int mDX, mDY;
};

#endif