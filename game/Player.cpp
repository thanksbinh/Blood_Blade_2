#include "Player.h"
#include <math.h>

#define M_PI 3.14159265358979323846

Player::Player()
{
    mPos.set(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);
    mCollider = mAttackCollider = { mPos.x, mPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    mForce = mVelX = mVelY = 0;

    mHP = PLAYER_MAX_HP;
    mStrength = 0;
    scoreToUltimate = SCORE_TO_NEXT_ULTIMATE;

    gotHit = false;
    gotBlock = false;
    canUltimate = false;
    isUltimate = false;
    isAlive = true;
    isAppear = true;
}

void Player::init(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);
    mCollider = mAttackCollider = { mPos.x, mPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    mForce = mVelX = mVelY = 0;

    mHP = PLAYER_MAX_HP;
    mStrength = 0;
    scoreToUltimate = SCORE_TO_NEXT_ULTIMATE;

    gotHit = false;
    gotBlock = false;
    canUltimate = false;
    isUltimate = false;
    isAlive = true;
    isAppear = true;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mAttackCollider, gRedTexture);
    }
}

Player::Player(SDL_Renderer* gRenderer, LTexture& gRedTexture)
{
    renderer = gRenderer;

    mPos.set(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);
    mCollider = mAttackCollider = { mPos.x, mPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    mForce = mVelX = mVelY = 0;

    mHP = PLAYER_MAX_HP;
    mStrength = 0;
    scoreToUltimate = SCORE_TO_NEXT_ULTIMATE;

    gotHit = false;
    gotBlock = false;
    canUltimate = false;
    isUltimate = false;
    isAlive = true;
    isAppear = true;

    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mCollider, gRedTexture);
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
    swordAngle = SDL_atan2((initPos.y - lastPos.y), (initPos.x - lastPos.x)) * (180.0 / M_PI) + 90;
    
    if (e.type == SDL_MOUSEBUTTONDOWN && !isHold)
    {
        SDL_GetMouseState(&initPos.x, &initPos.y);
        SDL_Rect mPicture = { mPos.x - camera.x, mPos.y - camera.y, PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_Rect mouse = { initPos.x, initPos.y, 0, 0 };
        if (checkCollision(mPicture, mouse))
        {
            isHold = true;
            mTime.start();
        }
    }
    SDL_GetMouseState(&lastPos.x, &lastPos.y);
    if (e.type == SDL_MOUSEBUTTONUP && isHold) {
        updateVel((initPos.x - lastPos.x), (initPos.y - lastPos.y));
        isHold = false;
        mTime.stop();
    }

    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_SPACE:
            if (canUltimate) isUltimate = true;
            break;
        }
    }
}

void Player::updateVel(const int& x, const int& y)
{
    if (pytago(x, y) != 0) 
    {
        mVelX = round((double)PLAYER_VEL * x / pytago(x, y));
        mVelY = round((double)PLAYER_VEL * y / pytago(x, y));
    }
}

void Player::updateAttackCollider(const int& score)
{
    if (score >= scoreToUltimate)
    {
        canUltimate = true;
    }
    if (isUltimate)
    {
        mForce = FORCE_CAPABILITY;
        mAttackCollider = { 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT };

        if (mTime.wait(ULTIMATE_TIME))
        {
            scoreToUltimate += SCORE_TO_NEXT_ULTIMATE;
            canUltimate = false;
            isUltimate = false;
            mTime.stop();
            mHP = PLAYER_MAX_HP;
        }
    }
    else
    {
        mStrength = (score < PLAYER_MAX_STRENGTH) ? (score) : PLAYER_MAX_STRENGTH;
        mAttackCollider = { mPos.x - mStrength, mPos.y - mStrength, PLAYER_WIDTH + mStrength * 2, PLAYER_HEIGHT + mStrength * 2 };
    }
}

void Player::react(const bool& eIsAttack, const SDL_Rect& eAttackCollider)
{
    if (gotBlock)
    {
        mForce = 0;

        mPos.x -= mVelX;
        mCollider.x = mPos.x;

        mPos.y -= mVelY;
        mCollider.y = mPos.y;

        gotBlock = false;
    }
    if (!getIsAttack() && eIsAttack && checkCollision(mCollider, eAttackCollider))
    {
        gotHit = true;
        mHP--;
    }
    if (mHP <= 0)
    {
        mHP = 0;
        isAlive = false;
        mForce = FORCE_CAPABILITY;
    }
    if (!isAlive && mTime.wait(TIME_BEFORE_DISAPPEAR))
    {
        isAppear = false;
        mTime.stop();
    }
}

void Player::move(Tile* tiles[])
{
    if (isAlive)
    {
        if (mVelX < 0) bodyFlip = SDL_FLIP_HORIZONTAL;
        else if (mVelX > 0) bodyFlip = SDL_FLIP_NONE;

        updateForce();
        if (mForce > 0)
        {
            mPos.x += mVelX;
            mCollider.x = mPos.x;

            mPos.y += mVelY;
            mCollider.y = mPos.y;

            if ((mPos.x < 0) || (mPos.x + PLAYER_WIDTH > LEVEL_WIDTH) || touchesWall(mCollider, tiles) == TILE_LEFTRIGHT)
            {
                mVelX = -mVelX;
                mPos.x += mVelX;
            }

            if ((mPos.y < 0) || (mPos.y + PLAYER_HEIGHT > LEVEL_HEIGHT) || touchesWall(mCollider, tiles) == TILE_TOPBOTTOM)
            {
                mVelY = -mVelY;
                mPos.y += mVelY;
            }

            mForce -= FORCE_LOSS;
        }
        else
        {
            mForce = mVelX = mVelY = 0;
        }
    }
}

void Player::updateForce()
{
    if (isHold)
    {
        mForce = (mTime.getTicks() / FORCE_SLOW_DOWN) % (2 * FORCE_CAPABILITY);
        if (mForce > FORCE_CAPABILITY) mForce = 2 * FORCE_CAPABILITY - mForce;
    }
}

void Player::setCamera(SDL_Rect& camera)
{
    camera.x = (mPos.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mPos.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > LEVEL_WIDTH - camera.w)
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if (camera.y > LEVEL_HEIGHT - camera.h)
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

void Player::renderParticles(LTexture& gRedTexture, const SDL_Rect& camera)
{
    for (int i = 0; i < mForce/5 && i < TOTAL_PARTICLES; ++i)
    {
        if (particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(mAttackCollider, gRedTexture);
        }

        particles[i]->render(renderer, camera);
    }
}

void Player::render(LTexture& gPlayerTexture, LTexture& gRedTexture, LTexture& gBlueSlashTexture, LTexture& gWeaponTexture, LTexture& gRedCircleTexture, LTexture& gUltimateTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound)
{
    if (isAlive)
    {
        //Lose blood -> player lose red color
        double mHPpercent = (double)mHP / PLAYER_MAX_HP;
        gPlayerTexture.setColor(255 * mHPpercent, 255, 255);
        gPlayerTexture.render(renderer, mPos.x - camera.x, mPos.y - camera.y, NULL, 0.0, 0, bodyFlip);

        if (isHold)
        {
            //Show red cicle at first and present mouse position
            gRedCircleTexture.render(renderer, initPos.x - RED_CIRCLE_SIZE / 2, initPos.y - RED_CIRCLE_SIZE / 2);
            gRedCircleTexture.render(renderer, lastPos.x - RED_CIRCLE_SIZE / 2, lastPos.y - RED_CIRCLE_SIZE / 2);

            //Show player direction
            gWeaponTexture.render(renderer, mPos.x - camera.x - PLAYER_WIDTH, mPos.y - camera.y - PLAYER_HEIGHT, NULL, swordAngle);
        }
        if (canUltimate)
        {
            gUltimateTexture.render(renderer, SCREEN_MARGIN, SCREEN_MARGIN);
        }
        if (isUltimate)
        {
            //Sword rain
            for (int i = 0; i < 10; i++)
                gWeaponTexture.render(renderer, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, NULL, 180);
        }
        if (gotHit)
        {
            gBlueSlashTexture.render(renderer, mPos.x - camera.x - PLAYER_WIDTH / 2, mPos.y - camera.y - PLAYER_HEIGHT / 2, NULL, 0.0, 0, bodyFlip);
            Mix_PlayChannel(-1, gSwordSlashSound, 0);
            gotHit = false;
        }
    }
    
    renderParticles(gRedTexture, camera);
}


