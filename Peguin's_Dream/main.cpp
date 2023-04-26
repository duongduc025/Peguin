#include "game.h"
#include "peguin.h"
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
    short int frame = 0;
    bool isDark = 0;
    int idx = 0;

    while(!g.isQuit())
    {
        frameStart = SDL_GetTicks();

        if (g.isDie())
        {
            if (isMenu)
            {
                g.sound.playHit();
                g.bird.render(0);

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
                    g.bird.render(0);
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
                    g.bird.render(0);
                    g.renderMessage();
                    if(isSetting)
                    {
                        g.renderSettingTab();
                        g.sound.renderSound();
                        g.nextButton();
                        if (!skin) g.lightskin();
                        else g.darkskin();

                        if(!isDark) g.sunbackgr();
                        else g.moonbackgr();

                        if (g.userInput.Type == game::input::PLAY)
                        {
                            if (g.sound.checkSound())
                            {
                                isSound = abs(1 - isSound);
                            }
                            else if (g.changeskin())
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
            if(isPause == 0)
            {
            frame++;
            if (frame == 20)
            {
                idx++;
                if (idx == 3)
                    idx = 0;
                frame = 0;
            }
            }
            g.bird.render(idx);
            g.renderScoreLarge();

            if (!isPause)
            {
                g.bird.update(g.getPipeWidth(), g.getPipeHeight());
                g.pipe.update();
                g.land.update();
                g.pause();
            }
            else
            {
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
