#include "Rogue.h"
#include "Enemy.h"
#include "Player.h"

Rogue::Rogue()
{
    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;
}

Rogue::Rogue(SDL_Renderer* gRenderer, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;

    do {
        mCollider.x = mPos.x = rand() % (LEVEL_WIDTH - ENEMY_WIDTH);
        mCollider.y = mPos.y = rand() % (LEVEL_HEIGHT - ENEMY_HEIGHT);
    } while (checkCollision(mCollider, camera));
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

