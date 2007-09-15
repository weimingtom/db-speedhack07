#include "energyorb.hpp"
#include "level.hpp"

EnergyOrb::EnergyOrb(int x, int y)
: Entity(x, y, Level::BLOCK_SIZE, Level::BLOCK_SIZE, true),
mFrameCount(0)
{
    mAnimation = new Animation("energyorb.bmp");
}

EnergyOrb::~EnergyOrb()
{
    delete mAnimation;
}

void EnergyOrb::logic(Level* level)
{
    mFrameCount++;
}

void EnergyOrb::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
    mAnimation->drawFrame(dest, mFrameCount, getX(), getY() - scrolly);
}

bool EnergyOrb::isToBeDeleted()
{
    return false;
}

void EnergyOrb::handleCollision(Entity *other, Level *level)
{

}