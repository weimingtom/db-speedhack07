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
	unsigned int getPoints() {return mPoints; };
	void setPoints(unsigned int points) {mPoints = points; };
	void addPoints(unsigned int points) {mPoints += points; };
	void setCannonLevel(unsigned int cannonLevel);
	unsigned int getCannonLevel();
    void setMegaBlasts(unsigned int megaBlasts);
    unsigned int getMegaBlasts();

private:
    GameState();
    static GameState* mInstance;
    unsigned int mLives;
    unsigned int mLevel;
    unsigned int mEnergyOrbs;
    unsigned int mPods;

	unsigned int mPoints;
	
	unsigned int mCannonLevel;
    unsigned int mMegaBlasts;
};
#endif