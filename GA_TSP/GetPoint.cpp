#include"GetPoint.h"
//int city[100][2] = { { 392, 372 }, { 340, 82 }, { 383, 101 }, { 416, 105 }, { 437, 127 }, { 458, 159 }, { 483, 193 }, { 574, 194 }, { 683, 163 }, { 734, 210 }, { 684, 252 }, { 633, 271 }, { 530, 345 }, { 475, 366 }, { 409, 424 }, { 366, 426 }, { 401, 317 }, { 418, 278 }, { 408, 234 }, { 356, 229 }, { 296, 303 }, { 239, 363 }, { 176, 404 }, { 102, 402 }, { 159, 332 }, { 208, 263 }, { 178, 210 }, { 175, 152 }, { 251, 110 }, { 302, 89 } };
int city[100][2] = {};
int pointcount = 0;
char* filename;
//int CITYCOUNT = 30;	//城市个数
void my_mouse_callback(													//回调函数
   int event, int x, int y, int flags, void* param 
);
 
cv::Rect box;																//绘制的矩形定义
bool DRAWBOX = false;

void draw_box( cv::Mat img, cv::Rect rect ) {							//矩形绘制函数
  cv::rectangle (
    img, 
	cv::Point2d(box.x - 5, box.y - 5),
	//cvPoint(box.x+box.width,box.y+box.height),
	cv::Point2d(box.x + 5, box.y + 5),
    cv::Scalar(0,0,255),											//红色
	2
  );
}
 
int GetPoint() { 
  filename = "china.jpg";
  box = cv::Rect(-1,-1,0,0);

  cv::Mat image = cv::Mat::zeros(										
    cv::Size(800,600),
    CV_8UC3
  );
  image = cv::imread(filename);
  //cvZero( image );
  cv::Mat temp; image.copyTo(temp);
  
  cv::namedWindow( "城市节点获取" );
 

  cv::setMouseCallback(													//鼠标回调函数
    "城市节点获取", 
    my_mouse_callback, 
	  (void*)&image
  );

  while( 1 ) {
 
    image.copyTo(temp);
    if( DRAWBOX ) draw_box( temp, box ); 
    cv::imshow( "城市节点获取", temp );
 
    if( cv::waitKey( 15 )==27 ) break;
  }

  image.release();
  temp.release();
  cv::destroyWindow( "城市节点获取" );
  return 0 ;
}
 
void my_mouse_callback(int event, int x, int y, int flags, void* param )
{
 
  cv::Mat& image = *(cv::Mat*)param;

  switch( event ) {

  case cv::EVENT_LBUTTONDOWN: {
      DRAWBOX = true;
      box = cv::Rect( x, y, 0, 0 );
    }
    break;   
    case cv::EVENT_LBUTTONUP: {
      DRAWBOX = false; 

	  city[pointcount][0] = x;
	  city[pointcount][1] = y;
      draw_box( image, box );
    }
	pointcount++;
    break;   
  }
}
