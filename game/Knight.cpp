#include "Knight.h"
#include "Enemy.h"
#include "Player.h"

Knight::Knight()
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

Knight::Knight(SDL_Renderer* gRenderer, const SDL_Rect& camera)
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

Knight::~Knight()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Knight::move(const SDL_Rect& playerCollider)
{
    if (isAlive)
    {
        Point playerPos(playerCollider.x, playerCollider.y);

        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        if (distance(playerPos, mPos) <= ATTACK_RANGE)
        {
            if (!mTime.isStarted()) mTime.start();
            if (mTime.getTicks() > TIME_BEFORE_ATTACK)
            {
                attack();
            }
        }
        else
        {
            isAttack = false;
            bodyAngle = 0;

            //Move close to player
            updateVel(playerPos);

            mPos.x += mVelX;
            mCollider.x = mPos.x;

            mPos.y += mVelY;
            mCollider.y = mPos.y;

            mTime.stop();
        }
    }
    else
    {
        isAttack = false;
    }
}

void Knight::attack()
{
    isAttack = true;

    bodyAngle += SPIN_SPEED;
}

