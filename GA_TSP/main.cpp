#include"GA_TSP.h"
#include<stdlib.h>
#define  BUFSIZE 256
using namespace std;

void main()
{
	double t =(double) cv::getTickCount();
	
	system("cls");
	generation = 0;

	GetPoint();
	//fileRead();
	//fileWrite();
	CITYCOUNT = CHROMLENGTH = pointcount;//城市节点不限制，可以随意改，pointcount为输入的城市节点的个数
	CalculatorDistance();
    //第一代
	GenerateInitialPopulation();
	CalculateFitness();
	FindBestIndividual();
	OutputTextReport();

	//然而，不断进化
	while (generation<MaxGeneration)
	{
		generation++;
		GenerateNextPopulation();
		CalculateFitness();
		FindBestIndividual();
		ShowResult();
	}
	//最后一代输出
	OutputTextReport();
	//结果展示，绘制出了路径
	ShowResult();
	//fileWrite();
	t = cv::getTickCount() - t;
	cout << "程序运行时间：" << t / ((double)cv::getTickFrequency()*1000.) <<"ms"<< endl;
	//cvWaitKey(0);
}