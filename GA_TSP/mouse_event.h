#ifndef __MOUSE_EVENT_H__
#define __MOUSE_EVENT_H__

#include<iostream>
#include<stdio.h>
#include<vector>

#include<opencv\cv.h>
#include<opencv2\highgui\highgui.hpp>
const unsigned int X = 0;
const unsigned int Y = 1;
extern int city[100][2]; //³ÇÊÐ×ø±ê´æ´¢
extern CvRect box;
//extern bool drawing_box = false;
extern IplImage* image;
extern IplImage* temp;
int mouse_main( ) ;
void my_mouse_callback(int event, int x, int y, int flags, void* param );
void draw_box( IplImage* img, CvRect rect ) ;

#endif