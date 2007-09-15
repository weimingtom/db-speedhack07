#include "level.hpp"

#include "exception.hpp"
#include "fileutil.hpp"
#include "stringutil.hpp"
#include "block.hpp"
#include "mine.hpp"
#include "starsbackground.hpp"
#include "resourcehandler.hpp"
#include "planet.hpp"

#include <iostream>

Level::Level(const std::string& filename)
: mGameScrollY(0),
  mBackgroundScrollY(0),
  mScrollSpeed(0.0f),
  mGameScrollFloat(0.0f),
  mLevelLength(0)
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

	drawMousePointer(subdest);

    destroy_bitmap(subdest);

    mGraphics->setTarget(dest);
    mGui->draw();
}

void Level::drawMousePointer(BITMAP *dest)
{
	int mx = getMouseX();
	int my = getMouseY()- mGameScrollY;
	circle(dest, mx, my, 5, makecol(180, 180, 180));
	arc(dest, mx, my, itofix(mGameScrollY * 3), itofix(mGameScrollY * 3 + 100), 5, makecol(255, 255, 255));
	arc(dest, mx, my, itofix(mGameScrollY * 3), itofix(mGameScrollY * 3 + 100), 7, makecol(140, 140, 140));
	hline(dest, mx - 3, my, mx - 2, makecol(255,255,255));
	hline(dest, mx + 2, my, mx + 3, makecol(255,255,255));
	vline(dest, mx, my - 3, my - 2, makecol(255,255,255));
	vline(dest, mx, my + 2, my + 3, makecol(255,255,255));
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
        checkStaticCollision(mPlayerEntities);
        checkStaticCollision(mPlayerBulletEntities);
        //checkCollision(mEnemyEntities, mEnemyEntities);
     
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
                if ((*it)->getType() == Entity::BLOCK_TYPE)
                {
                    mStaticEntities[(*it)->getX() / BLOCK_SIZE + (*it)->getY() / BLOCK_SIZE * BLOCK_WIDTH] = NULL;
                }

                mPlayerEntities.remove((*it));
                mPlayerBulletEntities.remove((*it));
                mEnemyEntities.remove((*it));
                mEnemyBulletEntities.remove((*it));

		        delete (*it);

		        *it = NULL;
	        }
        }

        mEntities.remove_if(isNull);

        //mBackgroundScrollY++;

	    updateScrolling();

        mGui->logic();
    }
}

void Level::spawnNewPlayer()
{
    mPlayer = new Player();
    mPlayer->setY(mGameScrollY);
    addEntity(mPlayer);
}

void Level::updateScrolling()
{
    if (mPlayer->getState() == Player::KILLED)
    {
        // Do nothing, that is freeze the camera.
    }
    else if (isBurnPressed())
    {
	    float wantedScroll = mPlayer->getY() - 40.0f + mPlayer->getSpeed();
	    float scrollAmount = wantedScroll - mGameScrollFloat;
	    float wantedSpeed = mPlayer->getSpeed();
	    float speedAdjust = wantedSpeed - mScrollSpeed;
        mScrollSpeed += speedAdjust / 40.0f;
        mGameScrollFloat += mScrollSpeed + scrollAmount / 20.0f;
    	
	    mGameScrollY = (int)mGameScrollFloat;
    }
    else
    {
	    float wantedScroll = mPlayer->getY() - 40.0f + mPlayer->getSpeed() * 5.0f;
	    float scrollAmount = wantedScroll - mGameScrollFloat;
	    float wantedSpeed = mPlayer->getSpeed();
	    float speedAdjust = wantedSpeed - mScrollSpeed;
        
        mScrollSpeed += speedAdjust / 20.0f;
        mGameScrollFloat += mScrollSpeed + scrollAmount / 10.0f;
    	
        if ((int)mGameScrollFloat > mGameScrollY)
        {
	       mGameScrollY = (int)mGameScrollFloat;
        }
    }
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
		mEntities.push_back(new Planet());
    }
    else
    {
        throw DBSH07_EXCEPTION("Unknown motif (Available is SPACE)!");
    }

    data.erase(data.begin());

    // Load entities
    for (row = 0; row < data.size(); row++)
    {
        if (data[row] == "DIALOG")
        {
            break;
        }

		mLevelLength = row * BLOCK_SIZE + BLOCK_SIZE;

        for (col = 0; col < data[row].size(); col++)
        {
            Entity* staticEntity = NULL;
			Entity* entity = NULL;

            switch(data[row].at(col))
            {
                case '.':
					// Ignore
                    break;
                case '0':
                    if (mMotif == SPACE_MOTIF)
                    {
                        staticEntity = new Block(col * BLOCK_SIZE,
                                                 row * BLOCK_SIZE, 
                                                 BLOCK_SIZE,
                                                 BLOCK_SIZE, 
                                                 "spaceblock.bmp", 2);

                        mHibernatingEntities.push_back(staticEntity);
                    }
                   break;
                case 'M':
                    entity = new Mine(col*BLOCK_SIZE,row*BLOCK_SIZE, false);
                    mHibernatingEntities.push_back(entity);
                   break;
                case 'm':
                    entity = new Mine(col*BLOCK_SIZE,row*BLOCK_SIZE, true);
                    mHibernatingEntities.push_back(entity);
                   break;

				case '1':
                    if (mMotif == SPACE_MOTIF)
                    {
                        staticEntity = new Block(col * BLOCK_SIZE,
                                                 row * BLOCK_SIZE, 
                                                 BLOCK_SIZE,
                                                 BLOCK_SIZE, 
                                                 "spacesolidblock.bmp", 0);

                        mHibernatingEntities.push_back(staticEntity);
                    }
                   break;
                default:
                    throw DBSH07_EXCEPTION("Unknown entity " + toString(data[row].at(col)));
            }
            mStaticEntities.push_back(staticEntity);
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

Player* Level::getPlayer()
{
	return mPlayer;
}

bool Level::isFirePressed()
{
	return (mouse_b & 1) != 0;
}

bool Level::isBrakePressed()
{
    return key[KEY_W] != 0;
}

bool Level::isBurnPressed()
{
    return key[KEY_S] != 0;
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

void Level::checkStaticCollision(std::list<Entity*>& list)
{
    std::list<Entity *>::iterator it;

    for (it = list.begin(); it != list.end(); it++)
    {
        Entity* entity = (*it);

        if (!entity->isCollidable())
        {
            continue;
        }
    
        int minx = entity->getX() / BLOCK_SIZE;
        int miny = entity->getY() / BLOCK_SIZE;
        int maxx = (entity->getX() + entity->getWidth() - 1) / BLOCK_SIZE;
        int maxy = (entity->getY() + entity->getHeight() - 1) / BLOCK_SIZE;

		if (minx >= BLOCK_WIDTH || maxx < 0)
		{
			continue;
		}

		if (minx < 0)
		{
			minx = 0;
		}

		if (miny < 0)
		{
			miny = 0;
		}

		if (maxx >= BLOCK_WIDTH)
		{
			maxx = BLOCK_WIDTH - 1;
		}

        for (int x = minx; x <= maxx; x++)
        {
            for (int y = miny; y <= maxy; y++)
            {
				unsigned int i = x + y * BLOCK_WIDTH;
				
				if (i >= mStaticEntities.size())
				{
					continue;
				}

                Entity* otherEntity = mStaticEntities[i];
      
                if (otherEntity != NULL 
                    && otherEntity->isCollidable())
                {
                    entity->handleCollision(otherEntity, this);
                    otherEntity->handleCollision(entity, this);
                }   
            }
        }
    }
}