#include "Enemy.h"
#include "Player.h"

#define SPIN_SPEED 15
#define TIME_BEFORE_DISAPPEAR 500
#define TIME_BEFORE_ATTACK 500
#define PLAYER_SWORD_AREA 25

Enemy::Enemy() {}

Enemy::Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mCollider = { mPos.x, mPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    mVelX = mVelY = 0;

    spawn(camera);
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

void Enemy::init(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mCollider = { mPos.x, mPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    mVelX = mVelY = 0;

    spawn(camera);
}

void Enemy::spawn(const SDL_Rect& camera)
{
    mHP = ENEMY_MAX_HP;
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;

    do {
        mCollider.x = mPos.x = rand() % (LEVEL_WIDTH - PLAYER_WIDTH);
        mCollider.y = mPos.y = rand() % (LEVEL_HEIGHT - PLAYER_HEIGHT);
    } while (checkCollision(mCollider, camera));
}

void Enemy::react(const SDL_Rect& playerCollider, const bool playerIsMoving)
{
    SDL_Rect playerAttackCollider = { playerCollider.x - PLAYER_SWORD_AREA, playerCollider.y - PLAYER_SWORD_AREA, 
                                      playerCollider.w + 2 * PLAYER_SWORD_AREA, playerCollider.h + 2 * PLAYER_SWORD_AREA };

    if (isAlive)
    {
        if (playerIsMoving && checkCollision(mCollider, playerAttackCollider))
        {
            gotHit = true;
            mHP--;

            std::cerr << "enemy's hp: " << mHP << std::endl;
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

void Enemy::attack()
{
    mPos.x += mVelX * 2;
    mCollider.x = mPos.x;

    mPos.y += mVelY * 2;
    mCollider.y = mPos.y;

    bodyAngle += SPIN_SPEED;
}

void Enemy::updateVel(const Point& playerPos)
{
    double x = playerPos.x - mPos.x;
    double y = playerPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}

void Enemy::move(const SDL_Rect& playerCollider)
{
    if (isAlive)
    {
        Point playerPos(playerCollider.x, playerCollider.y);

        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        if (distance(playerPos, mPos) <= 100)
        {
            if (!mTime.isStarted()) mTime.start();
            if (mTime.getTicks() > TIME_BEFORE_ATTACK)
            {
                attack();
            }
            
        }
        else
        {
            //Move close to player
            updateVel(playerPos);
            bodyAngle = 0;

            mPos.x += mVelX;
            mCollider.x = mPos.x;

            mPos.y += mVelY;
            mCollider.y = mPos.y;

            mTime.stop();
        }
    }
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
            gRedSlash.render(renderer, mPos.x - PLAYER_WIDTH / 2 - camera.x, mPos.y - PLAYER_HEIGHT / 2 - camera.y, NULL, 0.0, 0, bodyFlip);
            gotHit = false;
        }
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
}

