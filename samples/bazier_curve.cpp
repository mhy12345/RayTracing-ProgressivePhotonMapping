#include <iostream>
#include <string.h>
#include <cxcore.h>
#include <cv.h>
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core.hpp"
#include <highgui.h>
#include <fstream>

using namespace std;

const int WW_MAX_MARK_COUNT = 40; //最大40个控制点
int mark_count =4;
int conner_pt_index =-1;
CvPoint3D32f Control_pts[WW_MAX_MARK_COUNT];
IplImage *image = 0 ; //原始图像
bool is_showControlLines = true;


// 两个向量相加，p=p+q
CvPoint3D32f pointAdd(CvPoint3D32f p, CvPoint3D32f q) {
	p.x += q.x;        p.y += q.y;        p.z += q.z;
	return p;
}

// 向量和标量相乘p=c*p
CvPoint3D32f pointTimes(float c, CvPoint3D32f p) {
	p.x *= c;    p.y *= c;    p.z *= c;
	return p;
}

// 计算贝塞尔方程的值
// 变量u的范围在0-1之间
//P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pnew 
CvPoint3D32f Bernstein(float u, CvPoint3D32f *p) {
	CvPoint3D32f    a, b, c, d, r;

	a = pointTimes(pow(u,3), p[0]);
	b = pointTimes(3*pow(u,2)*(1-u), p[1]);
	c = pointTimes(3*u*pow((1-u),2), p[2]);
	d = pointTimes(pow((1-u),3), p[3]);

	r = pointAdd(pointAdd(a, b), pointAdd(c, d));

	return r;
}

//画控制线
void DrawControlLine(CvPoint3D32f *p) {

	CvPoint pc[4];
	for(int i=0;i<4;i++)
	{
		pc[i].x = (int)p[i].x;
		pc[i].y = (int)p[i].y;
	}

	cvLine(image,pc[0],pc[1],CV_RGB(0,0,255),1,CV_AA,0);
	cvLine(image,pc[2],pc[3],CV_RGB(0,0,255),1,CV_AA,0);    
}

//得到最近Control_pts的index
int getNearPointIndex(CvPoint mouse_pt)
{
	CvPoint pt;
	for(int i =0; i<mark_count;i++)
	{
		pt.x= mouse_pt.x - (int)Control_pts[i].x;
		pt.y= mouse_pt.y - (int)Control_pts[i].y;
		float distance = sqrt ((float)( pt.x*pt.x + pt.y*pt.y ));
		if(distance<10) return i;

	}
	return -1;
}

void on_mouse( int event, int x, int y, int flags, void *param )
{
	if( event == CV_EVENT_LBUTTONDOWN )
	{
		CvPoint pt = cvPoint(x,y);        

		if(conner_pt_index >-1)
			conner_pt_index = -1;
		else
		{
			conner_pt_index = getNearPointIndex(pt);    
			//添加新的控制点
			if(conner_pt_index==-1)
			{
				if(mark_count<=(WW_MAX_MARK_COUNT-1))
				{
					Control_pts[mark_count].x = (float)pt.x;
					Control_pts[mark_count].y = (float)pt.y;
					Control_pts[mark_count].z = 0;
					mark_count++;
				}
			}
		}
	} 
	else if ( event == CV_EVENT_MOUSEMOVE ) //修改控制点坐标
	{
		if(conner_pt_index >-1)
		{
			Control_pts[conner_pt_index].x = (float)x;
			Control_pts[conner_pt_index].y = (float)y;
		}
		for(int i =0;i<mark_count;i++)
			printf("#%d : %d %d\n",i,(int) Control_pts[i].x,(int) Control_pts[i].y);
	}

};


int main(int argc, char* argv[])
{

	CvSize image_sz = cvSize( 500,500); 
	image = cvCreateImage(image_sz , 8, 3 );
	cvResizeWindow("Win",500,500);
	cvNamedWindow("Win",0);
	cvSetMouseCallback( "Win", on_mouse, 0 );

	cout<<"==============   Bezier curve DEMO  =============="<<endl;
	cout<<" "<<endl;
	cout<<"1.use mouse to click control point (red) to select a control point"<<endl;
	cout<<"2.use mouse to modify control point"<<endl;
	cout<<"3.click mouse on somewhere to add a control point,add three points for add a new curve"<<endl;
	cout<<"4.use 'W','S' to add precision or reduce precision."<<endl;
	cout<<"5.press 'Z' to show control points."<<endl;
	cout<<"===press ESC to exit==="<<endl;

	//初始化四个控制点
	Control_pts[0].x = 100;
	Control_pts[0].y = 200;
	Control_pts[0].z = 0;

	Control_pts[1].x = 200;
	Control_pts[1].y = 100;
	Control_pts[1].z = 0;

	Control_pts[2].x = 300;
	Control_pts[2].y = 400;
	Control_pts[2].z = 0;

	Control_pts[3].x = 400;
	Control_pts[3].y = 300;
	Control_pts[3].z = 0;

	int divs = 50; //控制精细度

	for(;;)
	{    

		CvPoint pt_now,pt_pre;
		cvZero(image);


		//绘制控制点
		if(is_showControlLines)
		{
			for(int i =0;i<mark_count;i++)
			{
				CvPoint ptc;
				ptc.x = (int) Control_pts[i].x;
				ptc.y = (int) Control_pts[i].y;
				cvCircle( image, ptc, 4, CV_RGB(255,0,0), 1,CV_AA, 0);    
			}
		}

		//绘制Bezier曲线
		CvPoint3D32f *pControls = Control_pts;
		for(int j=0;j<mark_count-3;j+=3)
		{            
			for (int i=0;i<=divs;i++) 
			{
				float u  = (float)i/divs;
				CvPoint3D32f newPt = Bernstein(u,pControls);                

				pt_now.x = (int)newPt.x;
				pt_now.y = (int)newPt.y;

				if(i>0)    cvLine(image,pt_now,pt_pre,CV_RGB(230,255,0),2,CV_AA, 0 );
				pt_pre = pt_now;
			}

			//画控制线
			if(is_showControlLines)DrawControlLine(pControls);
			pControls+=3;
		}

		cvShowImage("Win",image);
		int keyCode = cvWaitKey(20);
		if (keyCode==27) break;
		if(keyCode=='w'||keyCode=='W') divs+=2; 
		if(keyCode=='s'||keyCode=='S') divs-=2; 
		if(keyCode=='z'||keyCode=='Z') is_showControlLines = is_showControlLines^1;

		//cout<<"precision : "<<divs<<endl;

	}
	return 0;
}
