#pragma once
#include"GetPoint.h"

#define POPSIZE 1000 
//#define CITYCOUNT 30		//���и���
//#define CHROMLENGTH 100   //Ⱦɫ�峤��

extern int CITYCOUNT;	//���и���
extern int CHROMLENGTH ;
extern int PopSize;	//��Ⱥ��ģ
extern int MaxGeneration;//��������
extern double Pc ;//�������
extern double Pm ;//�������

extern int TempCitySet[100];//���Ⱦɫ�������ɺ��������ĳ�������

extern  struct individual;
extern int generation;
extern individual bestindividual;//ĳһ��Ⱥ�������Ÿ���
extern individual currentbest;//��ǰ���Ÿ���
extern individual population[POPSIZE];//

extern cv::Mat tsp_image;

void GenerateInitialPopulation();       /* ��ʼ��Ⱥ��*/
void GenerateNextPopulation();          /* ������һ��Ⱥ�� */
void DecodeChromosome(int *chrom, int *Result);    /*����Ⱦɫ��Chrom������Result�� */
void DeleteOneCity(int *workset, int pos, int n);  /* �ӳ���������ɾȥһ���� */
void FindBestIndividual();              /* Ѱ�����Ÿ��� */
void Select(int start, int stop, individual *newpopulation);
void SelectionOperator();               /* ѡ������ */
void CrossoverOperator();               /* �������� */
void MutationOperator();                /* �������� */
void OutputTextReport();                /* ������ */
void CalculateFitness();                /* ������Ӧ��  */

void CalculatorDistance();
void ShowResult();
int CalculatorEuler(int *src1, int *src2);

int fileWrite();
int fileRead();
class GA_TSP
{
public:
	GA_TSP();
	~GA_TSP();
};

