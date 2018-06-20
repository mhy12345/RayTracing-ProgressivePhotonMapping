#include "paintboard.h"
#include "glog/logging.h"
#include <iostream>
using namespace cv;

PaintBoard::PaintBoard() {
	sizX = -1,sizY = -1;
	image = NULL;
	board = new(Color**);
}

void PaintBoard::init(int sizX,int sizY,Color*** _board) {
	this->sizX = sizX;
	this->sizY = sizY;
	image = new Mat(this->sizX,this->sizY,CV_64FC3,Scalar(1.0,1.0,1.0));
	*board = *_board;
}

void PaintBoard::update() {
	assert(image);
	for (int i=0;i<sizX;i++) {
		for (int j=0;j<sizY;j++) {
			if (!(*board)[(sizX-i-1)*sizY+j])
				continue;
			Color& c = *(*board)[(sizX-i-1)*sizY+j];
			Vec3f v(c.getR(),c.getG(),c.getB());
			image->at<Vec3d>(i,j) = v;
		}
	}
}
void PaintBoard::display() {
	imshow("picture", *image);
	waitKey(0);
}
void PaintBoard::save() {
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9); 
	imwrite("result.bmp", (*image)*255);
}
