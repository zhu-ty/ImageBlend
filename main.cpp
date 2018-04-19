#include<iostream>
#include<vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

Mat src1;
Mat src2;
Mat dst;

//int main(int argc, char* argv[])
//{
//	if (argc < 2)
//		printf("error!\n");
//
//	return 0;
//}

void on_trackbar(int alpha_slider, void*)
{
	double alpha = (double)alpha_slider / 100;
	double beta = (1.0 - alpha);

	addWeighted(src1, alpha, src2, beta, 0.0, dst);

	imshow("Linear Blend", dst);
}

int main(int argc, char* argv[]) {

	src1 = imread(argv[1]);
	src2 = imread(argv[2]);
	int width = src1.cols;
	int height = src1.rows;
	if (argc == 4) {
		width = atoi(argv[3]);
		height = width * src1.rows / src1.cols;
		resize(src1, src1, Size(width, height));
		resize(src2, src2, Size(width, height));
	}
	else if (argc == 8) {
		// crop image first
		Rect rect;
		rect.x = atof(argv[4]) * (src1.cols - 1);
		rect.y = atof(argv[5]) * (src1.rows - 1);
		rect.width = atof(argv[6]) * (src1.cols - 1) - rect.x + 1;
		rect.height = atof(argv[7]) * (src1.rows - 1) - rect.y + 1;
		src1(rect).copyTo(src1);
		src2(rect).copyTo(src2);
		width = atoi(argv[3]);
		height = width * src1.rows / src1.cols;
		resize(src1, src1, Size(width, height));
		resize(src2, src2, Size(width, height));
	}

	if (!src1.data) { printf("Error loading src1 \n"); return -1; }
	if (!src2.data) { printf("Error loading src2 \n"); return -1; }
	/// Initialize values
	int alpha_slider = 0;
	/// Create Windows
	namedWindow("Linear Blend", 1);
	/// Create Trackbars
	char TrackbarName[50];
	sprintf(TrackbarName, "Alpha x %d", 100);
	createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, 100, on_trackbar);
	/// Show some stuff
	on_trackbar(alpha_slider, 0);
	/// Wait until user press some key
	waitKey(0);

	return 0;
}