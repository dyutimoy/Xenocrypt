#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define krnl 3
#define THRESHOLD 127
#define max 100

int ctr = 0;
int help[max][max];

using namespace cv;
using namespace std;

void d(String windowname, Mat image)
{
	namedWindow(windowname, CV_WINDOW_AUTOSIZE);
	imshow(windowname, image);
}

Mat r(String filename, int type = 1)
{
	if (type)
		return imread(filename);
	else
		return imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
}

Mat hgs(Mat img)
{
	Mat imggs1(img.rows, img.cols, CV_8UC1, 255);  //human eye
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			imggs1.at<uchar>(i, j) = img.at<Vec3b>(i, j)[0] * 0.114 + img.at<Vec3b>(i, j)[1] * 0.587 + img.at<Vec3b>(i, j)[2] * 0.299;
		}
	}
	return imggs1;
}

Mat binary(Mat img1)
{
	Mat img2(img1.rows, img1.cols, CV_8UC1, 255);  //avg
	for (int i = 0; i < img1.rows; i++)
	{
		for (int j = 0; j < img1.cols; j++)
		{
			if (img1.at<uchar>(i, j)>THRESHOLD)
				img2.at<uchar>(i, j) = 255;
			else
				img2.at<uchar>(i, j) = 0;
		}
	}
	return img2;
}

void blobptdetdfsr(int i, int j, Mat img,Mat &aux)
{
	aux.at<uchar>(i, j) = ctr;
	for (int i1 = i - krnl / 2; i1 <= i + krnl / 2; i1++)
	{
		for (int j1 = j - krnl / 2; j1 <= j + krnl / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
				continue;
			if (img.at<uchar>(i1, j1) == 255 && aux.at<uchar>(i1, j1) == 0)
			{
				blobptdetdfsr(i1, j1, img, aux);
			}
		}
	}
}

void blobdetdfsr(Mat img,Mat &aux)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<uchar>(i, j) == 255 && aux.at<uchar>(i, j) == 0)
			{
				ctr= ctr +50;
				blobptdetdfsr(i,j,img,aux);
			}
		}
	}
}

void init(Mat img)
{
	for (int i = 0; i < img.rows; i++)
	for (int j = 0; j < img.cols; j++)
		img.at<uchar>(i, j) = 0;
}

void printarr(Mat img)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			cout <<(int) img.at<uchar>(i, j);
		}
		cout << endl;
	}
}

int main()
{
	int th = 127;
	Mat img1 = r("binedge.png");
	d("Original image", img1);
	Mat img2 = hgs(img1);
	d("Grey Scale Image for Human Eye", img2);
	Mat img = binary(img2);
	d("Binary Image", img);
	Mat aux = img.clone();
	init(aux);
	blobdetdfsr(img, aux);
	printarr(aux);
        imshow("aux",aux);
	int iKey = waitKey(50);
	waitKey(0);
	return 0;
}