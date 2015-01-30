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


#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d")
#pragma comment(lib, "opencv_calib3d2410d")
#pragma comment(lib, "opencv_highgui2410d")
#pragma comment(lib, "opencv_imgproc2410d")
#pragma comment(lib, "opencv_features2d2410d")
#else
#pragma comment(lib, "opencv_core2410")
#pragma comment(lib, "opencv_calib3d2410")
#pragma comment(lib, "opencv_highgui2410")
#pragma comment(lib, "opencv_imgproc2410")
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class undistort3
{
private:
	string f_name, dev_name;
	int dev_id;
	FileStorage fs;
	FileNode fn;
	Mat intrinsic, distCoeffs;
	Mat img_undistorted, img_distorted;
	VideoCapture cap;
	
	void import_fs();

public:
	undistort3();
	~undistort3();
	void set(string file_name, int device_id);
	Mat get_img_undistorted();
	Mat get_img_distorted();
	Mat get_intrinsic();
	Mat get_distCoeffs();
	void undistort_img(Mat  &src, Mat &dst);
};