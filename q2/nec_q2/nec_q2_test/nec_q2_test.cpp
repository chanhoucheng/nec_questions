#include "pch.h"
#include "CppUnitTest.h"
#include <opencv2/face.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../nec_q2/main.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cv::face;
namespace necq2test
{
	TEST_CLASS(necq2test)
	{
	public:

		//test to ensure the div_ratio gives a decimal number with the larger number as denominator
		TEST_METHOD(div_ratio_test)
		{

			int res;
			res = (int)(div_ratio(1, 2) * 100);
			Assert::AreEqual(res, 50);
			res = (int)(div_ratio(2, 1) * 100);
			Assert::AreEqual(res, 50);

		}

		//test to ensure that the function returns at least 1 contour
		TEST_METHOD(detect_contour_test)
		{
			Mat img = imread("..\\..\\..\\test_images\\test2.jpg");
			Mat gray;
			cvtColor(img, gray, COLOR_BGR2GRAY);

			//apply smoothing
			GaussianBlur(gray, gray, Size(3, 3), 0, 0);
			vector<vector<Point>> contours;
			contours = detect_contour(gray);
			//based on my testing with the input image, it must at least return 1 contour
			int size_vector = contours.size();
			Assert::IsTrue(size_vector > 0);
		}

		//test to ensure score caculation formula works
		TEST_METHOD(caculate_test)
		{
			// Load Face Detector, ensure the file is in the dll folder
			CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");

			// Create an instance of Facemark
			Ptr<Facemark> facemark = FacemarkLBF::create();

			// Load landmark detector,ensure the file is in the dll folder
			facemark->loadModel("lbfmodel.yaml");

			//ensure the iamge file is availabe
			Mat img = imread("..\\..\\..\\test_images\\test2.jpg");
			// Variable to store a video frame and its grayscale 
			Mat gray_face, gray_contour;


			// Find face
			vector<Rect> faces;
			// Convert frame to grayscale because
			// faceDetector requires grayscale image.

			cvtColor(img, gray_face, COLOR_BGR2GRAY);
			cvtColor(img, gray_contour, COLOR_BGR2GRAY);

			//apply smoothing
			GaussianBlur(gray_contour, gray_contour, Size(3, 3), 0, 0);

			// Detect faces
			faceDetector.detectMultiScale(gray_face, faces);

			vector<vector<Point>> contours = detect_contour(gray_contour);
			vector< vector<Point2f> > landmarks;

			// Run landmark detector
			bool success = facemark->fit(img, faces, landmarks);

			if (success)
			{
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
				Assert::IsTrue(res > 0);
			}
			else
			{
				Assert::Fail((wchar_t*)"no face can be detected in the test image");
			}
		}
	};
}
