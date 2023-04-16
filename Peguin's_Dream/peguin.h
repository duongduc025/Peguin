#pragma once

#include "lib.h"
#include "pipe.h"

using namespace std;

class peguin:LTexture
{
public:
    bool init(bool isDark);

    void animation();

    void render();

    void Free();

    void resetTime()
    {
        time = 0;
    }

    void fall();

    void update(short int pileWidth, short int pileHeight);
    short int angle, time, x0;
    short int ahead = 0;

    string saved_path = "";
    position pospeguin;
};
