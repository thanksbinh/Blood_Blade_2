#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game.h"
#include "LTexture.h"

class Enemy
{
public:
	//Maximum axis velocity of the dot
	//static const int PLAYER_VEL = 10;

	//Initializes the variables
	Enemy();

	//Takes key presses and adjusts the dot's velocity
	//void handleEvent(SDL_Event& e);
	void response(SDL_Rect playerCollider, SDL_Rect wall);

	//Moves the dot and checks collision
	//void move(SDL_Rect& wall);

	//Shows the dot on the screen
	void render(SDL_Renderer* gRenderer, LTexture& gDotTexture);

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	//int mVelX, mVelY;

	//Dot's collision box
	SDL_Rect mCollider;
};

#endif // !ENEMY_H_
