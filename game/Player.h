#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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
	static const int PLAYER_MAX_HP = 100;

	Player();
	Player(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Player();

	void init(SDL_Renderer* gRenderer, LTexture& gRedTexture);

	//Update velocity base on (initPos - lastPos)
	void handleEvent(SDL_Event& e, const SDL_Rect& camera);
	void updateVel(const int& x, const int& y);

	//Strength = attack range = damame/frame
	void updateStrength(const int& score);

	//Lose HP if (player not attacking, enemy attacking, collide with enemy)
	void react(const SDL_Rect& enemyCollider, const bool& enemyAttack);

	//Update position base on velocity, update force base on mouse hold time
	void move(Tile* tiles[]);
	void updateForce();

	//Centers the camera over the dot
	void setCamera(SDL_Rect& camera);

	//Render player, effects, sounds
	void render(LTexture& gPlayerTexture, LTexture& gRedTexture, LTexture& gBlueSlash, LTexture& gRedSword, LTexture& gRedCircle, const SDL_Rect& camera, Mix_Chunk* gSwordSlash);
	void renderParticles(LTexture& gRedTexture, const SDL_Rect& camera);

	//Effect when die
	void die();

	SDL_Rect getCollider() { return mCollider; }
	SDL_Rect getAttackCollider() { return mAttackCollider; }
	bool getIsAttack() { return (mVelX != 0) || (mVelY != 0); }
	bool getIsAlive() { return isAlive; }

private:
	SDL_Renderer* renderer;
	Particle* particles[TOTAL_PARTICLES];

	Point mPos;
	SDL_Rect mCollider;
	int mVelX, mVelY;
	int mForce;

	int mHP;
	int mStrength;
	SDL_Rect mAttackCollider;
	
	bool gotHit;
	bool isAlive;
	bool isAppear;

	//Mouse stuff
	Point initPos, lastPos;
	LTimer mTime;
	bool isHold = false;
	double swordAngle = 0;

	//Player flip when change direction
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

#endif // !PLAYER_H_

