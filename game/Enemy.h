#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game.h"
#include "LTexture.h"
#include "Others.h"

class Enemy
{
public:
	//static const int PLAYER_VEL = 10;

	Enemy(SDL_Renderer* gRenderer);

	void spawn();

	//void handleEvent(SDL_Event& e);

	void response(SDL_Rect playerCollider);

	//void move(SDL_Rect& wall);

	void render(LTexture& gDotTexture, SDL_Rect& camera);

private:
	SDL_Renderer* renderer;

	//Enemy position
	Point mPos;

	//int mVelX, mVelY;

	SDL_Rect mCollider;
};

#endif // !ENEMY_H_
