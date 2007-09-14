#include "playerbullet.hpp"

PlayerBullet::PlayerBullet(int x, int y, float dx, float dy, unsigned int damage, float drawingAngle)
:Bullet(x, y, 5, 5, Entity::PLAYER_BULLET_TYPE, dx, dy, "bullet.bmp", 1)
{

}


