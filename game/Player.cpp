#include "Player.h"
#include "Others.h"
#include <math.h>

Player::Player(const int& x, const int& y)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Set collision box dimension
    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

double pytago(int a, int b)
{
    double c = sqrt(a * a + b * b);
    return c;
}

void Player::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int xt1, yt1;
        SDL_GetMouseState(&xt1, &yt1);
        if (0 < xt1 - mPosX && xt1 - mPosX < PLAYER_WIDTH && 0 < yt1 - mPosY && yt1 - mPosY < PLAYER_HEIGHT)
        {
            int xt2, yt2;
            while (true) {
                SDL_Delay(10);
                SDL_PollEvent(&e);

                if (e.type == SDL_MOUSEBUTTONUP) {
                    SDL_GetMouseState(&xt2, &yt2);
                    break;
                }
            }

            mVelX = PLAYER_VEL * (xt1 - xt2) * 1.0 / pytago((xt1 - xt2), (yt1 - yt2));
            mVelY = PLAYER_VEL * (yt1 - yt2) * 1.0 / pytago((xt1 - xt2), (yt1 - yt2));
            remainVel = 10;

            std::cout << "x = " << mPosX << ", y = " << mPosY << std::endl;
        }
    }
    else
    {
        if (remainVel > 0)
        {
            remainVel--;
            std::cout << "remainVel = " << remainVel << std::endl;
        }
        else
        {
            mVelX = 0;
            mVelY = 0;
        }
    }

    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY -= PLAYER_VEL; break;
        case SDLK_DOWN: mVelY += PLAYER_VEL; break;
        case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
        case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY += PLAYER_VEL; break;
        case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
        case SDLK_LEFT: mVelX += PLAYER_VEL; break;
        case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
        }
    }
}

void Player::move(SDL_Rect& wall)
{
    //Move the dot left or right
    mPosX += mVelX;
    mCollider.x = mPosX;

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH))
    {
        //Change direction
        mVelX *= -1;
    }

    //Move the dot up or down
    mPosY += mVelY;
    mCollider.y = mPosY;

    //If the dot went too far up or down
    if ((mPosY < 0) || (mPosY + PLAYER_HEIGHT > SCREEN_HEIGHT))
    {
        //Change direction
        mVelY *= -1;
    }
}

void Player::render(SDL_Renderer* gRenderer, LTexture& gDotTexture)
{
    //Show the dot
    gDotTexture.render(gRenderer, mPosX, mPosY);
}

