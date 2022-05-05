#pragma once
#ifndef ROGUE_H_
#define ROGUE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Game_Base.h"
#include "LTexture.h"
#include "Others.h"
#include "Particle.h"
#include "LTimer.h"
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
