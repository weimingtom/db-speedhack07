#ifndef DBSH07_GAME_HPP
#define DBSH07_GAME_HPP

#include <allegro.h>

#include "splashscreen.hpp"

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
    };

    State mState;
    BITMAP* mBuffer;
    BITMAP* mScreenBuffer;
    SplashScreen* mSplashScreen;
};

#endif