#ifndef OTHERS_H_
#define OTHERS_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include "LTimer.h"
#include "Tile.h"

struct Point
{
public:
	int x;
	int y;
	Point() { x = 0; y = 0; }
	Point(int _x, int _y) { x = _x; y = _y; }
	void set(int _x, int _y) { x = _x; y = _y; }
};

double pytago(int a, int b);

int distance(const Point& p1, const Point& p2);

//Checks collision box against set of tiles
int touchesWall(const SDL_Rect& box, Tile* tiles[]);

//Sets tiles from tile map
bool setTiles(Tile* tiles[], SDL_Rect gTileClips[]);

#endif // !OTHER_H_
