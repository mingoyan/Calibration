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

#ifdef _DEBUG
#pragma comment(lib, "opencv_highgui2410d")
#pragma comment(lib, "opencv_core2410d")
#pragma comment(lib, "opencv_calib3d2410d")
#pragma comment(lib, "opencv_imgproc2410d")
#else
#pragma comment(lib, "opencv_highgui2410")
#pragma comment(lib, "opencv_core2410")
#pragma comment(lib, "opencv_calib3d2410")
#pragma comment(lib, "opencv_imgproc2410")
#endif

#define MANUAL 1
#define AUTO 0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class TakeChess
{
private:
	int board_w, board_h, n_boards;
	int device;
	int mode_flag;
	int count = 0;
	int key;
	char device_name[20];
	char mode[2];
	char temp[30];
	bool found, search;
	Mat rgb_img, gray_img;
	Size board_sz;
	vector<Point2f> corners;
	VideoCapture cap;
	FileStorage fs;

	void manu_loop();
	void auto_loop();

public:
	TakeChess();
	~TakeChess();
	void run();
};