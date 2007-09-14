#include "game.hpp"

#include <allegro.h>

#include "resourcehandler.hpp"
#include "timer.hpp"
#include "exception.hpp"

#include <iostream>

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
					throw DBSH07_EXCEPTION("Unable to set graphics mode.");
			    }
		    }
	    }
    }

    initTimer();
    mState = SPLASHSCREEN;

    mScreenBuffer = create_bitmap(640, 480);
	mBuffer = create_bitmap(320, 240);

    mSplashScreen = new SplashScreen();
	initGui();
    mLevel = new Level(ResourceHandler::getInstance()->getRealFilename("level1.txt"));

}

Game::~Game()
{
    ResourceHandler::getInstance()->destroy();

    delete mSplashScreen;
	destroy_bitmap(mScreenBuffer);
	destroy_bitmap(mBuffer);

    delete mLevel;

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
                mState = MENU;
            }
            break;
        case MENU:
        
            mGui->logic();
			break;
		
//            case GAME:
//                mGui->logic();
//				mRoom->logic();
//                break;

            case LEVEL:
                mLevel->logic();
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
}

void Game::draw()
{
	acquire_bitmap(mBuffer);

     // TODO Remove the clear line. 
    clear_to_color(mBuffer, makecol(120, 160, 160));

    switch (mState) 
    {
    case SPLASHSCREEN:
        mSplashScreen->draw(mBuffer);
        stretch_blit(mBuffer, mScreenBuffer, 0, 0, 320, 240, 0, 0, 640, 480);
        break;
	case MENU:
        mAllegroGraphics->setTarget(mBuffer);
        mGui->draw();
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
void Game::initGui()
{
	mGui = new gcn::Gui();
//		mGui->addGlobalKeyListener(this);
	mAllegroGraphics = new gcn::AllegroGraphics();
	mAllegroImageLoader = new gcn::AllegroImageLoader();
	mAllegroInput = new gcn::AllegroInput();

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

	mMainMenuListModel = new MainMenuListModel();
	mMainMenuListBox = new BallzListBox();
	mMainMenuListBox->setListModel(mMainMenuListModel);
	mMainMenuListBox->setTabOutEnabled(false);
	mMainMenuListBox->setSelected(0);
	mMainMenuContainer->add(mMainMenuListBox, 125, 150);
	mMainMenuListBox->requestFocus();
//	mMainMenuListBox->addActionListener(this);


	mCreditsContainer = new gcn::Container();
	mCreditsContainer->setSize(320, 240);
	mCreditsContainer->setOpaque(false);
	mCreditsContainer->setVisible(false);
	mTop->add(mCreditsContainer);

	mOlofImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("olof.bmp"));
	mOlofIcon = new gcn::Icon(mOlofImage);
	mCreditsContainer->add(mOlofIcon, 80, 95);

	mOlofLabel = new gcn::Label("OLOF NAESSEN");
	mCreditsContainer->add(mOlofLabel, 80, 165);

	mPerImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("per.bmp"));
	mPerIcon = new gcn::Icon(mPerImage);
	mCreditsContainer->add(mPerIcon, 170, 95);

	mPerLabel = new gcn::Label("PER LARSSON");
	mCreditsContainer->add(mPerLabel, 170, 165);

	mCreditsText = new gcn::TextBox("We would like to thank Marcus Matern and Tomas Almgren\n"
									"for helping out with making levels. Finally we would like\n"
									"to thank Per's girlfriend Johanna for feeding us and putting\n"
									"up with the whole idea of making this game in Per's and\n"
									"Johanna's apartment.");
	mCreditsText->setOpaque(false);
	mCreditsText->setBorderSize(0);
	mCreditsContainer->add(mCreditsText, 40, 180);

	mLevelsContainer = new gcn::Container();
	mLevelsContainer->setSize(320, 240);
	mLevelsContainer->setOpaque(false);
	mLevelsContainer->setVisible(false);
	mTop->add(mLevelsContainer);

//	mLevelSelector = new LevelSelector();
//	mLevelSelector->setTabOutEnabled(false);
//	mLevelSelector->setSelected(0);
//	mLevelSelector->addActionListener(this);
  
	mLevelSelectorScrollArea = new BallzScrollArea();
	mLevelSelectorScrollArea->setSize(310, 88);
//	mLevelSelectorScrollArea->setContent(mLevelSelector);
	mLevelsContainer->add(mLevelSelectorScrollArea, 5, 140);

	mCollectedStars = new gcn::Label("Total ~ collected: 0");
	mLevelsContainer->add(mCollectedStars, 115, 120);
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