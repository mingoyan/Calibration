//Copyright(C) 2015 Nao Kasahara
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

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "undistort3.h"

int main(int argc, char **argv)
{
	undistort3 undist;
	undist.set(argv[1], 1);
	Mat img, img_undistorted;
	namedWindow("disp_distorted");
	namedWindow("disp_undistorted");

	while (true)
	{
		img = undist.get_img_distorted();
		imshow("disp_distorted", img);
		img_undistorted = undist.get_img_undistorted();
		imshow("disp_undistorted", img_undistorted);

		if ((waitKey(30)) == 27)
		{
			cout << "esc key pressed." << endl;
			break;
		}
	}

	cout << "See you." << endl;
	return 0;
}