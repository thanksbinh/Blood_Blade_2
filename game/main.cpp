#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Game.h"
#include "LTexture.h"
#include "Others.h"
#include "Player.h"
#include "Enemy.h"
#include "Rogue.h"
#include "LTimer.h"
//#include "Tile.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

LTexture gBGTexture;
LTexture gPlayerTexture;
LTexture gRogueTexture;
LTexture gKnightTexture;

LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

LTexture gTimeTextTexture;
LTexture gPromptTextTexture;
LTexture gDataTextures[TOTAL_DATA];

//Data points
int gData[TOTAL_DATA];

//Effect, items
LTexture gRedDot;
LTexture gRedSlash;
LTexture gBlueSlash;
LTexture gRedSword;
LTexture gRedCircle;

//Music, sound effects
Mix_Music* gMusic = NULL;
Mix_Chunk* gSwordSlash = NULL;

bool init();
bool loadMedia(Tile* tiles[]);
void close(Tile* tiles[]);

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			//Text rendering color
			SDL_Color textColor = { 255, 255, 255, 255 };
			SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

			//Current input point
			int currentData = 0;

			//In memory text stream
			std::stringstream timeText;

			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//Score = enemy killed
			int score = 0;
			int rank = -1;

			Player player(gRenderer, gRedDot, camera);

			Enemy* enemy = new Rogue[TOTAL_ENEMY];
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
					//Handle key press
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_r:
							//Reset score board
							for (int i = TOTAL_DATA - 1; i >= 0; --i)
							{
								gData[i] = 0;
							}
							break;
						}
					}
					player.handleEvent(e, camera);
				}

				player.move(tileSet);
				player.setCamera(camera);

				for (int i = 0; i < TOTAL_ENEMY && i < score / 5 + 1; i++)
				{
					enemy[i].move(player.getCollider(), tileSet);

					player.react(enemy[i].getCollider(), enemy[i].getIsAttack());
					
					enemy[i].react(player.getCollider(), player.getIsAttack());
					
					//Respawn enemy
					if (!enemy[i].getIsAppear())
					{
						enemy[i].respawn(tileSet, camera);
						score++;
					}
				}

				//Text
				timeText.str("");
				timeText << " SCORE: " << score << " PLAYER'S HP: " << player.getHP();

				if (!gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Click, drag and release to move.", textColor))
				{
					printf("Failed to render text texture!\n");
				}
				if (!gTimeTextTexture.loadFromRenderedText(gRenderer, gFont, timeText.str().c_str(), textColor))
				{
					printf("Unable to render time texture!\n");
				}

				//Background music
				if (Mix_PlayingMusic() == 0)
				{
					Mix_PlayMusic(gMusic, -1);
				}

				//Draw
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render(gRenderer, gTileTexture, gTileClips, camera);
				}
				
				//Render background
				//gBGTexture.render(gRenderer, 0, 0, &camera);

				//Render player
				player.render(gPlayerTexture, gRedDot, gBlueSlash, gRedSword, gRedCircle, camera, gSwordSlash);

				//Render enemies
				for (int i = 0; i < TOTAL_ENEMY && i < score / 5 + 1; i++)
				{
					enemy[i].render(gRogueTexture, gRedDot, gRedSlash, camera, gSwordSlash);
				}

				//Render text
				gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
				gTimeTextTexture.render(gRenderer, (SCREEN_WIDTH - gTimeTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTimeTextTexture.getHeight()));
				if (!player.getIsAlive())
				{
					if (rank == -1)
					{
						//Rank calculation
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

					for (int i = 0; i < TOTAL_DATA; ++i)
					{
						if (i == rank) gDataTextures[i].loadFromRenderedText(gRenderer, gFont, std::to_string(gData[i]), highlightColor);
						else gDataTextures[i].loadFromRenderedText(gRenderer, gFont, std::to_string(gData[i]), textColor);
						
						gDataTextures[i].render(gRenderer, (SCREEN_WIDTH - gDataTextures[i].getWidth()) / 2, gPromptTextTexture.getHeight() + gDataTextures[0].getHeight() * i);
					}
				}
				
				SDL_RenderPresent(gRenderer);
			}
		}

		close(tileSet);
	}

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(Tile* tiles[])
{
	//Text rendering color
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

	//Loading success flag
	bool success = true;

	//Load background texture
	if (!gBGTexture.loadFromFile(gRenderer, "assets/bg.png", LEVEL_WIDTH, LEVEL_HEIGHT))
	{
		printf("Failed to load background texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile(gRenderer, "assets/tiles.png", 240 / 2, 480 / 2))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles, gTileClips))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Load red dot
	if (!gRedDot.loadFromFile(gRenderer, "assets/red.bmp", 5, 5))
	{
		printf("Failed to load red dot!\n");
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

	//Load player texture
	if (!gPlayerTexture.loadFromFile(gRenderer, "assets/player.png", PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}

	//Load rogue texture
	if (!gRogueTexture.loadFromFile(gRenderer, "assets/rogue.png", ENEMY_WIDTH, ENEMY_HEIGHT))
	{
		printf("Failed to load rogue texture!\n");
		success = false;
	}

	//Load knight texture
	if (!gKnightTexture.loadFromFile(gRenderer, "assets/knight.png", ENEMY_WIDTH, ENEMY_HEIGHT))
	{
		printf("Failed to load knight texture!\n");
		success = false;
	}

	//Set texture transparency
	gRedDot.setAlpha(192);
	gRedSlash.setAlpha(192);
	gBlueSlash.setAlpha(192);
	gRedCircle.setAlpha(96);

	//Open the font
	gFont = TTF_OpenFont("assets/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Open file for reading in binary
	SDL_RWops* file = SDL_RWFromFile("33_file_reading_and_writing/nums.bin", "r+b");

	//File does not exist
	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		//Create file for writing
		file = SDL_RWFromFile("33_file_reading_and_writing/nums.bin", "w+b");
		if (file != NULL)
		{
			printf("New file created!\n");

			//Initialize data
			for (int i = 0; i < TOTAL_DATA; ++i)
			{
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(gData[0]), 1);
			}

			//Close file handler
			SDL_RWclose(file);
		}
		else
		{
			printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
	}
	//File exists
	else
	{
		//Load data
		printf("Reading file...!\n");
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWread(file, &gData[i], sizeof(gData[0]), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}

	//Initialize data textures
	gDataTextures[0].loadFromRenderedText(gRenderer, gFont, std::to_string(gData[0]), highlightColor);
	for (int i = 1; i < TOTAL_DATA; ++i)
	{
		gDataTextures[i].loadFromRenderedText(gRenderer, gFont, std::to_string(gData[i]), textColor);
	}

	//Load music
	gMusic = Mix_LoadMUS("21_sound_effects_and_music/epic-cinematic-saga-trailer-myths.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effect
	gSwordSlash = Mix_LoadWAV("21_sound_effects_and_music/sword_slash.wav");
	if (gSwordSlash == NULL)
	{
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gBGTexture.free();
	gPlayerTexture.free();
	gRogueTexture.free();
	gKnightTexture.free();

	gTimeTextTexture.free();
	gPromptTextTexture.free();
	gRedDot.free();
	gRedSlash.free();
	gBlueSlash.free();
	gRedSword.free();
	gRedCircle.free();

	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile("33_file_reading_and_writing/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(gData[0]), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}

	//Free loaded images
	gPromptTextTexture.free();
	for (int i = 0; i < TOTAL_DATA; ++i)
	{
		gDataTextures[i].free();
	}

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Free the sound effects
	Mix_FreeChunk(gSwordSlash);
	gSwordSlash = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

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

