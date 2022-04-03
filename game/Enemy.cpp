#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    spawn();

    //mVelX = 0;
    //mVelY = 0;
}

void Enemy::spawn()
{
    mCollider.x = mPos.x = rand() % (SCREEN_WIDTH - PLAYER_WIDTH);
    mCollider.y = mPos.y = rand() % (SCREEN_HEIGHT - PLAYER_HEIGHT);
}

void Enemy::response(SDL_Rect playerCollider)
{
    if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, playerCollider))
    {
        std::cout << "Enemy killed!" << std::endl;
        spawn();
    }
}

void Enemy::render(SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    gDotTexture.render(gRenderer, mPos.x, mPos.y);
}

