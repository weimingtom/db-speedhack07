#ifndef DBSH07_GAME_HPP
#define DBSH07_GAME_HPP

#include <allegro.h>

#include "splashscreen.hpp"
#include "level.hpp"

#include "gui/ballzlistbox.hpp"
//#include "gui/levelselector.hpp"
#include "guichan.hpp"
#include "guichan/allegro.hpp"
#include "gui/ballzscrollarea.hpp"

class Game
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

	    class MainMenuListModel: public gcn::ListModel
    {
    public:
        int getNumberOfElements()
        {
            return 2;
        }

        std::string getElementAt(int i)
        {
            switch(i)
            {
            case 0:
                return std::string("PLAY");
            case 1:
                return std::string("CREDITS");
            default: // Just to keep warnings away
              return std::string("");
            }
        }
    };


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
    BallzListBox* mMainMenuListBox;
    MainMenuListModel* mMainMenuListModel;

    gcn::Container* mCreditsContainer;
    gcn::Image* mOlofImage;
    gcn::Icon* mOlofIcon;
    gcn::Label* mOlofLabel;
    gcn::Image* mPerImage;
    gcn::Icon* mPerIcon;
    gcn::Label* mPerLabel;

    gcn::Container* mLevelsContainer;
//    LevelSelector* mLevelSelector;
    BallzScrollArea* mLevelSelectorScrollArea;

    gcn::TextBox* mInfoText;
    gcn::ImageFont* mInfoTextFont;

    gcn::Image* mTopBackgroundImage;
    gcn::Icon* mTopBackgroundIcon;
    gcn::Image* mBallzLogoImage;
    gcn::Icon* mBallzLogoIcon;
    gcn::Label* mCollectedStars;

    gcn::TextBox* mCreditsText;

};

#endif