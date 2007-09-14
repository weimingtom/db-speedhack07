#ifndef DBSH07_PLAYER_HPP
#define DBSH07_PLAYER_HPP

#include "entity.hpp"
#include "animation.hpp"

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

	int getPodOffset(int i);
	float getPodOffsetFloat(int i);
	float getPodDepth(int i);

	int getSpeed();

	static const int AIR_RESISTANCE_LOW = 2;
	static const int AIR_RESISTANCE_MEDIUM = 3;
	static const int AIR_RESISTANCE_HIGH = 6;
	static const int SHOT_BURST_LENGTH = 15;
	static const int SHOT_FRAME_DELAY = 3;

private:
	int mFrameCounter;
	int mDX, mDY;
	int mAirResistance;
	int mTargetX, mTargetY;
	bool mShotPressed;
	bool mShotReleased;
	int mShotBurstCounter;
	int mNumPods;

	Animation mPlayerAni;
	Animation mPodAni;
};

#endif