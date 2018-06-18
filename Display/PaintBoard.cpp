#include "PaintBoard.h"

PaintBoard::PaintBoard(int sizX,int sizY) {
	image = new cv::Mat(sizX,sizY,CV_64FC3,cv::Scalar(1.0,1.0,1.0));
	cv::imshow("picture", *image);
	cv::waitKey(0);
}
