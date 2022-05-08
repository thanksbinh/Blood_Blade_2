#include "Assassin.h"

Assassin::Assassin()
{
    mPos.set(0, 0);
    mCollider = mAttackCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    isAttack = false;
}

Assassin::Assassin(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(0, 0);
    mCollider = mAttackCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
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

Assassin::~Assassin()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Assassin::move(const SDL_Rect& pCollider, Tile* tiles[])
{
    if (isAlive)
    {
        Point pPos(pCollider.x, pCollider.y);

        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        if (distance(pPos, mPos) <= ATTACK_RANGE)
        {
            if (mTime.wait(TIME_AFTER_ATTACK))
            {
                updateVel(pPos);
                mAttackCollider = { mCollider.x + ENEMY_WIDTH / 3, mCollider.y + ENEMY_HEIGHT / 3, SHURIKEN_SIZE, SHURIKEN_SIZE };
                mTime.stop();
            }
            else
            {
                attack();
            }
        }
        else
        {
            isAttack = false;
            mTime.stop();

            updateVel(pPos);

            mPos.x += mVelX;
            mPos.y += mVelY;

            mCollider = { mPos.x, mPos.y, ENEMY_WIDTH, ENEMY_HEIGHT };
            mAttackCollider = { mCollider.x + ENEMY_WIDTH / 3, mCollider.y + ENEMY_HEIGHT / 3, SHURIKEN_SIZE, SHURIKEN_SIZE };
        }
    }
    else isAttack = false;
}

void Assassin::attack()
{
    isAttack = true;

    mAttackCollider.x += mVelX * ATTACK_SPEED;
    mAttackCollider.y += mVelY * ATTACK_SPEED;

    weaponAngle += SPIN_SPEED;
}
