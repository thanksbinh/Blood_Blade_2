#include "Game_Utils.h"

void rankCalculation(const int& score, int& rank, int gData[])
{
	if (rank == -1)
	{
		for (int i = TOTAL_DATA - 1; i >= 1; --i)
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