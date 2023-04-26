#pragma once

#include "lib.h"
#include "pipe.h"

using namespace std;

class peguin:LTexture
{
public:
    bool init(bool isDark);

    void animation();

    void render(int i);

    void Free();

    void resetTime()
    {
        time = 0;
    }

    void fall();

    void update(short int pileWidth, short int pileHeight);
    short int angle, time, x0;
    short int ahead = 0;


    LTexture textures[4];
    position pospeguin;
};
