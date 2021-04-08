#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
double div_ratio(double a, double b);
double calculate_score(vector<Rect> faces, vector< vector<Point2f> > landmarks, Rect contour);
vector<vector<Point>> detect_contour(Mat src_gray);
