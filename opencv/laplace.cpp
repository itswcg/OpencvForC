#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat a = imread("zp.jpg");
	imshow("原图", a);
	cvtColor(a, a, CV_RGB2GRAY);     //转为灰度图
	Mat aa;
	Laplacian(a, aa, CV_16S, 3);
	Mat aaa;
	convertScaleAbs(aa, aaa);     //将CV_16S转为CV_8U
	imshow("效果图", aaa);
	cvWaitKey(10000);
}