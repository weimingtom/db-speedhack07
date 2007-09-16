#include "gamestate.hpp"
#include "fileutil.hpp"
#include "exception.hpp"
#include "stringutil.hpp"

GameState* GameState::mInstance = 0;

GameState* GameState::getInstance()
{
    if (mInstance == 0)
    {
        mInstance = new GameState();
        mInstance->loadLevelsData();
    }

    return mInstance;
}

GameState::GameState()
{
	mHighScore = new HighScore();
    reset();
}

void GameState::reset()
{
    mLives = 3;
    mLevel = 0;
    mPoints = 0;
    mEnergyOrbs = 1;
    mPods = 1;
    mCannonLevel = 1;
    mMegaBlasts = 3;
	mHighScore->load("highscore.txt");
}

unsigned int GameState::getLives()
{
    return mLives;
}

void GameState::setLives(unsigned int lives)
{
    mLives = lives;
}

unsigned int GameState::getLevel()
{
    return mLevel;
}

void GameState::setLevel(unsigned int level)
{
    mLevel = level;
}

void GameState::setEnergyOrbs(unsigned int energyOrbs)
{
    mEnergyOrbs = energyOrbs;
}

unsigned int GameState::getEnergyOrbs()
{
    return mEnergyOrbs;
}

void GameState::setPods(unsigned int pods)
{
    mPods = pods;
}

unsigned int GameState::getPods()
{
    return mPods;
}

void GameState::setCannonLevel(unsigned int cannonLevel)
{
	mCannonLevel = cannonLevel;
}

unsigned int GameState::getCannonLevel()
{
	return mCannonLevel;
}

void GameState::setMegaBlasts(unsigned int megaBlasts)
{
	mMegaBlasts = megaBlasts;
}

unsigned int GameState::getMegaBlasts()
{
	return mMegaBlasts;
}

void GameState::loadLevelsData()
{
    std::vector<std::string> data = tokenize(loadFile("levels.txt"), "\n");

    if (data.size() == 0)
    {
        throw DBSH07_EXCEPTION("levels.txt is empty!");
    }

    int numberOfLevels = 0;
    int numberOfBonusLevels = 0;
    for (unsigned int i = 0; i < data.size(); i++)
    {
        std::vector<std::string> row = tokenize(data[i], " ");

        if (row.size() < 2)
        {
            throw DBSH07_EXCEPTION("Corrupt levels.txt file at row " + toString(i) + ".");
        }

        if (row[0] == "N")
        {
            numberOfLevels++;
            mLevelsData.push_back(LevelData(row[1], false, "LEVEL " + toString(numberOfLevels)));
        }
        else if (row[0] == "B")
        {
            numberOfBonusLevels++;
            mLevelsData.push_back(LevelData(row[1], true, "BONUS LEVEL " + toString(numberOfBonusLevels)));
        }
        else
        {
            throw DBSH07_EXCEPTION("Corrupt levels.txt file at row " 
                                    + toString(i) +". Unknown token " + row[0] + ".");
        }
    }
}

const std::string& GameState::getLevelFilename(unsigned int level)
{
    return mLevelsData[level - 1].filename;
}

bool GameState::isLevelBonusLevel(unsigned int level)
{
    return mLevelsData[level - 1].bonus;
}

const std::string& GameState::getLevelDesignation(unsigned int level)
{
    return mLevelsData[level - 1].designation;
}

unsigned int GameState::getNumberOfLevels()
{
    return mLevelsData.size();
}