#include <stdio.h>
#include <iostream>

#include "pointsummary.hpp"
#include "stringutil.hpp"
#include "gamestate.hpp"
#include "resourcehandler.hpp"

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
mFrameCounter(50),
mState(0),
mCountingPoints(false),
mBlockPointsCounter(0),
mEnemyPointsCounter(0),
mOrbPointsCounter(0),
mTimePointsCounter(0),
mTotalPointsCounter(0)
{
	initGui(mTop);
	mBlockPoints = blocks*10; //todo
	mEnemyPoints = enemies*20; //todo
	mOrbPoints = orbs*30; //todo
	mTimePoints = time/10; //todo

	//test
	mBlockPoints = 2400;
	mEnemyPoints = 3220;
	mOrbPoints = 1000;
	mTimePoints = 0;

	mTotalPoints = mBlockPoints + mEnemyPoints + mOrbPoints + mTimePoints;
	
	mBeepSample = ResourceHandler::getInstance()->getSample("beep.wav");

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
	return mState==13;
}

void PointSummary::logic()
{
	if(!mCountingPoints)
	{
		mFrameCounter--;
		if (mFrameCounter<=0)
		{
			mState++;
			mFrameCounter=10;
			if(mState == 1)
			{
				mBlocksDestroyedLabel->setVisible(true);
				//play_sample(mBeepSample, 128, 128, 500, 0);
			}
			else if (mState == 2)
			{
				mBlockPointsLabel->setVisible(true);
				mCountingPoints=true;
				//play_sample(mBeepSample, 128, 128, 800, 0);
			}
			else if (mState == 3)
			{
				mEnemiesKilledLabel->setVisible(true);
				//play_sample(mBeepSample, 128, 128, 500, 0);
			}
			else if (mState == 4)
			{
				mEnemyKillPointsLabel->setVisible(true);
				mCountingPoints=true;
				//play_sample(mBeepSample, 128, 128, 800, 0);
			}
			else if (mState == 5)
			{
				mOrbsTakenLabel->setVisible(true);
				//play_sample(mBeepSample, 128, 128, 500, 0);
			}
			else if (mState == 6)
			{
				mOrbsTakenPointsLabel->setVisible(true);
				mCountingPoints=true;
				//play_sample(mBeepSample, 128, 128, 800, 0);
			}
			else if (mState == 7)
			{
				mTimePassedLabel->setVisible(true);
				//play_sample(mBeepSample, 128, 128, 500, 0);
			}
			else if (mState == 8)
			{
				mTimeBonusLabel->setVisible(true);
				mCountingPoints=true;
				//play_sample(mBeepSample, 128, 128, 800, 0);
			}
			else if (mState == 9)
			{
				mTotalLabel->setVisible(true);
				//play_sample(mBeepSample, 128, 128, 500, 0);
			}
			else if (mState == 10)
			{
				mTotalPointsLabel->setVisible(true);
				mFrameCounter = 60;
				//play_sample(mBeepSample, 128, 128, 800, 0);
			}
			else if(mState == 11)
			{
				mCountingPoints=true;
			}
		}
	}

	if (mCountingPoints)
	{
		int total;
		int target;
		if(mState==2) { target = mBlockPoints; total=mBlockPointsCounter; }
		if(mState==4) { target = mEnemyPoints; total=mEnemyPointsCounter; }
		if(mState==6) { target = mOrbPoints; total=mOrbPointsCounter; }
		if(mState==8) { target = mTimePoints; total=mTimePointsCounter; }
		if(mState==11) { target = mTotalPoints; total=mTotalPointsCounter; }

		int diff = (target-total);
		
		int add = diff<123?diff:123;

		if(mState==2) { mBlockPointsCounter += add; }
		if(mState==4) { mEnemyPointsCounter += add; }
		if(mState==6) { mOrbPointsCounter += add; }
		if(mState==8) { mTimePointsCounter += add; }
		if(mState==11) { mTotalPointsCounter += add; GameState::getInstance()->addPoints(add); }
		
		if(add == 0) { mCountingPoints = false; }

		//mTotal -= sub;
//		GameState::getInstance()->addPoints(sub);
		play_sample(mBeepSample, 128, 128, 2500, 0);

		//if (mTotal <= 0) { mState = 12; mFrameCounter = 0; }
	}
		mBlocksDestroyedLabel->setCaption("^x" + toString(mBlocks));
		mBlocksDestroyedLabel->adjustSize();
		mBlockPointsLabel->setCaption(": " + toString((int)mBlockPointsCounter));
		mBlockPointsLabel->adjustSize();

		mEnemiesKilledLabel->setCaption("_x" + toString(mEnemies));
		mEnemiesKilledLabel->adjustSize();
		mEnemyKillPointsLabel->setCaption(": " + toString((int)mEnemyPointsCounter));
		mEnemyKillPointsLabel->adjustSize();

		mOrbsTakenLabel->setCaption("}x" + toString(mOrbs));
		mOrbsTakenLabel->adjustSize();
		mOrbsTakenPointsLabel->setCaption(": " + toString((int)mOrbPointsCounter));
		mOrbsTakenPointsLabel->adjustSize();

		mTimePassedLabel->setCaption("Time bonus");
		mTimePassedLabel->adjustSize();
		mTimeBonusLabel->setCaption(": " + toString((int)mTimePointsCounter));
		mTimeBonusLabel->adjustSize();

		std::string str = toString((int)(mTotalPoints - mTotalPointsCounter));
		
		if(str.length() < 5) { std::string str2 = ""; for(int i=0; i < 5-str.length(); i++) {str2 += "0"; }; str = str2+str; }
		
		mTotalLabel->setCaption("TOTAL");
		mTotalLabel->adjustSize();
		mTotalPointsLabel->setCaption(": " + str);
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

