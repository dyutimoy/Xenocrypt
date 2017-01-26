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
int ctr_n =0;
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
/*
void dfs_ker_n(int i, int j, Mat img,Mat &aux_n)
{
	aux_n.at<Vec3b>(i, j) = ctr;
        
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
dfs_n(Mat img,Mat &aux_n)
{
    for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<uchar>(i, j) > 0 && aux_n.at<Vec3b>(i, j)[0] == 0)
			{
				 ctr_n = ctr_n +50;
				dfs_ker(i,j,img,aux_n);
			}
		}
	}
}
*/
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
 
    /// Load source image and con
       VideoCapture vid("conquest_sample_arena.webm");
        //VideoCapture webcam(0);
       
	Mat frame;
	double tframe;
	double cframe;
	tframe = vid.get(CV_CAP_PROP_FRAME_COUNT);
	cframe = vid.get(CV_CAP_PROP_POS_FRAMES);
	
	while(1)
        {
            
                src = imread( argv[1]);
		 //webcam.read(src);
                //vid >> src;
                 
		 Mat resize_src(src.rows/7,src.cols/7,CV_8UC3);
  /// Convert image to gray and blur it
                 cvtColor( src, src_gray, CV_BGR2GRAY );
                blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
                 char* source_window = "Source";
                 namedWindow( source_window, CV_WINDOW_AUTOSIZE );
                  resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
                  imshow( source_window, resize_src );
  

                
                thresh_callback( 0, 0 );
                
              // int k = 0;
            //cout << "give value of k";
            //cin >>  k ;
               
                   int iKey = waitKey(50);
                    if (iKey == 27)break;
		
	}
    
    
   
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output,drawing_edge;
  Mat resize_draw(src_gray.rows/7,src_gray.cols/7,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

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
       drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
       cout << mc[i].x<< "  "<<i<<"   "<<"\n";
               cout <<mc[i].y << "\n";
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
  
   
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( img_dfs, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
  for( int i = 0; i < new_contours.size(); i++ )
     { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  for( int i = 0; i < new_contours.size(); i++ )
     { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding rects + circles
  Mat new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
     {
       Scalar color = Scalar(255,255,255 );
      // drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       rectangle( new_drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
      // circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
          
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
     }  
 
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
 imshow( "goodfeatures",new_drawing );
  
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  
  imshow( "Contours",resize_draw);
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
    
}


