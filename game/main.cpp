#include "Game_Base.h"
#include "Game_Utils.h"
#include "LTexture.h"
#include "Others.h"
#include "Player.h"
#include "Enemy.h"
#include "Rogue.h"
#include "Paladin.h"
#include "Assassin.h"
#include "LTimer.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
SDL_Color gTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };
SDL_Color gHighlightColor = { 0xFF, 0, 0, 0xFF };

//Characters
LTexture gLogoTexture;
LTexture gPlayerTexture;
LTexture gRogueTexture;
LTexture gPaladinTexture;
LTexture gAssassinTexture;

//Effect, items
LTexture gRedDotTexture;
LTexture gRedSlashTexture;
LTexture gBlueSlashTexture;
LTexture gRedSwordTexture;
LTexture gShurikenTexture;
LTexture gRedCircleTexture;

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
LTexture gDataTextures[TOTAL_DATA];
int gData[TOTAL_DATA];

Player player;
Enemy* enemy1 = new Rogue[TOTAL_ENEMY_1];
Enemy* enemy2 = new Paladin[TOTAL_ENEMY_2];
Enemy* enemy3 = new Assassin[TOTAL_ENEMY_3];

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
			bool quitMenu = false;
			bool playGame = false;

			Mix_PlayMusic(gOpeningMusic, -1);

			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			while (!quitMenu)
			{
				SDL_Event e;

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quitMenu = true;
						playGame = false;
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						quitMenu = true;
						playGame = true;
					}
				}

				camera.x = (camera.x + 1) % (LEVEL_WIDTH - SCREEN_WIDTH);

				//Draw to screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render(gRenderer, gTileTexture, gTileClips, camera);
				}

				//Render prompt text
				gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Click to play", gTextColor);
				gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gPromptTextTexture.getHeight()) / 2);

				gLogoTexture.render(gRenderer, (SCREEN_WIDTH - gLogoTexture.getWidth()) / 2, 100);

				SDL_RenderPresent(gRenderer);
			}

			while (playGame)
			{
				bool quit = false;
				bool pause = false;

				Mix_PlayMusic(gBattleMusic, -1);
				
				SDL_Event e;

				LTimer time;

				std::stringstream scoreText;

				//Score = enemy killed
				int score = 0;
				int rank = 0;

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

				while (!quit)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							quit = true;
							playGame = false;
						}
						else if (e.type == SDL_KEYDOWN)
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_r:
								//Reset game
								quit = true;
								break;
							case SDLK_p:
								//Pause game
								pause = !pause;
								break;
							}
						}
						else if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							if (pause) pause = false;
							if (!player.getIsAppear()) quit = true;
						}

						if (!pause)
							player.handleEvent(e, camera);
					}
					
					if (player.getIsAppear() && !pause)
					{
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
						for (int i = 0; i < TOTAL_TILES; ++i)
						{
							tileSet[i]->render(gRenderer, gTileTexture, gTileClips, camera);
						}

						//Render score text
						scoreText.str("");
						scoreText << "YOUR SCORE: " << score;

						gScoreTextTexture.loadFromRenderedText(gRenderer, gFont, scoreText.str().c_str(), gTextColor);
						gScoreTextTexture.render(gRenderer, SCREEN_WIDTH - gScoreTextTexture.getWidth() - 10, 10);

						//Render characters
						player.render(gPlayerTexture, gRedDotTexture, gBlueSlashTexture, gRedSwordTexture, gRedCircleTexture, camera, gSwordSlashSound);

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

					if (pause)
					{
						Mix_PauseMusic();
						gamePauseScreen();
					}
					else
					{
						if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
					}

					if (!player.getIsAppear())
					{
						gameEndScreen(score, rank);
					}
				}
			}
			close(tileSet);
		}
	}
	return 0;
}

void gamePauseScreen()
{
	gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Game paused", gHighlightColor);
	gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 50);

	SDL_RenderPresent(gRenderer);
}

void gameEndScreen(const int& score, int& rank)
{
	std::stringstream rankText;
	rankCalculation(score, rank, gData);

	gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "You die!", gHighlightColor);
	gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 50);

	gPromptTextTexture.loadFromRenderedText(gRenderer, gFont, "Click to play again", gHighlightColor);
	gPromptTextTexture.render(gRenderer, (SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, SCREEN_HEIGHT - 50);

	//Show rank board
	for (int i = 0; i < TOTAL_DATA; ++i)
	{
		rankText.str("");
		if (i == 0) rankText.str("Rank");
		else rankText << i;

		gDataTextures[i].loadFromRenderedText(gRenderer, gFont, rankText.str().c_str(), (i == rank || i == 0) ? gHighlightColor : gTextColor);
		gDataTextures[i].render(gRenderer, SCREEN_WIDTH / 3 - gDataTextures[i].getWidth() / 2, 100 + gDataTextures[0].getHeight() * i);

		rankText.str("");
		if (i == 0) rankText.str("Score");
		else rankText << gData[i];

		gDataTextures[i].loadFromRenderedText(gRenderer, gFont, rankText.str().c_str(), (i == rank || i == 0) ? gHighlightColor : gTextColor);
		gDataTextures[i].render(gRenderer, SCREEN_WIDTH * 2 / 3 - gDataTextures[i].getWidth() / 2, 100 + gDataTextures[0].getHeight() * i);
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
	//Loading success flag
	bool success = true;

	//Load logo texture
	if (!gLogoTexture.loadFromFile(gRenderer, "assets/logo.png", 274, 34))
	{
		printf("Failed to load logo texture!\n");
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

	//Load tile texture
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
	if (!gRedSlashTexture.loadFromFile(gRenderer, "assets/red-slash.png", PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2))
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
	if (!gShurikenTexture.loadFromFile(gRenderer, "assets/shuriken.png", PLAYER_WIDTH / 4, PLAYER_HEIGHT / 4))
	{
		printf("Failed to load red sword!\n");
		success = false;
	}

	//Load red circle
	if (!gRedCircleTexture.loadFromFile(gRenderer, "assets/red-circle.png", PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2))
	{
		printf("Failed to load red circle!\n");
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

	//Load music
	gOpeningMusic = Mix_LoadMUS("assets/Need to Be Strong.mp3");
	if (gOpeningMusic == NULL)
	{
		printf("Failed to load Opening Music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gBattleMusic = Mix_LoadMUS("assets/Raikiri.mp3");
	if (gBattleMusic == NULL)
	{
		printf("Failed to load Battle Music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effect
	gSwordSlashSound = Mix_LoadWAV("assets/sword_slash.wav");
	if (gSwordSlashSound == NULL)
	{
		printf("Failed to load Sword Slash effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	gSwordSlashMetalSound = Mix_LoadWAV("assets/sword_slash_metal.wav");
	if (gSwordSlashMetalSound == NULL)
	{
		printf("Failed to load Sword Slash Metal effect! SDL_mixer Error: %s\n", Mix_GetError());
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
	gLogoTexture.free();
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

	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile("save/nums.bin", "w+b");
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

