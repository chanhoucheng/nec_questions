# nec_questions
-q2 is built upon opencv

-opencv is installed with extra modules to use the face detection api

-OpenCV 4.5.2 is used

-to run the project, first need to install opencv with the following guide: https://docs.opencv.org/master/d3/d52/tutorial_windows_install.html

-currently the application is built for debug mode only
-to copy the haarcascade_frontalface_alt2.xml and lbfmodel.yaml to output directory of exe before the program can run as these are the necesssary files for the face detector


The face detector is designed as such
1) use haar cascade classifier to identify the facial features
2) an open source facial model lbfmodel.yaml is used
3) the code currently is designed for an headshot image only
4) to detect the length of the face, edge detector is used to find the biggest contour in the image, it is assumed that the the biggest contour in an headshot image is will be the contour of the head
5)the generated executables is in executable.zip you can extract the data and run the sample with the test_exe.bat