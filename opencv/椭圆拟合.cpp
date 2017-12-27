#include "cv.h"
#include "highgui.h"

int slider_pos = 70;//定义阈值
IplImage *image1 = 0, *image2 = 0, *image3 = 0;//定义IplImage变量
void process_image(int h);

int main(int argc, char ** argv)
{
	cv::Mat src = cv::imread("view.jpg");
	imshow("原始图", src);
	cv::Mat qz;
	medianBlur(src, qz, 3);//去噪
	imshow("去噪图", qz);
	cv::Mat src_gray;
	cvtColor(qz, src_gray, CV_RGB2GRAY);//灰度图像
	cv::Mat dst = src_gray.clone();//sobel
	int nRows = dst.rows;
	int nCols = dst.cols;
	for (int i = 1; i < nRows - 1; i++)
	{
		for (int j = 1; j < nCols - 1; j++)
		{
			int temp1 = ((-1)*src_gray.at<uchar>(i - 1, j - 1) + src_gray.at<uchar>(i + 1, j - 1)
				+ (-2)*src_gray.at<uchar>(i - 1, j) + 2 * src_gray.at<uchar>(i + 1, j)
				+ (-1)*src_gray.at<uchar>(i - 1, j + 1) + src_gray.at<uchar>(i + 1, j + 1));

			int temp2 = (src_gray.at<uchar>(i - 1, j - 1) + 2 * src_gray.at<uchar>(i, j + 1) + src_gray.at<uchar>(i + 1, j - 1)
				+ (-1)*src_gray.at<uchar>(i - 1, j + 1) + (-2)*src_gray.at<uchar>(i, j + 1) + (-1)*src_gray.at<uchar>(i + 1, j + 1));

			dst.at<uchar>(i, j) = (uchar)sqrt(temp1*temp1 + temp2*temp2);
		}
	}
	imshow("边缘提取", dst);
	imwrite("bytq.jpg", dst);

	const char *filename = "view.jpg";//因为前面的边缘提取处理的图像很差，这里还是用原图
	if ((image1 = cvLoadImage(filename, 0)) == 0)//强制转为灰度值
	{
		return -1;
	}
	image2 = cvCloneImage(image1);
	image3 = cvCloneImage(image1);

	cvNamedWindow("椭圆拟合", 1);

	cvCreateTrackbar("阈值", "椭圆拟合", &slider_pos, 255, process_image);//可以修改阈值

	process_image(0);
	cvWaitKey(0);
	cvReleaseImage(&image2);
	cvDestroyWindow("椭圆拟合");
	return 0;
}

//这个函数寻找出轮廓、用椭圆拟合画出
void process_image(int h)
{
	CvMemStorage *stor;//定义数据结构变量
	CvSeq *cont;
	CvBox2D32f *box;
	CvPoint *PointArray;
	CvPoint2D32f *PointArray2D32f;

	stor = cvCreateMemStorage(0);// 创建内存块
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);//创建序列

	cvThreshold(image1, image2, slider_pos, 255, CV_THRESH_BINARY);//二值化

	cvFindContours(image2, stor, &cont, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));//检索轮廓

	cvZero(image2);
	cvZero(image3);

	//绘制所有轮廓并用椭圆拟合
	for (; cont; cont = cont->h_next)
	{
		int i;
		int count = cont->total;//轮廓个数
		CvPoint center;
		CvSize size;
		int x, y, l, s;

		//个数必须大于6，这是cvFitEllipse_32f的要求
		if (count<6)
		{
			continue;
		}

		//分配内存给点集
		PointArray = (CvPoint *)malloc(count * sizeof(CvPoint));
		PointArray2D32f = (CvPoint2D32f*)malloc(count * sizeof(CvPoint2D32f));

		//分配内存给椭圆数据
		box = (CvBox2D32f *)malloc(sizeof(CvBox2D32f));

		//得到点集
		cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);

		//将CvPoint点集转化为CvBox2D32f集合
		for (i = 0; i<count; i++)
		{
			PointArray2D32f[i].x = (float)PointArray[i].x;
			PointArray2D32f[i].y = (float)PointArray[i].y;
		}

		//拟合当前轮廓
		cvFitEllipse(PointArray2D32f, count, box);

		//绘制当前轮廓
		cvDrawContours(image3, cont, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255),
			0, 1, 8, cvPoint(0, 0));

		//将椭圆数据从浮点转化为整数表示
		center.x = cvRound(box->center.x);
		center.y = cvRound(box->center.y);
		size.width = cvRound(box->size.width*0.5);
		size.height = cvRound(box->size.height*0.5);
		box->angle = -box->angle;

		//画椭圆
		cvEllipse(image3, center, size, box->angle, 0, 360, CV_RGB(0, 0, 255), 1, CV_AA, 0);

		free(PointArray);
		free(PointArray2D32f);
		free(box);
		//输出数据
		x = cvRound(center.x);
		y = cvRound(center.y);
		s = cvRound(size.width*0.5);
		l = cvRound(size.height*0.5);
		printf("椭圆的中心坐标：x=%d, y=%d, 长轴值：l=%d, 短轴值：s=%d \n", x, y, l, s);
	}
	cvShowImage("椭圆拟合", image3);
}
