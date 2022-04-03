#ifndef OTHERS_H_
#define OTHERS_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

struct Point
{
public:
	int x;
	int y;
	Point() { x = 0; y = 0; }
	Point(int _x, int _y) { x = _x; y = _y; }
};

bool checkCollision(SDL_Rect a, SDL_Rect b);

double pytago(int a, int b);

#endif // !OTHER_H_
