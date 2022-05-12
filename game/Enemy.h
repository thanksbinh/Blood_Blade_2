#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

#include "Game_Base.h"
#include "LTexture.h"
#include "Player.h"
#include "Tile.h"
#include "Geometry.h"
#include "Particle.h"
#include "LTimer.h"

class Enemy
{
public:
	static const int ENEMY_VEL = 5;
	static const int ENEMY_MAX_HP = 6;
	static const int TIME_BEFORE_ATTACK = 250;
	static const int ATTACK_RANGE = 100;

	Enemy();
	Enemy(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	virtual ~Enemy();

	//Init enemy
	void init(SDL_Renderer* gRenderer, LTexture& gRedTexture);

	//Respawn enemy in wall tiles and not in camera
	virtual void respawn(Tile* tiles[], const SDL_Rect& camera);

	//Update enemy's stats
	virtual void react(Player& player);

	//Move close to player
	virtual void move(const SDL_Rect& pCollider, Tile* tiles[]);

	//Attack player
	virtual void attack(Tile* tiles[]) {}

	//Update enemy's velocity
	virtual void updateVel(const Point& pPos);

	//Render particles
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	//Render enemy, effects, sounds
	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound);

	//Render with weapon
	void render(LTexture& gEnemyTexture, LTexture& gRedTexture, LTexture& gRedSlashTexture, LTexture& gWeaponTexture, const SDL_Rect& camera, Mix_Chunk* gSwordSlashSound);

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
