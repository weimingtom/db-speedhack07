#include <stdio.h>
#include <iostream>

#include "pointsummary.hpp"
#include "stringutil.hpp"
#include "gamestate.hpp"

PointSummary::PointSummary(gcn::Container* mTop, unsigned int blocks, unsigned int parBlocks,  unsigned int maxBlocks, unsigned int enemies, unsigned int parEnemies, unsigned int maxEnemies, unsigned int orbs, unsigned int time, unsigned int parTime)
:
mBlocks(blocks),
mParBlocks(parBlocks),
mMaxBlocks(maxBlocks),
mEnemies(enemies),
mParEnemies(parEnemies),
mMaxEnemies(maxEnemies),
mOrbs(orbs),
mTime(time),
mParTime(time),
mFrameCounter(0),
mState(0)
{
	initGui(mTop);
	mBlockPoints = blocks*10; //todo
	mEnemyPoints = enemies*20; //todo
	mOrbPoints = orbs*30; //todo
	mTimePoints = time/10; //todo
	mTotal = mBlockPoints + mEnemyPoints + mOrbPoints + mTimePoints;
}

PointSummary::~PointSummary()
{
	//point count gui
	delete mBlocksDestroyedLabel;
	delete mEnemiesKilledLabel;
	delete mOrbsTakenLabel;
	delete mTimePassedLabel;
	delete mTotalLabel;

	delete mBlockPointsLabel;
	delete mEnemyKillPointsLabel;
	delete mOrbsTakenPointsLabel;
	delete mTimeBonusLabel;
	delete mTotalPointsLabel;
}

bool PointSummary::isDone()
{
	return mState==13 && mFrameCounter >= 200;
}

void PointSummary::logic()
{
	mFrameCounter++;
	if(mFrameCounter%20==0 && mState != 11 && mState != 13)
	{
		mState++;
		if(mState == 1)
		{
			mBlocksDestroyedLabel->setVisible(true);
		}
		else if (mState == 2)
		{
			mBlockPointsLabel->setVisible(true);
		}
		else if (mState == 3)
		{
			mEnemiesKilledLabel->setVisible(true);
		}
		else if (mState == 4)
		{
			mEnemyKillPointsLabel->setVisible(true);
		}
		else if (mState == 5)
		{
			mOrbsTakenLabel->setVisible(true);
		}
		else if (mState == 6)
		{
			mOrbsTakenPointsLabel->setVisible(true);
		}
		else if (mState == 7)
		{
			mTimePassedLabel->setVisible(true);
		}
		else if (mState == 8)
		{
			mTimeBonusLabel->setVisible(true);
		}
		else if (mState == 9)
		{
			mTotalLabel->setVisible(true);
		}
		else if (mState == 10)
		{
			mTotalPointsLabel->setVisible(true);
		}
	}
	if (mState == 11)
	{
		int sub = mTotal > 100?100:mTotal;
		mTotal -= sub;
		GameState::getInstance()->addPoints(sub);

		if (mTotal <= 0) { mState = 12; mFrameCounter = 0; }
	}
		mBlocksDestroyedLabel->setCaption("^x" + toString(mBlocks));
		mBlocksDestroyedLabel->adjustSize();
		mBlockPointsLabel->setCaption(": " + toString(mBlockPoints));
		mBlockPointsLabel->adjustSize();

		mEnemiesKilledLabel->setCaption("_x" + toString(mEnemies));
		mEnemiesKilledLabel->adjustSize();
		mEnemyKillPointsLabel->setCaption(": " + toString(mEnemyPoints));
		mEnemyKillPointsLabel->adjustSize();

		mOrbsTakenLabel->setCaption("}x" + toString(mOrbs));
		mOrbsTakenLabel->adjustSize();
		mOrbsTakenPointsLabel->setCaption(": " + toString(mOrbPoints));
		mOrbsTakenPointsLabel->adjustSize();

		mTimePassedLabel->setCaption("Time bonus");
		mTimePassedLabel->adjustSize();
		mTimeBonusLabel->setCaption(": " + toString(mTimePoints));
		mTimeBonusLabel->adjustSize();

		mTotalLabel->setCaption("TOTAL");
		mTotalLabel->adjustSize();
		mTotalPointsLabel->setCaption(": " + toString(mTotal));
		mTotalPointsLabel->adjustSize();
}

void PointSummary::initGui(gcn::Container* mTop)
{
	//point count gui stuff
	mBlocksDestroyedLabel = new gcn::Label(); 
	mEnemiesKilledLabel = new gcn::Label(); 
	mOrbsTakenLabel = new gcn::Label(); 
	mTimePassedLabel = new gcn::Label();
	mTotalLabel = new gcn::Label();
	
	mBlockPointsLabel = new gcn::Label();
	mEnemyKillPointsLabel = new gcn::Label();
	mOrbsTakenPointsLabel = new gcn::Label();
	mTimeBonusLabel = new gcn::Label();
	mTotalPointsLabel = new gcn::Label();

	mBlocksDestroyedLabel->setVisible(false);
	mEnemiesKilledLabel->setVisible(false);
	mOrbsTakenLabel->setVisible(false);
	mTimePassedLabel->setVisible(false);
	mBlockPointsLabel->setVisible(false);
	mEnemyKillPointsLabel->setVisible(false);
	mOrbsTakenPointsLabel->setVisible(false);
	mTimeBonusLabel->setVisible(false);
	mTotalLabel->setVisible(false);
	mTotalPointsLabel->setVisible(false);
	

	mTop->add(mBlocksDestroyedLabel, 100, 80);
	mTop->add(mEnemiesKilledLabel, 100, 100);
	mTop->add(mOrbsTakenLabel, 100, 120);
	mTop->add(mTimePassedLabel, 100, 140);
	mTop->add(mTotalLabel, 100, 160);

	mTop->add(mBlockPointsLabel, 180, 80);
	mTop->add(mEnemyKillPointsLabel, 180, 100);
	mTop->add(mOrbsTakenPointsLabel, 180, 120);
	mTop->add(mTimeBonusLabel, 180, 140);
	mTop->add(mTotalPointsLabel, 180, 160);
}

