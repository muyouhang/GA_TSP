#include"mouse_event.h"
#include"main.h"
using namespace std;
using namespace cv;
void my_mouse_callback(													//回调函数
   int event, int x, int y, int flags, void* param 
);
IplImage* image = cvCreateImage(
	cvSize(600, 600),
	IPL_DEPTH_8U,
	3
	);
IplImage* temp;
CvRect box;																//绘制的矩形定义
bool drawing_box = false;
//int city[100][2] = { { 110, 84 }, { 296, 2 }, { 202, 130 }, { 215, 7 }, { 21, 43 }, { 299, 94 }, { 191, 20 }, { 63, 74 }, { 111, 13 }, { 140, 129 }, {283,121},
//                      { 237, 25 }, { 279, 6 }, { 157, 71 }, { 199, 100 }, { 198, 121 }, { 161, 140 }, { 134, 34 }, { 253, 86 }, { 272, 58 }, {207,130},
//					  { 156, 98 }, { 83, 31 }, { 236, 42 }, { 214, 115 }, { 23, 17 }, { 90, 101 }, { 240, 96 }, { 235, 113 }, {15,75}}; //城市坐标存储
//int city[100][2] = { { 110, 84 }, { 296, 2 }, { 202, 130 }, { 215, 7 }, { 21, 43 }, { 299, 94 }, { 191, 20 }, { 63, 74 }, { 111, 13 }, { 140, 129 }
///*{ 237, 25 }, { 279, 6 }, { 157, 71 }, { 199, 100 }, { 198, 121 }, { 161, 140 }, { 134, 34 }, { 253, 86 }, { 272, 58 }, { 207, 130 },
//{ 156, 98 }, { 83, 31 }, { 236, 42 }, { 214, 115 }, { 23, 17 }, { 90, 101 }, { 240, 96 }, { 235, 113 }, { 15, 75 }*/ }; //城市坐标存储
int city[100][2] = {};
int i_count = 0;
void draw_box( IplImage* img, CvRect rect ) {							//矩形块绘制函数
  cvRectangle (
    img, 
    cvPoint(box.x-5,box.y-5),
    //cvPoint(box.x+box.width,box.y+box.height),
	cvPoint(box.x + 5, box.y + 5),
    cvScalar(0xff,0x00,0x00)											//红色
  );


}
 
int mouse_main( ) {
  
  box = cvRect(-1,-1,0,0);


  cvZero( image );
  temp = cvCloneImage( image );

  for (int i = 0; i < 1000; i++){
	  city[i][X] = -1;
	  city[i][Y] = -1;
  }
  cvNamedWindow( "遗传算法解决旅行商问题仿真" );
 

  cvSetMouseCallback(													//鼠标回调函数
    "遗传算法解决旅行商问题仿真", 
    my_mouse_callback, 
    (void*) image 
  );

  while( 1 ) {
	cvCopyImage( image, temp );
	if (drawing_box) {
		draw_box(temp, box);
	}
    cvShowImage( "遗传算法解决旅行商问题仿真", temp );
	//城市坐标 检测
	//for (int i = 0; city[i+1][X] != -1 && city[i+1][Y] != -1; i++){
	//	printf("( %d,%d) ", city[i][X], city[i][Y]);
	//}
	//printf("\n");
    if( cvWaitKey( 15 )==27 ) break;
  }

  cvReleaseImage( &image );
  cvReleaseImage( &temp );
  cvDestroyWindow( "遗传算法解决旅行商问题仿真" );
  return 0 ;
}
 
void my_mouse_callback(
						int event, 
						int x, 
						int y, 
						int flags, 
						void* param )
{
 
  IplImage* image = (IplImage*) param;

  switch( event ) {
    case CV_EVENT_MOUSEMOVE: { 
 
	  box = cvRect(x, y, 0, 0);
    }
    break;
    case CV_EVENT_LBUTTONDOWN: {
      drawing_box = true;
      box = cvRect( x, y, 0, 0 );
    }
    break;   
    case CV_EVENT_LBUTTONUP: {
      drawing_box = true; 
	  box = cvRect(x, y, 0, 0);
	  city[i_count][0] = x;
	  city[i_count][1] = y;
	 // printf("( %d,%d)\n", city[i_count][X], city[i_count][Y]);
	  i_count++;
      draw_box( image, box );
    }
    break;   
  }
}

