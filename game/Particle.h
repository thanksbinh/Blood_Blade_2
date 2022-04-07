#pragma once
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "LTexture.h"

class Particle
{
public:
	//Initialize position and animation
	Particle(int x, int y, LTexture& gRedTexture);

	//Shows the particle
	void render(SDL_Renderer* gRenderer, SDL_Rect& camera);

	//Checks if particle is dead
	bool isDead();

private:
	//Offsets
	int mPosX, mPosY;

	//Current frame of animation
	int mFrame;

	//Type of particle
	LTexture* mTexture;
};

#endif /* PARTICLE_H_ */