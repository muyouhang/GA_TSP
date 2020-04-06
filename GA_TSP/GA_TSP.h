#pragma once
#include"GetPoint.h"

#define POPSIZE 1000 
//#define CITYCOUNT 30		//城市个数
//#define CHROMLENGTH 100   //染色体长度

extern int CITYCOUNT;	//城市个数
extern int CHROMLENGTH ;
extern int PopSize;	//种群规模
extern int MaxGeneration;//进化代数
extern double Pc ;//交叉概率
extern double Pm ;//变异概率

extern int TempCitySet[100];//存放染色体解码完成后所经过的城市序列

extern  struct individual;
extern int generation;
extern individual bestindividual;//某一代群体中最优个体
extern individual currentbest;//当前最优个体
extern individual population[POPSIZE];//

extern cv::Mat tsp_image;

void GenerateInitialPopulation();       /* 初始化群体*/
void GenerateNextPopulation();          /* 生成下一代群体 */
void DecodeChromosome(int *chrom, int *Result);    /*解码染色体Chrom，存于Result中 */
void DeleteOneCity(int *workset, int pos, int n);  /* 从城市序列中删去一个体 */
void FindBestIndividual();              /* 寻找最优个体 */
void Select(int start, int stop, individual *newpopulation);
void SelectionOperator();               /* 选择算子 */
void CrossoverOperator();               /* 交叉算子 */
void MutationOperator();                /* 变异算子 */
void OutputTextReport();                /* 输出结果 */
void CalculateFitness();                /* 计算适应度  */

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

