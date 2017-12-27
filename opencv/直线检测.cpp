#include "opencv2/opencv.hpp"
#define PI 3.1415926

int main(int argc, char *argv[])
{
	cv::Mat image = cv::imread("zp.jpg");
	cv::Mat result;
	cv::cvtColor(image, result, CV_BGRA2GRAY);
	cv::Mat contours;
	cv::Canny(result, contours, 125, 350);  //边缘检测
	std::vector<cv::Vec2f> lines;
	/*霍夫变换,获得一组极坐标参数（rho，theta）,每一对对应一条直线，保存到lines
	第3,4个参数表示在（rho，theta)坐标系里横纵坐标的最小单位，即步长*/
	cv::HoughLines(contours, lines, 1, PI / 180, 80);
	std::vector<cv::Vec2f>::const_iterator iter = lines.begin();
	std::cout << lines.size() << std::endl;
	while (iter != lines.end())
	{
		float rho = (*iter)[0];
		float theta = (*iter)[1];
		if (theta<PI / 4. || theta>3.*PI / 4)
		{   //画交点在上下两边的直线
			cv::Point pt1(rho / cos(theta), 0);
			cv::Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
			cv::line(image, pt1, pt2, cv::Scalar(255), 1);
		}
		else
		{   //画交点在左右两边的直线
			cv::Point pt1(0, rho / sin(theta));
			cv::Point pt2(result.cols, (rho - result.cols*cos(theta) / sin(theta)));
			cv::line(image, pt1, pt2, cv::Scalar(255), 1);
		}
		++iter;
	}
	cv::namedWindow("hough");
	cv::imshow("hough", image);
	cv::waitKey(0);
}