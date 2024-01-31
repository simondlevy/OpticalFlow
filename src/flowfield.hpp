/*
   Optical flow fields

   Copyright (C) 2024 Simon D. Levy

   GPL
 */

#pragma once

#include <stdint.h>

#include <vector>

class Arrow {

    public:

        uint16_t x;
        uint16_t y;

        uint16_t dx;
        uint16_t dy;

        Arrow(
                const uint16_t _x,
                const uint16_t _y,
                const uint16_t _dx,
                const uint16_t _dy) 
            : x(_x), y(_y), dx(_dx), dy(_dy)
        {
        }

        Arrow(Arrow & a)
            : x(a.x), y(a.y), dx(a.dx), dy(a.dy)
        {
        }

        static std::vector<Arrow> getField(const uint8_t * pixels)
        {
            std::vector<Arrow> field;

            return field;
        }
}; 

