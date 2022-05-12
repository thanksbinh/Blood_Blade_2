#include "Game_Utils.h"

void increaseAlpha(Uint8& alpha, const int& n)
{
	if (alpha + n <= 255)
	{
		alpha += n;
	}
	else alpha = 255;
}

void updateNumOfEnemy(const int& score, int& numOfEnemy1, int& numOfEnemy2, int& numOfEnemy3)
{
	if (score < 3)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 0;
		numOfEnemy3 = 0;
	}
	if (score >= 3)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 0;
		numOfEnemy3 = 0;
	}
	if (score >= 10)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 1;
		numOfEnemy3 = 0;
	}
	if (score >= 25)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 1;
		numOfEnemy3 = 1;
	}
	if (score >= 50)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 2;
		numOfEnemy3 = 2;
	}
	if (score >= 100)
	{
		numOfEnemy1 = TOTAL_ENEMY_1;
		numOfEnemy2 = TOTAL_ENEMY_2;
		numOfEnemy3 = TOTAL_ENEMY_3;
	}
}

void calculateRank(const int& score, int& rank, int gData[])
{
	if (rank == -1)
	{
		for (int i = TOTAL_DATA - 1; i >= 0; --i)
		{
			if (score >= gData[i])
			{
				rank = i;
				if (i != TOTAL_DATA - 1) gData[i + 1] = gData[i];
				gData[i] = score;
			}
		}
	}
}