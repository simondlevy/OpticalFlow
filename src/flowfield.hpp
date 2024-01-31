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

            _imgprev = new uint8_t [_rows * _cols];

            _ready = false;

            for (uint16_t j=0; j<_rows/_patchSize; ++j) {

                for (uint16_t k=0; k<_cols/_patchSize; ++k) {

                    auto ulx = k * _patchSize;
                    auto uly = j * _patchSize;

                    auto lrx = (k + 1) * _patchSize;
                    auto lry = (j + 1) * _patchSize;

                    auto ctrx = (ulx + lrx)  / 2;
                    auto ctry = (uly + lry)  / 2;

                    _arrows.push_back(new Arrow(ctrx, ctry, 0, 0));
                }
            }

        }

        ~FlowField(void)
        {
            for (auto arrow : _arrows) {
                delete arrow;
            }

            delete _imgprev;

            delete _patch;
        }

        std::vector<Arrow *> getField(const uint8_t * imgcurr)
        {
            if (_ready) {
            }

            memcpy(_imgprev, imgcurr, _rows * _cols);

            _ready = true;

            return _arrows;
        }

    private:

        uint16_t _rows;
        uint16_t _cols;
        uint16_t _patchSize;

        uint8_t * _imgprev;

        uint8_t * _patch;

        bool _ready;

        std::vector<Arrow *> _arrows;
}; 

