#include "level.hpp"

Level::Level()
: mScrollY(0)
{

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
            mPlayerBulletsEntities.remove((*it));
            mEnemyEntities.remove((*it));
            mEnemyBulletsEntities.remove((*it));
		    *it = NULL;
	    }
    }

    mEntities.remove_if(isNull);
}