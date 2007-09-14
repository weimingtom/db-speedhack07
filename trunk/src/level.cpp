#include "level.hpp"

#include "exception.hpp"
#include "fileutil.hpp"
#include "stringutil.hpp"
#include "block.hpp"
#include "starsbackground.hpp"
#include "resourcehandler.hpp"

#include <iostream>

Level::Level(const std::string& filename)
: mGameScrollY(0),
mBackgroundScrollY(0)
{
    load(filename);
	mPlayer = new Player();
	addEntity(mPlayer);

    mGui = new gcn::Gui();
    mGraphics = new gcn::AllegroGraphics();
    mInput = new gcn::ExtendedAllegroInput();
    mImageLoader = new gcn::AllegroImageLoader();
    gcn::Image::setImageLoader(mImageLoader);
    mGui->setInput(mInput);
    mGui->setGraphics(mGraphics);
    mImageFont = new gcn::ImageFont(ResourceHandler::getInstance()->getRealFilename("font.bmp"), 32, 126);
	mImageFont->setGlyphSpacing(-1);
	gcn::Widget::setGlobalFont(mImageFont);
    mTop = new gcn::Container();
    mTop->setSize(320, 240);
    mTop->setOpaque(false);
    mGui->setTop(mTop);
    mDialog = new Dialog();
    mDialog->setSize(240, 75);
    mDialog->setVisible(false);
    mTop->add(mDialog, 40, 240 - 75);
}

Level::~Level()
{
    delete mGui;
    delete mImageLoader;
    delete mGraphics;
    delete mInput;
    delete mTop;
    delete mImageFont;
    delete mDialog;

    std::list<Entity *>::iterator it;

    for (it = mEntities.begin(); it != mEntities.end(); it++)
    {
        delete (*it);
    }
}

// Predicate function used to remove entities.
static bool isNull(Entity *e) 
{
	return e == NULL;
}

void Level::draw(BITMAP* dest)
{
    BITMAP* subdest = create_sub_bitmap(dest, 40, 0, 240, 240); 	

    mBackground->draw(subdest, mBackgroundScrollY + mGameScrollY, Entity::BACKGROUND_LAYER);

    std::list<Entity *>::iterator it;

	for (int layer = 0; layer < Entity::NUM_LAYERS; layer++)
	{
		for (it = mEntities.begin(); it != mEntities.end(); it++)
		{
			if ((*it)->drawInLayer(layer))
			{
				(*it)->draw(subdest, mGameScrollY, layer);
			}
		}
	}

    destroy_bitmap(subdest);

    mGraphics->setTarget(dest);
    mGui->draw();
}

void Level::logic()
{
	std::cout << mPlayerEntities.size() << " " 
		<< mPlayerBulletEntities.size() << " "
		<< mEnemyEntities.size() << " "
		<< mEnemyBulletEntities.size() << std::endl;

    if (mState == DIALOG)
    {
         mGui->logic();

        if (!mDialog->isVisible() && mDialogText.empty())
        {
            mState = GAME;
        }
        else if (!mDialog->isVisible())
        {
            mDialog->setText(mDialogText.front());
            mDialog->setVisible(true);
            mDialogText.pop_front();
        } 

        mBackgroundScrollY++;
    }
    else if (mState == GAME)
    {
        checkCollision(mEnemyEntities, mPlayerBulletEntities);
        checkCollision(mPlayerEntities, mEnemyBulletEntities);
        checkCollision(mEnemyEntities, mPlayerEntities);
        checkCollision(mEnemyEntities, mEnemyEntities);
     
        std::list<Entity *>::iterator it;

        while (!mHibernatingEntities.empty() 
                && mHibernatingEntities.front()->getY() <= mGameScrollY + 240 + 20)
        {
            addEntity(mHibernatingEntities.front());
            mHibernatingEntities.pop_front();
        }

        for (it = mEntities.begin(); it != mEntities.end(); it++)
        {
	        (*it)->logic(this);
        }

        for (it = mEntities.begin(); it != mEntities.end(); it++)
        {
	        if ((*it)->isToBeDeleted()) 
            {
		        delete (*it);

                mPlayerEntities.remove((*it));
                mPlayerBulletEntities.remove((*it));
                mEnemyEntities.remove((*it));
                mEnemyBulletEntities.remove((*it));
		        *it = NULL;
	        }
        }

        mEntities.remove_if(isNull);

        //mBackgroundScrollY++;

	    updateScrolling();

        mGui->logic();
    }
}

void Level::updateScrolling()
{
	int wantedScroll = mPlayer->getY() - 20;
	int scrollAmount = wantedScroll - mGameScrollY;
	mGameScrollY += scrollAmount / 5;
}

void Level::load(const std::string& filename)
{
    std::vector<std::string> data = tokenize(loadFile(filename), "\n");

    unsigned int row;
    unsigned int col;

    std::string backgroundName = data[0];

    if (backgroundName == "SPACE")
    {
        mMotif = SPACE_MOTIF;
        mBackground = new StarsBackground();
    }
    else
    {
        throw DBSH07_EXCEPTION("Unknown motif (Available is SPACE)!");
    }

    // Load entities
    for (row = 1; row < data.size(); row++)
    {
        if (data[row] == "DIALOG")
        {
            break;
        }

        for (col = 0; col < data[row].size(); col++)
        {
            switch(data[row].at(col))
            {
                case '.':
					// ignore
                    break;
                case '0':
                    if (mMotif == SPACE_MOTIF)
                    {
                        mHibernatingEntities.push_back(new Block(col*10,row*10, 10, 10, "spaceblock.bmp", -1));
                    }
                   break;
                default:
                    throw DBSH07_EXCEPTION("Unknown entity " + toString(data[row].at(col)));
            }
        }
    }

    // Load dialog
    if (row < data.size() && data[row] == "DIALOG")
    {
        mState = DIALOG;

        for (unsigned int i = row + 1; i < data.size(); i ++)
        {
            mDialogText.push_back(data[i]);
        }
    }
    else
    {
        mState = GAME;
    }
}

void Level::addEntity(Entity* entity)
{
    switch (entity->getType())
    {
        case Entity::ENEMY_BULLET_TYPE:
            mEnemyBulletEntities.push_back(entity);
            break;
        case Entity::ENEMY_TYPE:
            mEnemyEntities.push_back(entity);
            break;
        case Entity::PLAYER_BULLET_TYPE:
            mPlayerBulletEntities.push_back(entity);
            break;
        case Entity::PLAYER_TYPE:
            mPlayerEntities.push_back(entity);
            break;
    }

    mEntities.push_back(entity);
}

int Level::getMouseX()
{
	return mouse_x / 2 - 40;
}

int Level::getMouseY()
{
	return mouse_y / 2 + mGameScrollY;
}

bool Level::isFirePressed()
{
	return mouse_b & 1;
}

bool Level::isBrakePressed()
{
    return mouse_b & 2;
}

void Level::checkCollision(std::list<Entity*>& list1, std::list<Entity*>& list2)
{
    std::list<Entity *>::iterator it1;
    std::list<Entity *>::iterator it2;

    for (it1 = list1.begin(); it1 != list1.end(); it1++)
    {
        for (it2 = list2.begin(); it2 != list2.end(); it2++)
        {			
            Entity* entity1 = (*it1);
            Entity* entity2 = (*it2);

            if (entity1->collidesWith(entity2))
            {
                entity1->handleCollision(entity2, this);
                entity2->handleCollision(entity1, this);
            }
        }
    }
}