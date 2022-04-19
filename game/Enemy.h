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
#include "Particle.h"
#include "LTimer.h"

class Enemy
{
public:
	static const int ENEMY_VEL = 5;
	static const int ENEMY_MAX_HP = 7;

	Enemy();
	Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture, const SDL_Rect& camera);
	~Enemy();

	void spawn(const SDL_Rect& camera);

	void react(const SDL_Rect& playerCollider, const bool playerIsMoving);
	void attack();

	void move(const Point& playerPos);
	void updateVel(const Point& playerPos);

	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlash, const SDL_Rect& camera);
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	SDL_Rect getCollider() { return mCollider; }
	bool getIsAlive() { return isAlive; }
	bool getHit() { return gotHit; }
	bool getIsAppear() { return isAppear; }

private:
	SDL_Renderer* renderer;
	Particle* particles[TOTAL_PARTICLES];

	Point mPos;
	int mVelX, mVelY;
	SDL_Rect mCollider;
	int angle = 0;

	int mHP;
	bool gotHit;
	bool isAlive;
	bool isAppear;
	bool hasParticle;

	LTimer mTime;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif // !ENEMY_H_
