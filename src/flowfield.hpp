/*
   Optical flow fields

   Copyright (C) 2024 Simon D. Levy

   GPL
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

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
}; 

class FlowField {

    public:

        FlowField(
                const uint16_t rows, 
                const uint16_t cols, 
                const uint16_t patchSize)
        {
            if (rows % patchSize || cols % patchSize) {
                printf("Image size %d x %d not a multiple of patch size %d\n",
                        cols, rows, patchSize);
                exit(1);
            }

            _rows = rows;
            _cols = cols;
            _patchSize = _patchSize;

            _patch = new uint8_t [patchSize];
        }

        ~FlowField(void)
        {
            delete _patch;
        }

        std::vector<Arrow> getField(const uint8_t * pixels)
        {
            std::vector<Arrow> field;

            return field;
        }

    private:

        uint16_t _rows;
        uint16_t _cols;
        uint16_t _patchSize;

        uint8_t * _patch;
}; 

