#ifndef PAINT_BOARD_H
#define PAINT_BOARD_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

class PaintBoard {
	cv::Mat *image;
	public:
		PaintBoard(int sizX = 648,int sizY = 1024); 
};

#endif
