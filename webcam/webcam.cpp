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

        cv::Mat gray;
        cv::cvtColor(orig, gray, cv::COLOR_BGR2GRAY);

        static cv::Mat grayprev;

        if (grayprev.data != NULL) {

            int16_t ofx = 0;
            int16_t ofy = 0;

            // OpticalFlow::LK_Plus_2D(
            OpticalFlow::LK_Square_2D(
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
