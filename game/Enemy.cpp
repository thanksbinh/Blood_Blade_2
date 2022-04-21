#include "Enemy.h"
#include "Player.h"

#define SPIN_SPEED 15
#define TIME_BEFORE_DISAPPEAR 100
#define PLAYER_SWORD_AREA 25

Enemy::Enemy() 
{
    mPos.init(0, 0);
    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;
}

Enemy::Enemy(SDL_Renderer* gRenderer, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mPos.init(0, 0);
    mCollider = { 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT };
    mVelX = mVelY = 0;

    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;
}

Enemy::~Enemy()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Enemy::init(SDL_Renderer* gRenderer, const SDL_Rect& camera)
{
    renderer = gRenderer;
}

void Enemy::respawn(const SDL_Rect& camera)
{
    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    isAttack = false;

    do {
        mCollider.x = rand() % (LEVEL_WIDTH - ENEMY_WIDTH);
        mCollider.y = rand() % (LEVEL_HEIGHT - ENEMY_HEIGHT);
    } while (checkCollision(mCollider, camera));
    mPos.init(mCollider.x, mCollider.y);
}

void Enemy::react(const SDL_Rect& playerCollider, const bool& playerIsMoving)
{
    SDL_Rect playerAttackCollider = { playerCollider.x - PLAYER_SWORD_AREA, playerCollider.y - PLAYER_SWORD_AREA, 
                                      playerCollider.w + 2 * PLAYER_SWORD_AREA, playerCollider.h + 2 * PLAYER_SWORD_AREA };

    if (isAlive)
    {
        if (playerIsMoving && checkCollision(mCollider, playerAttackCollider))
        {
            gotHit = true;
            mHP--;
        }

        if (!playerIsMoving && mHP <= 0)
        {
            isAlive = false;
            mTime.start();
        }
    }
    else if (mTime.getTicks() > TIME_BEFORE_DISAPPEAR)
    {
        isAppear = false;
        mTime.stop();
    }
}

void Enemy::move(const SDL_Rect& playerCollider)
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

void Enemy::updateVel(const Point& playerPos)
{
    double x = playerPos.x - mPos.x;
    double y = playerPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}

void Enemy::attack()
{
    isAttack = true;

    mPos.x += mVelX * 2;
    mCollider.x = mPos.x;

    mPos.y += mVelY * 2;
    mCollider.y = mPos.y;

    bodyAngle += SPIN_SPEED;
}

void Enemy::renderParticles(LTexture& gRedTexture, const SDL_Rect& camera)
{
    if (!hasParticle) {
        for (int i = 0; i < TOTAL_PARTICLES; ++i)
        {
            particles[i] = new Particle(mCollider, gRedTexture);
        }
        hasParticle = true;
    }
    for (int i = 0; i < 20; ++i)
    {
        if (!particles[i]->isDead()) particles[i]->render(renderer, camera);
    }
}

void Enemy::render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlash, const SDL_Rect& camera)
{
    if (isAlive)
    {
        gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, bodyAngle, 0, bodyFlip);
        if (gotHit)
        {
            gRedSlash.render(renderer, mPos.x - ENEMY_WIDTH / 2 - camera.x, mPos.y - ENEMY_HEIGHT / 2 - camera.y, NULL, 0.0, 0, bodyFlip);
            gotHit = false;
        }
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
}

