#pragma once
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

class ImageProcessing
{
	int  Digit;
	string filename;
	cv::Mat img, thresh, ROI, threshROI, allignedSudoku, threshAllignedSudoku;
	void threshold(cv::Mat& image, cv::Mat& threshimage);
	void extract(cv::Mat image);
	void findHoughLines(cv::Mat img, vector <cv::Vec2f>& Lines);
	void mergeHoughLines(vector <cv::Vec2f>* lines);
	void drawLines(vector <cv::Vec2f>& lines);
	void allignSudoku(const vector <cv::Vec2f>& lines);
	void showImg(cv::Mat image); /* Function used only for testing */
	
public:
	int sudoku[9][9];
	void loadImage(string filename);
	cv::Mat getImage();
	void process();

};