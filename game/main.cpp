#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "LTexture.h"
#include "Game.h"
#include "Others.h"
#include "Player.h"
#include "Enemy.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture gPlayerTexture;

LTexture gEnemyTexture;

LTexture gBGTexture;

bool init();

bool loadMedia();

void close();

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Player player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
			Enemy enemy;

			//Set the wall
			SDL_Rect wall;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle input for the dot
					player.handleEvent(e);
				}

				//Move the dot and check collision
				player.move(wall);
				enemy.response(player.getCollider(), wall);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render wall
				//SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				//SDL_RenderDrawRect(gRenderer, &wall);

				//Render dot
				player.render(gRenderer, gPlayerTexture);
				enemy.render(gRenderer, gEnemyTexture);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if (!gPlayerTexture.loadFromFile(gRenderer, "assets/player.png", PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	if (!gEnemyTexture.loadFromFile(gRenderer, "assets/enemy.png", PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gPlayerTexture.free();
	gEnemyTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

