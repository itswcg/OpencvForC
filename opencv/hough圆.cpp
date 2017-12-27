#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp> 


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	Mat srcImage = imread("view.jpg"); 
	Mat midImage, dstImage;//临时变量和目标图的定义
	cv::namedWindow("原始图", CV_WINDOW_NORMAL);
	imshow("原始图", srcImage);

	//【3】转为灰度图并进行图像平滑
	cvtColor(srcImage, midImage, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
	GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);

	//【4】进行霍夫圆变换
	vector<Vec3f> circles;
	HoughCircles(midImage, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

	//【5】依次在图中绘制出圆
	for (size_t i = 0; i < circles.size(); i++)
	{
		//参数定义
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//绘制圆心
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制圆轮廓
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
	}

	//【6】显示效果图  
	cv::namedWindow("效果图", CV_WINDOW_NORMAL);
	imshow("效果图", srcImage);

	waitKey(0);

	return 0;

}