#include "game.hpp"

#include <allegro.h>

#include "gamestate.hpp"
#include "resourcehandler.hpp"
#include "timer.hpp"
#include "exception.hpp"
#include "music.hpp"

#include <iostream>

Game::Game()
:mPauseButtonPressed(false),
mLevel(NULL)
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

    mScreenBuffer = create_bitmap(640, 480);
	mBuffer = create_bitmap(320, 240);

    mSplashScreen = new SplashScreen();
	initGui();
	initMusic();

    setState(MENU);
}

Game::~Game()
{
    ResourceHandler::getInstance()->destroy();

    delete mSplashScreen;
	destroy_bitmap(mBuffer);

    if (mLevel != NULL)
    {
        delete mLevel;
    }

    delete mGui;
    delete mAllegroGraphics;
    delete mAllegroImageLoader;
    delete mAllegroInput;
    delete mImageFont;
    delete mTop;
    delete mStartButton;

    delete mShop;
    delete mOptionalDialog;
    delete mDialog;
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
                setState(MENU);
            }
            break;
        case BONUS_LEVEL_OR_SHOP:
            mLevel->logic();
            if (mOptionalDialog->getState() == OptionalDialog::NONE
                && !mOptionalDialog->isVisible()
                    && !mDialog->isVisible())
            {
                mOptionalDialog->setVisible(true);
            }
            else if (!mDialog->isVisible()
                && mOptionalDialog->getState() == OptionalDialog::SHOP)
            {
                setState(SHOP);
            }
            else if (!mDialog->isVisible()
                && mOptionalDialog->getState() == OptionalDialog::BONUS_LEVEL)
            {
                startNextLevel();
            }

            mGui->logic();
            break;
        case SHOP:
        case MENU:
            mGui->logic();
			break;
        case LEVEL:
            mLevel->logic();
            if (mLevel->isGameOver())
            {
                setState(MENU);
            }
            else if (mLevel->isLevelComplete())
            {
                std::cout << "LEVEL COMPLETE" << std::endl;
                prepareNextLevel();
            }
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
        setState(EXIT);
    }

    if (key[KEY_P] && !mPauseButtonPressed && mState == PAUSE)
    {
        setState(LEVEL);
    }
    else if (key[KEY_P] && !mPauseButtonPressed && mState == LEVEL)
    {
        setState(PAUSE);
    }

    mPauseButtonPressed = key[KEY_P] != 0;
}

void Game::draw()
{
	acquire_bitmap(mBuffer);

    switch (mState) 
    {
    case SPLASHSCREEN:
        mSplashScreen->draw(mBuffer);
        break;
    case BONUS_LEVEL_OR_SHOP:
        clear_to_color(mBuffer, makecol(0, 0, 0));
        mLevel->draw(mBuffer);
        mAllegroGraphics->setTarget(mBuffer);
        mGui->draw();
        draw_sprite(mBuffer, mouse_sprite, mouse_x / 2, mouse_y / 2);
        break;
    case SHOP:
	case MENU:
        mAllegroGraphics->setTarget(mBuffer);
        mGui->draw();
        draw_sprite(mBuffer, mouse_sprite, mouse_x / 2, mouse_y / 2);
		break;
    case LEVEL:
        clear_to_color(mBuffer, makecol(0, 0, 0));
        mLevel->draw(mBuffer);
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

	mImageFont = new gcn::ImageFont(ResourceHandler::getInstance()->getRealFilename("font.bmp"), 32, 131);
	mImageFont->setGlyphSpacing(-1);
	gcn::Widget::setGlobalFont(mImageFont);

	mTop = new gcn::Container();
	mTop->setSize(320, 240);
	mTop->setOpaque(false);
	mGui->setTop(mTop);

	mTopBackgroundImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("menubackground.bmp"));
	mTopBackgroundIcon = new gcn::Icon(mTopBackgroundImage);
	mTop->add(mTopBackgroundIcon);

    mShop = new Shop();
    mShop->setSize(320, 240);
    mShop->setOpaque(false);
    mShop->addActionListener(this);
    mTop->add(mShop);
    mShop->setVisible(false);

    mOptionalDialog = new OptionalDialog();
    mOptionalDialog->setSize(320, 75);
    mOptionalDialog->setVisible(false);
    mOptionalDialog->addActionListener(this);
    mTop->add(mOptionalDialog, 0, 240 - 75);

    mDialog = new Dialog();
    mDialog->setSize(320, 75);
    mTop->add(mDialog, 0, 240 - 75);
    mDialog->setVisible(false);

    mMainMenuContainer = new gcn::Container();
	mMainMenuContainer->setSize(320, 240);
	mMainMenuContainer->setOpaque(false);
	mTop->add(mMainMenuContainer);

	mBallzLogoImage = gcn::Image::load(ResourceHandler::getInstance()->getRealFilename("logo.bmp"));
	mBallzLogoIcon = new gcn::Icon(mBallzLogoImage);
	mMainMenuContainer->add(mBallzLogoIcon, 0, 10);

    mStartButton = new DBSH07Button("START GAME");
    mStartButton->addActionListener(this);
    mMainMenuContainer->add(mStartButton, 125, 150);
    mCreditsButton = new DBSH07Button("CREDITS");
    mCreditsButton->addActionListener(this);
    mMainMenuContainer->add(mCreditsButton, 125, 150 + mStartButton->getHeight());
    mExitButton = new DBSH07Button("EXIT");
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
        prepareNextLevel();
    }
    else if (actionEvent.getSource() == mCreditsButton)
    {
        mMainMenuContainer->setVisible(false);
        mCreditsContainer->setVisible(true);
    }
    else if (actionEvent.getSource() == mExitButton)
    {
        setState(EXIT);
    }
    else if (actionEvent.getSource() == mShop)
    {
        prepareNextLevel();
    }
    else if (actionEvent.getSource() == mOptionalDialog)
    {
        if (mOptionalDialog->getState() == OptionalDialog::BONUS_LEVEL)
        {
            mOptionalDialog->setVisible(false);
            mDialog->setText("B I think I'll try my luck in a bonus level!");
            mDialog->setVisible(true);
        }
        else if (mOptionalDialog->getState() == OptionalDialog::SHOP)
        {
            mOptionalDialog->setVisible(false);
            mDialog->setText("B I think I'll head for the shop!");
            mDialog->setVisible(true);
        }
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
        else if (mState != BONUS_LEVEL_OR_SHOP)
        {
            setState(EXIT);
        }
    }
}

void Game::setState(State state)
{
    if (state == SHOP)
    {
        mShop->setVisible(true);
        mMainMenuContainer->setVisible(false);
        mCreditsContainer->setVisible(false);
        mOptionalDialog->setVisible(false);
        mTopBackgroundIcon->setVisible(true);
        playMusic("shop.xm", 2.0f);
    }
    else if (state == MENU)
    {
        mShop->setVisible(false);
        mMainMenuContainer->setVisible(true);
        mCreditsContainer->setVisible(false);
        mOptionalDialog->setVisible(false);
        mTopBackgroundIcon->setVisible(true);
    }
    else if (state == BONUS_LEVEL_OR_SHOP)
    {
        mTopBackgroundIcon->setVisible(false);
        mShop->setVisible(false);
        mMainMenuContainer->setVisible(false);
        mCreditsContainer->setVisible(false);
        mOptionalDialog->setVisible(false);
        mOptionalDialog->setState(OptionalDialog::NONE);
        mDialog->setVisible(true);
        mDialog->setText("F FLUFFY LOVE: You can now choose to either head for a bonus lever or enter" 
                         " a shop where you can buy things from Cuddelz!");
    }
   
    mState = state; 
}

void Game::prepareNextLevel()
{
    GameState::getInstance()->setLevel(GameState::getInstance()->getLevel() + 1);

    if (GameState::getInstance()->getLevel() > GameState::getInstance()->getNumberOfLevels())
    {
        //setState(END);
        setState(MENU);
        GameState::getInstance()->reset();
        return;
    }

    if (GameState::getInstance()->isLevelBonusLevel(GameState::getInstance()->getLevel()))
    {
        setState(BONUS_LEVEL_OR_SHOP);
    }
    else
    {
        startNextLevel();
    }
}

void Game::startNextLevel()
{
    if (mLevel != NULL)
    {
        delete mLevel;
    }

    mLevel = new Level(GameState::getInstance()->getLevelFilename(GameState::getInstance()->getLevel()));
    setState(LEVEL);
}

