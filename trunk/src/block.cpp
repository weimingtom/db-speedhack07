#include "block.hpp"

Block::Block(int x, int y, int width, int height, const std::string& filename, int hitCount)
: Enemy(x, y, width, height, hitCount)
{
    mAnimation = new Animation(filename);
	
}

Block::~Block()
{
    delete mAnimation;
}

void Block::logic(Level* level)
{

}

void Block::handleCollision(Entity *other, Level *level)
{
	
}

void Block::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
    mAnimation->drawFrame(dest, 0, getX(), getY() - scrolly);
}

bool Block::isToBeDeleted()
{
    return false;
}