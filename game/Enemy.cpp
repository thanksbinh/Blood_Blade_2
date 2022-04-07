#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(SDL_Renderer* gRenderer)
{
    renderer = gRenderer;

    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    spawn();

    //mVelX = 0;
    //mVelY = 0;
}

void Enemy::spawn()
{
    mCollider.x = mPos.x = rand() % (LEVEL_WIDTH - PLAYER_WIDTH);
    mCollider.y = mPos.y = rand() % (LEVEL_HEIGHT - PLAYER_HEIGHT);
}

void Enemy::response(SDL_Rect playerCollider)
{
    if (checkCollision(mCollider, playerCollider))
    {
        std::cout << "Enemy killed!" << std::endl;
        spawn();
    }
}

void Enemy::render(LTexture& gDotTexture, SDL_Rect& camera)
{
    gDotTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y);
}

