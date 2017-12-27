#include "cv.h"
#include "highgui.h"

using namespace cv;

int main(int argc, char* argv[])
{
	Mat src = imread("zp.jpg");
	Mat dst;

	//输入图像
	//输出图像
	//输入图像颜色通道数
	//x方向阶数
	//y方向阶数
	Sobel(src, dst, src.depth(), 1, 1);
	imwrite("sobel.jpg", dst);

	//输入图像
	//输出图像
	//输入图像颜色通道数
	Laplacian(src, dst, src.depth());
	imwrite("laplacian.jpg", dst);

	//输入图像
	//输出图像
	//彩色转灰度
	cvtColor(src, src, CV_BGR2GRAY);  //canny只处理灰度图

									  //输入图像
									  //输出图像
									  //低阈值
									  //高阈值，opencv建议是低阈值的3倍
									  //内部sobel滤波器大小
	Canny(src, dst, 50, 150, 3);
	imwrite("canny.jpg", dst);

	imshow("dst", dst);
	waitKey();

	return 0;
}