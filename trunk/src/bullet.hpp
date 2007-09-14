#ifndef DBSH07_BULLET_HPP
#define DBSH07_BULLET_HPP
#include <string>

#include "entity.hpp"

class Bullet: public Entity
{
public:
    Bullet(int x, int y, int w, int h, unsigned int type, float dx, float dy, const std::string& filename, int damage);
    unsigned int getType() { return mType; }
	void logic(Level* level);

protected:
    unsigned int mType;
	float mDx;
	float mDy;
	float mX, mY;
	//std::string& mFilename;
	int mDamage;
};

#endif