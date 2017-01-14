#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#define size1 3
#define threshold 81
#define hth 40
#define lth 70
using namespace cv;
using namespace std;

void help()
{
 cout << "\nMy first attempt in deteecting corners in multiple squares.\n"
         "\nused canny , findcountours,approxPolyDP,drawcountours then cornerharris "
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

Mat imgin;
Mat cnt_img(imgin.rows, imgin.cols, CV_8UC1, Scalar(0));

const int w = 500;
int levels = 3;
int thresh = 200;
int max_thresh = 255;
int maxCorners = 7;
int maxTrackbar = 25;

RNG rng(12345);
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
/*
static void on_trackbar(int, void*)
{
    Mat cnt_img = Mat::zeros(w, w, CV_8UC3);
    int _levels = levels - 3;
    drawContours( cnt_img, contours, _levels <= 0 ? 3 : -1, Scalar(128,255,255),
                  3, LINE_AA, hierarchy, std::abs(_levels) );

    imshow("contours", cnt_img);
}*/
void displayimage(Mat image, String windowname)
{
	namedWindow(windowname, CV_WINDOW_AUTOSIZE);
	imshow(windowname, image);
}

/** @function cornerHarris_demo */
void cornerHarris_demo( int, void*,Mat);

void goodFeaturesToTrack_Demo( int, void*,Mat,Mat );

int main(int argc, char** argv)
{
    imgin = imread( argv[1],0);
    
    namedWindow("colours",CV_WINDOW_AUTOSIZE);
    namedWindow("cannyedge", CV_WINDOW_AUTOSIZE);
	
	 
    
    Mat imgout(imgin.rows, imgin.cols, CV_8UC1);

    Canny(imgin, imgout, lth, hth, size1);
    
    vector<vector<Point> > contours0;
            findContours(imgout, contours0, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

            contours.resize(contours0.size());
            for( size_t k = 0; k < contours0.size(); k++ )
            approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

               /*
               for (int i = 0; i < contours.size(); i++)
                   {
                           vector<Point> d;
                           approxPolyDP(contours[i], d, size1, 1);
                           printf("the no of corners of %d is %d\n", i + 1, d.size());
                   }
               */
              // displayimage(contours,"contours");
            Mat cnt_img(imgin.rows, imgin.cols, CV_8UC1, Scalar(0));
               /*
               int _levels = levels - 3;
               drawContours( cnt_img, contours, _levels <= 0 ? 3 : -1, Scalar(128,255,255),
                             3, LINE_AA, hierarchy, std::abs(_levels) );
               */
            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0] )
            {
                Scalar color(100, 0, 0 );
                drawContours(cnt_img, contours, idx, color, CV_FILLED, 8, hierarchy );
                cout<< idx;
            }
    
            for (int i = 0; i < contours.size(); i++)
            {
                vector<Point> d;
                approxPolyDP(contours[i], d, size1, 1);
                printf("the no of corners of %d is %li  \n", i + 1, d.size());
            }           

    

    createTrackbar( "Threshold: ", "colours", &thresh, max_thresh );
    createTrackbar( "Max  corners:","colours", &maxCorners, maxTrackbar);
    
    while(1)
    {  
        int key = 0 ;
        while(key != 'q' &&  key != 'Q')
        
        {   
            displayimage(imgin, "colours");
           
            imshow("cannyedge",imgout);
            
            
            
            imshow("contours", cnt_img);
            //cornerHarris_demo( 0, 0 ,cnt_img);
            
            goodFeaturesToTrack_Demo(0,0,imgin,cnt_img);
            
            // cvtColor( cnt_img, cnt_img_gray, CV_BGR2GRAY );

             // on_trackbar(0,0);
            key = waitKey(10);
        }   
        int iKey = waitKey(10);
	if (iKey == 27)break;
    }    
    return 0;

}

/*
void cornerHarris_demo( int, void*,Mat img_corner )
{

  Mat  dst_norm, dst_norm_scaled,img_corner_gray;
  Mat dst(imgin.rows, imgin.cols, CV_8UC1, Scalar(0));
  //cvtColor( img_corner, img_corner_gray, CV_BGR2GRAY );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 5;
  double k = 0.04;

  /// Detecting corners
  cornerHarris( img_corner, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
               cout << Point(i,j).x<<"\n";
               cout << Point(i,j).y << "\n";
               int k = 0;
            cout << "give value of k";
            cin >>  k ;
              }
          }
     }
    
  /// Showing the result
  namedWindow("coners_detected", CV_WINDOW_AUTOSIZE );
  imshow( "coners_detected", dst_norm_scaled);
}
*/

void goodFeaturesToTrack_Demo( int, void*,Mat image,Mat image_gray )
{
  if( maxCorners < 1 ) { maxCorners = 1; }

  /// Parameters for Shi-Tomasi algorithm
  vector<Point2f> corners;
  double qualityLevel = 0.01;
  double minDistance = 50;
  int blockSize = 3;
  bool useHarrisDetector = false;
  double k = 0.04;

  /// Copy the source image
  Mat copy;
  copy = image.clone();

  /// Apply corner detection
  goodFeaturesToTrack( image_gray,
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
    { circle( copy, corners[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),
                                                 rng.uniform(0,255)), -1, 8, 0 ); 
       cout<<" -- Refined Corner ["<<i<<"]  ("<< corners[i].x<<","<<corners[i].y<<")"<<endl; 

    }

  /// Show what you got
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  imshow( "goodfeatures", copy );

  /// Set the neeed parameters to find the refined corners
 // Size winSize = Size( 5, 5 );
 // Size zeroZone = Size( -1, -1 );
 // TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 );

  /// Calculate the refined corner locations
 // cornerSubPix( image_gray, corners, winSize, zeroZone, criteria );

  /// Write them down
  //for( int i = 0; i < corners.size(); i++ )
  //   { cout<<" -- Refined Corner ["<<i<<"]  ("<< corners[i].x<<","<<corners[i].y<<")"<<endl; }
}