#pragma once
#ifndef GAME_UTILS_H_
#define GAME_UTILS_H_

#include "Game_Base.h"
#include "Tile.h"

bool init();

bool loadMedia(Tile* tiles[]);

void close(Tile* tiles[]);

void increaseAlpha(Uint8& alpha, const int& n);

void updateNumOfEnemy(const int& score, int& numOfEnemy1, int& numOfEnemy2, int& numOfEnemy3);

void calculateRank(const int& score, int& rank, int gData[]);

void renderMenu(SDL_Rect& camera, Uint8& alpha, Tile* tileSet[], bool& menuHelp, bool& menuCredit);

void renderPauseScreen();

void renderEndScreen(const int& rank);

#endif