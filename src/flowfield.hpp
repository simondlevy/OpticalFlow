/*
   Optical flow fields

   Copyright (C) 2024 Simon D. Levy

   GPL
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

#include <vector>

class FlowField {

    public:

        typedef struct {

            uint16_t x1;
            uint16_t y1;
            uint16_t x2;
            uint16_t y2;

        } arrow_t;

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

                    uint16_t ulx = k * _patchSize;
                    uint16_t uly = j * _patchSize;

                    uint16_t lrx = (k + 1) * _patchSize;
                    uint16_t lry = (j + 1) * _patchSize;

                    uint16_t ctrx = (ulx + lrx)  / 2;
                    uint16_t ctry = (uly + lry)  / 2;

                    _arrows.push_back(new arrow_t {ctrx, ctry, 0, 0});
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

        std::vector<arrow_t *> get(const uint8_t * imgcurr)
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

        std::vector<arrow_t *> _arrows;
}; 

