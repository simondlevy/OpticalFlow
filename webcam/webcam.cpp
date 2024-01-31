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
#include <flowfield.hpp>

using namespace std;

static const uint16_t FLOWSCALE = 20;

static const auto ARROWCOLOR = cv::Scalar(255, 255, 255);

static const uint16_t PATCHSIZE = 20;

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

int main(int, char**)
{
    cv::VideoCapture cap;


    cap.open(0, cv::CAP_ANY); 

    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    while (true) {

        cv::Mat orig;

        cap.read(orig);

        if (orig.empty()) {
            cerr << "ERROR! blank orig grabbed\n";
            break;
        }

        const auto rows = orig.rows;
        const auto cols = orig.cols;

        if (rows % PATCHSIZE || cols % PATCHSIZE) {
            printf("Image size %d x %d not a multiple of patch size %d\n",
                    cols, rows, PATCHSIZE);
            exit(0);
        }

        cv::Mat gray;
        cv::cvtColor(orig, gray, cv::COLOR_BGR2GRAY);

        static cv::Mat grayprev;

        if (grayprev.data != NULL) {

            for (uint16_t j=0; j<rows/PATCHSIZE; ++j) {

                for (uint16_t k=0; k<cols/PATCHSIZE; ++k) {

                    auto ulx = k * PATCHSIZE;
                    auto uly = j * PATCHSIZE;

                    auto lrx = (k + 1) * PATCHSIZE;
                    auto lry = (j + 1) * PATCHSIZE;

                    auto ctrx = (ulx + lrx)  / 2;
                    auto ctry = (uly + lry)  / 2;

                    cv::circle(orig, cv::Point(ctrx, ctry), 1, ARROWCOLOR);
                }
            }

            int16_t ofx = 0;
            int16_t ofy = 0;

            OpticalFlow::LK_Plus_2D(
                    gray.data,
                    grayprev.data,
                    rows,
                    cols,
                    FLOWSCALE,
                    &ofx,
                    &ofy);

            const auto ctrx = cols / 2;
            const auto ctry = rows / 2;

            cv::arrowedLine(
                    orig, 
                    cv::Point(ctrx, ctry),
                    cv::Point(ctrx - ofx, ctry - ofy),
                    ARROWCOLOR);
        }

        grayprev = gray.clone();

        cv::imshow("Live", orig);

        if (cv::waitKey(1) >= 0) {
            break;
        }

        report();
    }

    return 0;
}
