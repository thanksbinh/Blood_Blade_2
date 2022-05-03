#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game.h"
#include "LTexture.h"
#include "Others.h"
#include "Particle.h"
#include "LTimer.h"

#define TIME_BEFORE_DISAPPEAR 100

class Enemy
{
public:
	static const int ENEMY_VEL = 5;
	static const int ENEMY_MAX_HP = 5;
	static const int SPIN_SPEED = 15;
	static const int TIME_BEFORE_ATTACK = 100;
	static const int ATTACK_RANGE = 100;

	Enemy();
	Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Enemy();

	void init(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	void respawn(Tile* tiles[], const SDL_Rect& camera);

	//Lose HP if (player attacking, collide with player), die when player not attacking, disappear after 0.1s
	void react(const SDL_Rect& pAttackCollider, const bool& pIsAttacking);

	//Attack, move base on player's position
	void move(const SDL_Rect& pCollider, Tile* tiles[]);
	void attack(Tile* tiles[]);
	void updateVel(const Point& pPos);

	//Render enemy, effects, sounds
	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlash, const SDL_Rect& camera, Mix_Chunk* gSwordSlash);
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	SDL_Rect getCollider() { return mCollider; }
	bool getIsAppear() { return isAppear; }
	bool getIsAttack() { return isAttack; }

protected:
	SDL_Renderer* renderer;
	Particle* particles[TOTAL_PARTICLES];

	Point mPos;
	SDL_Rect mCollider;
	int mVelX, mVelY;

	int mHP;

	bool gotHit;
	bool isAlive;
	bool isAppear;
	bool hasParticle;
	bool isAttack;

	LTimer mTime;

	int bodyAngle = 0;
	SDL_RendererFlip bodyFlip = SDL_FLIP_NONE;
};

#endif // !ENEMY_H_
