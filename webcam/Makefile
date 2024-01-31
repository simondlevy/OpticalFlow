# Copyright (C) 2024 Simon D. Levy
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, in version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

CFLAGS = -Wall

SRC = ../src

INCLUDE = -I$(SRC)

ifeq ($(OS),Windows_NT)
	OPENCV = "D:/opencv-mingw"
	INCLUDE = -I$(OPENCV)/include
	LIBRARIES = -L$(OPENCV)/x64/mingw/bin 
	LIBRARIES += -lopencv_core455 -lopencv_videoio455 -lopencv_highgui455 \
				 -lopencv_imgproc455
				
else
	INCLUDE += -I/usr/include/opencv4
	LIBRARIES = -L/usr/lib/x86_64-linux-gnu
    LIBRARIES += -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgproc
endif

webcam: webcam.o 
	g++ -o webcam webcam.o $(LIBRARIES)

webcam.o: webcam.cpp $(SRC)/opticalflow.hpp
	g++ -c $(CFLAGS) $(INCLUDE) webcam.cpp 

run: webcam
	./webcam

clean:
	rm -rf webcam *.o

