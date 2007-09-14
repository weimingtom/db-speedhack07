#ifndef DBSH07_GAME_HPP
#define DBSH07_GAME_HPP

#include <allegro.h>

#include "splashscreen.hpp"
#include "level.hpp"

class Game
{
public:
    Game();
    ~Game();

	void logic();
	void draw();
    void run();

protected:

    enum State
    {
        EXIT,
        SPLASHSCREEN,
        LEVEL
    };

    State mState;
    BITMAP* mBuffer;
    BITMAP* mScreenBuffer;
    SplashScreen* mSplashScreen;
    Level* mLevel;
};

#endif