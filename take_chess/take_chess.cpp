//Copyright(C) 2015  Nao Kasahara
//
//This program is free software : you can redistribute it and / or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details./>
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see <http://www.gnu.org/licenses>

#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "time.h"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "take_chess.h"

using namespace cv;
using namespace std;

TakeChess::TakeChess()
{
	/* Initialize some valiables. */
	printf("Enter number of corners along width: ");
	scanf("%d", &board_w);

	printf("Enter number of corners along height: ");
	scanf("%d", &board_h);

	board_sz = Size(board_w, board_h);

	printf("Enter number of boards: ");
	scanf("%d", &n_boards);

	printf("Enter device name: ");
	scanf("%s", device_name);

	printf("Enter device_id: ");
	scanf("%d", &device);

	printf("Which modes do you want to use,"
		"\"auto saving mode\" or \"manual saving mode\"?\n");
	printf("Press a (auto) or m (manual) to decide a mode: ");

	while (true)
	{
		scanf("%s", mode);
		
		if (strcmp(mode, "a") == 0)
		{
			mode_flag = AUTO;
			break;
		}

		if (strcmp(mode, "m") == 0)
		{
			mode_flag = MANUAL;
			break;
		}
		else
		{
			printf("Invalid key pressed.\n"
				"Press a (auto) or m (manual) to decide a mode: ");
		}

	}

	/* Open a camera. */
	cap.open(device);

	if (!cap.isOpened())
	{
		fprintf(stderr, "Couldn't open the camera.\n");
		fprintf(stderr, "device id: %d\n", device);
		exit(1);
	}

	/* Initialize xml file. */
	sprintf(temp, "chess_img_list(%s).xml", device_name);
	fs.open(temp, FileStorage::WRITE);
	fs << "device_name" << "["
		<< device_name << "]";
	fs << "chessboard" << "{";
	fs << "width" << board_w;
	fs << "height" << board_h;
	fs << "}";
	fs << "images" << "[";

	/* Print valiables. */
	printf("\n\n--------------Information-------------------\n");
	printf("number of corners along width: %d\n", board_w);
	printf("number of corners along height: %d\n", board_h);
	printf("number of boards: %d\n", n_boards);
	printf("device name: %s\n", device_name);
	printf("device id: %d\n\n\n", device);
	printf("mode: %s\n", mode);
}

TakeChess::~TakeChess()
{
	/* Terminate this process. */
	cap.release();
	fs.release();
	destroyAllWindows();
	printf("See you.\n");
}

void TakeChess::manu_loop()
{
	namedWindow("camera");

	printf("Storing chessboard  images process started.\n");
	printf("Searching chessboard...\n");

	time_t t1, t2;
	search = false;
	t1 = time(NULL);

	while (true)
	{
		cap >> rgb_img;
		imshow("camera", rgb_img);

		if (search)
		{
			cvtColor(rgb_img, gray_img, CV_BGR2GRAY);
			found = findChessboardCorners(gray_img, board_sz, corners,
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		}

		imshow("camera", rgb_img);

		if (found)
		{
			search = false;
			printf("Chessboard found.\n");
			printf("Press s (store), esc (close)," 
				"\n or other key (not store) to decide whether to save.\n");
			cornerSubPix(gray_img, corners, Size(11, 11),
				Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_img, board_sz, corners, found);
			imshow("camera", gray_img);
			key = waitKey();

			if (key == 's')
			{
				sprintf(temp, "chess_img%03d.jpg", count);
				imwrite(temp, rgb_img);
				fs << temp;
				printf("%s stored.\n", temp);
				count++;

				if (count == n_boards)
				{
					printf("%d images stored.\n", count);
					break;
				}

				printf("Searching chessboard...\n");
				t1 = time(NULL);
			}
			else if (key == 27)
			{
				printf("esc key pressed.\n"
					"Process interrupted.\n");
				break;
			}
			else
			{
				printf("Not saved.\n");
				printf("Searching chessboard...\n");
				t1 = time(NULL);
			}

		}

		if (waitKey(30) == 27)
		{
			printf("esc key pressed.\n"
				"Process interrupted.\n");
			printf("%d images stored.\n", count);
			break;
		}

		t2 = time(NULL);
		found = false;

		if ((t2 - t1) > 3)
			search = true;

	}

	fs << "]";
}

void TakeChess::auto_loop()
{
	printf("Sorry, this mode is not supported yet.\n");
}

void TakeChess::run()
{
	if (mode_flag == MANUAL)
	{
		manu_loop();
	}
	else
	{
		auto_loop();
	}
}