#include "SudokuNumbers.h"
#include "ImageProcessing.h"
#include <iostream>
#include <cstddef>
using namespace std;
SudokuNumbers::SudokuNumbers(cv::Mat Image) {
	this->img_sn = Image;
	
}


Matrix SudokuNumbers::extractNumbers() {
	cv::Mat resizedNew, newimg;
	vector<vector<cv::Point>>contours;
	vector<cv::Vec4i>hierarchy;
	this->width = img_sn.size().width;
	this->height = img_sn.size().height;
	int i = 0, j = 0;
	float k = 0.0, l = 0.0;
	cv::Mat cropped, cellTest;
	cout << "width: " << this->width << "height: " << this->height << "j: " << (float(1 / 9) * this->height) << endl;
	int cellLength = floor((float)this->width / 9);
	for ( i = 0; i < 9; i++) {

	
		for ( j = 0; j < 9; j ++) {
				cv::Rect rect = cv::Rect((j * cellLength + 6), 6 + (i * cellLength), cellLength - 6, cellLength - 6);
				cropped = cv::Mat(img_sn, rect);
				cropped.copyTo(cellTest);
				
				cv::resize(cellTest, cellTest, cv::Size(150,150));
				cv::Mat croppedImage = cellTest(cv::Rect(30, 30, cellTest.size().width - 50, cellTest.size().height - 50));

				string a = knn.initialize(croppedImage);
				if (a.empty()) {
					Sudoku->at(i, j) = 0;
				}
				else {
					int number = stoi(a);
					Sudoku->at(i, j) = number;
				}
		}
	
	}
	return *Sudoku;
}