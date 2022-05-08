#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

#include "Game_Base.h"
#include "Game_Utils.h"
#include "LTexture.h"
#include "Tile.h"
#include "Player.h"
#include "Enemy.h"
#include "Rogue.h"
#include "Paladin.h"
#include "Assassin.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
const SDL_Color gTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
const SDL_Color gHighlightColor = { 0xFF, 0, 0, 0xFF };

//Screen texture
LTexture gMenuTexture;
LTexture gMenuHelpTexture;
LTexture gMenuCreditTexture;
LTexture gGameOverTexture;

//Characters texture
LTexture gPlayerTexture;
LTexture gRogueTexture;
LTexture gPaladinTexture;
LTexture gAssassinTexture;

//Effect, items texture
LTexture gRedDotTexture;
LTexture gRedSlashTexture;
LTexture gBlueSlashTexture;
LTexture gRedSwordTexture;
LTexture gShurikenTexture;
LTexture gRedCircleTexture;
LTexture gUltimateTexture;

//Music, sound effects
Mix_Music* gOpeningMusic = NULL;
Mix_Music* gBattleMusic = NULL;
Mix_Chunk* gSwordSlashSound = NULL;
Mix_Chunk* gSwordSlashMetalSound = NULL;

//Tiles
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

//Text
LTexture gScoreTextTexture;
LTexture gPromptTextTexture;
LTexture gRankDataTextures[TOTAL_DATA];
int gRankData[TOTAL_DATA];

//Characters
Player player;
Rogue* enemy1 = new Rogue[TOTAL_ENEMY_1];
Paladin* enemy2 = new Paladin[TOTAL_ENEMY_2];
Assassin* enemy3 = new Assassin[TOTAL_ENEMY_3];

int main(int argc, char* args[])
{
	srand(time(0));

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Tile* tileSet[TOTAL_TILES];

		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool gameIsRunning = true;
			bool gameInMenu = true;

			while (gameIsRunning)
			{
				SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

				SDL_Event e;

				Uint8 alpha = 0;

				bool gameOver = false;
				bool gamePause = false;

				bool menuHelp = false;
				bool menuCredit = false;

				Mix_PlayMusic(gOpeningMusic, -1);

				while (gameInMenu)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							//Quit game
							gameInMenu = false;
							gameOver = true;
							gameIsRunning = false;
						}
						else if (e.type == SDL_KEYDOWN)
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_h:
								//Help menu
								menuHelp = !menuHelp;
								menuCredit = false;
								break;
							case SDLK_c:
								//Credit menu
								menuCredit = !menuCredit;
								menuHelp = false;
								break;
							}
						}
						else if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							if (menuHelp || menuCredit)
							{
								menuHelp = false;
								menuCredit = false;
							}
							else
							{
								gameInMenu = false;
								alpha = 0;
							}
						}
					}

					//Motion
					increaseAlpha(alpha, 3);
					camera.x = (camera.x + 1) % (LEVEL_WIDTH - SCREEN_WIDTH);

					//Draw
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					gTileTexture.setAlpha(alpha);
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tileSet[i]->render(gRenderer, gTileTexture, gTileClips, camera);
					}

					gMenuTexture.setAlpha(alpha);
					gMenuTexture.render(gRenderer, 0, 0);

					if (menuHelp) gMenuHelpTexture.render(gRenderer, 0, 0);
					if (menuCredit) gMenuCreditTexture.render(gRenderer, 0, 0);

					SDL_RenderPresent(gRenderer);
				}

				int score = 0;
				int rank = -1;

				std::stringstream scoreText;

				int numOfEnemy1 = 0;
				int numOfEnemy2 = 0;
				int numOfEnemy3 = 0;

				player.init(gRenderer, gRedDotTexture);
				for (int i = 0; i < TOTAL_ENEMY_1; i++)
				{
					enemy1[i].init(gRenderer, gRedDotTexture);
				}
				for (int i = 0; i < TOTAL_ENEMY_2; i++)
				{
					enemy2[i].init(gRenderer, gRedDotTexture);
				}
				for (int i = 0; i < TOTAL_ENEMY_3; i++)
				{
					enemy3[i].init(gRenderer, gRedDotTexture);
				}

				Mix_PlayMusic(gBattleMusic, -1);

				while (!gameOver)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							//Quit game
							gameOver = true;
							gameIsRunning = false;
						}
						else if (e.type == SDL_KEYDOWN && player.getIsAppear())
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_r:
								//Reset game
								gameOver = true;
								break;
							case SDLK_p:
								//Pause, unpause game
								gamePause = !gamePause;
								break;
							case SDLK_q:
								//Quit to menu
								gameOver = true;
								gameInMenu = true;
								break;
							}
						}
						else if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							if (!player.getIsAppear())
							{
								gameOver = true;
								gameInMenu = true;
							}
							if (gamePause)
								gamePause = false;
						}
						if (!gamePause)
							player.handleEvent(e, camera);
					}

					if (gamePause)
					{
						Mix_PauseMusic();
						gamePauseScreen();
					}
					else if (!player.getIsAppear())
					{
						rankCalculation(score, rank, gRankData);
						gameEndScreen(rank);
					}
					else
					{
						if (Mix_PausedMusic() == 1) Mix_ResumeMusic();

						increaseAlpha(alpha, 3);

						updateNumOfEnemy(score, numOfEnemy1, numOfEnemy2, numOfEnemy3);

						player.setCamera(camera);
						player.updateStrength(score);
						player.move(tileSet);

						for (int i = 0; i < numOfEnemy1; i++)
						{
							if (enemy1[i].getIsAppear())
							{
								player.react(enemy1[i].getIsAttack(), enemy1[i].getAttackCollider());

								enemy1[i].move(player.getCollider(), tileSet);
								enemy1[i].react(player);
							}
							else
							{
								enemy1[i].respawn(tileSet, camera);
								score++;
							}
						}

						for (int i = 0; i < numOfEnemy2; i++)
						{
							if (enemy2[i].getIsAppear())
							{
								player.react(enemy2[i].getIsAttack(), enemy2[i].getAttackCollider());

								enemy2[i].move(player.getCollider(), tileSet);
								enemy2[i].react(player);
							}
							else
							{
								enemy2[i].respawn(tileSet, camera);
								score++;
							}
						}

						for (int i = 0; i < numOfEnemy3; i++)
						{
							if (enemy3[i].getIsAppear())
							{
								player.react(enemy3[i].getIsAttack(), enemy3[i].getAttackCollider());

								enemy3[i].move(player.getCollider(), tileSet);
								enemy3[i].react(player);
							}
							else
							{
								enemy3[i].respawn(tileSet, camera);
								score++;
							}
						}
						
						//Draw to screen
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						//Render level
						gTileTexture.setAlpha(alpha);
						for (int i = 0; i < TOTAL_TILES; ++i)
						{
							tileSet[i]->render(gRenderer, gTileTexture, gTileClips, camera);
						}

						//Calculate score (= enemy killed)
						scoreText.str("");
						scoreText << "YOUR SCORE: " << score;

						//Render score text
						gScoreTextTexture.loadFromRenderedText(gRenderer, gFont, scoreText.str().c_str(), gTextColor);
						gScoreTextTexture.render(gRenderer, SCREEN_WIDTH - (gScoreTextTexture.getWidth() + SCREEN_MARGIN), SCREEN_MARGIN);

						//Render characters
						gPlayerTexture.setAlpha(alpha);
						player.render(gPlayerTexture, gRedDotTexture, gBlueSlashTexture, gRedSwordTexture, gRedCircleTexture, gUltimateTexture, camera, gSwordSlashSound);

						for (int i = 0; i < numOfEnemy1; i++)
						{
							enemy1[i].render(gRogueTexture, gRedDotTexture, gRedSlashTexture, camera, gSwordSlashSound);
						}
						for (int i = 0; i < numOfEnemy2; i++)
						{
							enemy2[i].render(gPaladinTexture, gRedDotTexture, gRedSlashTexture, camera, gSwordSlashMetalSound);
						}
						for (int i = 0; i < numOfEnemy3; i++)
						{
							enemy3[i].render(gAssassinTexture, gRedDotTexture, gRedSlashTexture, gShurikenTexture, camera, gSwordSlashSound);
						}

						SDL_RenderPresent(gRenderer);
					}
				}
			} 
		} 

		close(tileSet);
	}
	return 0;
}

void gamePauseScreen()
{
	gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Game paused", gHighlightColor);
	gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 50);

	SDL_RenderPresent(gRenderer);
}

void gameEndScreen(const int& rank)
{
	std::stringstream rankText;
	
	gGameOverTexture.render(gRenderer, 0, 0);

	//Rank board
	for (int i = 0; i < TOTAL_DATA; ++i)
	{
		rankText.str("");
		rankText << i + 1;

		gRankDataTextures[i].loadFromRenderedText(gRenderer, gFont, rankText.str().c_str(), (i == rank) ? gHighlightColor : gTextColor);
		gRankDataTextures[i].render(gRenderer, SCREEN_WIDTH / 3 - gRankDataTextures[i].getWidth() / 2, 150 + gRankDataTextures[0].getHeight() * i);

		rankText.str("");
		rankText << gRankData[i];

		gRankDataTextures[i].loadFromRenderedText(gRenderer, gFont, rankText.str().c_str(), (i == rank) ? gHighlightColor : gTextColor);
		gRankDataTextures[i].render(gRenderer, SCREEN_WIDTH * 2 / 3 - gRankDataTextures[i].getWidth() / 2, 150 + gRankDataTextures[0].getHeight() * i);
	}

	SDL_RenderPresent(gRenderer);
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
		gWindow = SDL_CreateWindow("Blood Blade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	//Loading success flag
	bool success = true;

	//Load menu texture
	if (!gMenuTexture.loadFromFile(gRenderer, "assets/home-screen.png", SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to load menu texture!\n");
		success = false;
	}

	//Load menu help texture
	if (!gMenuHelpTexture.loadFromFile(gRenderer, "assets/menu-help.png", SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to load menu help texture!\n");
		success = false;
	}

	//Load menu credit texture
	if (!gMenuCreditTexture.loadFromFile(gRenderer, "assets/menu-credit.png", SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to load menu credit texture!\n");
		success = false;
	}

	//Load game over texture
	if (!gGameOverTexture.loadFromFile(gRenderer, "assets/game-over.png", SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to load game over texture!\n");
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

	//Load paladin texture
	if (!gPaladinTexture.loadFromFile(gRenderer, "assets/paladin.png", ENEMY_WIDTH, ENEMY_HEIGHT))
	{
		printf("Failed to load paladin texture!\n");
		success = false;
	}

	//Load assassin texture
	if (!gAssassinTexture.loadFromFile(gRenderer, "assets/assassin.png", ENEMY_WIDTH, ENEMY_HEIGHT))
	{
		printf("Failed to load assassin texture!\n");
		success = false;
	}

	//Load tile set texture
	if (!gTileTexture.loadFromFile(gRenderer, "assets/tiles.png", 120, 240))
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
	if (!gRedDotTexture.loadFromFile(gRenderer, "assets/red.bmp", 5, 5))
	{
		printf("Failed to load red dot!\n");
		success = false;
	}

	//Load red slash
	if (!gRedSlashTexture.loadFromFile(gRenderer, "assets/red-slash.png", ENEMY_WIDTH * 2, ENEMY_HEIGHT * 2))
	{
		printf("Failed to load red slash!\n");
		success = false;
	}

	//Load blue slash
	if (!gBlueSlashTexture.loadFromFile(gRenderer, "assets/blue-slash.png", PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2))
	{
		printf("Failed to load blue slash!\n");
		success = false;
	}

	//Load red sword
	if (!gRedSwordTexture.loadFromFile(gRenderer, "assets/red-sword.png", PLAYER_WIDTH * 3, PLAYER_HEIGHT * 3))
	{
		printf("Failed to load red sword!\n");
		success = false;
	}

	//Load shuriken
	if (!gShurikenTexture.loadFromFile(gRenderer, "assets/shuriken.png", SHURIKEN_SIZE, SHURIKEN_SIZE))
	{
		printf("Failed to load red sword!\n");
		success = false;
	}

	//Load red circle
	if (!gRedCircleTexture.loadFromFile(gRenderer, "assets/red-circle.png", RED_CIRCLE_SIZE, RED_CIRCLE_SIZE))
	{
		printf("Failed to load red circle!\n");
		success = false;
	}

	//Load ultimate
	if (!gUltimateTexture.loadFromFile(gRenderer, "assets/ultimate.png", 40, 40))
	{
		printf("Failed to load ultimate !\n");
		success = false;
	}

	//Set texture transparency
	gRedDotTexture.setAlpha(192);
	gRedSlashTexture.setAlpha(192);
	gBlueSlashTexture.setAlpha(192);
	gRedCircleTexture.setAlpha(96);

	//Open the font
	gFont = TTF_OpenFont("assets/PlaymegamesReguler-2OOee.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Open file for reading in binary
	SDL_RWops* file = SDL_RWFromFile("save/nums.bin", "r+b");

	//File does not exist
	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		//Create file for writing
		file = SDL_RWFromFile("save/nums.bin", "w+b");
		if (file != NULL)
		{
			printf("New file created!\n");

			//Initialize data
			for (int i = 0; i < TOTAL_DATA; ++i)
			{
				gRankData[i] = 0;
				SDL_RWwrite(file, &gRankData[i], sizeof(gRankData[0]), 1);
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
			SDL_RWread(file, &gRankData[i], sizeof(gRankData[0]), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}

	//Load opening music
	gOpeningMusic = Mix_LoadMUS("assets/Need to Be Strong.mp3");
	if (gOpeningMusic == NULL)
	{
		printf("Failed to load Opening Music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load battle music
	gBattleMusic = Mix_LoadMUS("assets/Raikiri.mp3");
	if (gBattleMusic == NULL)
	{
		printf("Failed to load Battle Music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load Sword Slash Sound effect
	gSwordSlashSound = Mix_LoadWAV("assets/sword_slash.wav");
	if (gSwordSlashSound == NULL)
	{
		printf("Failed to load Sword Slash Sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load Sword Slash Metal Sound effect
	gSwordSlashMetalSound = Mix_LoadWAV("assets/sword_slash_metal.wav");
	if (gSwordSlashMetalSound == NULL)
	{
		printf("Failed to load Sword Slash Metal Sound effect! SDL_mixer Error: %s\n", Mix_GetError());
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
	gMenuTexture.free();
	gMenuHelpTexture.free();
	gMenuCreditTexture.free();
	gGameOverTexture.free();

	gPlayerTexture.free();
	gRogueTexture.free();
	gPaladinTexture.free();
	gAssassinTexture.free();

	gScoreTextTexture.free();
	gPromptTextTexture.free();

	gRedDotTexture.free();
	gRedSlashTexture.free();
	gBlueSlashTexture.free();
	gRedSwordTexture.free();
	gShurikenTexture.free();
	gRedCircleTexture.free();
	gUltimateTexture.free();

	gPromptTextTexture.free();
	for (int i = 0; i < TOTAL_DATA; ++i)
	{
		gRankDataTextures[i].free();
	}

	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile("save/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gRankData[i], sizeof(gRankData[0]), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: Unable to save file! %s\n", SDL_GetError());
	}

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Free the sound effects
	Mix_FreeChunk(gSwordSlashSound);
	gSwordSlashSound = NULL;
	Mix_FreeChunk(gSwordSlashMetalSound);
	gSwordSlashMetalSound = NULL;

	//Free the music
	Mix_FreeMusic(gOpeningMusic);
	gOpeningMusic = NULL;
	Mix_FreeMusic(gBattleMusic);
	gBattleMusic = NULL;

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

