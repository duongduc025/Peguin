#include "game.h"
#include "lib.h"
#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const int FPS = 60;
const int frameDelay = 1000 / FPS;

using namespace std;

int main(int argc, char** argv)
{
    Uint32 frameStart;
    short int frameTime;
    game g;
    bool isMenu = 0;
    bool isPause = 0;
    bool isSetting = 0;
    bool isSound = 1;
    bool skin = 0;
    bool isDark = 0;

    while(!g.isQuit())
    {
        frameStart = SDL_GetTicks();

        if (g.isDie())
        {
            if (isMenu)
            {
                cout << "hi";
                g.sound.playHit();
                g.bird.render();

            }
            g.userInput.Type = game::input::NONE;
            while(g.isDie() && !g.isQuit())
            {
                g.takeInput();
                if (isMenu == 1 && g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        isMenu = 0;
                    }
                    g.userInput.Type = game::input::NONE;
                }
                if(!isDark) g.renderBackground();
                else g.renderBackgroundNight();
                g.pipe.render();
                g.land.render();
                if (isMenu)
                {
                    g.bird.render();
                    g.bird.fall();
                    g.renderGameOver();
                    g.renderMedal();
                    g.renderScoreSmall();
                    g.renderBestScore();
                    g.replay();
                }
                else
                {
                    if(g.userInput.Type == game::input::SETTING)
                    {
                        isSetting = abs(1-isSetting);
                        g.userInput.Type = game::input::NONE;
                    }

                    g.pipe.init();
                    g.bird.init(skin);
                    g.bird.render();
                    g.renderMessage();
                    if(isSetting)
                    {
                        g.renderPauseTab();
                        g.sound.renderSound();
                        g.nextButton();
                        if (!skin) g.lightTheme();
                         else g.darkTheme();

                        if(!isDark) g.sunbackgr();
                        else g.moonbackgr();

                        if (g.userInput.Type == game::input::PLAY)
                        {
                            if (g.sound.checkSound())
                            {
                                isSound = abs(1 - isSound);
                            }
                            else if (g.changeTheme())
                            {
                                skin = abs(1 - skin);

                            }
                            else if(g.changebackgr())
                            {
                                isDark = abs(1 - isDark);
                            }
                            g.userInput.Type = game::input::NONE;
                        }
                    }
                    if (g.userInput.Type == game::input::PLAY)
                    {
                        cout << "CHOI";
                        g.Restart();
                        isMenu = 1;
                        g.userInput.Type = game::input::NONE;
                    }
                    g.land.update();
                }
                g.display();
            }
            g.pipe.init();
        }
        else
        {

            g.takeInput();

            if (g.userInput.Type == game::input::PAUSE)
            {
                isPause = abs(1 - isPause);
                g.userInput.Type = game::input::NONE;
            }

            if (isPause == 0 && g.userInput.Type == game::input::PLAY)
            {
                if (isSound) g.sound.playBreath();
                g.bird.resetTime();
                g.userInput.Type = game::input::NONE;
            }
            if(!isDark) g.renderBackground();
            else g.renderBackgroundNight();
            g.pipe.render();
            g.land.render();
            g.bird.render();
            g.renderScoreLarge();

            if (!isPause)
            {
                cout << "Huy pause";
                g.bird.update(g.getPipeWidth(), g.getPipeHeight());
                g.pipe.update();
                g.land.update();
                g.pause();
            }
            else
            {
                cout << "else";
                g.resume();
                g.renderPauseTab();
                g.renderScoreSmall();
                g.renderBestScore();
                g.replay();
                g.sound.renderSound();
                 if (g.userInput.Type == game::input::PLAY)
                        {
                            if (g.checkReplay())
                            {
                                isPause = 0;
                            }
                             if (g.sound.checkSound())
                            {
                                isSound = abs(1 - isSound);
                            }
                g.userInput.Type = game::input::NONE;
                            }
            }
            g.display();
        }

        //Limit FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}
