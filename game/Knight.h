#pragma once
#ifndef KNIGHT_H_
#define KNIGHT_H_

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
#include "Enemy.h"

class Knight : public Enemy
{
public:
	static const int ATTACK_RANGE = 240;

	Knight();
	Knight(SDL_Renderer* gRenderer, const SDL_Rect& camera);
	~Knight();

	void move(const SDL_Rect& playerCollider);
	void attack();

private:

};

#endif // !KNIGHT_H_
