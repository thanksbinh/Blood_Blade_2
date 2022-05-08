#pragma once
#ifndef PALADIN_H_
#define PALADIN_H_

#include "Enemy.h"

class Paladin : public Enemy
{
public:
	static const int ENEMY_VEL = 3;

	Paladin();
	Paladin(SDL_Renderer* gRenderer, LTexture& gRedTexture);
	~Paladin();
	
	void react(Player& player);

	void updateVel(const Point& pPos);

private:

};

#endif // !PALADIN_H_
