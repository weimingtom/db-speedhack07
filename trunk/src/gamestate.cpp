#include "gamestate.hpp"

GameState* GameState::mInstance = 0;

GameState* GameState::getInstance()
{
    if (mInstance == 0)
    {
        mInstance = new GameState();
    }

    return mInstance;
}

GameState::GameState()
: mLives(1),
mLevel(1),
mEnergyOrbs(0)
{

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