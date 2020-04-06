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
	CITYCOUNT = CHROMLENGTH = pointcount;//���нڵ㲻���ƣ���������ģ�pointcountΪ����ĳ��нڵ�ĸ���
	CalculatorDistance();
    //��һ��
	GenerateInitialPopulation();
	CalculateFitness();
	FindBestIndividual();
	OutputTextReport();

	//Ȼ�������Ͻ���
	while (generation<MaxGeneration)
	{
		generation++;
		GenerateNextPopulation();
		CalculateFitness();
		FindBestIndividual();
		ShowResult();
	}
	//���һ�����
	OutputTextReport();
	//���չʾ�����Ƴ���·��
	ShowResult();
	//fileWrite();
	t = cv::getTickCount() - t;
	cout << "��������ʱ�䣺" << t / ((double)cv::getTickFrequency()*1000.) <<"ms"<< endl;
	//cvWaitKey(0);
}