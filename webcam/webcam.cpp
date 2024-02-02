/**
 * Copyright (C) 2024 Simon D. Levy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include <opticalflow.hpp>

using namespace std;

static const uint16_t FLOWSCALE = 20;

static const auto ARROWCOLOR = cv::Scalar(255, 255, 255);

static const uint16_t R = 480;
static const uint16_t C = 640;
static const uint16_t P = 20;

void report(void)
{
    static uint32_t count;
    static uint32_t timeprev;

    struct timeval tval = {};
    gettimeofday(&tval, NULL);

    if (tval.tv_sec - timeprev >= 1) {
        if (count > 0) {
            printf("%d fps\n", count);
        }
        timeprev = tval.tv_sec;
        count = 0;
    }

    count++;
}

static void makeTiles(const uint8_t * image, uint8_t tiles[R][C][P*P])
{
    for (uint32_t j=0; j<R*C; ++j) {
        const auto r = j / (P * C);
        const auto c = (j % C) / P;
        const auto k = j % (P*P) ;
        tiles[r][c][k] = image[j];
    }
}


int main(int, char**)
{
    cv::VideoCapture cap;

    cap.open(0, cv::CAP_ANY); 

    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    while (true) {

        cv::Mat image;

        cap.read(image);

        if (image.empty()) {
            cerr << "ERROR! blank image grabbed\n";
            break;
        }

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        static uint8_t tiles[R][C][P*P];

        makeTiles(gray.data, tiles);

        for (uint16_t r=0; r<R; ++r) {
            for (uint16_t c=0; c<C; ++c) {
                auto p2 = P / 2;
                auto ctrx = (c + 1) * p2;
                auto ctry = (r + 1) * p2;
                cv::circle(image, cv::Point(ctrx, ctry), 1, ARROWCOLOR);
            }
        }

        cv::imshow("Live", image);

        if (cv::waitKey(1) >= 0) {
            break;
        }

        report();
    }

    return 0;
}
