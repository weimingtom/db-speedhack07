#include "game.hpp"

#include <allegro.h>

#include "resourcehandler.hpp"
#include "timer.hpp"
#include "exception.hpp"

#include <iostream>

namespace dbsh07
{
    Game::Game()
    {
        allegro_init();
	    install_keyboard();
        install_mouse();
    	
	    set_window_title("Darkbits - SpeedHack07");

	    int driver = GFX_AUTODETECT_WINDOWED;
	    set_color_depth(32);
	    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
		    set_color_depth(24);
		    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
			    set_color_depth(16);
			    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) {
				    set_color_depth(15);
				    if (set_gfx_mode(driver, 640, 480, 0, 0) < 0) 
                    {
						throw std::string("Unable to set graphics mode.");
				    }
			    }
		    }
	    }

	    initTimer();
	    mState = SPLASHSCREEN;

        mScreenBuffer = create_bitmap(640, 480);
		mBuffer = create_bitmap(320, 240);

        mSplashScreen = new SplashScreen();
    }

    Game::~Game()
    {
        ResourceHandler::getInstance()->destroy();
    
        delete mSplashScreen;
		destroy_bitmap(mScreenBuffer);
		destroy_bitmap(mBuffer);

    }

	void Game::logic()
	{
        switch (mState) 
        {
            case SPLASHSCREEN:
                mSplashScreen->logic();

                if (mSplashScreen->isDrawingDone())
                {
					clear_keybuf();
                    mState = EXIT;
                }
                break;

//            case GAME:
//                mGui->logic();
//				mRoom->logic();
//                break;
            case EXIT:
                break;
       	    default:
				throw DBSH07_EXCEPTION("Unknown game state.");
	   }
       
        if (key[KEY_Q]) 
        {
	        mState = EXIT;
	    }
	}

	void Game::draw()
	{
		acquire_bitmap(mBuffer);

         // TODO Remove the clear line. 
        //clear_to_color(mBuffer, makecol(120, 160, 160));

        switch (mState) 
        {
        case SPLASHSCREEN:
            mSplashScreen->draw(mBuffer);
            stretch_blit(mBuffer, mScreenBuffer, 0, 0, 320, 240, 0, 0, 640, 480);
            break;
//		case GAME:
//			mRoom->draw(mBuffer);
//			release_bitmap(mBuffer);
//          mAllegroGraphics->setTarget(mBuffer);
//            mGui->draw();
//			stretch_blit(mBuffer, mScreenBuffer, 0, 0, 320, 240, 0, 0, 640, 480);
//			break;
		case EXIT:
			break;
		default:
			throw DBSH07_EXCEPTION("Unknown game state.");
	   }
	}

    void Game::run()
    {
      
		long frame = getTick();
		int graphicframes = 0;
		int second = getTick() / TICKS_PER_SECOND;
		
		while (mState != EXIT) 
        {
			logic();

			if (getTick() - frame > 20)
            {
				frame = getTick();
				std::cout << "Logic frame drop! =(" << std::endl;
			}

			frame++;

			if (frame > getTick())
			{
				draw();
                blit(mScreenBuffer, screen, 0, 0, 0, 0, 640, 480); 

				graphicframes++;
			}

			while (frame > getTick()) 
            {
				rest(1);
			}

			if (second != getTick() / TICKS_PER_SECOND) 
            {			
				std::cout << "FPS: " << graphicframes << std::endl;
				second = getTick() / TICKS_PER_SECOND;
				graphicframes = 0;
			}
		}
    }
}