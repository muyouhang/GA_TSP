#ifndef __GETPOINT_H__
#define __GETPOINT_H__
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>

#define X 0 
#define Y 1

extern int city[100][2];
extern int pointcount;
extern char* filename;
int GetPoint( ) ;
void my_mouse_callback(int event, int x, int y, int flags, void* param );
void draw_box( cv::Mat img, cv::Rect rect ) ;
#endif