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
	static const int ENEMY_VEL = 5;

	Enemy(SDL_Renderer* gRenderer);
	~Enemy();

	void spawn(const SDL_Rect& camera);

	void move(const Point& playerPos, const SDL_Rect& playerCollider);
	void updateVel(const Point& playerPos);

	int playerDis(const Point& playerPos);

	void render(LTexture& gEnemyTexture, LTexture& gRedSlash, const SDL_Rect& camera);

	bool getHit() { return gotHit; }

private:
	SDL_Renderer* renderer;

	Point mPos;
	int mVelX, mVelY;
	SDL_Rect mCollider;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool gotHit = false;
};

#endif // !ENEMY_H_
