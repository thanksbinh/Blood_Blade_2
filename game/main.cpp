#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "LTexture.h"
#include "Game.h"
#include "Others.h"
#include "Player.h"
#include "Enemy.h"
#include "LTimer.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

LTexture gBGTexture;
LTexture gPlayerTexture;
LTexture gEnemyTexture;
LTexture gTimeTextTexture;
LTexture gPromptTextTexture;
LTexture gRedDot;
LTexture gRedSlash;
LTexture gBlueSlash;
LTexture gRedSword;
LTexture gRedCircle;

//const int WALKING_ANIMATION_FRAMES = 8;
//SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
//LTexture gSpriteSheetTexture;

bool init();
bool loadMedia();
void close();

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 255, 255, 255, 255 };

			//In memory text stream
			std::stringstream timeText;

			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//Score = enemy killed
			int score = 0;

			Player player(gRenderer, gRedDot, camera);
			Enemy* enemy = new Enemy[TOTAL_ENEMY];
			for (int i = 0; i < TOTAL_ENEMY; i++)
			{
				enemy[i].init(gRenderer, gRedDot, camera);
			}
			
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					player.handleEvent(e, camera);
				}
				player.move();
				for (int i = 0; i < TOTAL_ENEMY && i < score / 5 + 1; i++)
				{
					enemy[i].move(player.getCollider());

					player.react(enemy[i].getCollider());
					enemy[i].react(player.getCollider(), player.isMoving());
					
					//Respawn enemy 1s after dead
					if (!enemy[i].getIsAppear())
					{
						enemy[i].spawn(camera);
						score++;
					}
				}

				//Text
				timeText.str("");
				timeText << " SCORE: " << score << " PLAYER'S HP: " << player.getHP();

				if (!gTimeTextTexture.loadFromRenderedText(gRenderer, gFont, timeText.str().c_str(), textColor))
				{
					printf("Unable to render time texture!\n");
				}

				//Camera over the player
				camera.x = (player.getPos().x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
				camera.y = (player.getPos().y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

				if (camera.x < 0) camera.x = 0;
				if (camera.y < 0) camera.y = 0;
				if (camera.x > LEVEL_WIDTH - camera.w) camera.x = LEVEL_WIDTH - camera.w;
				if (camera.y > LEVEL_HEIGHT - camera.h) camera.y = LEVEL_HEIGHT - camera.h;

				//Draw
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				
				//Render background
				gBGTexture.render(gRenderer, 0, 0, &camera);

				//Render characters
				player.render(gPlayerTexture, gRedDot, gBlueSlash, gRedSword, gRedCircle, camera);
				for (int i = 0; i < TOTAL_ENEMY && i < score / 5 + 1; i++)
				{
					enemy[i].render(gEnemyTexture, gRedDot, gRedSlash, camera);
				}

				//Render text
				gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
				gTimeTextTexture.render(gRenderer, (SCREEN_WIDTH - gTimeTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTimeTextTexture.getHeight()));

				SDL_RenderPresent(gRenderer);
			}

			std::cerr << "Your score: " << score << std::endl;

		}
	}

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

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

	//Load background texture
	if (!gBGTexture.loadFromFile(gRenderer, "assets/bg.png", LEVEL_WIDTH, LEVEL_HEIGHT))
	{
		printf("Failed to load background texture!\n");
		success = false;
	}

	//Load red texture
	if (!gRedDot.loadFromFile(gRenderer, "assets/red.bmp", 5, 5))
	{
		printf("Failed to load red texture!\n");
		success = false;
	}

	//Load red slash
	if (!gRedSlash.loadFromFile(gRenderer, "assets/red-slash.png", PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2))
	{
		printf("Failed to load red slash!\n");
		success = false;
	}

	//Load blue slash
	if (!gBlueSlash.loadFromFile(gRenderer, "assets/blue-slash.png", PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2))
	{
		printf("Failed to load blue slash!\n");
		success = false;
	}

	//Load red sword
	if (!gRedSword.loadFromFile(gRenderer, "assets/red-sword.png", PLAYER_WIDTH * 3, PLAYER_HEIGHT * 3))
	{
		printf("Failed to load red sword!\n");
		success = false;
	}

	//Load red circle
	if (!gRedCircle.loadFromFile(gRenderer, "assets/red-circle.png", PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2))
	{
		printf("Failed to load red circle!\n");
		success = false;
	}

	//Load press texture
	if (!gPlayerTexture.loadFromFile(gRenderer, "assets/player.png", PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Set texture transparency
	gRedDot.setAlpha(192);
	gRedSlash.setAlpha(192);
	gBlueSlash.setAlpha(192);
	gRedCircle.setAlpha(96);

	if (!gEnemyTexture.loadFromFile(gRenderer, "assets/rogue.png", PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("assets/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Click, drag and release to move.", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	//gSpriteSheetTexture.free();

	//Free loaded images
	gBGTexture.free();
	gPlayerTexture.free();
	gEnemyTexture.free();
	gTimeTextTexture.free();
	gPromptTextTexture.free();

	gRedDot.free();
	gRedSlash.free();
	gBlueSlash.free();
	gRedSword.free();
	gRedCircle.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

