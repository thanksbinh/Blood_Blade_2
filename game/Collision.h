#pragma once
#ifndef COLLISION_H_
#define COLLISION_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "LTimer.h"
//#include "Tile.h"

int checkCollision(SDL_Rect a, SDL_Rect b);

#endif // !COLLISION_H_
