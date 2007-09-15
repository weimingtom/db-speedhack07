#include <cmath>
#include "skybackground.hpp"
#include "resourcehandler.hpp"
#include "level.hpp"
#include "util.hpp"

SkyBackground::SkyBackground() :
	Entity(false),
	mLevelLength(0),
	mFrameCounter(0)
{
	mBackgroundColors = ResourceHandler::getInstance()->getBitmap("skybackground.bmp");

	for (int i = 0; i < 100; i++)
	{
		float x = frand() * 240 * 20;
		float y = frand() * 240 * 20;
		float z = frand() * 15 + 5;
        float dy = frand() * 5 + 1;
		mParticles.push_back(Particle(x, y, z, dy));
	}	
}

SkyBackground::~SkyBackground()
{
}

void SkyBackground::logic(Level* level)
{
	mLevelLength = level->getLevelLength();

	for (unsigned int i = 0; i < mParticles.size(); i++) 
    {
		mParticles[i].y += mParticles[i].dy;
	}

	mFrameCounter++;
}

void SkyBackground::draw(BITMAP *dest, int scrolly, unsigned int layer)
{
	float depth = scrolly / (mLevelLength - 240.0f);

	int c = (int)(depth * 16.0f);
	float t = depth * 16.0f - c;
	int bgColor;

	if (c < 0)
	{
		bgColor = getpixel(mBackgroundColors, 0, 0);
	}
	else if (c >= 15)
	{
		bgColor = getpixel(mBackgroundColors, 0, 15);
	}
	else
	{
		int c1 = getpixel(mBackgroundColors, 0, c);
		int c2 = getpixel(mBackgroundColors, 0, c + 1);
		int r = int(getr(c1) * (1.0f - t) + getr(c2) * t);
		int g = int(getg(c1) * (1.0f - t) + getg(c2) * t);
		int b = int(getb(c1) * (1.0f - t) + getb(c2) * t);
		bgColor = makecol(r, g, b);
	}

	clear_to_color(dest, bgColor);

	for (unsigned int i = 0; i < mParticles.size(); i++)
	{
		int x = mod(int(mParticles[i].x / mParticles[i].z), 240);
		int y = mod(int((mParticles[i].y - scrolly) / mParticles[i].z), 240);

		int c = (i * 100) / mParticles.size();

		putpixel(dest, x, y, makecol(100 + c, 120 + c, 150 + c));
	}
}