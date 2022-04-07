#include "Player.h"
#include "Others.h"
#include "LTimer.h"
#include <math.h>
#include <sstream>
#include <SDL_ttf.h>

Player::Player(SDL_Renderer* gRenderer, LTexture& gRedTexture, SDL_Rect& camera)
{
    renderer = gRenderer;

    mPos.x = LEVEL_WIDTH / 2;
    mPos.y = LEVEL_HEIGHT / 2;

    mCollider.w = PLAYER_WIDTH;
    mCollider.h = PLAYER_HEIGHT;

    mVelX = 0;
    mVelY = 0;

    redTexture = gRedTexture;
    //Initialize particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mPos.x - camera.x, mPos.y - camera.y, redTexture);
    }
}

Player::~Player()
{
    //Delete particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Player::updateVel(const int& x, const int& y)
{
    mVelX = PLAYER_VEL * (x * 1.0) / pytago(x, y);
    mVelY = PLAYER_VEL * (y * 1.0) / pytago(x, y);
}

void Player::handleEvent(SDL_Event& e, SDL_Rect& camera)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && !isHold)
    {
        SDL_GetMouseState(&initPos.x, &initPos.y);
        if (mPos.x - camera.x < initPos.x && initPos.x < mPos.x - camera.x + PLAYER_WIDTH && mPos.y - camera.y < initPos.y && initPos.y < mPos.y - camera.y + PLAYER_HEIGHT)
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
    if (mVelX < 0) flip = SDL_FLIP_HORIZONTAL;
    else if (mVelX > 0) flip = SDL_FLIP_NONE;

    if (isHold)
    {
        mForce = (mTime.getTicks() / 10) % 100;
    }
    else if (mForce > 0)
    {
        mPos.x += mVelX;
        mCollider.x = mPos.x;
        if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > LEVEL_WIDTH))
        {
            mVelX = -mVelX;
            mForce += PLAYER_VEL / 10;
        }

        mPos.y += mVelY;
        mCollider.y = mPos.y;
        if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > LEVEL_HEIGHT))
        {
            mVelY = -mVelY;
            mForce += PLAYER_VEL / 10;
        }

        mForce -= PLAYER_VEL / 10;
    }
    else
    {
        mForce = mVelX = mVelY = 0;
    }
}

void Player::renderParticles(SDL_Rect& camera)
{
    //Go through particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        //Delete and replace dead particles
        if (particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(mPos.x, mPos.y, redTexture);
        }
    }

    //Show particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i]->render(renderer, camera);
    }
}

void Player::render(LTexture& gPlayerTexture, SDL_Rect& camera)
{
    gPlayerTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, 0.0, 0, flip);
    //gPlayerTexture.render(renderer, mPos.x, mPos.y, NULL, 0.0, 0, flip);

    //Show particles on top of dot
    renderParticles(camera);
}


