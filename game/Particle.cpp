#include "Particle.h"

Particle::Particle(const SDL_Rect& area, LTexture& gRedTexture)
{
    //Set offsets
    mPosX = area.x + (rand() % area.w);
    mPosY = area.y + (rand() % area.h);

    //Initialize animation
    mFrame = rand() % 5;

    mTexture = &gRedTexture;
}

void Particle::reset(const SDL_Rect& area)
{
    //Set offsets
    mPosX = area.x + (rand() % area.w);
    mPosY = area.y + (rand() % area.h);
}

void Particle::render(SDL_Renderer* gRenderer, const SDL_Rect& camera)
{
    //Show image
    mTexture->render(gRenderer, mPosX - camera.x, mPosY - camera.y);

    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}