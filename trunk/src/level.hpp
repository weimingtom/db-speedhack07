#ifndef DBSH07_LEVEL_HPP
#define DBSH07_LEVEL_HPP

#include "entity.hpp"
#include "player.hpp"
#include "guichan.hpp"
#include "guichan/allegro.hpp"
#include "gui/dialog.hpp"
#include "gui/extendedallegroinput.hpp"

#include <list>

class Level
{
public:
    Level(const std::string& filename);
    ~Level();
    void draw(BITMAP* dest);
    void logic();
    void load(const std::string& filename);
    void addEntity(Entity* entity);

	int getMouseX();
	int getMouseY();
	bool isFirePressed();
    bool isBrakePressed();
    bool isBurnPressed();

    int getScrollY() { return mGameScrollY; }

    static const int SPACE_MOTIF = 0;
    static const int SKY_MOTIF = 1;
    static const int WATER_MOTIF = 2;
    static const int CAVE_MOTIF = 3;

    static const int BLOCK_SIZE = 16;
    static const int LEVEL_WIDTH = 240;
    static const int BLOCK_WIDTH = LEVEL_WIDTH / BLOCK_SIZE;

protected:
    enum State
    {
        DIALOG,
        GAME
    };

	void updateScrolling();
	void drawMousePointer(BITMAP *dest);
    void checkCollision(std::list<Entity*>& list1, std::list<Entity*>& list2);
    void checkStaticCollision(std::list<Entity*>& list);

    std::list<Entity*> mEntities;
    std::list<Entity*> mHibernatingEntities;
    std::list<Entity*> mEnemyEntities;
    std::list<Entity*> mEnemyBulletEntities;
    std::list<Entity*> mPlayerEntities;
    std::list<Entity*> mPlayerBulletEntities;
    int mGameScrollY;
    int mBackgroundScrollY;
    Player* mPlayer;
    Entity* mBackground;
    int mMotif;
    gcn::Gui* mGui;
    gcn::ExtendedAllegroInput* mInput;
    gcn::AllegroGraphics* mGraphics;
    gcn::AllegroImageLoader* mImageLoader;
    gcn::ImageFont* mImageFont;
    gcn::Container* mTop;
    Dialog* mDialog;
    unsigned int mState;
    std::list<std::string> mDialogText;

	float mScrollSpeed;
	float mGameScrollFloat;

    std::vector<Entity*> mStaticEntities;
};

#endif