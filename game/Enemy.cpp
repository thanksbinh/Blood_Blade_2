#include "Enemy.h"

#include "Others.h"
#include "Player.h"

Enemy::Enemy()
{
    //Initialize the offsets
    mCollider.x = mPosX = rand() % SCREEN_WIDTH;
    mCollider.y = mPosY = rand() % SCREEN_HEIGHT;

    //Set collision box dimension
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    //Initialize the velocity
    //mVelX = 0;
    //mVelY = 0;
}

void Enemy::response(SDL_Rect playerCollider, SDL_Rect wall)
{
    if ((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, playerCollider) || checkCollision(mCollider, wall))
    {
        std::cout << "Enemy killed!" << std::endl;
        mCollider.x = mPosX = rand() % SCREEN_WIDTH;
        mCollider.y = mPosY = rand() % SCREEN_HEIGHT;
    }
}

void Enemy::render(SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render(gRenderer, mPosX, mPosY);
}

