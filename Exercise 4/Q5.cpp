/****************************************************************************************************
* 
* Reference examples from http://mercury.pr.erau.edu/~siewerts/cs415/code/computer_vision_cv3_tested/
*           - example-stereo-transform-improved
*           - simple-canny-interactive
*           - simple-hough-eliptical-interactive
****************************************************************************************************/


#include <X11/Xlib.h>
   
  
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
/*definitions for each resolution: Large, Medium, and Small*/
#define HRES_COLS_L (640)
#define VRES_ROWS_L (480)
#define HRES_COLS_M (320)
#define VRES_ROWS_M (240)
#define HRES_COLS_S (160)
#define VRES_ROWS_S (120)

/* Threads for each transform */
pthread_t thread1_canny;
pthread_t thread2_canny;
pthread_t thread3_canny;
pthread_t thread1_elip;
pthread_t thread2_elip;
pthread_t thread3_elip;
pthread_t thread1_hough;
pthread_t thread2_hough;
pthread_t thread3_hough;



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

/*canny transform function has one argument for resolution 3 options L, M, S. */
void CannyTransform(char resolution)
{ 	

	CvCapture* capture;
	int COL;
	int ROW;
	int dev=0;
   
    // set resoloution depending on input
	if (resolution == 'L')
	{
		COL= HRES_COLS_L;
		ROW= VRES_ROWS_L ;
	}
	else if (resolution == 'M')
	{
		COL= HRES_COLS_M;
		ROW= VRES_ROWS_M ;
	}
	else if (resolution == 'S')
	{
		COL= HRES_COLS_S;
		ROW= VRES_ROWS_S ;
	}
	else
	{
		printf("no resolution specified \n"); 
	}
    
   
	namedWindow( "Canny Transform", CV_WINDOW_AUTOSIZE );
    // Create a Trackbar for user to enter threshold
    createTrackbar( "Min Threshold:", "Canny Transform", &lowThreshold, max_lowThreshold, CannyThreshold );

    capture = (CvCapture *)cvCreateCameraCapture(dev);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, COL);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT,ROW);

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





/*ellipical hough transform function has one argument for 
 * resolution 3 options L, M, S. */
void EllipticalTransform(char resolution)
{
	
	
	int COL;
	int ROW;
   
    // set resoloution depending on input
	if (resolution == 'L')
	{
		COL= HRES_COLS_L;
		ROW= VRES_ROWS_L ;
	}
	else if (resolution == 'M')
	{
		COL= HRES_COLS_M;
		ROW= VRES_ROWS_M ;
	}
	else if (resolution == 'S')
	{
		COL= HRES_COLS_S;
		ROW= VRES_ROWS_S ;
	}
	else
	{
		printf("no resolution specified \n"); 
	}
	
	cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture;
    IplImage* frame;
    int dev=0;
    Mat gray;
    vector<Vec3f> circles;
    capture = (CvCapture *)cvCreateCameraCapture(dev);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, COL);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, ROW);
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


/*hough transform function has one argument for resolution 3 options L, M, S. */
void HoughTransform(char resolution)
{

	int COL;
	int ROW;
   
    // set resoloution depending on input
	if (resolution == 'L')
	{
		COL= HRES_COLS_L;
		ROW= VRES_ROWS_L ;
	}
	else if (resolution == 'M')
	{
		COL= HRES_COLS_M;
		ROW= VRES_ROWS_M ;
	}
	else if (resolution == 'S')
	{
		COL= HRES_COLS_S;
		ROW= VRES_ROWS_S ;
	}
	else
	{
		printf("no resolution specified \n"); 
	}
	
	/* Common parameters */
	CvCapture *capture;

	Mat disp,gray;
	int dev=0;
	printf("Will open DEFAULT video device video0\n");
	capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, COL);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, ROW);

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

/*thread definition for canny transform.*/
void *CannyTransform_thread(void *void_resolution)
{
    char *resolution = (char *)void_resolution;
	CannyTransform(*resolution);
	pthread_exit(NULL);
}


/*thread definition for elliptical transform.*/
void *EllipticalTransform_thread(void *void_resolution)
{
    char *resolution = (char *)void_resolution;
	EllipticalTransform(*resolution);
	pthread_exit(NULL);
}


/*thread definition for hough transform.*/
void *HoughTransform_thread(void *void_resolution)
{
    char *resolution = (char *)void_resolution;
	HoughTransform(*resolution);
	pthread_exit(NULL);
}







int main( int argc, char** argv )
{
   
    /* Switch Control */
    int applyCannyTransform = 0;
    int applyHoughTransform = 0;
    int applyHoughElliptical = 0;
    
    /*resolution arguments for threads*/
    char large_resolution = 'L';
	char medium_resolution = 'M';
	char small_resolution = 'S';

    if(argc == 1)
    {
      printf("Enter arguments: c- Canny h- Hough, e-Hough Elliptical\n\r");
    }

    else if(argc == 2)
    {
      
	   /* Enable Transform based on user input */
	    
	  /*create three canny transform threads for Large, medium, and small resolutions*/
      if(strncmp(argv[1],"c",1) == 0)
      {
		   
 
      	if(pthread_create(&thread1_canny, NULL, CannyTransform_thread, &large_resolution)){
				perror("ERROR; pthread_create:thread1");
				exit(-1);
			}
		//if(pthread_create(&thread2_canny, NULL, CannyTransform_thread, &small_resolution)){
				//perror("ERROR; pthread_create:thread1");
				//exit(-1);
			//}			
		if(pthread_join(thread1_canny, NULL)) {
				fprintf(stderr, "Error joining thread\n");
				return 2;
			}
		//if(pthread_join(thread2_canny, NULL)) {
				//fprintf(stderr, "Error joining thread\n");
				//return 2;
			//}
	  }
	  
	  /*create three Hough transform threads for Large, medium, and small resolutions*/
      else if(strncmp(argv[1],"h",1) == 0)
      {
        
		
		if(pthread_create(&thread1_hough, NULL, HoughTransform_thread, &large_resolution)){
				perror("ERROR; pthread_create:thread1");
				exit(-1);
		}
							
		if(pthread_join(thread1_hough, NULL)) {
				fprintf(stderr, "Error joining thread\n");
				return 2;
		}	
      }
      
      
      /*create three hough elliptical transform threads for Large, medium, and small resolutions*/
      else if(strncmp(argv[1],"e",1) == 0)
      {
        	if(pthread_create(&thread1_elip, NULL, EllipticalTransform_thread, &large_resolution)){
				perror("ERROR; pthread_create:thread1");
				exit(-1);
			}
				
			
			if(pthread_join(thread1_elip, NULL)) {
				fprintf(stderr, "Error joining thread\n");
				return 2;
			}
      }


		      

   	}
   

}
