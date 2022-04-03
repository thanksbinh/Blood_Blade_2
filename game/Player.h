#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game.h"
#include "LTexture.h"

class Player
{
public:
	//Maximum axis velocity of the dot
	static const int PLAYER_VEL = 10;

	//Initializes the variables
	Player(const int& x, const int& y);

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and checks collision
	void move(SDL_Rect& wall);

	//Shows the dot on the screen
	void render(SDL_Renderer* gRenderer, LTexture& gDotTexture);

	SDL_Rect getCollider()
	{
		return mCollider;
	}

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Remaining velocity
	int remainVel = 0;

	//Dot's collision box
	SDL_Rect mCollider;
};

#endif // !PLAYER_H_

