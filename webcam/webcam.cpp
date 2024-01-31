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

using namespace std;

static const uint8_t DOWNSIZE = 10;

int main(int, char**)
{
    cv::Mat image;
    
    cv::VideoCapture cap;

    cap.open(0, cv::CAP_ANY); 

    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    while (true) {

        cap.read(image);
        
        if (image.empty()) {
            cerr << "ERROR! blank image grabbed\n";
            break;
        }

        auto rows = image.rows;
        auto cols = image.cols;

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        static cv::Mat downprev;

        cv::Mat downsized;
        cv::resize(gray, downsized, cv::Size(cols/DOWNSIZE, rows/DOWNSIZE), 
                cv::INTER_NEAREST);

        if (downprev.data != NULL) {
        }

        downprev = downsized.clone();

        //uint8_t * data = downsized.data;

        //for (uint8_t k=0; k<35; ++k) {
        //    data[k*35 + k] = 0;
       // }

        cv::Mat upsized;
        cv::resize(downsized, upsized, cv::Size(cols, rows), cv::INTER_LINEAR);

        cv::imshow("Live", upsized);

        if (cv::waitKey(1) >= 0)
            break;
    }

    return 0;
}
