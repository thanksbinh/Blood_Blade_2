#ifndef GeometryS_H_
#define GeometryS_H_

#include <SDL.h>
#include <iostream>
#include <math.h>

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

double distance(const Point& p1, const Point& p2);

bool checkCollision(SDL_Rect a, SDL_Rect b);

#endif // !Geometry_H_
