#include "Player.h"
#include "Others.h"
#include "LTimer.h"
#include <math.h>
#include <sstream>
#include <SDL_ttf.h>

#define FORCE_SPEED 1/10
#define FORCE_SPEED_CAPABILITY 100
#define FORCE_LOSS 2
#define AREA_CLOSE_ATTACK 25

Player::Player(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera)
{
    renderer = gRenderer;

    mCollider.x = mPos.x = LEVEL_WIDTH / 2;
    mCollider.y = mPos.y = LEVEL_HEIGHT / 2;
    mCollider.w = PLAYER_WIDTH + AREA_CLOSE_ATTACK * 2;
    mCollider.h = PLAYER_HEIGHT + AREA_CLOSE_ATTACK * 2;

    mForce = mVelX = mVelY = 0;

    gotHit = false;
    isAlive = true;
    isAppear = true;
    mHP = PLAYER_MAX_HP;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mPos.x, mPos.y, gRedTexture);
    }
}

Player::~Player()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        delete particles[i];
    }
}

void Player::handleEvent(SDL_Event& e, const SDL_Rect& camera)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && !isHold)
    {
        SDL_GetMouseState(&initPos.x, &initPos.y);
        //Add arrow
        if (1 || mPos.x - camera.x < initPos.x && initPos.x < mPos.x - camera.x + PLAYER_WIDTH && mPos.y - camera.y < initPos.y && initPos.y < mPos.y - camera.y + PLAYER_HEIGHT)
        {
            mTime.start();
            isHold = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP && isHold) {
        isHold = false;
        SDL_GetMouseState(&lastPos.x, &lastPos.y);
        updateVel((initPos.x - lastPos.x), (initPos.y - lastPos.y));

        std::cerr << "Final remainVel " << mForce << std::endl;
    }
}

void Player::react(const SDL_Rect& enemyCollider)
{
    if (!isMoving() && checkCollision(mCollider, enemyCollider))
    {
        gotHit = true;
        mHP--;
        std::cerr << mVelX << " " << mVelY << std::endl;
    }

    if (mHP <= 0)
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

void Player::updateVel(const int& x, const int& y)
{
    mVelX = PLAYER_VEL * (x * 1.0) / pytago(x, y);
    mVelY = PLAYER_VEL * (y * 1.0) / pytago(x, y);
}

void Player::updateForce()
{
    if (isHold)
    {
        mForce = (mTime.getTicks() * FORCE_SPEED) % (2 * FORCE_SPEED_CAPABILITY);
        if (mForce > FORCE_SPEED_CAPABILITY) mForce = 2 * FORCE_SPEED_CAPABILITY - mForce;
    }
}

void Player::move()
{
    if (mVelX < 0) flip = SDL_FLIP_HORIZONTAL;
    else if (mVelX > 0) flip = SDL_FLIP_NONE;

    updateForce();

    if (mForce > 0)
    {
        mPos.x += mVelX;
        mCollider.x = mPos.x - AREA_CLOSE_ATTACK;
        
        mPos.y += mVelY;
        mCollider.y = mPos.y - AREA_CLOSE_ATTACK;

        if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > LEVEL_WIDTH))
        {
            mVelX = -mVelX;
            mForce += FORCE_LOSS;
        }

        if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > LEVEL_HEIGHT))
        {
            mVelY = -mVelY;
            mForce += FORCE_LOSS;
        }

        mForce -= FORCE_LOSS;
    }
    else
    {
        mForce = mVelX = mVelY = 0;
    }
}

void Player::renderParticles(LTexture& gRedTexture, const SDL_Rect& camera)
{
    for (int i = 0; i < mForce/5; ++i)
    {
        if (particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(mPos.x, mPos.y, gRedTexture);
        }
    }

    for (int i = 0; i < mForce/5; ++i)
    {
        particles[i]->render(renderer, camera);
    }
}

void Player::render(LTexture& gPlayerTexture, LTexture& gRedTexture, LTexture& gBlueSlash, const SDL_Rect& camera)
{
    if (isAlive)
    {
        gPlayerTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, 0.0, 0, flip);
        if (gotHit)
        {
            gBlueSlash.render(renderer, mPos.x - PLAYER_WIDTH / 2 - camera.x, mPos.y - PLAYER_HEIGHT / 2 - camera.y, NULL, 0.0, 0, flip);
            gotHit = false;
        }
    }
    else
    {
        //finality
    }
    renderParticles(gRedTexture, camera);
}


