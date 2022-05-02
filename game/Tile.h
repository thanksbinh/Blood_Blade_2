#pragma once
#ifndef TILE_H_
#define TILE_H_

#include <SDL.h>

#include "Game.h"
#include "LTexture.h"
#include "Collision.h"

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Renderer* gRenderer, LTexture& gTileTexture, SDL_Rect gTileClips[], const SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

#endif // !TILE_H_
