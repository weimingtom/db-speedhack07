#include "game.hpp"

#include <allegro.h>

#include "resourcehandler.hpp"
#include "timer.hpp"
#include "exception.hpp"
#include "music.hpp"

#include <iostream>

Game::Game()
:mPauseButtonPressed(false)
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
					throw DBSH07_EXCEPTION("Unable to set graphics mode.");
			    }
		    }
	    }
    }

	install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);

    initTimer();
    mState = MENU; //SPLASHSCREEN;

    mScreenBuffer = create_bitmap(640, 480);
	mBuffer = create_bitmap(320, 240);

    mSplashScreen = new SplashScreen();
	initGui();
    mLevel = new Level("level1.txt");

	initMusic();
}

Game::~Game()
{
    ResourceHandler::getInstance()->destroy();

    delete mSplashScreen;
	destroy_bitmap(mBuffer);

    delete mLevel;

    delete mGui;
    delete mAllegroGraphics;
    delete mAllegroImageLoader;
    delete mAllegroInput;
    delete mImageFont;
    delete mTop;
    delete mStartButton;
}

void Game::logic()
{
	pollMusic();

    switch (mState) 
    {
        case SPLASHSCREEN:
            mSplashScreen->logic();

            if (mSplashScreen->isDrawingDone())
            {
				clear_keybuf();
                mState = MENU;
            }
            break;
        case MENU:
            mGui->logic();
			break;
        case LEVEL:
            mLevel->logic();
            break;
        case PAUSE:
            break;
        case EXIT:
            break;
   	    default:
			throw DBSH07_EXCEPTION("Unknown game state.");
   }
   
    if (key[KEY_Q]) 
    {
        mState = EXIT;
    }

    if (key[KEY_P] && !mPauseButtonPressed && mState == PAUSE)
    {
        mState = LEVEL;
    }
    else if (key[KEY_P] && !mPauseButtonPressed && mState == LEVEL)
    {
        mState = PAUSE;
    }

    mPauseButtonPressed = key[KEY_P];
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
        break;
	case MENU:
        mAllegroGraphics->setTarget(mBuffer);
        mGui->draw();
        draw_sprite(mBuffer, mouse_sprite, mouse_x / 2, mouse_y / 2);
		break;
    case LEVEL:
        clear_to_color(mBuffer, makecol(0, 0, 0));
        mLevel->draw(mBuffer);
//          mAllegroGraphics->setTarget(mBuffer);
//            mGui->draw();
        break;
    case PAUSE:
        break;
	case EXIT:
		break;
	default:
		throw DBSH07_EXCEPTION("Unknown game state.");
   }
	release_bitmap(mBuffer);
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
			stretch_blit(mBuffer, screen, 0, 0, 320, 240, 0, 0, 640, 480);

			graphicframes++;
		}

		while (frame > getTick()) 
        {
			rest(1);
		}

		if (second != getTick() / TICKS_PER_SECOND) 
        {			
			std::cout << "FPS: " << graphicframes << std::endl;
			std::cout << "State: " << mState << std::endl;

			second = getTick() / TICKS_PER_SECOND;
			graphicframes = 0;
		}
	}
}
void Game::initGui()
{
	mGui = new gcn::Gui();
	mGui->addGlobalKeyListener(this);
	mAllegroGraphics = new gcn::AllegroGraphics();
	mAllegroImageLoader = new gcn::AllegroImageLoader();
	mAllegroInput = new gcn::ExtendedAllegroInput();

	mGui->setGraphics(mAllegroGraphics);
	mGui->setInput(mAllegroInput);
	gcn::Image::setImageLoader(mAllegroImageLoader);

	mImageFont = new gcn::ImageFont(ResourceHandler::getInstance()->getRealFilename("font.bmp"), 32, 126);
	mImageFont->setGlyphSpacing(-1);
	gcn::Widget::setGlobalFont(mImageFont);

	mTop = new gcn::Container();
	mTop->setSize(320, 240);
	mTop->setOpaque(false);
	mGui->setTop(mTop);

	mTopBackgroundImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("menubackground.bmp"));
	mTopBackgroundIcon = new gcn::Icon(mTopBackgroundImage);
	mTop->add(mTopBackgroundIcon);

	mBallzLogoImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("logo.bmp"));
	mBallzLogoIcon = new gcn::Icon(mBallzLogoImage);
	mTop->add(mBallzLogoIcon, 0, 10);

	mMainMenuContainer = new gcn::Container();
	mMainMenuContainer->setSize(320, 240);
	mMainMenuContainer->setOpaque(false);
	mTop->add(mMainMenuContainer);

    mStartButton = new DBSH07Button("Start game");
    mStartButton->addActionListener(this);
    mMainMenuContainer->add(mStartButton, 125, 150);
    mCreditsButton = new DBSH07Button("Credits");
    mCreditsButton->addActionListener(this);
    mMainMenuContainer->add(mCreditsButton, 125, 150 + mStartButton->getHeight());
    mExitButton = new DBSH07Button("Exit");
    mExitButton->addActionListener(this);
    mMainMenuContainer->add(mExitButton, 125, 150 + mStartButton->getHeight()*2);


	mCreditsContainer = new gcn::Container();
	mCreditsContainer->setSize(320, 240);
	mCreditsContainer->setOpaque(false);
	mCreditsContainer->setVisible(false);
	mTop->add(mCreditsContainer);

	mOlofImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("olof.bmp"));
	mOlofIcon = new gcn::Icon(mOlofImage);
	mCreditsContainer->add(mOlofIcon, 50, 95);
	mOlofLabel = new gcn::Label("OLOF NAESSEN");
	mCreditsContainer->add(mOlofLabel, 35, 165);

	mPerImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("per.bmp"));
	mPerIcon = new gcn::Icon(mPerImage);
	mCreditsContainer->add(mPerIcon, 131, 95);
	mPerLabel = new gcn::Label("PER LARSSON");
	mCreditsContainer->add(mPerLabel, 130, 165);

	mTedImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("ted.bmp"));
	mTedIcon = new gcn::Icon(mTedImage);
	mCreditsContainer->add(mTedIcon, 215, 92);
	mTedLabel = new gcn::Label("TED STEEN");
	mCreditsContainer->add(mTedLabel, 215, 165);

	mCreditsText = new gcn::TextBox("We would like to thank Marcus Matern and Tomas Almgren\n"
									"for helping out with making levels. Finally we would like\n"
									"to thank Per's girlfriend Johanna for feeding us and putting\n"
									"up with the whole idea of making this game in Per's and\n"
									"Johanna's apartment.");
	mCreditsText->setOpaque(false);
	mCreditsText->setBorderSize(0);
	mCreditsContainer->add(mCreditsText, 40, 180);

/*
	mInfoText = new gcn::TextBox("Team Darkbits Tins 07\n"
								 "     http://darkbits.org\n"
									   "           Version 1.0");
	mInfoTextFont = new gcn::ImageFont(ResourceHandler::getInstance()->getRealFilename("fontsmallgrey.bmp"), 32, 126);
	mInfoTextFont->setGlyphSpacing(-1);
	mInfoText->setFont(mInfoTextFont);
	mInfoText->setOpaque(false);
	mInfoText->setBorderSize(0);
	mMainMenuContainer->add(mInfoText, 110, 200);
*/
}
void Game::action(const gcn::ActionEvent& actionEvent)
{
    if (actionEvent.getSource() == mStartButton)
    {
        mState = LEVEL;
    }
    else if (actionEvent.getSource() == mCreditsButton)
    {
        mMainMenuContainer->setVisible(false);
        mCreditsContainer->setVisible(true);
    }
    else if (actionEvent.getSource() == mExitButton)
    {
        mState = EXIT;
    }
}
void Game::keyPressed(gcn::KeyEvent &keyEvent)
{
    if (keyEvent.getKey().getValue() == gcn::Key::ESCAPE)
    {
        if (mCreditsContainer->isVisible())
        {
            mCreditsContainer->setVisible(false);
            mMainMenuContainer->setVisible(true);
        }
        else
        {
            mState = EXIT;
        }
    }
}