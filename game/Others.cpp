#include "Others.h"

double pytago(int a, int b)
{
	double c = sqrt(a * a + b * b);
	if (c) return c;
	return 1;
}

int distance(const Point& p1, const Point& p2)
{
	return pytago(p1.x - p2.x, p1.y - p2.y);
}

int touchesWall(const SDL_Rect& box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= TILE_LEFTRIGHT) && (tiles[i]->getType() <= TILE_CORNER))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return tiles[i]->getType();
			}
		}
	}

	//If no wall tiles were touched
	return 0;
}

bool setTiles(Tile* tiles[], SDL_Rect gTileClips[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("39_tiling/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_0].x = 0;
			gTileClips[TILE_0].y = 0;
			gTileClips[TILE_0].w = TILE_WIDTH;
			gTileClips[TILE_0].h = TILE_HEIGHT;

			gTileClips[TILE_1].x = TILE_WIDTH;
			gTileClips[TILE_1].y = 0;
			gTileClips[TILE_1].w = TILE_WIDTH;
			gTileClips[TILE_1].h = TILE_HEIGHT;

			gTileClips[TILE_2].x = TILE_WIDTH * 2;
			gTileClips[TILE_2].y = 0;
			gTileClips[TILE_2].w = TILE_WIDTH;
			gTileClips[TILE_2].h = TILE_HEIGHT;

			gTileClips[TILE_3].x = 0;
			gTileClips[TILE_3].y = TILE_HEIGHT;
			gTileClips[TILE_3].w = TILE_WIDTH;
			gTileClips[TILE_3].h = TILE_HEIGHT;

			gTileClips[TILE_4].x = TILE_WIDTH;
			gTileClips[TILE_4].y = TILE_HEIGHT;
			gTileClips[TILE_4].w = TILE_WIDTH;
			gTileClips[TILE_4].h = TILE_HEIGHT;

			gTileClips[TILE_LEFTRIGHT].x = 80;
			gTileClips[TILE_LEFTRIGHT].y = 140;
			gTileClips[TILE_LEFTRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_LEFTRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_TOPBOTTOM].x = 0;
			gTileClips[TILE_TOPBOTTOM].y = TILE_HEIGHT * 3;
			gTileClips[TILE_TOPBOTTOM].w = TILE_WIDTH;
			gTileClips[TILE_TOPBOTTOM].h = TILE_HEIGHT;

			gTileClips[TILE_CORNER].x = 0;
			gTileClips[TILE_CORNER].y = TILE_HEIGHT * 3;
			gTileClips[TILE_CORNER].w = TILE_WIDTH;
			gTileClips[TILE_CORNER].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}