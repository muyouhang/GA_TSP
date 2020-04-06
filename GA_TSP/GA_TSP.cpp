#include "GA_TSP.h"


GA_TSP::GA_TSP()
{
}


GA_TSP::~GA_TSP()
{
}
int PopSize =500;	//种群规模
int MaxGeneration = 10000;//进化代数
double Pc = 0.2;//交叉概率
double Pm = 0.005;//变异概率
int CITYCOUNT   = 3;	//城市个数
int CHROMLENGTH = 3;
int CitySet[500];//城市集合
int WorkSet[100];//染色体解码时的临时工作数组，存放还未访问的城市序列
int TempCitySet[100];//存放染色体解码完成后所经过的城市序列
FILE *f= fopen("city.txt", "rb+");

//int disaster_control = 0;
//double disaster_Pc = 0.3;
//double disaster_Pm = 0.01;
//double m = Pm;//缓存不变
//double c = Pc;
cv::Scalar line = cv::Scalar(255, 0,0);
typedef struct individual{
	int chrom[100];//染色体编码
	double value;          //value值表示路径长度
	double fitness;        //适应度
}individual;

int generation;
individual bestindividual;//某一代群体中最优个体
individual currentbest;//当前最优个体
individual population[POPSIZE];//

cv::Mat tsp_image = cv::Mat::zeros(cv::Size(800, 600),CV_8UC3);
int CityDistance[100][100] = {};//城市之间的距离矩阵,来回相等

//选择算子
void Select(int start, int stop, individual *newpopulation){
	int i, index;
	double p, sum = 0.0;
	double cfitness[POPSIZE];//个体适应度
	//求总适应度sum
//#pragma omp parallel for
	for (i = start; i < stop; i++){
		sum += population[i].fitness;
	}
	//求每个个体的适应度所占的百分比cfitness[i]
#pragma omp parallel for
	for (i = start; i < stop; i++){
		cfitness[i] = population[i].fitness / sum;
	}
	//适应度求和，类似概率分布：0 1 2 3 4 -> 0 1 3 4 10
	for (i = start + 1; i < stop; i++){
		cfitness[i] = cfitness[i - 1] + cfitness[i];
	}
	//选择个体，
//#pragma omp parallel for
	for (i = start; i < stop; i++){
		p = rand() % 1000 / 1000.0;//0到1之间
		index = 0;
		while (p>cfitness[index]){
			index++;
		}
		newpopulation[i] = population[index];
	}
	for (i = start; i < stop; i++){
		population[i] = newpopulation[i];
	}
}

void SelectionOperator(void){
	individual newpopulation[POPSIZE];
	int start = 0;
	int stop = (int)PopSize / 3;
	Select(start, stop, newpopulation);
	start = stop + 1;
	stop = (int)(2 * PopSize / 3);
	Select(start, stop, newpopulation);
	start = stop + 1;
	stop = PopSize;
	Select(start, stop, newpopulation);
}
//随机数生成
int random(int randmax){
	return rand() % randmax;
}
//交叉算子
void CrossoverOperator(void){
	//TODO:变量命名不规范，需要重新设计
	int i, j;
	int index[POPSIZE];
	int point, temp;
	double p;
	int CityNo;  //定义为染色体编码交换的 暂存BUFF

	for (i = 0; i < PopSize; i++){
		index[i] = i;//TODO:编号
	}
	//交换i 和 point+i 的编号
	for (i = 0; i < PopSize; i++){
		point = random(PopSize - i);  //生成的随机数小于PopSize - i
		temp = index[i];
		index[i] = index[point + i];
		index[point + i] = temp;
	}
	//交换 point 至 CHROMLENGTH 长度的 染色体编码
#pragma omp parallel for
	for (i = 0; i < PopSize - 1; i += 2)
	{
		p = rand() % 1000 / 1000.0;
		if (p < Pc){
			point = random(CHROMLENGTH - 1) + 1;
			for (j = point; j < CHROMLENGTH; j++){
				CityNo = population[index[i]].chrom[j];
				population[index[i]].chrom[j] = population[index[i + 1]].chrom[j];
				population[index[i + 1]].chrom[j] = CityNo;
			}
		}
	}
}
//变异算子
void MutationOperator(){
	int i, j;
	double p;

	for (i = 0; i < PopSize; i++){
		for (j = 1; j < CHROMLENGTH; j++){// 染色体第一个元素为1，不能变异
			p = rand() % 1000 / 1000.0;
			//随机修改基因
			if (p < Pm){
				population[i].chrom[j] = random(CITYCOUNT - j) + 1;
			}
		}
	}
}
//生成初始群体
void GenerateInitialPopulation(){
	int i, j;
	for (i = 0; i < PopSize; i++){
		CitySet[i] = i + 1;
	}
	srand((unsigned)time(NULL));
	for (i = 0; i < PopSize; i++){
		//出发城市固定为第一个城市
		population[i].chrom[0] = 1;
		for (j = 1; j < CITYCOUNT; j++){
			population[i].chrom[j] = random(CITYCOUNT - j) + 1;
			//std::cout << population[i].chrom[j];
		}
	}
}
//生成下一代群体
void GenerateNextPopulation(){
	SelectionOperator();
	CrossoverOperator();
	MutationOperator();
}
//从城市序列中删去一个体,删除i个体
void DeleteOneCity(int *workset, int position, int n){
	for (int i = position - 1; i < n - 1; i++){
		workset[i] = workset[i + 1];
	}
}
//对染色体Chrom解码，结果在Result中
void DecodeChromosome(int *chrom, int *Result){
	int *p;   //指向将要访问的城市
	int i = 0;
	int n = CITYCOUNT;

	p = chrom;//出发城市
	//初始工作城市数组
	for (i = 0; i < CITYCOUNT; i++){
		WorkSet[i] = CitySet[i];
	}
	//TODO:
	for (i = 0; i < CITYCOUNT; i++){
		Result[i] = WorkSet[*p - 1];//工作数组中第*p个元素是即将访问的城市
		DeleteOneCity(WorkSet, *p, n);//从未访问的n个城市中删去刚访问的第*p个
		n--;//未访问城市个数减1
		p++;//即将访问城市指向下一个
	}
}
//输出结果
void OutputTextReport(){
	int i = 0;
	DecodeChromosome(currentbest.chrom, TempCitySet);
	printf("群体代数：%d , 最短路径： %f \n", generation, currentbest.value);
	printf("个体编码：[");
	for (i = 0; i < CHROMLENGTH; i++){
		printf(" %d", currentbest.chrom[i]);
	}
	printf("]\n");
	printf("访问顺序：[");
	for (i = 0; i < CHROMLENGTH; i++){
		printf(" %d", TempCitySet[i]);

	}
	printf("]\n\n");
}
//计算适应度
void CalculateFitness(){
	int i, j;
	double TotalDistance = 0.0;
	//对每个个体，求其路径总长度
	for (i = 0; i < PopSize; i++){
		DecodeChromosome(population[i].chrom, TempCitySet);
		for (j = 1; j < CITYCOUNT; j++){
			TotalDistance = TotalDistance + CityDistance[TempCitySet[j - 1] - 1][TempCitySet[j] - 1];
		}
		TotalDistance = TotalDistance + CityDistance[TempCitySet[CITYCOUNT - 1] - 1][0];
		population[i].value = TotalDistance;
		TotalDistance = 0.0;
	}
	//城市数目 除以  个体路径总长度 = 个体适应度
	//路径越短，适应度越高
	for (i = 0; i < PopSize; i++){
		population[i].fitness = CITYCOUNT / population[i].value;
	}
}
//寻找最优个体
void FindBestIndividual(void)
{
	int i;

	bestindividual = population[0];
	//通过比较适应度来筛选，越大越好
	for (i = 1; i<PopSize; i++)
	{
		if (population[i].fitness>bestindividual.fitness)
			bestindividual = population[i];
	}
	//对第一代
	if (generation == 0)
	{
		currentbest = bestindividual;
	}
	else
	{
		if (bestindividual.fitness>currentbest.fitness)
		{
			currentbest = bestindividual;
			OutputTextReport();    /* 每更新一次当前最优值，输出结果 */
		
			//灾变控制清零
			//disaster_control = 0;
			//Pc = c ;
			//Pm = m ;
		}
		//else{
		//	disaster_control++;
		//	if (disaster_control > 50){
		//		Pc = disaster_Pc;
		//		Pm = disaster_Pm;

		//	}
		//}
	}
}
//距离计算公式
int CalculatorEuler(int *src1, int *src2){
	return sqrt(pow((src1[X] - src2[X]), 2) + pow((src1[Y] - src2[Y]), 2));

}
//计算两两城市之间的距离
void CalculatorDistance(){
	int i, j;
	for (i = 0; i<CITYCOUNT; i++){
		for (j = 0; j<CITYCOUNT; j++){
			CityDistance[i][j] = CalculatorEuler(city[i], city[j]);
			//CityDistance[i][j] = 100;
			//printf(" %d", CityDistance[i][j]);
		}
		//printf("\n");
	}

}
//结果图形化显示：出发城市-绿色   路线-红色  城市-蓝色 
void ShowResult(){
	//cvZero(tsp_image);
	tsp_image = cv::imread("china.jpg");
	cv::rectangle(
		tsp_image,
		cv::Point(city[0][X] - 5, city[0][Y] - 5),
		//cvPoint(box.x+box.width,box.y+box.height),
		cv::Point(city[0][X] + 5, city[0][Y] + 5),
		cv::Scalar(0, 0,255),											//绿色
		2
		);
#pragma omp parallel for
	for (int i = 1; i < CITYCOUNT; i++){
		cv::rectangle(
			tsp_image,
			cv::Point(city[i][X] - 5, city[i][Y] - 5),
			//cvPoint(box.x+box.width,box.y+box.height),
			cv::Point(city[i][X] + 5, city[i][Y] + 5),
			cv::Scalar(0, 0, 255),											
			2
			);//
	}
#pragma omp parallel for
	for (int i = 0; i < CITYCOUNT - 1; i++){
		cv::line(tsp_image, cv::Point(city[TempCitySet[i] - 1][X], city[TempCitySet[i] - 1][Y]), cv::Point(city[TempCitySet[i + 1] - 1][X], city[TempCitySet[i + 1] - 1][Y]), line, 2);
		if ((i + 1) == (CITYCOUNT - 1)){
			cv::line(tsp_image, cv::Point(city[TempCitySet[i + 1] - 1][X], city[TempCitySet[i + 1] - 1][Y]), cv::Point(city[0][X], city[0][Y]), line, 2);

		}
	}
	
	cv::imshow("遗传算法解决旅行商问题仿真", tsp_image);
	cv::waitKey(33);
}
int fileWrite()
{
	int icount = pointcount;
	fprintf(f, "pointcount:%d    ", pointcount);
	while (icount--)
		fprintf(f, "{%d,%d},", city[icount][0], city[icount][1]);
	fclose(f);
	return 1;
}
int fileRead(){
	int icount = 0;
	char buff=',';
	while (!feof(f)){
		fscanf(f, "%f%d%f%d%f ", buff, city[icount][0], buff, city[icount][1],buff);
		icount++;
	}
	fclose(f);
	pointcount = icount;
	return 1;
}