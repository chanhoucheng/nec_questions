#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "main.hpp"

using namespace std;
using namespace cv;
using namespace cv::face;


int main(int argc, char** argv)
{

	if (argc < 2) {
		// Tell the user how to run the program
		std::cerr << "Usage: please pass in the filepath of image"<< std::endl;
		/* "Usage messages" are a conventional way of telling the user
		 * how to run a program if they enter the command incorrectly.
		 */

		return 1;
	}

	// Load Face Detector
	CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

	// Create an instance of Facemark
	Ptr<Facemark> facemark = FacemarkLBF::create();

	// Load landmark detector
	facemark->loadModel("lbfmodel.yaml");

	Mat img = imread(argv[1]);
	// Variable to store a video frame and its grayscale 
	Mat gray_face,gray_contour;
	
	
	// Find face
	vector<Rect> faces;
	// Convert frame to grayscale because
	// faceDetector requires grayscale image.

	cvtColor(img, gray_face, COLOR_BGR2GRAY);
	cvtColor(img, gray_contour, COLOR_BGR2GRAY);

	//apply smoothing
	GaussianBlur(gray_contour, gray_contour, Size(3, 3),0,0);

	// Detect faces
	faceDetector.detectMultiScale(gray_face, faces);

	vector<vector<Point>> contours=detect_contour(gray_contour);
	// Variable for landmarks. 
	// Landmarks for one face is a vector of points
	// There can be more than one face in the image. Hence, we 
	// use a vector of vector of points. 
	vector< vector<Point2f> > landmarks;

	// Run landmark detector
	bool success = facemark->fit(img, faces, landmarks);

	if (success)
	{
		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::rectangle(img, faces[i], Scalar(255, 0, 0));
		}
		for (unsigned long i = 0; i < faces.size(); i++) {
			for (unsigned long k = 0; k < landmarks[i].size(); k++)
				cv::circle(img, landmarks[i][k], 5, cv::Scalar(0, 0, 255), FILLED);
		}

		double largest_area = 0;
		int largest_contour_index = -1;
		Rect bounding_rect;
		for (int i = 0; i < contours.size(); i++) // iterate through each contour.
		{
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if (a > largest_area) {
				largest_area = a;
				largest_contour_index = i;                //Store the index of largest contour
				bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}

		}
		
		rectangle(img, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);



		namedWindow("Detected_shape");
		imshow("Detected_shape", img);
		double res = calculate_score(faces, landmarks, bounding_rect);
		if (res < 0)
		{
			cout << "unable to process image";
		}
		else
		{
			cout << "scoure is " << res;
		}
		waitKey(0);
	}





	return 0;
}

vector<vector<Point>> detect_contour(Mat src_gray)
{

	Mat canny_output,morph_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;



	Mat element = getStructuringElement(MORPH_CROSS,
		Size(3, 3),
		Point(1, 1));

	//the number of erosion and dilation is obtained empirically while doing this work
	erode(src_gray, morph_output, element);
	erode(morph_output, morph_output, element);

	// Detect edges using canny
	Canny(morph_output, canny_output, 50, 100, 3);
	dilate(canny_output, canny_output, element);

	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	return contours;
}

double calculate_score(vector<Rect> faces, vector< vector<Point2f> > landmarks,Rect contour)
{
	//index of landmarks based on https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/
	double ret = 0;
	/*first check if contour rectangle has a larger size than face rect*/
	//although input image should only have 1 headshot picture, but facial detector may return multiple faces
	if (faces.size()>1) //for the case where more than 1 face is returned
		return -1;
	else
	{
		Rect face = faces[0];
		vector<Point2f> landmark = landmarks[0];
		//get width of face
		double face_width = abs(landmark[0].x - landmark[16].x);
		double face_length;
		double nose_chin=abs(landmark[33].y - landmark[8].y);
		double nose_eyes=abs(landmark[39].y - landmark[33].y);
		double eyes_forehead=abs(face.tl().y- landmark[39].y);
		double nose=abs(landmark[27].y-landmark[33].y);
		double ear = abs(landmark[0].y - landmark[2].y); ;
		double eyes2 = abs(landmark[39].x - landmark[42].x);
		double eye = abs(landmark[45].x - landmark[42].x);
		//check if contour height is bigger than face height
		if (contour.height > face.height)
		{
			face_length = abs(contour.tl().y - landmark[8].y);
		}
		else // in this case use the face bounded rectangle as an estimate
		{
			face_length = abs(face.tl().y - landmark[8].y); //since face rectangle normally terminates at forhead, we can just simply give a 20% increment of overall length
			face_length *= 1.2;
		}

		//formula from https://www.oprah.com/oprahshow/measuring-facial-perfection-the-golden-ratio
		double A = face_length / face_width / 1.6;
		double B = (div_ratio(nose_chin , nose_eyes) + div_ratio(nose_chin , eyes_forehead) + div_ratio(nose_eyes,eyes_forehead) )/3.0;
		double C = (div_ratio(ear , nose) + div_ratio(eyes2 , eye)) / 2.0;

		ret = ((A + B + C) / 3.0)*10.0;
	}
	return ret;

}

double div_ratio(double a, double b)
{
	if (a > b)
		return b / a;
	else
		return a / b;
}