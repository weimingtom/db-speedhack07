#ifndef DBSH07_POINTSUMMARY_HPP
#define DBSH07_POINTSUMMARY_HPP

#include "guichan.hpp"
class PointSummary
{
public:
    PointSummary(gcn::Container* mTop, unsigned int blocks, unsigned int parBlocks,  unsigned int maxBlocks, unsigned int enemies, unsigned int parEnemies, unsigned int maxEnemies, unsigned int orbs, unsigned int time, unsigned int parTime);
    ~PointSummary();
	void logic();
	bool isDone();

protected:
	void initGui(gcn::Container* mTop);

	unsigned int mOrbs;
	unsigned int mEnemies;
	unsigned int mParEnemies;
	unsigned int mMaxEnemies;
	unsigned int mBlocks;
	unsigned int mParBlocks;
	unsigned int mMaxBlocks;
	unsigned int mTime;
	unsigned int mParTime;
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