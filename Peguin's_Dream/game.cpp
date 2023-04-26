#include "game.h"
#include "lib.h"
#include <iostream>

void game::takeInput()
{
    while(SDL_PollEvent(&event) != 0)
    {
        if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT)
        {
            userInput.Type = input::QUIT;
            quit = true;
        }
        else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q && event.key.repeat == 0)
        {
            userInput.Type = input::SETTING;
        }
        else if ( event.type == SDL_MOUSEBUTTONDOWN || (event.type == SDL_KEYDOWN &&
		(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP) && event.key.repeat == 0) )
        {
            userInput.Type = input::PLAY;
        }
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p && event.key.repeat == 0)
		{
			userInput.Type = input::PAUSE;
		}
    }
}

game::game()
{
    initGraphic();
    pipe.init();
    land.init();
    sound.init();
}

game::~game()
{
    bird.Free();
    pipe.Free();
    land.Free();
    sound.Free();
    free();
    releaseGraphic();
}

void game::releaseGraphic()
{
    //clean
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool game::initGraphic()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "Peguin's dream", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void game::display()
{
    SDL_RenderPresent(gRenderer);
    SDL_RenderClear(gRenderer);
}

void game::renderScoreSmall()
{
	string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len - 1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 0.45);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 0.45);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 0.45);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 0.45);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 0.45);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 0.45);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 0.45);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 0.45);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 0.45);
		}
		else
		{
			image.Load("res/number/0.png", 0.45);
		}
		image.Render(245 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 262);
	}
	image.free();
}

void game::renderScoreLarge()
{
    string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = 0; i < len; i++)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 1);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 1);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 1);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 1);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 1);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 1);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 1);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 1);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 1);
		}
		else
		{
			image.Load("res/number/0.png", 1);
		}
		image.Render((SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
	}
	image.free();
}

void game::renderBestScore()
{
	ifstream fileIn("res/data/bestScore.txt");
	fileIn >> bestScore;
	ofstream fileOut("res/data/bestScore.txt", ios::trunc);

	if (score > bestScore)
	{
		bestScore = score;
	}
	string s = to_string(bestScore);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len-1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("res/number/1.png", 0.45);
		}
		else if (number == 2)
		{
			image.Load("res/number/2.png", 0.45);
		}
		else if (number == 3)
		{
			image.Load("res/number/3.png", 0.45);
		}
		else if (number == 4)
		{
			image.Load("res/number/4.png", 0.45);
		}
		else if (number == 5)
		{
			image.Load("res/number/5.png", 0.45);
		}
		else if (number == 6)
		{
			image.Load("res/number/6.png", 0.45);
		}
		else if (number == 7)
		{
			image.Load("res/number/7.png", 0.45);
		}
		else if (number == 8)
		{
			image.Load("res/number/8.png", 0.45);
		}
		else if (number == 9)
		{
			image.Load("res/number/9.png", 0.45);
		}
		else
		{
			image.Load("res/number/0.png", 0.45);
		}
		image.Render(245 - image.getWidth()*(len-i-1)*0.75 - 5*(len - i - 1), 315);
	}
	image.free();

	fileOut << bestScore;
	fileIn.close();
	fileOut.close();
}

void game::renderMessage()
{
	LTexture image;
	image.Load("res/image/message.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 180);
	image.free();
}

void game::renderBackground()
{
	LTexture image;
	image.Load("res/image/background.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderBackgroundNight()
{
	LTexture image;
	image.Load("res/image/background-night.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderLand()
{
	LTexture image;
	image.Load("res/image/land.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, SCREEN_HEIGHT- image.getHeight());
	image.free();
}

void game::resume()
{
	LTexture image;
	image.Load("res/image/resume.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::pause()
{
	LTexture image;
	image.Load("res/image/pause.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::renderPauseTab()
{
	LTexture image;
	image.Load("res/image/pausetabice.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 225);
	image.free();
}
void game::renderSettingTab()
{
	LTexture image;
	image.Load("res/image/setting.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 225);
	image.free();
}

void game::lightskin()
{
	LTexture image;
	image.Load("res/image/rsz_xanh.png", 0.8);
	image.Render(105, 315);
	image.free();
}

void game::darkskin()
{
	LTexture image;
	image.Load("res/image/rsz_hong.png", 0.8);
	image.Render(105, 315);
	image.free();
}
void game::sunbackgr()
{
    LTexture image;
	image.Load("res/image/sunicon.png", 0.8);
	image.Render(235, 315);
	image.free();
}
void game::moonbackgr()
{
    LTexture image;
	image.Load("res/image/moonicon.png", 1.2);
	image.Render(225, 293);
	image.free();
}
bool game::changebackgr()
{
    int x, y;
	SDL_GetMouseState(&x, &y);
	if ((x > 235 && x < 300) && (y > 322 && y < 322 + 50))
	{
		return true;
	}
	return false;

}

void game::nextButton()
{
	LTexture image;
	image.Load("res/image/nextRight.png", 1);
	image.Render(149, 322);
	image.Load("res/image/nextLeft.png", 1);
	image.Render(88, 322);
	image.free();
}

bool game::changeskin()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (((x > 149 && x < 149+13)  || (x > 88 && x < 88 + 13)) && (y > 322 && y < 322 + 16))
	{
		return true;
	}
	return false;
}

void game::renderGameOver()
{
	LTexture image;
	image.Load("res/image/gameover.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 150);
	image.free();
}

void game::renderMedal()
{
	LTexture image;

	if (score >= 20 && score <= 50)
	{
		image.Load("res/image/trophy/silver.png", 0.45);
	}
	else if (score > 50)
	{
		image.Load("res/image/trophy/gold.png", 0.45);
	}
	else
	{
		image.Load("res/image/trophy/cu.png", 0.45);
	}
	image.Render(82, 275);

	image.free();
}

void game::replay()
{
	LTexture image;
	image.Load("res/image/replay.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 380);
	image.free();
}

bool game::checkReplay()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > (SCREEN_WIDTH - 100)/2 && x < (SCREEN_WIDTH + 100) / 2 && y > 380 && y < 380 + 60) //cai nut
	{
		return true;
	}
	return false;
}

void game::Restart()
{
    die = false;
    score = 0;
    bird.resetTime();
}
