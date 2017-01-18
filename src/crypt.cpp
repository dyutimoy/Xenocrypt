#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
int size = 3;
int maxCorners=25;
int ctr=0;
/// Function header
void thresh_callback(int, void* );


void dfs_ker(int i, int j, Mat img,Mat &aux)
{
	aux.at<uchar>(i, j) = ctr;
        
	for (int i1 = i - size / 2; i1 <= i + size / 2; i1++)
	{
		for (int j1 = j - size / 2; j1 <= j + size / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
				continue;
			if (img.at<uchar>(i1, j1) > 0 && aux.at<uchar>(i1, j1) == 0)
			{
				dfs_ker(i1, j1, img, aux);
			}
		}
	}
}

void dfs(Mat img,Mat &aux)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<uchar>(i, j) > 0 && aux.at<uchar>(i, j) == 0)
			{
				 ctr = ctr +50;
				dfs_ker(i,j,img,aux);
			}
		}
	}
}


int Isvalid(Mat img, int a, int b)
{
	if (a < 0 || a >= img.rows || b < 0 || b >= img.cols)
	{
		return 0;
	}
	else return 1;
}

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );
  Mat resize_src(src.rows/7,src.cols/7,CV_8UC3);
  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
  imshow( source_window, resize_src );
  

  createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output,drawing_edge;
  Mat resize_draw(src_gray.rows/7,src_gray.cols/7,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  vector<Vec4i> new_hierarchy;

  /// Detect edges using Threshold
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );
  
  
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
  
  
  for( int i = 0; i < contours.size(); i++ )
     { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }
   
  vector<vector<Point> >hull( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
      {  convexHull( Mat(contours[i]), hull[i], false ); }

  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar(255,255,255 );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       count = count + 20;
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);
   Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  dfs(resize_gray,img_dfs);
  
/*
  /// Parameters for Shi-Tomasi algorithm
  vector<Point2f> corners;
  double qualityLevel = 0.01;
  double minDistance = 50;
  int blockSize = 3;
  bool useHarrisDetector = true;
  double k = 0.04;

  /// Copy the source image
  Mat copy,resize_gray;
  copy = resize_draw.clone();
  
  cvtColor( resize_draw, resize_gray, CV_BGR2GRAY );

  /// Apply corner detection
  goodFeaturesToTrack( resize_gray,
                       corners,
                       maxCorners,
                       qualityLevel,
                       minDistance,
                       Mat(),
                       blockSize,
                       useHarrisDetector,
                       k );


  /// Draw corners detected
  cout<<"** Number of corners detected: "<<corners.size()<<endl;
  int r = 4;
  for( int i = 0; i < corners.size(); i++ )
    { circle( copy, corners[i], r, Scalar(100,0,0), -1, 8, 0 ); 
       cout<<" -- Refined Corner ["<<i<<"]  ("<< corners[i].x<<","<<corners[i].y<<")"<<endl; 

    }
*/
  /// Show what you got
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  imshow( "goodfeatures",img_dfs );
  
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  
  imshow( "Contours",resize_draw);
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
    
}

