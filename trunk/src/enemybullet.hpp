#ifndef DBSH07_ENEMYBULLET_HPP
#define DBSH07_ENEMYBULLET_HPP
#include <string>

#include "bullet.hpp"
#include "animation.hpp"

class EnemyBullet: public Bullet
{
public:
	EnemyBullet(int x, int y, float dx, float dy, unsigned int damage, float drawingAngle);

	void EnemyBullet::draw(BITMAP *dest, int scrolly, unsigned int layer);
protected:
	Animation mAnimation;
	int frame;
	bool vflip;
};

#endif