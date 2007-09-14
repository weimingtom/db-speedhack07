#ifndef DBSH07_GAME_HPP
#define DBSH07_GAME_HPP

#include <allegro.h>

#include "splashscreen.hpp"
#include "level.hpp"

#include "gui/dbsh07listbox.hpp"
//#include "gui/levelselector.hpp"
#include "guichan.hpp"
#include "guichan/allegro.hpp"
#include "gui/dbsh07button.hpp"

class Game: public gcn::ActionListener, public gcn::KeyListener
{
public:
    Game();
    ~Game();

	void logic();
	void draw();
    void run();
private:
	void initGui();
protected:

	void action(const gcn::ActionEvent& actionEvent);
	void keyPressed(gcn::KeyEvent &keyEvent);

    enum State
    {
        SPLASHSCREEN,
		MENU,
		EXIT,
        LEVEL
    };

    State mState;
    BITMAP* mBuffer;
    BITMAP* mScreenBuffer;
    SplashScreen* mSplashScreen;
    
    Level* mLevel;


	gcn::AllegroGraphics* mAllegroGraphics;
    gcn::AllegroImageLoader* mAllegroImageLoader;
    gcn::AllegroInput* mAllegroInput;
    gcn::ImageFont* mImageFont;
	gcn::Gui* mGui;

	gcn::Container* mTop;

    gcn::Container* mMainMenuContainer;

    gcn::Container* mCreditsContainer;
    gcn::Image* mOlofImage;
    gcn::Icon* mOlofIcon;
    gcn::Label* mOlofLabel;
    gcn::Image* mPerImage;
    gcn::Icon* mPerIcon;
    gcn::Label* mPerLabel;
    gcn::Image* mTedImage;
	gcn::Icon* mTedIcon;
    gcn::Label* mTedLabel;

//    gcn::Container* mLevelsContainer;
//    LevelSelector* mLevelSelector;
//    BallzScrollArea* mLevelSelectorScrollArea;

//    gcn::TextBox* mInfoText;
//    gcn::ImageFont* mInfoTextFont;

    gcn::Image* mTopBackgroundImage;
    gcn::Icon* mTopBackgroundIcon;
    gcn::Image* mBallzLogoImage;
    gcn::Icon* mBallzLogoIcon;
    //gcn::Label* mCollectedStars;

    gcn::TextBox* mCreditsText;

    DBSH07Button* mStartButton;
    DBSH07Button* mCreditsButton;
    DBSH07Button* mExitButton;

};

#endif