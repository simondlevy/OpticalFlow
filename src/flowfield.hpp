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

        uint16_t x1;
        uint16_t y1;

        uint16_t x2;
        uint16_t y2;

        Arrow(
                const uint16_t _x1,
                const uint16_t _y1,
                const uint16_t _x2,
                const uint16_t _y2) 
            : x1(_x1), y1(_y1), x2(_x2), y2(_y2)
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
            _patchSize = patchSize;

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

        std::vector<Arrow *> get(const uint8_t * imgcurr)
        {
            if (_ready) {

                for (auto arrow : _arrows) {
                    arrow->x2 = arrow->x1 + 5;
                    arrow->y2 = arrow->y1 + 5;
                }
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

