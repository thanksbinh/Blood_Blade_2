#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game_Base.h"
#include "LTexture.h"
#include "Player.h"
#include "Others.h"
#include "Particle.h"
#include "LTimer.h"

class Enemy
{
public:
	static const int ENEMY_VEL = 5;
	static const int ENEMY_MAX_HP = 5;
	static const int TIME_BEFORE_ATTACK = 200;
	static const int ATTACK_RANGE = 100;

	Enemy();
	Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Enemy();

	void init(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	void respawn(Tile* tiles[], const SDL_Rect& camera);

	//Lose HP if (player attacking, collide with player), die when player not attacking, disappear after 0.1s
	virtual void react(Player& player);

	//Attack, move base on player's position
	virtual void move(const SDL_Rect& pCollider, Tile* tiles[]);
	virtual void attack(Tile* tiles[]);
	virtual void updateVel(const Point& pPos);

	//Render enemy, effects, sounds
	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound);
	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, LTexture& gWeaponTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound);
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	SDL_Rect getCollider() { return mCollider; }
	SDL_Rect getAttackCollider() { return mAttackCollider; }
	bool getIsAppear() { return isAppear; }
	bool getIsAttack() { return isAttack; }

protected:
	SDL_Renderer* renderer;
	Particle* particles[TOTAL_PARTICLES];

	Point mPos;
	SDL_Rect mCollider;
	int mVelX, mVelY;

	int mHP;
	SDL_Rect mAttackCollider;

	bool gotHit;
	bool isAlive;
	bool isAppear;
	bool isAttack;

	LTimer mTime;

	int bodyAngle = 0;
	int weaponAngle = 0;
	SDL_RendererFlip bodyFlip = SDL_FLIP_NONE;
};

#endif // !ENEMY_H_
