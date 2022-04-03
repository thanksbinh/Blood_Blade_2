#include "Player.h"
#include "Others.h"
#include "LTimer.h"
#include <math.h>
#include <sstream>
#include <SDL_ttf.h>

Player::Player(SDL_Renderer* gRenderer)
{
    renderer = gRenderer;

    mPos.x = SCREEN_WIDTH / 2;
    mPos.y = SCREEN_HEIGHT / 2;

    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    mVelX = 0;
    mVelY = 0;
}

void Player::updateVel(const int& x, const int& y)
{
    mVelX = PLAYER_VEL * (x * 1.0) / pytago(x, y);
    mVelY = PLAYER_VEL * (y * 1.0) / pytago(x, y);
}

void Player::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && !isHold)
    {
        SDL_GetMouseState(&initPos.x, &initPos.y);
        if (0 < initPos.x - mPos.x && initPos.x - mPos.x < PLAYER_WIDTH && 0 < initPos.y - mPos.y && initPos.y - mPos.y < PLAYER_HEIGHT)
        {
            mTime.start();
            isHold = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP && isHold) {
        isHold = false;
        SDL_GetMouseState(&lastPos.x, &lastPos.y);
        updateVel((initPos.x - lastPos.x), (initPos.y - lastPos.y));

        std::cout << "Final remainVel " << mForce << std::endl;
    }
}

void Player::move()
{
    if (isHold)
    {
        mForce = (mTime.getTicks() / 10) % 100;
    }
    else if (mForce > 0)
    {
        mPos.x += mVelX;
        mCollider.x = mPos.x;
        if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > SCREEN_WIDTH))
        {
            mVelX = -mVelX;
            mForce += PLAYER_VEL / 10;
        }

        mPos.y += mVelY;
        mCollider.y = mPos.y;
        if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > SCREEN_HEIGHT))
        {
            mVelY = -mVelY;
            mForce += PLAYER_VEL / 10;
        }

        mForce -= PLAYER_VEL / 10;
    }
    else
    {
        mForce = 0;
        mVelX = 0;
        mVelY = 0;
    }
}

void Player::render(LTexture& gDotTexture)
{
    gDotTexture.render(renderer, mPos.x, mPos.y);
}

