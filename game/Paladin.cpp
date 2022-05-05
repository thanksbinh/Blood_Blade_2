#include "Paladin.h"

Paladin::Paladin()
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

Paladin::Paladin(SDL_Renderer* gRenderer, LTexture& gRedTexture)
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

Paladin::~Paladin()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Paladin::react(Player& player)
{
    if (isAlive)
    {
        //Take damage
        if (player.getIsAttack() && checkCollision(mCollider, player.getAttackCollider()))
        {
            gotHit = true;
            mHP -= 2;
        }
        //Block player
        if (player.getIsAttack() && (mHP > 0) && checkCollision(mCollider, player.getCollider()))
        {
            player.pBlock();
        }
        //Die
        if (!player.getIsAttack() && mHP <= 0)
        {
            isAlive = false;
            for (int i = 0; i < TOTAL_PARTICLES; ++i)
            {
                particles[i]->reset(mCollider);
            }
        }
    }
    //Gone
    if (!isAlive && mTime.wait(TIME_BEFORE_DISAPPEAR))
    {
        isAppear = false;
        mTime.stop();
    }
}

void Paladin::updateVel(const Point& pPos)
{
    double x = pPos.x - mPos.x;
    double y = pPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}