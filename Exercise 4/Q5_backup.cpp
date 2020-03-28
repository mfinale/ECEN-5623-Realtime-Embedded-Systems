/****************************************************************************************************
* 
* Reference examples from http://mercury.pr.erau.edu/~siewerts/cs415/code/computer_vision_cv3_tested/
*           - example-stereo-transform-improved
*           - simple-canny-interactive
*           - simple-hough-eliptical-interactive
****************************************************************************************************/

/*
 *
 *  Example by Sam Siewert  - modified for dual USB Camera capture to use to
 *                            experiment and learn Stereo vision concepts pairing with
 *                            "Learning OpenCV" by Gary Bradski and Adrian Kaehler
 *
 *  For more advanced stereo vision processing for camera calibration, disparity, and
 *  point-cloud generation from a left and right image, see samples directory where you
 *  downloaded and built Opencv latest: e.g. opencv-2.4.7/samples/cpp/stero_match.cpp
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

#define HRES_COLS (640)
#define VRES_ROWS (480)

// Should always work for uncompressed USB 2.0 dual cameras
//#define HRES_COLS (320)
//#define VRES_ROWS (240)

//#define HRES_COLS (160)
//#define VRES_ROWS (120)

/* Threads for each transform */
pthread_t thread_canny;
pthread_t thread_hough_elip;
pthread_t thread_hough;

#define ESC_KEY (27)

char snapshotname[80] = "snapshot_xxx.jpg";

/* Canny transform parameters */
int lowThreshold = 0;
int const max_lowThreshold = 100;
int kernel_size = 3;
int edgeThresh = 1;
int ratio = 3;
Mat canny_frame,cdst, timg_gray, timg_grad;



IplImage *frame;

void CannyThreshold(int, void*)
{
    Mat mat_frame(cvarrToMat(frame));

    cvtColor(mat_frame, timg_gray, CV_RGB2GRAY);

    /// Reduce noise with a kernel 3x3
    blur( timg_gray, canny_frame, Size(3,3) );

    /// Canny detector
    Canny( canny_frame, canny_frame, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    timg_grad = Scalar::all(0);

    mat_frame.copyTo( timg_grad, canny_frame);

    imshow( "Canny Transform", timg_grad );

}


void CannyTransform(int,void*)
{ 	CvCapture* capture;
	int dev=0;
   
	namedWindow( "Canny Transform", CV_WINDOW_AUTOSIZE );
    // Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", "Canny Transform", &lowThreshold, max_lowThreshold, CannyThreshold );

    capture = (CvCapture *)cvCreateCameraCapture(dev);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT,VRES_ROWS);

    while(1)
    {
        frame=cvQueryFrame(capture);
        if(!frame) break;


        CannyThreshold(0, 0);

        char q = cvWaitKey(33);
        if( q == 'q' )
        {
            printf("got quit\n"); 
            break;
        }
    }

    cvReleaseCapture(&capture);
}

/* Function to perform Canny transform based on threshold */
void *CannyThreshold(void *unused)
{
	
	CvCapture *capture;

	Mat disp,gray;
	int dev=0;
	
	printf("Will open DEFAULT video device video0\n");
	capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
    
    
    while(1){
		
	//frame=cvQueryFrame(capture); 
    //Mat mat_frame(cvarrToMat(frame));

    //cvtColor(mat_frame, timg_gray, CV_RGB2GRAY);

    ///// Reduce noise with a kernel 3x3
    //blur( timg_gray, canny_frame, Size(3,3) );

    ///// Canny detector
    //Canny( canny_frame, canny_frame, lowThreshold, lowThreshold*ratio, kernel_size );

    ///// Using Canny's output as a mask, we display our result
    //timg_grad = Scalar::all(0);

    //mat_frame.copyTo( timg_grad, canny_frame);

    //imshow( "Canny Transform", timg_grad );

    printf("ipsumblahlbalhaldwweawdawd");
    char c = cvWaitKey(33);
	}
	pthread_exit(NULL);
}



void EllipticalTransform(int, void*)
{
	 cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    //CvCapture* capture = (CvCapture *)cvCreateCameraCapture(0);
    //CvCapture* capture = (CvCapture *)cvCreateCameraCapture(argv[1]);
    CvCapture* capture;
    IplImage* frame;
    int dev=0;
    Mat gray;
    vector<Vec3f> circles;
    capture = (CvCapture *)cvCreateCameraCapture(dev);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
    while(1)
    {
        frame=cvQueryFrame(capture);

        //Mat mat_frame(frame);
        Mat mat_frame(cvarrToMat(frame));

        cvtColor(mat_frame, gray, CV_BGR2GRAY);
        GaussianBlur(gray, gray, Size(9,9), 2, 2);

        HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 100, 50, 0, 0);

        printf("circles.size = %d\n", circles.size());

        for( size_t i = 0; i < circles.size(); i++ )
        {
          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          int radius = cvRound(circles[i][2]);
          // circle center
          circle( mat_frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
          // circle outline
          circle( mat_frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

     
        if(!frame) break;

        cvShowImage("Capture Example", frame);

        char c = cvWaitKey(10);
        if( c == 27 ) break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");

}


void HoughTransform(int, void*)
{

	 
	/* Common parameters */
	CvCapture *capture;

	Mat disp,gray;
	int dev=0;
	printf("Will open DEFAULT video device video0\n");
	capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);

	while(1)
	{
		
		frame=cvQueryFrame(capture); 
		if(!frame) break;
		
		
		vector<Vec4i> lines;
		Mat mat_frame(cvarrToMat(frame));
		Canny(mat_frame, canny_frame, 50, 200, 3);	
		HoughLinesP(canny_frame, lines, 1, CV_PI/180, 50, 50, 10);

		for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			line(mat_frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		 }
		 
		 cvShowImage("Hough Transform", frame);
			
		// Set to pace frame capture and display rate
		char c = cvWaitKey(33);
	 }
	 cvReleaseCapture(&capture);
	cvDestroyWindow("Capture");
	
	

 

}

int main( int argc, char** argv )
{


    
   

    /* Switch Control */
    int applyCannyTransform = 0;
    int applyHoughTransform = 0;
    int applyHoughElliptical = 0;
    


    if(argc == 1)
    {
      printf("Enter arguments: c- Canny h- Hough, e-Hough Elliptical\n\r");
    }

    else if(argc == 2)
    {
      
	    /* Enable Transform based on user input */
      if(strncmp(argv[1],"c",1) == 0)
      {
      	applyCannyTransform = 1;

     }
      else if(strncmp(argv[1],"h",1) == 0)
      {
        applyHoughTransform= 1 ;
      }
      else if(strncmp(argv[1],"e",1) == 0)
      {
        applyHoughElliptical = 1;
      }

	

	  
        
		/* Apply Canny Transform */
        if(applyCannyTransform)
        {
          
			CannyTransform(0,0);
          //if(pthread_create(&thread_canny, NULL, CannyThreshold, NULL)){
			//perror("ERROR; pthread_create:thread1");
			//exit(-1);
			//}
				    
        }
		/* Apply Hough Transform */
  	    else if(applyHoughTransform)
		{
			HoughTransform(0,0);
		
		}
		/* Apply Hough Elliptical transform */
		else if(applyHoughElliptical)
		{
			EllipticalTransform(0,0);
		}


	 
	
   	}
   

}
