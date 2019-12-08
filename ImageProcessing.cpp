#include "ImageProcessing.h"
#include <string>

using namespace std;

void ImageProcessing::loadImage(string filename) {
	this->filename = filename;
	cv::Mat inputImg;
	//img
	inputImg = cv::imread(this->filename, cv::IMREAD_UNCHANGED);
	if (inputImg.empty()) {
		cout << "Could not find or open the image." << endl;
		exit(-1);
	}
	
	int w = inputImg.size().width;
	int h = inputImg.size().height;
	cout << "Load successful: width: " << w << "height: " << h << endl;
	if (w > 1000 || h > 1000) {
		
		cv::resize(inputImg, img, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
		cout << "width: " << img.size().width << "height: " << img.size().height << endl;
	}
	else {
		img = inputImg;
		cout << "Nothing to change, width: " << w << "height: " << h << endl;
	}
	
}
void ImageProcessing::showImg(cv::Mat image) { //used for testing purpose
	cv::imshow("ANY_TITLE", image);
	cv::waitKey(0);
}
void ImageProcessing::drawLines(vector <cv::Vec2f>& lines)
{
	for (int i = 0; i < lines.size(); i++) {

		if (lines[i][0] == 0 || lines[i][1] == -100) {

			cout << "Merged Lines....\n";
			continue;

		}

		float rho = lines[i][0];
		float theta = lines[i][1];

		double COS = cos(theta);
		double SINE = sin(theta);

		double x0 = rho * COS;
		double y0 = rho * SINE;

		cv::Point pt1, pt2;

		pt1.x = cvRound(x0 - (2000 * SINE));
		pt1.y = cvRound(y0 + (2000 * COS));
		pt2.x = cvRound(x0 + (2000 * SINE));
		pt2.y = cvRound(y0 - (2000 * COS));

		line(ROI, pt1, pt2, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
	}

	imshow("3", this->img);
}
void ImageProcessing::process() {
	vector <cv::Vec2f> Lines; //to store hough lines
	threshold(this->img, thresh); //used to find contours of Sudoku board
	extract(thresh);
	threshold(ROI, threshROI);
	findHoughLines(threshROI, Lines);
	mergeHoughLines(&Lines);
	drawLines(Lines);
	allignSudoku(Lines);

	threshold(allignedSudoku, threshAllignedSudoku);
	cv::dilate(threshAllignedSudoku, threshAllignedSudoku, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)), cv::Point(-1, -1), 1);
	//dilate(threshAllignedSudoku, threshAllignedSudoku, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)), cv::Point(-1, -1), 1);

	imshow("4", threshAllignedSudoku);
	cv::waitKey(0);
	//showImg(threshAllignedSudoku);
	//imshow("PerformOCR", threshAllignedSudoku);
	//showImg(this->img);
	//showImg(thresh);
	//showImg(ROI);

}
void ImageProcessing::threshold(cv::Mat& image, cv::Mat& threshimage) {
	cv::Mat grayImage;
	cv::Mat blurrImage;

	// Some preprocessing operations like blurring to see edges more clearly
	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(grayImage, blurrImage, cv::Size(7, 7), 0);
	cv::adaptiveThreshold(blurrImage, threshimage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 3, 2); //to calculate a threshold pixel by pixel
}

void ImageProcessing::extract(cv::Mat image) {
	cv::Mat copyImage = image.clone();

	vector < vector<cv::Point> > contours; /* matrix of x,y coordinates */
	vector <cv::Vec4i> hierarchy; /* structure for representing a vector with 4 dimensions*/
									/*each line is a Vec4i, the first two elements are the line's start point (x1,y1) and last two are the line's end point(x2,y2)*/
	int largestContourIndex;

	cv::findContours(copyImage, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

	double maxContourArea = cv::contourArea(contours[0], false); //<- return absolute value

	for (int i = 0; i < contours.size(); i++) {

		double area = cv::contourArea(contours[i]);
		if (area >= maxContourArea) {
			maxContourArea = area;
			largestContourIndex = i;
		}
	}
	//Set BoudingRect as bound of the largest contour, in our case the 9x9 Sudoku board 
	cv::Rect BoundingRect = boundingRect(contours[largestContourIndex]);
	ROI = this->img(BoundingRect);

}
void ImageProcessing::findHoughLines(cv::Mat image, vector<cv::Vec2f>& Lines) {
	HoughLines(image, Lines, 1, CV_PI / 180, 300, 0, 0);
}
void ImageProcessing::mergeHoughLines(vector <cv::Vec2f>* lines)
{
	vector <cv::Vec2f>::iterator it1;

	for (it1 = lines->begin(); it1 != lines->end(); it1++) {
		if ((*it1)[0] == 0 || (*it1)[1] == -100) continue;

		float rho1 = (*it1)[0];
		float theta1 = (*it1)[1];

		cv::Point point11, point12;

		if (((*it1)[1] > (CV_PI * 45 / 180)) && ((*it1)[1] < (CV_PI * 135 / 180))) {

			point11.x = 0;
			point11.y = (rho1 / sin(theta1));

			point12.x = ROI.size().width;
			point12.y = (rho1 / sin(theta1)) - (ROI.size().width / tan(theta1));
		}
		else {

			point11.y = 0;
			point11.x = (rho1 / cos(theta1));

			point12.y = ROI.size().height;
			point12.x = (rho1 / cos(theta1)) - (ROI.size().height * tan(theta1));
		}

		vector<cv::Vec2f>::iterator it2;

		for (it2 = lines->begin(); it2 != lines->end(); it2++) {

			cv::Point point21, point22;


			if ((*it1) == (*it2)) {

				continue;
			}

			if ((fabs(((*it2)[0] - (*it1)[0])) < 20) && (fabs(((*it2)[1] - (*it1)[1])) < CV_PI * 20 / 180)) {


				float rho2 = (*it2)[0];
				float theta2 = (*it2)[1];

				if (((*it2)[1] > (CV_PI * 45 / 180)) && ((*it2)[1] < (CV_PI * 135 / 180))) {

					point21.x = 0;
					point21.y = (rho2 / sin(theta2));

					point22.x = ROI.size().width;
					point22.y = (rho2 / sin(theta2)) - (ROI.size().width / tan(theta2));
				}
				else {

					point21.y = 0;
					point21.x = (rho2 / cos(theta2));

					point22.y = ROI.size().height;
					point22.x = (rho2 / cos(theta2)) - (ROI.size().height * tan(theta2));
				}

				if ((double)((point11.x - point21.x) * (point11.x - point21.x) + (point11.y - point21.y) * (point11.y - point21.y)) < 64 * 64 && (double)((point12.x - point22.x) * (point12.x - point22.x) + (point12.y - point22.y) * (point12.y - point22.y)) < 64 * 64) {

					//cout << "Merging....\n";

					(*it1)[0] = ((*it1)[0] + (*it2)[0]) / 2.0;
					(*it1)[1] = ((*it1)[1] + (*it2)[1]) / 2.0;

					(*it2)[0] = 0;
					(*it2)[1] = -100;
				}
			}
		}
	}

}

void ImageProcessing::allignSudoku(const vector <cv::Vec2f>& lines)
{
	//the edges
	cv::Vec2f topEdge = cv::Vec2f(1000, 1000);          //double topXintercept = 0;		double topYintercept = 10000;
	cv::Vec2f bottomEdge = cv::Vec2f(-1000, -1000);		//double bottomXintercept = 0;	double bottomYintercept = 0;
	cv::Vec2f leftEdge = cv::Vec2f(1000, 1000);			double leftXintercept = 10000;	double leftYintercept = 0;
	cv::Vec2f rightEdge = cv::Vec2f(-1000, -1000);		double rightXintercept = 0;		double rightYintercept = 0;

	for (int i = 0; i < lines.size(); i++) {

		cv::Vec2f currentLine = lines[i];

		float rho = currentLine[0];
		float theta = currentLine[1];

		float xinter = currentLine[0] / cos(currentLine[1]);
		float yinter = currentLine[0] / sin(currentLine[1]);

		if (rho == 0 && theta == -100) {

			continue;
		}

		//check the horizontal lines
		if ((theta > CV_PI * 60 / 180) && (theta < CV_PI * 120 / 180)) {

			if (rho < topEdge[0]) {

				topEdge = currentLine;
			}

			if (rho > bottomEdge[0]) {

				bottomEdge = currentLine;
			}
		}

		else if ((theta < CV_PI * 10 / 180) || (theta > CV_PI * 170 / 180)) {

			if (xinter > rightXintercept) {

				rightEdge = currentLine;
				rightXintercept = xinter;
			}
			if (xinter <= leftXintercept) {

				leftEdge = currentLine;
				leftXintercept = xinter;
			}
		}
	}


	cv::Point top1, top2, bottom1, bottom2, left1, left2, right1, right2;
	if (leftEdge[1] != 0) {

		left1.x = 0;								left1.y = leftEdge[0] / sin(leftEdge[1]);
		left2.x = ROI.size().width;		    left2.y = left1.y - (left2.x / tan(leftEdge[1]));
	}
	else {

		left1.y = 0;								left1.x = leftEdge[0] / cos(leftEdge[1]);
		left2.y = ROI.size().height;			left2.x = left1.x - (left2.y * tan(leftEdge[1]));
	}

	if (rightEdge[1] != 0) {

		right1.x = 0;								right1.y = rightEdge[0] / sin(rightEdge[1]);
		right2.x = ROI.size().width;			right2.y = right1.y - (right2.x / tan(rightEdge[1]));
	}
	else {

		right1.y = 0;								right1.x = rightEdge[0] / cos(rightEdge[1]);
		right2.y = ROI.size().height;		right2.x = right1.x - (right2.y * tan(rightEdge[1]));
	}

	top1.x = 0;										top1.y = topEdge[0] / sin(topEdge[1]);
	top2.x = ROI.size().width;				top2.y = top1.y - (top2.x / tan(topEdge[1]));

	bottom1.x = 0;									bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);
	bottom2.x = ROI.size().width;			bottom2.y = bottom1.y - (bottom2.x / tan(bottomEdge[1]));


	double leftA = left2.y - left1.y;
	double leftB = left1.x - left2.x;
	double leftC = leftB * left1.y + leftA * left1.x;

	double rightA = right2.y - right1.y;
	double rightB = right1.x - right2.x;
	double rightC = rightB * right1.y + rightA * right1.x;

	double topA = top2.y - top1.y;
	double topB = top1.x - top2.x;	cout << topB << endl;
	double topC = topB * top1.y + topA * top1.x;

	double bottomA = bottom2.y - bottom1.y;
	double bottomB = bottom1.x - bottom2.x;
	double bottomC = bottomB * bottom1.y + bottomA * bottom1.x;

	double topLeftDet = topA * leftB - topB * leftA;
	double topRightDet = topA * rightB - topB * rightA;
	double bottomLeftDet = bottomA * leftB - bottomB * leftA;
	double bottomRightDet = bottomA * rightB - bottomB * rightA;

	cv::Point topLeft, topRight, bottomLeft, bottomRight;

	topLeft.x = (topC * leftB - topB * leftC) / topLeftDet;
	topLeft.y = (topA * leftC - topC * leftA) / topLeftDet;

	topRight.x = (topC * rightB - topB * rightC) / topRightDet;
	topRight.y = (topA * rightC - topC * rightA) / topRightDet;

	bottomRight.x = (bottomC * rightB - bottomB * rightC) / bottomRightDet;
	bottomRight.y = (bottomA * rightC - bottomC * rightA) / bottomRightDet;

	bottomLeft.x = (bottomC * leftB - bottomB * leftC) / bottomLeftDet;
	bottomLeft.y = (bottomA * leftC - bottomC * leftA) / bottomLeftDet;

	int maxLength = ((topRight.x - topLeft.x) * (topRight.x - topLeft.x)) + ((topRight.y - topLeft.y) * (topRight.y - topLeft.y));

	int length = ((topRight.x - bottomRight.x) * (topRight.x - bottomRight.x) + (topRight.y - bottomRight.y) * (topRight.y - bottomRight.y));
	if (length > maxLength)
		maxLength = length;
	length = ((bottomRight.x - bottomLeft.x) * (bottomRight.x - bottomLeft.x)) + ((bottomRight.y - bottomLeft.y) * (bottomRight.y - bottomLeft.y));
	if (length > maxLength)
		maxLength = length;
	length = ((bottomLeft.x - topLeft.x) * (bottomLeft.x - topLeft.x)) + ((bottomLeft.y - topLeft.y) * (bottomLeft.y - topLeft.y));
	if (length > maxLength)
		maxLength = length;

	maxLength = sqrt(maxLength);

	cv::Point2f originalPoints[4], mappedPoints[4];

	originalPoints[0] = topLeft;			mappedPoints[0] = cv::Point(0, 0);
	originalPoints[1] = topRight;			mappedPoints[1] = cv::Point(maxLength - 1, 0);
	originalPoints[2] = bottomRight;		mappedPoints[2] = cv::Point(maxLength - 1, maxLength - 1);
	originalPoints[3] = bottomLeft;			mappedPoints[3] = cv::Point(0, maxLength - 1);

	allignedSudoku = cv::Mat(cv::Size(maxLength, maxLength), CV_8UC1);

	warpPerspective(ROI, allignedSudoku, cv::getPerspectiveTransform(originalPoints, mappedPoints), cv::Size(maxLength, maxLength));
	imshow("1", allignedSudoku);
	imshow("2", ROI);
	cv::waitKey(0);
}
cv::Mat ImageProcessing::getImage() {
	return allignedSudoku;
}