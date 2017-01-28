#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include<stack>

using namespace cv;
using namespace std;

typedef struct tag
{
	int x, y;
}point;
Mat search(stack<point>s, Mat img, Mat path);

Mat pic = imread("ps1.jpg", 1);
Mat img(pic.rows, pic.cols, CV_8UC1, 0);
Mat path(pic.rows, pic.cols, CV_8UC1, 0);
int visited[500][1000] = { 0 };

int main()
{
	cvtColor(pic, img, CV_BGR2GRAY);
	cvtColor(pic, path, CV_BGR2GRAY);
	stack<point> s;
	point start = { 31, 27 };
	s.push(start);
	visited[start.x][start.y] = 1;
	path =search(s, img, path);
	imshow("Path", path);
	waitKey(0);
	
}

Mat search(stack<point>s, Mat img, Mat path)
{
	point present;
	char dirn;
	while (!s.empty())
	{
		present = s.top();
		int i = present.x, j = present.y;
		s.pop();
		if (i > 160 && i<168 && j > 166 && j<174) break;
		if (i>7 && j > 7 && i < pic.cols - 7 && j < pic.rows - 7)
		{

                        /*

			if (img.at<uchar>(i, j - 1) < 200 && !(visited[i][j - 1]))
			{
				s.push({ i, j - 1 }); visited[i][j - 1] = 1;
			}
			if (img.at<uchar>(i - 1, j + 1) < 200 && !(visited[i - 1][j + 1]))
			{
				s.push({ i - 1, j + 1 }); visited[i - 1][j + 1] = 1;
			}




			if (img.at<uchar>(i, j + 1) < 200 && !(visited[i][j + 1]))
			{
				s.push({ i, j + 1 }); visited[i][j + 1] = 1;
			}
			if (img.at<uchar>(i - 1, j - 1) < 200 && !(visited[i - 1][j - 1]))
			{
				s.push({ i - 1, j - 1 }); visited[i - 1][j - 1] = 1;
			}
			if (img.at<uchar>(i + 1, j) < 200 && !(visited[i + 1][j]))
			{
				s.push({ i + 1, j }); visited[i + 1][j] = 1;
			}

			if (img.at<uchar>(i + 1, j - 1) < 200 && !(visited[i + 1][j - 1]))
			{
				s.push({ i + 1, j - 1 }); visited[i + 1][j - 1] = 1;
			}
			if (img.at<uchar>(i - 1, j) < 200 && !(visited[i - 1][j]))
			{
				s.push({ i - 1, j }); visited[i - 1][j] = 1;
			}
			if (img.at<uchar>(i + 1, j + 1) < 200 && !(visited[i + 1][j + 1]))
			{
				s.push({ i + 1, j + 1 }); visited[i + 1][j + 1] = 1;
			}*/
			for (int a = i + 1; a >= i - 1; a--)
			{
			for (int b = j + 1; b >= j - 1; b--)
			{
			if (img.at<uchar>(a, b) < 200 && !(visited[a][b]))
			{
			s.push({ a, b });
			visited[a][b] = 1;
			}
			}
			}
		}
		path.at<uchar>(i, j) = 255;
		imshow("Path", path);
		cout << present.x << " " << present.y << "\n";
		waitKey(10);
	}
	return path;
}