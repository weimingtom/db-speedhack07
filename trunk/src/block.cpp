#include "block.hpp"

Block::Block(int x, int y, int width, int height, const std::string& filename, int hitCount)
: Enemy(x, y, width, height)
{
    mAnimation = new Animation(filename);
	mHitCount = hitCount;
}

Block::~Block()
{
    delete mAnimation;
}

void Block::logic(Level* level)
{

}

void Block::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
    mAnimation->drawFrame(dest, 0, getX(), getY());
}

bool Block::isToBeDeleted()
{
    return false;
}