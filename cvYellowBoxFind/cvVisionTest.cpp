// BinDetectorOpenCV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//
//#pragma comment(lib, "opencv_calib3d2410d.lib")
//#pragma comment(lib, "opencv_contrib2410d.lib")
//#pragma comment(lib, "opencv_core2410d.lib")
//#pragma comment(lib, "opencv_features2d2410d.lib")
//#pragma comment(lib, "opencv_flann2410d.lib")
//#pragma comment(lib, "opencv_gpu2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib") //gui and window stuff is here
#pragma comment(lib, "opencv_imgproc2410d.lib")
//#pragma comment(lib, "opencv_legacy2410d.lib")
//#pragma comment(lib, "opencv_ml2410d.lib")
//#pragma comment(lib, "opencv_nonfree2410d.lib")
//#pragma comment(lib, "opencv_objdetect2410d.lib")
//#pragma comment(lib, "opencv_ocl2410d.lib")
//#pragma comment(lib, "opencv_photo2410d.lib")
//#pragma comment(lib, "opencv_stitching2410d.lib")
//#pragma comment(lib, "opencv_superres2410d.lib")
//#pragma comment(lib, "opencv_ts2410d.lib")
//#pragma comment(lib, "opencv_video2410d.lib")
//#pragma comment(lib, "opencv_videostab2410d.lib")

using namespace cv;
using namespace std;

#pragma comment(lib, "opencv_core2410d.lib")

void warpMatrix(Size   sz,
	double theta,
	double phi,
	double gamma,
	double scale,
	double fovy,
	Mat&   M,
	vector<Point2f>* corners);

void warpImage(const Mat &src,
	double    theta,
	double    phi,
	double    gamma,
	double    scale,
	double    fovy,
	Mat&      dst,
	Mat&      M,
	vector<Point2f> &corners);



int _tmain2(int argc, _TCHAR* argv[])
{
	return 0;
}


using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 70;// 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);
void go(char * img);


//normvalue=val* 180/255
//labivew
//hue:24-49
//sat:39-255
//val/intensity:67-255

int iLowH = 34 * 180 / 255.0;
int iHighH = 172 * 180 / 255.0;
int iLowS = 60 * 180 / 255.0;
int iHighS = 255 * 180 / 255.0;
int iLowV = 131;// 162 * 180 / 255.0;
int iHighV = 190;// 255 * 180 / 255.0; 

int warpX = 0;
int warpY = 0;
int warpZ = 0;

Mat m, disp, warp;

//from image analyzer software
//h 177 - 199
//s 166 - 183
//i 95 - 124


//
//int iLowH = 0;
//int iHighH = 179;
//int iLowS = 0;
//int iHighS = 255;
//int iLowV = 0;
//int iHighV = 255;

vector<Point> match_template;
Mat MatMatch;



void getTemplate(char * filename, bool use_warp = false);
void colorThresh();

/** @function main */
void test(int state, void *)
{

}

char *img = R"(C:\Program Files (x86)\National Instruments\LabVIEW 2014\examples\FRC\roboRIO\Vision\2015 Vision Example\Sample Images\image%d.jpg)";
char *templateFile = R"(C:\Program Files (x86)\National Instruments\LabVIEW 2014\examples\FRC\roboRIO\Vision\2015 Vision Example\Sample Images\templates\2.png)";
char temp[355];



void cameraloop()
{

	Mat m, disp, warp;
	vector<Point2f> corners;
	VideoCapture cap(0);

	
	

	while (cap.isOpened())
	{
		printf("grabbing\n");
		cap >> m;
		//warpImage(m, 5, 50, 0, 1, 30, disp, warp, corners);
		imshow("Cam", m);
	}


}

void fileloop()
{




	vector<Point2f> corners;
	//	VideoCapture cap(0);


	//right arrow 2555904
	//left arrow 2424832

	for (int i = 24; i <= 24;)
	{

		sprintf(temp, img, i);
		//sprintf(temp, templates, i);
		go(temp);
	STAY:
		int k = waitKey(0); //97 is 'a'
		printf("key:%d\n", k);
		switch (k)
		{

		case 49: //1, an option


			//(Rtheta, Rphi and Rgamma), covering Z, X and Y rotation
			//cap >> m;
			warpImage(MatMatch, warpZ, warpX, warpY, 1, 30, disp, warp, corners);

			imshow("Disp", disp);



			break;

		case 50: //2

			getTemplate("", true); //grab the warped image



			break;



		case 113: //q
			i--;


			break;


		case 101: //e
			i++;

			break;

		case 27: //escape
			destroyAllWindows();
			exit(0);

			break;




		case 114: //reset 'r'

			iLowH = 34 * 180 / 255.0;
			iHighH = 172 * 180 / 255.0;
			iLowS = 60 * 180 / 255.0;
			iHighS = 255 * 180 / 255.0;
			iLowV = 131;// 162 * 180 / 255.0;
			iHighV = 190;// 255 * 180 / 255.0; 




			break;

		case 97: //try the color threshold
			colorThresh();
			goto STAY;

			break;

		case 2555904: //right arrow
			i++;
			break;

		case 2424832:// left arror
			i--;
			break;

		default:
			i++;
		}
	}


}

int main(int argc, char** argv)
{
	

	int i = 2; //for the regular images, first starts with 2. 
	//int i = 1;

	printf("template size is:%d\n", match_template.size());

	getTemplate(templateFile);

	printf("template size is:%d\n", match_template.size());


	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"


	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);



	cvCreateTrackbar("X", "Control", &warpX, 360); //Value (0 - 255)
	cvCreateTrackbar("Y", "Control", &warpY, 360);
	cvCreateTrackbar("Z", "Control", &warpZ, 360);


	//fileloop();
	cameraloop();


	// 


	return(0);

}

void colorThresh()
{

	Mat imgThresholded;

	inRange(src, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (remove small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	////morphological closing (fill small holes in the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	imshow("Thresholded Image", imgThresholded); //show the thresholded image
	// imshow("Original", imgOriginal); //show the original image



}

void go(char * img)
{
	printf(img);
	printf("\n");
	//img = R"(C:\Program Files (x86)\National Instruments\LabVIEW 2014\examples\FRC\roboRIO\Vision\2015 Vision Example\Sample Images\image20.jpg)";
	// printf(img);
	// printf("\n");

	/// Load source image and convert it to gray
	//src = imread(argv[1], 1);
	src = imread(img, 1);

	/// Convert image to gray and blur it
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(2, 2)); //I have also seen 5,5 in documentation.// was 3,3


	// cv::Mat yellow_image;
	//cv::inRange(src, cv::Scalar(40, 0, 180), cv::Scalar(135, 110, 255), yellow_image);


	namedWindow("grey", CV_WINDOW_NORMAL);//CV_WINDOW_AUTOSIZE);
	imshow("grey", src_gray);


	// cvThreshold(imgGrayScale,imgGrayScale,128,255,CV_THRESH_BINARY);  
	// threshold(src_gray, src_gray, 128, 255, CV_THRESH_BINARY); //new, but it seemed to create problems...was missing a lot so am temporarily removing it.
	//have also seen this but not tried it: threshold(img,img,128,255,0); //probably exact same thing
	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_NORMAL);// CV_WINDOW_AUTOSIZE);
	imshow(source_window, src);

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);

	// waitKey(0);
	//return(0);
}


void getTemplate(char * filename, bool use_warp)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Point> approx;

	vector<Vec4i> hierarchy;

	Mat src_out;

	if (!use_warp)
		MatMatch= imread(filename, 1);
	else
	{

		MatMatch = disp;

	}

	/// Convert image to gray and blur it
	
	
	cvtColor(MatMatch, src_out, CV_BGR2GRAY);
	
	
	/// Detect edges using canny
	Canny(src_out, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

	int i = 0; //can be only one image

	//for (int i = 0; i < contours.size(); i++)
	//{
	approxPolyDP(Mat(contours[i]), match_template, arcLength(Mat(contours[i]), true)*0.02, 0);
	
	//TODO: figure out how many times this loops and return approx somehow. 
	//}





}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Point> approx;

	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	//findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0)); //this returns double the amount of what I want. 
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	//epsolan arg is: cvContourPerimeter(contours)*0.02

	vector<vector<Point> > storeApprox;
	// string text = "Funny text inside the box";
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = .5;
	int thickness = 1;

	printf("Number of contours:%d\n", contours.size());

	for (int i = 0; i< contours.size(); i++)
		//for (int i = 0; i < 1; i++)
	{

		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, 0);


		vector<Moments> mu(contours.size());

		//auto something = mu[]

		//		for (auto m : mu)
			//		{

			//
			//printf("%.2f %.2f %.2f\n", m.m00, m.m01, m.)
			//			printf("%.2f\n", m.m00);

			//		}

		//		mu[i] = moments(contours[i], false);
		mu[i] = moments(approx, false);

		//below for reference
		//result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
		//if (approx.size() < 10 && approx.size() >= 4)

		double match_score = cv::matchShapes(approx, match_template, CV_CONTOURS_MATCH_I2, 0);

		//printf("mscore:%.4f, bool:%d\n", match_score, match_score < 1);

		if (//!isContourConvex(approx) 
			(fabs(contourArea(approx))>50)
			&& approx.size() >= 6 && approx.size()<8
			//&& fabs(match_score)<2 
			)
			//
			if (mu[i].m00>100)
			{
				double hu[7];
				char temp[25];

				printf("set:%d\tmatches:%f\t!convex:%d\t", i, match_score, !isContourConvex(approx));
				auto something = (CvMoments)mu[i];
				HuMoments(something, hu);
				//theoary: cvmatchshapes will take the hu moments for me and do the comparison.  Have not tried yet but will try next.  Need to get a shape example. 


				//				printf("set:%d\tsize is %d,\tarea:%.2f\tarclen:%.2f\t?:%.2f\t---\t", i, approx.size(), contourArea(contours[i]), arcLength(contours[i], true), mu[i].m00);
				printf("pointsize%d\tarea:%.2f\tarclen:%.2f\t?:%.2f\n", approx.size(), fabs(contourArea(approx)), arcLength(approx, true), mu[i].m00);
				//printf("HU:%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", hu[0], hu[1], hu[2], hu[3], hu[4], hu[5], hu[6]);
				for (auto p : approx)
				{
					//
					printf("marking ponts:%d, x:%d y:%d\n", i, p.x, p.y);
					sprintf(temp, "%d", i);
					circle(drawing, p, 5.0, Scalar(0, 255, 0), 1, 8);
					putText(drawing, temp, p, fontFace, fontScale,
						Scalar::all(255), thickness, 8);


				}



				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
				//printf("--------------\n");
			}



	}


	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}