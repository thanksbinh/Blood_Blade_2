#include "Enemy.h"
#include "Player.h"

#define SPIN_SPEED 15

Enemy::Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    mVelX = mVelY = 0;

    spawn(camera);
}

Enemy::~Enemy()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    //Delete particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Enemy::spawn(const SDL_Rect& camera)
{
    gotHit = false;
    isAlive = true;
    isAppear = true;
    hasParticle = false;
    mHP = ENEMY_MAX_HP;

    do {
        mCollider.x = mPos.x = rand() % (LEVEL_WIDTH - PLAYER_WIDTH);
        mCollider.y = mPos.y = rand() % (LEVEL_HEIGHT - PLAYER_HEIGHT);
    } while (checkCollision(mCollider, camera));
}

void Enemy::react(const SDL_Rect& playerCollider, const bool playerIsMoving)
{
    if (playerIsMoving && checkCollision(mCollider, playerCollider))
    {
        gotHit = true;
        mHP--;

        std::cerr << "enemy's hp: " << mHP << std::endl;
    }

    if (!playerIsMoving && mHP <= 0)
    {
        isAlive = false;
        if (!mTime.isStarted()) mTime.start();
    }

    if (!isAlive && mTime.getTicks() > 1000)
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

    angle += SPIN_SPEED;
}

void Enemy::updateVel(const Point& playerPos)
{
    double x = playerPos.x - mPos.x;
    double y = playerPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}

void Enemy::move(const Point& playerPos)
{
    if (mVelX < 0) flip = SDL_FLIP_HORIZONTAL;
    else if (mVelX > 0) flip = SDL_FLIP_NONE;

    if (distance(playerPos, mPos) > 100 && mHP > 0)
    {
        updateVel(playerPos);
        angle = 0;

        mPos.x += mVelX;
        mCollider.x = mPos.x;

        mPos.y += mVelY;
        mCollider.y = mPos.y;
    }
    else
    {
        attack();
    }
}

void Enemy::renderParticles(LTexture& gRedTexture, const SDL_Rect& camera)
{
    if (!hasParticle) {
        for (int i = 0; i < TOTAL_PARTICLES; ++i)
        {
            particles[i] = new Particle(mPos.x, mPos.y, gRedTexture);
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
        gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, angle, 0, flip);
        if (gotHit)
        {
            gRedSlash.render(renderer, mPos.x - PLAYER_WIDTH / 2 - camera.x, mPos.y - PLAYER_HEIGHT / 2 - camera.y, NULL, 0.0, 0, flip);
            gotHit = false;
        }
    }
    else
    {
        renderParticles(gRedTexture, camera);
    }
}

