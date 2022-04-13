#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(SDL_Renderer* gRenderer)
{
    renderer = gRenderer;

    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    mCollider.x = mPos.x = LEVEL_WIDTH / 2;
    mCollider.y = mPos.y = LEVEL_HEIGHT / 2 - 100;

    mVelX = mVelY = 0;
}

Enemy::~Enemy()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}

void Enemy::spawn(const SDL_Rect& camera)
{
    gotHit = false;

    do {
        mCollider.x = mPos.x = rand() % (LEVEL_WIDTH - PLAYER_WIDTH);
        mCollider.y = mPos.y = rand() % (LEVEL_HEIGHT - PLAYER_HEIGHT);
    } while (checkCollision(mCollider, camera));
}

void Enemy::updateVel(const Point& playerPos)
{
    double x = playerPos.x - mPos.x;
    double y = playerPos.y - mPos.y;

    mVelX = ENEMY_VEL * (x / pytago(x, y));
    mVelY = ENEMY_VEL * (y / pytago(x, y));
}

int Enemy::playerDis(const Point& playerPos)
{
    return pytago(playerPos.x - mPos.x, playerPos.y - mPos.y);
}

void Enemy::move(const Point& playerPos, const SDL_Rect& playerCollider)
{
    updateVel(playerPos);

    if (checkCollision(mCollider, playerCollider)) gotHit = true;

    if (playerDis(playerPos) > 100 && !gotHit)
    {
        if (mVelX < 0) flip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) flip = SDL_FLIP_NONE;

        mPos.x += mVelX;
        mCollider.x = mPos.x;

        mPos.y += mVelY;
        mCollider.y = mPos.y;
    }
}

void Enemy::render(LTexture& gEnemyTexture, LTexture& gRedSlash, const SDL_Rect& camera)
{
    gEnemyTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, 0.0, 0, flip);
    if (gotHit) gRedSlash.render(renderer, mPos.x - PLAYER_WIDTH / 2 - camera.x, mPos.y - PLAYER_HEIGHT / 2 - camera.y, NULL, 0.0, 0, flip);
}

