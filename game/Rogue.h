#pragma once
#ifndef ROGUE_H_
#define ROGUE_H_

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

class Rogue : public Enemy
{
public:
	Rogue();
	Rogue(SDL_Renderer* gRenderer, const SDL_Rect& camera);
	~Rogue();

private:
	
};

#endif // !ROGUE_H_
