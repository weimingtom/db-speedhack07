#ifndef DBSH07_GAMESTATE_HPP
#define DBSH07_GAMESTATE_HPP

class GameState
{
public:
    static GameState* getInstance();
    unsigned int getLives();
    void setLives(unsigned int lives);
    unsigned int getLevel();
    void setLevel(unsigned int level);
    void setEnergyOrbs(unsigned int energyOrbs);
    unsigned int getEnergyOrbs();
    void setPods(unsigned int pods);
    unsigned int getPods();

private:
    GameState();
    static GameState* mInstance;
    unsigned int mLives;
    unsigned int mLevel;
    unsigned int mEnergyOrbs;
    unsigned int mPods;
};
#endif