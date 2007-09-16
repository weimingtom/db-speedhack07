#ifndef DBSH07_POINTSUMMARY_HPP
#define DBSH07_POINTSUMMARY_HPP

#include "guichan.hpp"
class PointSummary
{
public:
    PointSummary(gcn::Container* mTop, unsigned int blocks, unsigned int enemies, unsigned int orbs, unsigned int mTime);
    ~PointSummary();
	void logic();
	bool isDone();

protected:
	void initGui(gcn::Container* mTop);

	unsigned int mOrbs;
	unsigned int mEnemies;
	unsigned int mBlocks;
	unsigned int mTime;
	unsigned int mState;

	unsigned int mOrbPoints;
	unsigned int mEnemyPoints;
	unsigned int mBlockPoints;
	unsigned int mTimePoints;
	unsigned int mTotal;

	unsigned int mFrameCounter;

//point count gui
	gcn::Label* mBlocksDestroyedLabel;
	gcn::Label* mEnemiesKilledLabel;
	gcn::Label* mOrbsTakenLabel;
	gcn::Label* mTimePassedLabel;
	gcn::Label* mTotalLabel;

	gcn::Label* mBlockPointsLabel;
	gcn::Label* mEnemyKillPointsLabel;
	gcn::Label* mOrbsTakenPointsLabel;
	gcn::Label* mTimeBonusLabel;
	gcn::Label* mTotalPointsLabel;

};

#endif