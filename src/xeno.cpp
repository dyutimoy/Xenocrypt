#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

//Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
int size = 3;
int maxCorners=25;
int ctr=0;
int ctr_n =0;

/// Function header
struct xeno {
    Mat c_img;
    vector<vector<Point> > contour;
    vector<Point2f> c_mc;
    
};
struct goal_count{
    xeno xeno_cnt;
    Point2f len;
    int s_t;
};
xeno square_det(Mat );
xeno triangle_det(Mat);
xeno rear_det(Mat);
xeno start_det(Mat);
xeno front_det(Mat);
bool color_y(float,float,Mat);
bool color_b(float,float,Mat);
bool color_rear(float,float,Mat);
bool color_front(float,float,Mat);


goal_count dir(xeno ,xeno ,xeno ,xeno);
float angle(Point2f ,Point2f,Point2f );
xeno refined(xeno,xeno,float);
Mat countour_detect(Mat);

float dist(Point2f ,Point2f );
/*
Mat dfs_ker(int i, int j, Mat img,Mat &aux)
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
				return aux = dfs_ker(i1, j1, img, aux);
			}
		}
	}
}

Mat dfs(Mat img,Mat aux)
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
        return aux;
}*/
//to check whether point is in the valid region
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
    Mat src,src_gray;    
    //src = imread( argv[1]);                                     //the image can loaded in 3 ways i.e . from webacam or videofile or image
   VideoCapture vid(argv[1]);
   
   int flag_start=0;
   int flag_goal=0;
   float distance,angle_rotate;
    //VideoCapture webcam(0);
   float frame_count = 0;
    Mat frame;
    double tframe;                                      //for videofile only 
    double cframe;
    tframe = vid.get(CV_CAP_PROP_FRAME_COUNT);
    cframe = vid.get(CV_CAP_PROP_POS_FRAMES);
    Point2f start,position ;
    goal_count goal;
    xeno old_start_image,start_image,final_start_image,old_square_image,old_triangle_image,final_square_image,final_triangle_image,square_image,triangle_image,front_image,rear_image; 
   while(1)
   {
          
       /*
        if(frame_count == 0)
        {
            
              vid >> src;            
	Mat resize_src(src.rows,src.cols,CV_8UC3);
  /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );                  //do some filtering later 
        
  /// Create Window
        char* source_window = "Source";
        namedWindow( source_window, CV_WINDOW_AUTOSIZE );
        resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
        imshow( source_window, resize_src );
        
            old_square_image = square_det(resize_src);
            old_triangle_image = triangle_det(resize_src);
            old_start_image = start_det(resize_src); 
            frame_count++;
        }    
        
        while(frame_count <= 100 && frame_count != 0)
        {
            vid >> src;
                        
	Mat resize_src(src.rows,src.cols,CV_8UC3);
  /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );                  //do some filtering later 
        
  /// Create Window
        char* source_window = "Source";
        namedWindow( source_window, CV_WINDOW_AUTOSIZE );
        resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
        imshow( source_window, resize_src );
        
        
            square_image = square_det(resize_src);
            triangle_image = triangle_det(resize_src);
            start_image = start_det(resize_src);
            
         // final_square_image = refined(square_image,old_square_image,frame_count);        //detect and refine it
           final_triangle_image= refined(triangle_image,old_triangle_image,frame_count);
         // final_start_image=refined(start_image,old_start_image,frame_count); 
            old_square_image= square_image;
            
            old_triangle_image=final_triangle_image;
            old_start_image= start_image;
            
            
            frame_count++;
            cout<<frame_count<<"\n";
            
        }
        vid >> src;
                        
	Mat resize_src(src.rows,src.cols,CV_8UC3);
  /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );                  //do some filtering later 
        
  /// Create Window
        char* source_window = "Source";
        namedWindow( source_window, CV_WINDOW_AUTOSIZE );
        resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
        imshow( source_window, resize_src );
        
        if(frame_count==101)
        {
            for( int i = 0; i< old_square_image.contour.size(); i++ )
            {
                if((i%2) == 0)
                {
                   // cout<<old_square_image.c_mc[i]<<"  "<<i/2<<"  "<<old_square_image.contour.size()<<"\n";
                    
                }

            }
            for( int i = 0; i< old_triangle_image.contour.size(); i++ )
            {
                if((i%2) == 0 && old_triangle_image.c_mc[i].x != 0 && old_triangle_image.c_mc[i].y != 0)
                {
                    //cout<<"actual size"<<"\n";
                   // cout<<old_triangle_image.c_mc[i]<<"  "<<i/2<<"  "<<old_triangle_image.contour.size()<<"\n";
                }

            }
            for( int i = 0; i< old_start_image.contour.size(); i++ )
            {
                if((i%2) == 0 && old_start_image.c_mc[i].x != 0 && old_start_image.c_mc[i].y != 0)
                {
                   // cout<<"actual size"<<"\n";
                   // cout<<old_start_image.c_mc[i]<<"  "<<i/2<<"  "<<old_start_image.contour.size()<<"\n";
                }

            }frame_count=500;
            
         
            
        }*/ 
       vid >> src;
                        
	Mat resize_src(src.rows,src.cols,CV_8UC3);
  /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );                  //do some filtering later 
        
  /// Create Window
        char* source_window = "Source";
        namedWindow( source_window, CV_WINDOW_AUTOSIZE );
        resize(src,resize_src,resize_src.size(),0,0,CV_INTER_AREA);
        imshow( source_window, resize_src ); 
       
      //  front_image=front_det(resize_src);
      //  rear_image = rear_det(resize_src);
        /*
        position= ( front_image.c_mc[0] + rear_image.c_mc[0] )/2;
        if (flag_goal==0 && flag_start==0)
        {
            //find the goal
            square_image = square_det(resize_src);
            triangle_image = triangle_det(resize_src);
            goal = dir(square_image,triangle_image,front_image,rear_image);
            if(goal.s_t==0)
            {
                triangle_image.c_img=goal.xeno_cnt.c_img;
            }    
            else square_image.c_img=goal.xeno_cnt.c_img;
            
            
            flag_goal=1;
            
        }    
        
        if(flag_goal==1 && flag_start==0)
        {
            distance = dist(goal.len,position);
            angle_rotate = angle(front_image.c_mc[0],rear_image.c_mc[0],goal.len);
            if(distance >= 30)
            {cout<<"d;"<<distance<<"    going to goal"<<angle_rotate<<"\n";}
            if(fabs(angle_rotate) > 4)
            { cout<<"a; "<<angle_rotate<<"   angle to goal\n";}
            
            if (distance <30)
            {
                flag_goal=2;
                flag_start=1;
                distance=0;
            }  
            
        }    
        if(flag_goal==2 && flag_start ==1)
        {
            start_image = start_det(resize_src);
            if (start_image.contour.size() != NULL)
                cout <<"Not null"<<"\n";
            start =start_image.c_mc[0];
            angle_rotate =angle(front_image.c_mc[0],rear_image.c_mc[0],start);
            if(fabs(angle_rotate) < 4 && dist(front_image.c_mc[0],start)<dist(rear_image.c_mc[0],start))
            {
                flag_goal=2;
                flag_start=2;
            }  
            else{ cout<<"a;"<<angle_rotate<<"   angle to start\n";}
            
        }    
        if(flag_goal==2 &&  flag_start==2)
        {
            distance = dist(position,start);
            angle_rotate = angle(front_image.c_mc[0],rear_image.c_mc[0],start);
            
             if(distance >= 30)
            cout<<"d;"<<distance<<"    going to statrt\n";
            if(dist(start,position)<30)
            {
                flag_goal=0;
                flag_start=0;
            }        
                 if(fabs(angle_rotate) > 4)
           cout<<"a; "<<angle_rotate<<"   angle to start\n";
        }    */
        
        
        //find the nearest point unvisted point
        //apply function dir 
        
        square_image = square_det(resize_src);
            triangle_image = triangle_det(resize_src);
        namedWindow("square",CV_WINDOW_AUTOSIZE );
        imshow("square",square_image.c_img);
        namedWindow("triangle",CV_WINDOW_AUTOSIZE );
        imshow("triangle",triangle_image.c_img);
        
        
            
        int iKey = waitKey(50);
        if (iKey == 27)break;
        
    }
    
       waitKey(0);
   
  return(0);
}

/** @function thresh_callback */
xeno square_det(Mat img )
{
  Mat canny_output,drawing_edge;
  xeno final_image;
  Mat resize_draw(img.rows,img.cols,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( img, canny_output, thresh, thresh*2, 3 );
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
      
     }
   
 
  float area;
  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
      if(contourArea(contours[i])>10 && contourArea(contours[i])<20000)
        { 
            if( color_y(mc[i].y,mc[i].x,img))
            {    
                Scalar color = Scalar(255,count,0 );
                Scalar color_c = Scalar(255,255,0 );
                drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
                // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

                 //circle( drawing, mc[i], 4, color_c, -1, 8, 0 );

                 count = count + 20;
            }   
        }
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);             //resizing the img
  Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )          //convert resize_draw to grayscale..don't know I didn't use direct func
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  //Mat img_dfs_n = dfs(resize_gray,img_dfs);         //Applying dfs
  
 //Apply find countour again on image obtained after dfs
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( resize_gray, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
 // for( int i = 0; i < new_contours.size(); i++ )
 //    { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  
  //for( int i = 0; i < new_contours.size(); i++ )
   //  { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
     //  new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding re=cts + circles
  Mat  new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );;
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
    {
      
      if((i%2) == 0 && new_contours_poly[i].size() ==4)
      {Scalar color = Scalar(255,255,255 );
        new_mu[i] = moments( new_contours[i], false );
        new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 );
         
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
       //drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      rectangle( new_drawing, new_boundRect[i].tl(), new_boundRect[i].br(), Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), 2, 8, 0 );
       circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
      
  //    cout<<new_mc[i]<<"  "<<i/2<<"   "<<new_contours.size()<<"\n";
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
      }
       
        
     }  
  
  final_image.c_img = new_drawing;
  final_image.contour= contours;
  final_image.c_mc=new_mc;
   
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 imshow( "final",drawing );
  
  return final_image;
    
}
xeno triangle_det(Mat img )
{
  Mat canny_output,drawing_edge;
  xeno final_image;
  Mat resize_draw(img.rows,img.cols,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( img, canny_output, thresh, thresh*2, 3 );
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
      
     }
   
 
  float area;
  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
      if(contourArea(contours[i])>50 && contourArea(contours[i])<2000)
        { 
          if(color_y(mc[i].y,mc[i].x,img))  
            {
                Scalar color = Scalar(255,count,0 );
               Scalar color_c = Scalar(255,255,0 );
               drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
              // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
              // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

               //circle( drawing, mc[i], 4, color_c, -1, 8, 0 );

               count = count + 20;
            }   
        }
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);             //resizing the img
  Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )          //convert resize_draw to grayscale..don't know I didn't use direct func
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  //Mat img_dfs_n = dfs(resize_gray,img_dfs);         //Applying dfs
  
 //Apply find countour again on image obtained after dfs
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( resize_gray, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
 // for( int i = 0; i < new_contours.size(); i++ )
 //    { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  
  //for( int i = 0; i < new_contours.size(); i++ )
   //  { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
     //  new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding re=cts + circles
  Mat  new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );;
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
    {
      
      if((i%2) == 0 && new_contours_poly[i].size() ==3)
      {Scalar color = Scalar(255,255,255 );
        new_mu[i] = moments( new_contours[i], false );
        new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 );
         
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
       drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     // rectangle( new_drawing, new_boundRect[i].tl(), new_boundRect[i].br(), Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), 2, 8, 0 );
       circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
      // cout<<"from direct"<<"\n";
   // cout<<new_mc[i]<<"  "<<i/2<<"   "<<new_contours.size()<<"\n";
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
      }
       
        
     }  
  
  final_image.c_img = new_drawing;
  final_image.contour= contours;
  final_image.c_mc=new_mc;
   
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
  
  return final_image;
    
}

xeno front_det(Mat img )
{
  Mat canny_output,drawing_edge;
  xeno final_image;
  Mat resize_draw(img.rows,img.cols,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( img, canny_output, thresh, thresh*2, 3 );
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
      
     }
   
 
  float area;
  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
      if(contourArea(contours[i])>50 && contourArea(contours[i])<2000 )
        { 
            if(img.at<Vec3b>(mc[i].y,mc[i].x)[0]>=100)                          //edit this accordingly
            {
                Scalar color = Scalar(255,count,0 );
               Scalar color_c = Scalar(255,255,0 );
               drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
              // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
              // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

               //circle( drawing, mc[i], 4, color_c, -1, 8, 0 );

               count = count + 20;
            }   
        }
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);             //resizing the img
  Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )          //convert resize_draw to grayscale..don't know I didn't use direct func
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  //Mat img_dfs_n = dfs(resize_gray,img_dfs);         //Applying dfs
  
 //Apply find countour again on image obtained after dfs
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( resize_gray, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
 // for( int i = 0; i < new_contours.size(); i++ )
 //    { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  
  //for( int i = 0; i < new_contours.size(); i++ )
   //  { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
     //  new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding re=cts + circles
  Mat  new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );;
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
    {
      
      if((i%2) == 0 && new_contours_poly[i].size() ==4)
      {Scalar color = Scalar(255,255,255 );
        new_mu[i] = moments( new_contours[i], false );
        new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 );
         
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
       drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     // rectangle( new_drawing, new_boundRect[i].tl(), new_boundRect[i].br(), Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), 2, 8, 0 );
       circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
    //   cout<<"from direct front "<<"\n";
    //cout<<new_mc[i]<<"  "<<i/2<<"   "<<new_contours.size()<<"\n";
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
      }
       
        
     }  
  
  final_image.c_img = new_drawing;
  final_image.contour= contours;
  final_image.c_mc=new_mc;
   
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
  
  return final_image;
    
}
xeno rear_det(Mat img )
{
  Mat canny_output,drawing_edge;
  xeno final_image;
  Mat resize_draw(img.rows,img.cols,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( img, canny_output, thresh, thresh*2, 3 );
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
      
     }
   
 
  float area;
  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
      if(contourArea(contours[i])>50 && contourArea(contours[i])<2000 )
        { 
            if(img.at<Vec3b>(mc[i].y,mc[i].x)[0]>=100)                          //edit this accordingly
            {
                Scalar color = Scalar(255,count,0 );
               Scalar color_c = Scalar(255,255,0 );
               drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
              // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
              // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

               //circle( drawing, mc[i], 4, color_c, -1, 8, 0 );

               count = count + 20;
            }   
        }
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);             //resizing the img
  Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )          //convert resize_draw to grayscale..don't know I didn't use direct func
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  //Mat img_dfs_n = dfs(resize_gray,img_dfs);         //Applying dfs
  
 //Apply find countour again on image obtained after dfs
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( resize_gray, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
 // for( int i = 0; i < new_contours.size(); i++ )
 //    { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  
  //for( int i = 0; i < new_contours.size(); i++ )
   //  { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
     //  new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding re=cts + circles
  Mat  new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );;
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
    {
      
      if((i%2) == 0 && new_contours_poly[i].size() ==4)
      {Scalar color = Scalar(255,255,255 );
        new_mu[i] = moments( new_contours[i], false );
        new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 );
         
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
       drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     // rectangle( new_drawing, new_boundRect[i].tl(), new_boundRect[i].br(), Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), 2, 8, 0 );
       circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
  //     cout<<"from direct rear"<<"\n";
   // cout<<new_mc[i]<<"  "<<i/2<<"   "<<new_contours.size()<<"\n";
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
      }
       
        
     }  
  
  final_image.c_img = new_drawing;
  final_image.contour= contours;
  final_image.c_mc=new_mc;
   
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
  
  return final_image;
    
}
xeno start_det(Mat img )
{
  Mat canny_output,drawing_edge;
  xeno final_image;
  Mat resize_draw(img.rows,img.cols,CV_8UC1);
  vector<vector<Point> > contours;
  
  vector<Vec4i> hierarchy;
  

  /// Detect edges using Threshold
  Canny( img, canny_output, thresh, thresh*2, 3 );
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
      
     }
   
 
  float area;
  
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  
  int count = 0;
  for( int i = 0; i< contours.size(); i++ )
     {
      if(contourArea(contours[i])>50 && contourArea(contours[i])<2000 )
        { 
            if(img.at<Vec3b>(mc[i].y,mc[i].x)[0]>=100)                          //edit this accordingly
            {
                Scalar color = Scalar(255,count,0 );
               Scalar color_c = Scalar(255,255,0 );
               drawContours( drawing, contours_poly, i, color,1, 8, vector<Vec4i>(), 0, Point() );
              // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
              // rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

               //circle( drawing, mc[i], 4, color_c, -1, 8, 0 );

               count = count + 20;
            }   
        }
     }
  
 
  resize(drawing,resize_draw,resize_draw.size(),0,0,CV_INTER_AREA);             //resizing the img
  Mat resize_gray = Mat::zeros( resize_draw.size(), CV_8UC1 );
  for (int i = 0; i < resize_draw.rows; i++)
	{
		for (int j = 0; j < resize_draw.cols; j++)
		{
			if (resize_draw.at<Vec3b>(i, j)[0]>0 )          //convert resize_draw to grayscale..don't know I didn't use direct func
			{
                            resize_gray.at<uchar>(i,j)=255;
			}
		}
	}
  
  
  Mat img_dfs = Mat::zeros( resize_gray.size(), CV_8UC1 );
  
  //Mat img_dfs_n = dfs(resize_gray,img_dfs);         //Applying dfs
  
 //Apply find countour again on image obtained after dfs
  Mat final_img;
  vector<vector<Point> > new_contours;
  
  vector<Vec4i> new_hierarchy;
  
  /// Detect edges using Threshold
  
  /// Find contours
  findContours( resize_gray, new_contours, new_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  
  vector<vector<Point> > new_contours_poly( new_contours.size() );
  vector<Rect> new_boundRect( new_contours.size() );
  vector<Point2f>new_center( new_contours.size() );
  vector<float>new_radius( new_contours.size() );
  
  
  vector<Moments> new_mu(new_contours.size() );
 // for( int i = 0; i < new_contours.size(); i++ )
 //    { new_mu[i] = moments( new_contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> new_mc( new_contours.size() );
  
  //for( int i = 0; i < new_contours.size(); i++ )
   //  { new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 ); }
  
  
  for( int i = 0; i < new_contours.size(); i++ )
     { approxPolyDP( Mat(new_contours[i]), new_contours_poly[i], 3, true );
     //  new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }

   
  /// Draw polygonal contour + bonding re=cts + circles
  Mat  new_drawing = Mat::zeros( resize_draw.size(), CV_8UC3 );;
  namedWindow( "goodfeatures", CV_WINDOW_AUTOSIZE );
  
  int new_count = 0;
  for( int i = 0; i< new_contours.size(); i++ )
    {
      
      if((i%2) == 0 && new_contours_poly[i].size() ==4)
      {Scalar color = Scalar(255,255,255 );
        new_mu[i] = moments( new_contours[i], false );
        new_mc[i] = Point2f( new_mu[i].m10/new_mu[i].m00 , new_mu[i].m01/new_mu[i].m00 );
         
       new_boundRect[i] = boundingRect( Mat(new_contours_poly[i]) );
       drawContours( new_drawing, new_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      // drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     // rectangle( new_drawing, new_boundRect[i].tl(), new_boundRect[i].br(), Scalar(rng.uniform(0,255), rng.uniform(0,255),rng.uniform(0,255)), 2, 8, 0 );
       circle( new_drawing, new_mc[i], 4, color, -1, 8, 0 );
   //    cout<<"from direct start"<<"\n";
   // cout<<new_mc[i]<<"  "<<i/2<<"   "<<new_contours.size()<<"\n";
      // circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       new_count = new_count + 20;
      }
       
        
     }  
  
  final_image.c_img = new_drawing;
  final_image.contour= contours;
  final_image.c_mc=new_mc;
   
 
  //namedWindow( "final", CV_WINDOW_AUTOSIZE );
  
  
 // imshow( "final",drawing_edge );
  
  return final_image;
    
}




float dist(Point2f a,Point2f b)
{
    float distance = sqrt(( a.x-b.x)*(a.x-b.x) +(a.y-b.y)*(a.y-b.y));
    return distance;
}

goal_count dir(xeno xeno_s,xeno xeno_t,xeno xeno_fr,xeno xeno_rr)
{
    
    Point2f center =( xeno_fr.c_mc[0] + xeno_rr.c_mc[0] )/2;
    Point2f prob_dest;
    goal_count final_goal;
    float min_s = 10000000;
    float min_val_s = 0;
    for(int i=0;i<xeno_s.contour.size();i++)
    {
         prob_dest = xeno_s.c_mc[i];
        if(xeno_s.c_mc[i].x!=0 || xeno_s.c_mc[i].y !=0 && xeno_s.c_img.at<Vec3b>(xeno_s.c_mc[i].y,xeno_s.c_mc[i].x)[2] != 50 && min_s>dist(prob_dest,center))
        {
            min_s = dist(prob_dest,center);
            min_val_s=i;        
        }    
    }    
    float min_t = 10000000;
    float min_val_t = 0;
    for(int i=0;i<xeno_t.contour.size();i++)
    {
        prob_dest = xeno_t.c_mc[i];
        if(xeno_t.c_mc[i].x!=0 || xeno_t.c_mc[i].y !=0 &&xeno_t.c_img.at<Vec3b>(xeno_t.c_mc[i].y,xeno_t.c_mc[i].x)[2] != 50 && min_t > dist(prob_dest,center) )
        {
            min_t = dist(prob_dest,center);
            min_val_t=i;        
        }    
    }    
    
    if(min_t/100 > min_s/200   )
    {   
        if(xeno_t.c_img.at<Vec3b>(xeno_t.c_mc[min_val_t].y,xeno_t.c_mc[min_val_t].x)[2] == 255 )
        {
            Scalar color = Scalar(100,100,100 );
            final_goal.xeno_cnt= xeno_t;
            final_goal.len =  xeno_t.c_mc[min_val_t];
            final_goal.s_t = 0;
            circle( final_goal.xeno_cnt.c_img,final_goal.len, 4, color, -1, 8, 0 );
            return final_goal;
        }
        else if(xeno_t.c_img.at<Vec3b>(xeno_t.c_mc[min_val_t].y,xeno_t.c_mc[min_val_t].x)[2] == 100)
        {
            Scalar color = Scalar(50,50,50 );
            final_goal.xeno_cnt= xeno_t;
            final_goal.len =  xeno_t.c_mc[min_val_t];
            final_goal.s_t = 0;
            circle( final_goal.xeno_cnt.c_img,final_goal.len, 4,color , -1, 8, 0 );
            return final_goal;
        }    
        
    }
    else
    {     if(xeno_s.c_img.at<Vec3b>(xeno_s.c_mc[min_val_s].y,xeno_s.c_mc[min_val_s].x)[2] == 255 )
        {
            Scalar color = Scalar(100,100,100 );
            final_goal.xeno_cnt= xeno_s;
            final_goal.len =  xeno_s.c_mc[min_val_s];
            final_goal.s_t = 1;
            circle( final_goal.xeno_cnt.c_img,final_goal.len, 4,color , -1, 8, 0 );
            return final_goal;
        }
        else if(xeno_s.c_img.at<Vec3b>(xeno_s.c_mc[min_val_s].y,xeno_s.c_mc[min_val_s].x)[2] == 100)
        {
            Scalar color = Scalar(50,50,50 );
            final_goal.xeno_cnt= xeno_s;
            final_goal.len =  xeno_s.c_mc[min_val_s];
            final_goal.s_t = 1;
            circle( final_goal.xeno_cnt.c_img,final_goal.len, 4, color, -1, 8, 0 );
            return final_goal;
        }    
    }
    
    
}   

/*
float min_val;
    if(min_t<30)
    {
        return xeno_t.c_mc[min_val_t];
    } 
    if(min_s<30)
    {
        return xeno_s.c_mc[min_val_s];
    }    */

float angle(Point2f front,Point2f rear,Point2f dest)
{ 
    float angle_rotate =0;
    long param;
    //finding angle 
    if(dist(front,dest) > 10)
    {
        
        param = ( dist(front,rear)*dist(front,rear) + dist(rear,dest)*dist(rear,dest) - dist(front,dest)*dist(front,dest))/(2*dist(front,rear)*dist(rear,dest));
        cout<<param<<"  param  "<<dist(front,dest)<<"  \n";
        angle_rotate =  acos (param) * 180 /3.14;
    }
    
    if(front.x >= dest.x)
    {
        angle_rotate = 0.0 -angle_rotate;
        return angle_rotate;
    }
    else
    {
     return angle_rotate;   
    }
    
 
}   


bool color_y(float a,float b,Mat img)
{
    int count = 0 ;
    for (int i1 = a - size / 2; i1 <= a + size / 2; i1++)
	{
		for (int j1 = b - size / 2; j1 <= b + size / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
			{	
                            if (img.at<Vec3b>(i1, j1)[0] > 30 && img.at<Vec3b>(i1, j1)[0]< 255 && img.at<Vec3b>(i1, j1)[1] > 110 && img.at<Vec3b>(i1, j1)[1]<255 && img.at<Vec3b>(i1, j1)[2] > 152 && img.at<Vec3b>(i1, j1)[0]< 255  )
                            {
                                count++;
                            }
                        }
		}
	}
    if(count > 3)
        return  true;
    else return false;
}


bool color_b(float a,float b,Mat img)
{
    int count = 0 ;
    for (int i1 = a - size / 2; i1 <= a + size / 2; i1++)
	{
		for (int j1 = b - size / 2; j1 <= b + size / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
				continue;
			if (img.at<Vec3b>(i1, j1)[0] > 110 && img.at<Vec3b>(i1, j1)[0]< 90 && img.at<Vec3b>(i1, j1)[1] > 88 && img.at<Vec3b>(i1, j1)[1]<120 && img.at<Vec3b>(i1, j1)[2] > 78 && img.at<Vec3b>(i1, j1)[2]< 150  )
			{
                            count++;
			}
		}
	}
    if(count > 3)
        return  true;
    else return false;
}

bool color_front(float a,float b,Mat img)
{
    int count = 0 ;
    for (int i1 = a - size / 2; i1 <= a + size / 2; i1++)
	{
		for (int j1 = b - size / 2; j1 <= b + size / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
				continue;
			if (img.at<Vec3b>(i1, j1)[0] > 162 && img.at<Vec3b>(i1, j1)[0]< 255 && img.at<Vec3b>(i1, j1)[1] > 122 && img.at<Vec3b>(i1, j1)[1]<226 && img.at<Vec3b>(i1, j1)[2] > 136 && img.at<Vec3b>(i1, j1)[2]< 204  )
			{
                            count++;
			}
		}
	}
    if(count > 3)
        return  true;
    else return false;
}
bool color_rear(float a,float b,Mat img)
{
    int count = 0 ;
    for (int i1 = a - size / 2; i1 <= a + size / 2; i1++)
	{
		for (int j1 = b - size / 2; j1 <= b + size / 2; j1++)
		{
			if (i1 < 0 || i1 >= img.rows || j1 < 0 || j1 >= img.cols)
				continue;
			//if (img.at<Vec3b>(i1, j1)[0] > 162 && img.at<Vec3b>(i1, j1)[0]< 255 && img.at<Vec3b>(i1, j1)[1] > 122 && img.at<Vec3b>(i1, j1)[1]<226 && img.at<Vec3b>(i1, j1)[2] > 136 && img.at<Vec3b>(i1, j1)[2]< 204  )
			//{
                            count++;
                            cout<<img.at<Vec3b>(i1, j1)[0]<<"\n";
			//}
		}
	}
    if(count > 3)
        return  true;
    else return false;
}