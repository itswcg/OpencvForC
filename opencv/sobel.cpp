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
	Mat ax, ay;
	Mat axx, ayy;
	Sobel(a, ax, CV_16S, 1, 0, -1);
	Sobel(a, ay, CV_16S, 0, 1, -1);
	convertScaleAbs(ax, axx);      //将CV_16S转为CV_8U
	convertScaleA, a);
	cvWaitKey(10000);
}