#ifndef DBSH07_BLOCK_HPP
#define DBSH07_BLOCK_HPP

#include "enemy.hpp"
#include "animation.hpp"

class Block: public Enemy
{
public:
    Block(int x, int y, int width, int height, const std::string& filename, int hitCount);
    ~Block();
    void logic(Level* level);
    void draw(BITMAP *dest, int scrolly, unsigned int layer);
    bool drawInLayer(unsigned int layer) { return layer == Entity::ENEMY_LAYER; }
    bool isToBeDeleted();

protected:
    Animation* mAnimation;
	int mHitCount;
};

#endif