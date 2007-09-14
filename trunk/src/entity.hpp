#ifndef DBSH07_ENTITY_HPP
#define DBSH07_ENTITY_HPP

#include <allegro.h>

class Level;

class Entity
{
public:
	Entity(bool collidable);
    Entity(int x, int y, int w, int h, bool collidable);
	virtual ~Entity() { };

	virtual void setSize(int w, int h);

	virtual void setPosition(int x, int y);

	virtual int getX();

	virtual int getY();

	virtual int getWidth(); 

	virtual int getHeight();

	virtual void setX(int x);

	virtual void setY(int y);

	virtual void setWidth(int w); 

	virtual void setHeight(int h);

	virtual int getCenterX();

	virtual int getCenterY();

	virtual bool collidesWith(Entity *other);

	virtual void draw(BITMAP *dest, int scrolly, unsigned int layer) = 0;

	virtual void logic(Level *level) = 0;

	virtual bool isToBeDeleted() = 0;

    virtual unsigned int getType() = 0;

	virtual bool isCollidable();

	virtual void handleCollision(Entity *other, Level *level);

    virtual bool drawInLayer(unsigned int layer) = 0;

    static const int BACKGROUND_LAYER = 0;
    static const int PLAYER_LAYER = 1;
    static const int ENEMY_LAYER = 2;
    static const int EXPLOSION_LAYER = 3;
    static const int FOREGROUND_LAYER = 4;
	static const int NUM_LAYERS = 5;

    static const int ENEMY_TYPE = 0;
    static const int PLAYER_TYPE = 1;
    static const int DECORATION_TYPE = 2;
    static const int ENEMY_BULLET_TYPE = 3;
    static const int PLAYER_BULLET_TYPE = 4;

protected:
	int mX, mY, mW, mH;
	bool mCollidable;
};
#endif