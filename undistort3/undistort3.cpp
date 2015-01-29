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

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include "undistort3.h"

using namespace cv;
using namespace std;

undistort3::undistort3(string file_name, int device_id)
{
	f_name = file_name;
	dev_id = device_id;
	import_fs();
	cap.open(dev_id);

	if (!cap.isOpened())
	{
		cerr << "Error: Couldn't open the camera." << endl
			<< "device_id: " << dev_id << endl;
		exit(EXIT_FAILURE);

	}
}

undistort3::~undistort3()
{
}

void undistort3::import_fs()
{
	fs.open(f_name, FileStorage::READ);

	if (!fs.isOpened())
	{
		cerr << "Error: Couldn't open "
			<< f_name << endl;
		exit(EXIT_FAILURE);
	}
 
	dev_name = (string)fs["device_name"];
	
	if (dev_name.empty())
	{
		cerr << "Error: device_name not read in " 
			<< f_name << "." << endl;
		exit(EXIT_FAILURE);
	}

	cout << "device name: " << dev_name << endl;
	fs["intrinsic"] >> intrinsic;

	if (intrinsic.empty())
	{
		cerr << "Error: intrinsic not read in "
			<< f_name << endl;
		exit(EXIT_FAILURE);
 	}

	cout << "intrinsic: " << endl << intrinsic << endl;
	fs["distCoeffs"] >> distCoeffs;

	if (distCoeffs.empty())
	{
		cerr << "Error: distCoeffs not read in "
			<< f_name << endl;
		exit(EXIT_FAILURE);
	}

	cout << "distCoeffs: " << endl << distCoeffs << endl;
}
Mat undistort3::get_img_distorted()
{
	cap >> img_distorted;
	return img_distorted;
}

Mat undistort3::get_img_undistorted()
{
	cap >> img_distorted;
	undistort(img_distorted, img_undistorted, intrinsic, distCoeffs);
	return img_undistorted;
}

void undistort3::undistort_img(Mat &src, Mat &dst)
{
	undistort(src, dst, intrinsic, distCoeffs);
}