#pragma once
#include <string>
#include <opencv2/imgcodecs.hpp>
#include "KNN.h"
#include "Matrix.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

class SudokuNumbers {
	cv::Mat img_sn, cell;
	Matrix* Sudoku = new Matrix(9,9);
	float width, height;
	
public:
	SudokuNumbers(cv::Mat Image);
	Train knn;
	Matrix extractNumbers();
};