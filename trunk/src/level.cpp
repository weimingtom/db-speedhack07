#include "level.hpp"

#include "exception.hpp"
#include "fileutil.hpp"
#include "stringutil.hpp"
#include "block.hpp"

Level::Level(const std::string& filename)
: mScrollY(0)
{
    mPlayer = new Player();
    mEntities.push_back(mPlayer);
    mPlayerEntities.push_back(mPlayer);
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
    std::list<Entity *>::iterator it;

    for (it = mEntities.begin(); it != mEntities.end(); it++)
	{
        if ((*it)->drawInLayer(Entity::BACKGROUND_LAYER))
        {
		    (*it)->draw(dest, mScrollY, Entity::BACKGROUND_LAYER);
        }
	}

    for (it = mEntities.begin(); it != mEntities.end(); it++)
	{
        if ((*it)->drawInLayer(Entity::PLAYER_LAYER))
        {
		    (*it)->draw(dest, mScrollY, Entity::PLAYER_LAYER);
        }
	}

    for (it = mEntities.begin(); it != mEntities.end(); it++)
	{
        if ((*it)->drawInLayer(Entity::ENEMY_LAYER))
        {
		    (*it)->draw(dest, mScrollY, Entity::ENEMY_LAYER);
        }
	}

    for (it = mEntities.begin(); it != mEntities.end(); it++)
	{
        if ((*it)->drawInLayer(Entity::EXPLOSION_LAYER))
        {
		    (*it)->draw(dest, mScrollY, Entity::EXPLOSION_LAYER);
        }
	}

    for (it = mEntities.begin(); it != mEntities.end(); it++)
	{
        if ((*it)->drawInLayer(Entity::FOREGROUND_LAYER))
        {
		    (*it)->draw(dest, mScrollY, Entity::FOREGROUND_LAYER);
        }
	}
}

void Level::logic()
{
    std::list<Entity *>::iterator it;

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
}

void Level::load(const std::string& filename)
{
    std::vector<std::string> data = tokenize(loadFile(filename), "\n");

    unsigned int row;
    unsigned int col;

    for (row = 0; row < data.size(); row++)
    {
        for (col = 0; col < data[row].size(); col++)
        {
            switch(data[row].at(col))
            {
                case '.':
                // ignore
                    break;
                case '0':
                    mHibernatingEntities.push_back(new Block(col*10,row*10, 10, 10));
                   break;
                default:
                    throw DBSH07_EXCEPTION("Unknown entity " + toString(data[row].at(col)));
                    break;
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