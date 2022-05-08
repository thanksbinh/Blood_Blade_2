#pragma once
#ifndef ASSASSIN_H_
#define ASSASSIN_H_

#include "Enemy.h"

class Assassin : public Enemy
{
public:
	static const int ATTACK_RANGE = 240;
	static const int TIME_AFTER_ATTACK = 1000;
	static const int ATTACK_SPEED = 2;
	static const int SPIN_SPEED = 15;

	Assassin();
	Assassin(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Assassin();

	void move(const SDL_Rect& pCollider, Tile* tiles[]);
	void attack();

private:

};

#endif // !ASSASSIN_H_
