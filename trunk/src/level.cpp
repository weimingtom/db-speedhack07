#include "level.hpp"

#include "exception.hpp"
#include "fileutil.hpp"
#include "stringutil.hpp"
#include "block.hpp"
#include "starsbackground.hpp"

#include <iostream>

Level::Level(const std::string& filename)
: mGameScrollY(0),
mBackgroundScrollY(0)
{
    load(filename);
	mPlayer = new Player();
	addEntity(mPlayer);
}

Level::~Level()
{
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
}

void Level::logic()
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

    for (row = 1; row < data.size(); row++)
    {
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
	return mouse_x / 2- 40;
}

int Level::getMouseY()
{
	return mouse_y / 2 + mGameScrollY;
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