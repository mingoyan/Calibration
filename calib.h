#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d")
#pragma comment(lib, "opencv_imgproc2410d")
#pragma comment(lib, "opencv_highgui2410d")
#pragma comment(lib, "opencv_calib3d2410d")
#else
#pragma comment(lib, "opencv_core2410")
#pragma comment(lib, "opencv_imgproc2410")
#pragma comment(lib, "opencv_highgui2410")
#pragma comment(lib, "opencv_calib3d2410")
#endif

#include <opencv2/core/core.hpp>

using namespace cv;

class calib
{
private:
	double r_error;
	char *f_name, *dev_name, *img_name;
	int board_w, board_h;
	int n_squares;
	size_t len;
	string temp;
	vector<Point3f> obj;
	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;
	vector<Point2f> corners;
	Size board_sz;
	FileStorage in_fs, out_fs;
	FileNode fn;
	FileNodeIterator it, it_end;
	Mat rgb_img, gray_img;
	Mat intrinsic = Mat(3, 3, CV_64FC1);
	Mat distCoeffs = Mat(8, 1, CV_64FC1);
	vector<Mat> rvecs;
	vector<Mat> tvecs;

	int import_fs();
	int load_imgs();
	void print_info();
	void calibrate_camera();
	void calibrate_fisheye();
	void undistort_imgs();
	int export_fs();

public:
	calib(char *file_name);
	~calib();
	int run();
};