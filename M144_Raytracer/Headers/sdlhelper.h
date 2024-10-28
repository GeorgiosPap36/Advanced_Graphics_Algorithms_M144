#ifndef SDLHELPER_H
#define SDLHELPER_H

#include <iostream>

#include "SDL.h"
#include "sdltemplate.h"

class sdlHelper
{
public:
	sdlHelper()
	{
	}

	sdlHelper(int width, int height) : w_width(width), w_height(height)
	{
	}

	void Run(int*** ar) const
	{
        std::cout << "Image resolution: " << w_width << ", " << w_height << "\n \n";
        sdltemplate::sdl("Ray Tracer", w_width, w_height);
        sdltemplate::loop();
        for (int j = 0; j < w_height; j++)
        {
            for (int i = 0; i < w_width; i++)
            {
                if (i % w_height == 1) sdltemplate::loop();
                sdltemplate::setDrawColor(sdltemplate::createColor(ar[j][i][0], ar[j][i][1], ar[j][i][2], 255));
                sdltemplate::drawPoint(i, w_height - j);
            }
        }

        while (sdltemplate::running)
        {
            sdltemplate::loop();
        }
	}

public:
	int w_width, w_height;
};

#endif
