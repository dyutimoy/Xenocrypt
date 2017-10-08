#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>



using namespace cv;
using namespace std;



void displayimage(Mat image, String windowname)
{
	namedWindow(windowname, CV_WINDOW_AUTOSIZE);
	imshow(windowname, image);
}

Mat Input()
{
	Mat image;
	image = imread("hsv.png");
	return (image);
}
Mat createimghumangrey(Mat img)
{

	Mat humangrey(img.rows, img.cols, CV_8UC1);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int val = img.at<Vec3b>(i, j)[0] * 0.114 + img.at<Vec3b>(i, j)[1] * 0.587 + img.at<Vec3b>(i, j)[2] * 0.299;
			humangrey.at<uchar>(i, j) = val;

		}
	}
	//namedWindow("humanscale", CV_WINDOW_AUTOSIZE);
	//namedWindow("colour", CV_WINDOW_AUTOSIZE);
	//imshow("humanscale", humangrey);
	//imshow("colour", img);
	return(humangrey);
}
Mat creatingbinary(Mat img1,int threshold)
{
	Mat binimg(img1.rows, img1.cols, CV_8UC1);
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			int val = img1.at<uchar>(i, j);
			if (val>threshold)
			{
				binimg.at<uchar>(i, j) = 255;

			}
			else { binimg.at<uchar>(i, j) = 0; }

		}
	}
	return (binimg);
}
/*int decidethresold(Mat img2)
{
	int thres, A[256] = { 0 };
	for (int i = 0; i < img2.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			A[img2.at<uchar>(i, j)]++;
		}
	}
	int temp = 0;
	for (int k = 0; k < 256; k++)
	{
		
		temp += A[k];
		if (temp >= (img2.rows*img2.cols) / 2)
		{
			thres = k;
			return thres;
		}
	}
}*/

Mat lochsv(Mat img,int hx,int hn ,int sx ,int sn,int vx, int vn)
{
	Mat dechsv(img.rows, img.cols, CV_8UC1,Scalar(0));

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<Vec3b>(i, j)[0] <= hx && img.at<Vec3b>(i, j)[0] >= hn &&img.at<Vec3b>(i, j)[1] < sx &&img.at<Vec3b>(i, j)[1] > sn &&img.at<Vec3b>(i, j)[2] < vx &&img.at<Vec3b>(i, j)[2] > vn)
				dechsv.at<uchar>(i, j) = 255;
		}
	}return dechsv;
}

int main()
{
	VideoCapture webcam(0);
	int threshold = 0;
        int frame_width= webcam.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height= webcam.get(CV_CAP_PROP_FRAME_HEIGHT);
	namedWindow("vidbin", CV_WINDOW_AUTOSIZE);
	createTrackbar("threshold", "vidbin", &threshold, 255);
        cout<<frame_width<<"   "<<frame_height<<"\n";
        VideoWriter video("comeout.mp4",CV_FOURCC('D','I','V','3'),1,Size(frame_width,frame_height),true);
	while (1)
	{
		Mat temp;
		webcam.read(temp);
                cout<<frame_width<<"   "<<frame_height<<"\n";
		//Mat grey = createimghumangrey(temp);
		//Mat binary = creatingbinary(grey, threshold);
		video.write(temp);
		imshow("vidbin", temp);
                

		waitKey(50);

	}return 0;
}
