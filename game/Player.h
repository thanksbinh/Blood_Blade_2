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

class Player
{
public:
	static const int PLAYER_VEL = 20;

	Player(SDL_Renderer* gRenderer);

	void handleEvent(SDL_Event& e);

	void move();

	void updateVel(const int& x, const int& y);

	void render(LTexture& gDotTexture);

	Point getPos() { return mPos; }

	SDL_Rect getCollider() { return mCollider; }

	int getForce() { return mForce; }

private:
	SDL_Renderer* renderer;

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

