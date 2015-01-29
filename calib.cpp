#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include "calib.h"

using namespace cv;
using namespace std;

int calib::run()
{
	import_fs();
	print_info();
	load_imgs();
	calibrate_fisheye();
	//calibrate_camera();
	undistort_imgs();
	export_fs();
	return EXIT_SUCCESS;
}

calib::calib(char *file_name)
{
	f_name = file_name;
}

calib::~calib()
{
	in_fs.release();
	out_fs.release();
	printf("See you.\n");
}

int calib::import_fs()
{	
	in_fs.open(f_name, FileStorage::READ);

	if (!in_fs.isOpened())
	{
		fprintf(stderr, "Erorr: Couldn't open %s.\n", f_name);
		return EXIT_FAILURE;
	}

	//string to char*
	temp = (string)in_fs["device_name"];
	len = temp.length();
	dev_name = new char[len + 1];
	memcpy(dev_name, temp.c_str(), len + 1);

	fn = in_fs["chessboard"];
	board_w = (int)fn["width"];
	board_h = (int)fn["height"];
	
	n_squares = board_w * board_h;
	board_sz = Size(board_h, board_w);

	/* Create a list of cordinates. */
	for (int j = 0; j < n_squares; j++)
		obj.push_back(Point3f(j / board_h, j%board_h, 0.0f));

	return EXIT_SUCCESS;
}

void calib::print_info()
{
	printf("--------information-------\n");
	printf("file name: %s\n", f_name);
	printf("device name: %s\n", dev_name);
	printf("width of chessboard: %d\n", board_w);
	printf("height of chessboard: %d\n", board_h);
}

int calib::load_imgs()
{
	namedWindow("disp");
	fn = in_fs["images"];
	it = fn.begin();
	it_end = fn.end();
	int successes = 0;
	int count = 0;

	for (; it != it_end; it++, count++)
	{
		//string to char*
		temp = (string)*it;
		len = temp.length();
		img_name = new char[len + 1];
		memcpy(img_name, temp.c_str(), len + 1);

		rgb_img = imread(img_name);
		
		if (rgb_img.empty())
		{
			fprintf(stderr, "Error: Couldn't read %s\n", img_name);
			return EXIT_FAILURE;
		}

		cvtColor(rgb_img, gray_img, CV_BGR2GRAY);
		bool found = findChessboardCorners(gray_img, board_sz, corners,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found)
		{
			cornerSubPix(gray_img, corners, Size(11, 11),
				Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(rgb_img, board_sz, corners, found);
			image_points.push_back(corners);
			object_points.push_back(obj);
			printf("Success: chessboard found in %s\n", img_name);
			successes++;
		}
		else
		{
			printf("Warning: Couldn't find the chessboard in %s\n", img_name);
			continue;
		}

		cv::imshow("disp", rgb_img);
		cv::waitKey(10);
	}

	printf("number of found chess boards / number of source images:"
		" %d / %d\n", successes, count);
	destroyAllWindows();
	return EXIT_SUCCESS;
}

void calib::calibrate_camera()
{
	//intrinsic.ptr<float>(0)[0] = 1;
	//intrinsic.ptr<float>(1)[1] = 1;
	printf("Calibrating....\n");
	r_error = calibrateCamera(object_points, image_points,
		gray_img.size(), intrinsic, distCoeffs, rvecs, tvecs);
	printf("Calibration finished.\n");
	printf("reprojection error: %f\n", r_error);
}

void calib::calibrate_fisheye()
{
	printf("Calibrating....\n");
	r_error = calibrateCamera(object_points, image_points,
		gray_img.size(), intrinsic, distCoeffs, rvecs, tvecs, CV_CALIB_FIX_K6);
	printf("Calibration finished.\n");
	printf("reprojection error: %f\n", r_error);
	cout << "intrinsic: "
		<< intrinsic << endl;
	cout << "distCoeffs: "
		<< distCoeffs << endl;
}
void calib::undistort_imgs()
{
	Mat img_undistorted;
	int key;

	fn = in_fs["images"];
	it = fn.begin();
	it_end = fn.end();
	
	namedWindow("src");
	namedWindow("undistorted");
	printf("Press space key to go next image,\n"
		" or esc key to close this application.\n");
	
	for (; it != it_end; it++)
	{
		//string to char*
		temp = (string)*it;
		len = temp.length();
		img_name = new char[len + 1];
		memcpy(img_name, temp.c_str(), len + 1);

		printf("%s called.\n", img_name);
		rgb_img = imread(img_name);
		undistort(rgb_img, img_undistorted, intrinsic, distCoeffs);
		imshow("src", rgb_img);
		imshow("undistorted", img_undistorted);
		key = waitKey();

		if (key == 27)
		{
			printf("esc key pressed.\n");
			break;
		}

		if (key == ' ')
			printf("space key pressed.\n");
	}
}

int calib::export_fs()
{
	temp = dev_name;
	temp.append("_conf.xml");
	printf("%s_conf.xml created.\n", dev_name);
	out_fs.open(temp, FileStorage::WRITE);
	out_fs << "device_name" << "["
		<< dev_name << "]";
	out_fs << "intrinsic" << intrinsic;
	out_fs << "distCoeffs" << distCoeffs;
	//out_fs << "rvecs" << rvecs;
	//out_fs << "tvecs" tvecs;
	out_fs << "reprojection_error" << "["
		<< r_error << "]";
	return EXIT_SUCCESS;
}