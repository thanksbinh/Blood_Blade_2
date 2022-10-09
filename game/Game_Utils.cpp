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
	if (score < LEVEL1)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 0;
		numOfEnemy3 = 0;
	}
	if (score >= LEVEL1)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 0;
		numOfEnemy3 = 0;
	}
	if (score >= LEVEL2)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 1;
		numOfEnemy3 = 0;
	}
	if (score >= LEVEL3)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 1;
		numOfEnemy3 = 0;
	}
	if (score >= LEVEL4)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 1;
		numOfEnemy3 = 1;
	}
	if (score >= LEVEL5)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 2;
		numOfEnemy3 = 1;
	}
	if (score >= LEVEL6)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 2;
		numOfEnemy3 = 2;
	}
	if (score >= LEVEL7)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 3;
		numOfEnemy3 = 2;
	}
	if (score >= LEVEL8)
	{
		numOfEnemy1 = 1;
		numOfEnemy2 = 3;
		numOfEnemy3 = 3;
	}
	if (score >= LEVEL9)
	{
		numOfEnemy1 = 2;
		numOfEnemy2 = 3;
		numOfEnemy3 = 3;
	}
	if (score >= LEVEL9)
	{
		numOfEnemy1 = 3;
		numOfEnemy2 = 3;
		numOfEnemy3 = 3;
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