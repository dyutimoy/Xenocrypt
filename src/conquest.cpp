#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>



using namespace cv;
using namespace std;

int main()
{
    
	VideoCapture vid("/home/dyutimoy/AUV_BAGS & VIDS/Videos/task_buoy_2_20_seconds.avi");
	int threshold = 0;
        int frame_width= vid.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height= vid.get(CV_CAP_PROP_FRAME_HEIGHT);
	namedWindow("vidbin", CV_WINDOW_AUTOSIZE);
        int count =0 ;
	string s,cap_img;
	while (1)
	{
		Mat temp;
		vid >> temp;
                //cout<<frame_width<<"   "<<frame_height<<"\n";
		//Mat grey = createimghumangrey(temp);
		//Mat binary = creatingbinary(grey, threshold);
		
                imshow("vidbin", temp);
                
                 int Key = waitKey(50);
		if (Key == (int)'c')
                {
                    s= to_string(count++);
                    cout<<s<<endl;
                    cap_img = "capture" + s + ".png";
                    cout<<cap_img<<endl;
                    
                    imwrite(cap_img,temp);
                    cout<<Key<<endl;
                    
                }   

                int iKey = waitKey(50);
		if (iKey == (int)'q')
                {
                    cout<<iKey<<endl;
                    break;
                }   
	}return 0;
}

