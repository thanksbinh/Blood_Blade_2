#include "Particle.h"

Particle::Particle(int x, int y, LTexture& gRedTexture)
{
    //Set offsets
    mPosX = x - 5 + (rand() % 25);
    mPosY = y - 5 + (rand() % 25);

    //Initialize animation
    mFrame = rand() % 5;

    mTexture = &gRedTexture;
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