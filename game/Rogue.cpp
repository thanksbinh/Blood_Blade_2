#include "Rogue.h"
#include "Enemy.h"
#include "Player.h"

Rogue::Rogue()
{
    mPos.set(0, 0);
    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;
}

Rogue::Rogue(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(0, 0);
    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
    }
}

Rogue::~Rogue()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Rogue::attack(Tile* tiles[])
{
    isAttack = true;

    mPos.x += mVelX * ATTACK_SPEED;
    mCollider.x = mAttackCollider.x = mPos.x;

    mPos.y += mVelY * ATTACK_SPEED;
    mCollider.y = mAttackCollider.y = mPos.y;

    if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > LEVEL_WIDTH) || touchesWall(mCollider, tiles) == TILE_LEFTRIGHT || touchesWall(mCollider, tiles) == TILE_CORNER)
    {
        mVelX = -mVelX;
    }

    if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > LEVEL_HEIGHT) || touchesWall(mCollider, tiles) == TILE_TOPBOTTOM || touchesWall(mCollider, tiles) == TILE_CORNER)
    {
        mVelY = -mVelY;
    }

    bodyAngle += SPIN_SPEED;
}

