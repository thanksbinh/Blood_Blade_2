#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game.h"
#include "LTexture.h"
#include "LTimer.h"
#include "Others.h"
#include "Particle.h"

class Player
{
public:
	static const int PLAYER_VEL = 20;

	Player(SDL_Renderer* gRenderer, LTexture& gRedTexture, SDL_Rect& camera);

	~Player();

	void handleEvent(SDL_Event& e, SDL_Rect& camera);

	void move();

	void updateVel(const int& x, const int& y);

	void render(LTexture& gPlayerTexture, SDL_Rect& camera);

	Point getPos() { return mPos; }

	SDL_Rect getCollider() { return mCollider; }

	int getForce() { return mForce; }

	//Shows the particles
	void renderParticles(SDL_Rect& camera);

private:
	SDL_Renderer* renderer;

	//The particles
	LTexture redTexture;
	Particle* particles[TOTAL_PARTICLES];

	//Player flip when change direction
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Player position
	Point mPos;

	//Player velocity
	int mVelX, mVelY;

	//Mouse stuff
	Point initPos;
	Point lastPos;
	bool isHold = false;
	LTimer mTime;

	//Remaining force
	int mForce = 0;

	//Collision box
	SDL_Rect mCollider;
};

#endif // !PLAYER_H_

