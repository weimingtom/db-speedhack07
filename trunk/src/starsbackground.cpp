#include "starsbackground.hpp"

StarsBackground::StarsBackground()
: Entity(false)
{
    for (unsigned int i = 0; i < 100; i++)
    {
        mStars.push_back(Star(rand() % 240, rand() % 240));
    }
}

StarsBackground::~StarsBackground()
{

}

void StarsBackground::logic(Level* level)
{

}

void StarsBackground::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
    for (unsigned int i = 0; i < mStars.size(); i++)
    {
        if (scrolly > mStars[i].y + 240)
        {
        }
        putpixel(dest, mStars[i].x, mStars[i].y + scrolly, makecol(255, 255, 255));
    }
}