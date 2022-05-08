#pragma once
#ifndef ROGUE_H_
#define ROGUE_H_

#include "Enemy.h"

class Rogue : public Enemy
{
public:
	static const int ATTACK_SPEED = 2;
	static const int SPIN_SPEED = 15;

	Rogue();
	Rogue(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Rogue();

	void attack(Tile* tiles[]);

private:
	
};

#endif // !ROGUE_H_
