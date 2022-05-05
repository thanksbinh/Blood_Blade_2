#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <time.h>

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Tile constants
const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;
const int TOTAL_TILES = 768;
const int TOTAL_TILE_SPRITES = 8;

//The different tile sprites
const int TILE_0 = 0;
const int TILE_1 = 1;
const int TILE_2 = 2;
const int TILE_3 = 3;
const int TILE_4 = 4;
const int TILE_LEFTRIGHT = 5;
const int TILE_TOPBOTTOM = 6;
const int TILE_CORNER = 7;

//The dimensions of the player
static const int PLAYER_WIDTH = 40;
static const int PLAYER_HEIGHT = 40;

//The dimensions of the enemy
static const int ENEMY_WIDTH = 40;
static const int ENEMY_HEIGHT = 40;
static const int BULLET_SIZE = 5;

//Particle count
static const int TOTAL_PARTICLES = 20;

static const int TOTAL_ENEMY_1 = 3;
static const int TOTAL_ENEMY_2 = 3;
static const int TOTAL_ENEMY_3 = 3;

//Number of data integers
static const int TOTAL_DATA = 11;

static const int TIME_BEFORE_DISAPPEAR = 200;

#endif /* Game_h */
