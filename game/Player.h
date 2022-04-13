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

	Player(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera);
	~Player();

	void handleEvent(SDL_Event& e, const SDL_Rect& camera);

	void move();
	void updateVel(const int& x, const int& y);

	void render(LTexture& gPlayerTexture, LTexture& gRedTexture, const SDL_Rect& camera);
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	Point getPos() { return mPos; }
	SDL_Rect getCollider() { return mCollider; }
	int getForce() { return mForce; }
	bool isMoving() { return (mForce > 0); }

private:
	SDL_Renderer* renderer;
	Particle* particles[TOTAL_PARTICLES];

	Point mPos;
	int mVelX, mVelY;
	int mForce = 0;
	SDL_Rect mCollider;

	//Mouse stuff
	Point initPos, lastPos;
	bool isHold = false;
	LTimer mTime;

	//Player flip when change direction
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif // !PLAYER_H_

