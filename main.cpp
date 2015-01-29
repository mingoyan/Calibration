#include <opencv2/core/core.hpp>
#include "calib.h"

using namespace cv;

int main(int argc, char **argv)
{
	calib cc(argv[1]);
	return cc.run();
}
