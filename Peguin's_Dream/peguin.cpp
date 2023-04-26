#include "peguin.h"
#include "lib.h"
#include <stdio.h>
#include <iostream>

bool peguin::init(bool skin)
{
    string path = "xanh";
    if(skin) path = "hong";
        pospeguin.getPos(100, SCREEN_HEIGHT / 2 - 10);
        ahead = 0;
        angle = 0;

    if (isNULL())
    {
        for (int i = 0; i < 3; i++)
        {
            string bird_path = "res/image/" + path + "/" + to_string(i + 1) + ".png";
            if (!textures[i].Load(bird_path.c_str(), 0.8))
                return false;
        }
        return true;
    }
    return false;
}
void peguin::Free()
{
    free();
}

void peguin::render(int i)
{
    textures[i].Render(pospeguin.x, pospeguin.y, angle);
}

void peguin::fall()
{
    if (die && pospeguin.y < SCREEN_HEIGHT - LAND_HEIGHT - PEGUIN_HEIGHT - 5)
    {
        if (time == 0)
        {
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
        if ( (pospeguin.x + PEGUIN_WIDTH -10 > posPipe[ahead].x +5) && (pospeguin.x + 5 < posPipe[ahead].x + pipeWidth) &&
                (pospeguin.y + 5 < posPipe[ahead].y + pipeHeight || pospeguin.y  + PEGUIN_HEIGHT > posPipe[ahead].y + pipeHeight + PIPE_SPACE + 5) )
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
