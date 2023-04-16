#include "peguin.h"
#include <stdio.h>
#include <iostream>

bool peguin::init(bool isDark)
{
    string bird_path;
    bird_path = "res/image/rsz_xanh.png";
    if (isDark) bird_path = "res/image/rsz_hong.png";
    if (saved_path == bird_path)
    {
        pospeguin.getPos(100, SCREEN_HEIGHT / 2 - 10);
        ahead = 0;
        angle = 0;
    }
    if (isNULL() || saved_path != bird_path)
    {
        saved_path = bird_path;
        if ( Load(bird_path.c_str() , 1) )
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    return false;
}

void peguin::Free()
{
    free();
}

void peguin::render()
{
    Render(pospeguin.x, pospeguin.y, angle);

}

void peguin::fall()
{
    if (die && pospeguin.y < SCREEN_HEIGHT - LAND_HEIGHT - PEGUIN_HEIGHT -5)
    {
        if (time == 0){
            x0 = pospeguin.y;
        }
        if (time >= 0)
        {
            pospeguin.y = x0 + time * time * 0.18 - 7.3 * time;
            time++;
        }
        angle = 180;
    }
    else return;
}

void peguin::update(short int pipeWidth, short int pipeHeight)
{
    if (!die)
    {
        if (time == 0)
        {
            x0 = pospeguin.y;
            angle = -25;
        }
        else if (angle < 70 && time > 30)
        {
            angle += 3;
        }

        if (time >= 0)
        {
            pospeguin.y = x0 + time * time * 0.18 - 7.3 * time;
            time++;
        }

        if ( (pospeguin.x + getWidth() > posPipe[ahead].x + 5) && (pospeguin.x + 5 < posPipe[ahead].x + pipeWidth) &&
             (pospeguin.y + 5 < posPipe[ahead].y + pipeHeight || pospeguin.y  + getHeight() > posPipe[ahead].y + pipeHeight + PIPE_SPACE + 5) )
        {
            die = true;
        }
        else if (pospeguin.x > posPipe[ahead].x + pipeWidth )
        {
            ahead = ( ahead + 1 ) % TOTAL_PIPE;
            score++;
        }

        if (pospeguin.y > SCREEN_HEIGHT - LAND_HEIGHT -  PEGUIN_HEIGHT - 5 || pospeguin.y < - 10 )
        {
            die = true;
        }
    }
}
