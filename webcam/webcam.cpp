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

static const uint16_t FLOWSCALE = 2;

static const auto ARROWCOLOR = cv::Scalar(255, 255, 255);
static const auto LINECOLOR = cv::Scalar(0, 0, 0);

static const uint16_t R = 480;
static const uint16_t C = 640;

static const uint16_t DOWNSCALE = 10;

static const uint16_t RR = R / DOWNSCALE;
static const uint16_t CC = C / DOWNSCALE;

static const uint16_t P = 4; // tile size

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

static void makeTiles(const uint8_t * image, uint8_t tiles[RR][CC][P*P])
{
    for (uint32_t j=0; j<RR*CC; ++j) {
        const auto r = j / (P * CC);
        const auto c = (j % CC) / P;
        const auto k = j % (P*P) ;
        tiles[r][c][k] = image[j];
    }
}

void computeFlow(
        uint8_t curr_tiles[RR][CC][P*P], 
        int16_t ofx[RR][CC],
        int16_t ofy[RR][CC])
{
    static uint8_t prev_tiles[RR][CC][P*P];

    for (uint16_t r=0; r<RR; ++r) {

        for (uint16_t c=0; c<CC; ++c) {

            OpticalFlow::LK_Square_2D(
                    curr_tiles[r][c], 
                    prev_tiles[r][c],
                    P,
                    P, 
                    FLOWSCALE, 
                    &ofx[r][c],
                    &ofy[r][c]);


            /*
            auto p2 = P / 2;
            auto ctrx = (c + 1) * p2;
            auto ctry = (r + 1) * p2;

            cv::arrowedLine(
                    image, 
                    cv::Point(ctrx, ctry), 
                    cv::Point(ctrx + ofx, ctry + ofy), 
                    ARROWCOLOR);
                    */
        }
    }

    memcpy(prev_tiles, curr_tiles, RR*CC*P*P);
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

        cv::Mat small;
        cv::resize(gray, small, cv::Size(CC, RR));

        uint8_t curr_tiles[RR][CC][P*P] = {};

        makeTiles(small.data, curr_tiles);

        int16_t ofx[RR][CC] = {};
        int16_t ofy[RR][CC] = {};

        computeFlow(curr_tiles, ofx, ofy);

        cv::resize(small, gray, cv::Size(C, R));

        for (uint16_t r=P; r<RR; r+=P) {
            auto rr = r * DOWNSCALE;
            cv::line(gray, cv::Point(0, rr), cv::Point(C, rr), LINECOLOR);
        }

        for (uint16_t c=P; c<CC; c+=P) {
            auto cc = c * DOWNSCALE;
            cv::line(gray, cv::Point(cc, 0), cv::Point(cc, R), LINECOLOR);
        }

        for (uint16_t r=P; r<RR; r+=P) {
            auto rr = r * DOWNSCALE;
            for (uint16_t c=P; c<CC; c+=P) {
                auto cc = c * DOWNSCALE;
                cv::circle(gray, cv::Point(cc, rr), 1, ARROWCOLOR);
            }
        }

        cv::imshow("Optical Flow", gray);

        if (cv::waitKey(1) >= 0) {
            break;
        }

        report();
    }

    return 0;
}
