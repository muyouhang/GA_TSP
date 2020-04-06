#include "GA_TSP.h"


GA_TSP::GA_TSP()
{
}


GA_TSP::~GA_TSP()
{
}
int PopSize =500;	//��Ⱥ��ģ
int MaxGeneration = 10000;//��������
double Pc = 0.2;//�������
double Pm = 0.005;//�������
int CITYCOUNT   = 3;	//���и���
int CHROMLENGTH = 3;
int CitySet[500];//���м���
int WorkSet[100];//Ⱦɫ�����ʱ����ʱ�������飬��Ż�δ���ʵĳ�������
int TempCitySet[100];//���Ⱦɫ�������ɺ��������ĳ�������
FILE *f= fopen("city.txt", "rb+");

//int disaster_control = 0;
//double disaster_Pc = 0.3;
//double disaster_Pm = 0.01;
//double m = Pm;//���治��
//double c = Pc;
cv::Scalar line = cv::Scalar(255, 0,0);
typedef struct individual{
	int chrom[100];//Ⱦɫ�����
	double value;          //valueֵ��ʾ·������
	double fitness;        //��Ӧ��
}individual;

int generation;
individual bestindividual;//ĳһ��Ⱥ�������Ÿ���
individual currentbest;//��ǰ���Ÿ���
individual population[POPSIZE];//

cv::Mat tsp_image = cv::Mat::zeros(cv::Size(800, 600),CV_8UC3);
int CityDistance[100][100] = {};//����֮��ľ������,�������

//ѡ������
void Select(int start, int stop, individual *newpopulation){
	int i, index;
	double p, sum = 0.0;
	double cfitness[POPSIZE];//������Ӧ��
	//������Ӧ��sum
//#pragma omp parallel for
	for (i = start; i < stop; i++){
		sum += population[i].fitness;
	}
	//��ÿ���������Ӧ����ռ�İٷֱ�cfitness[i]
#pragma omp parallel for
	for (i = start; i < stop; i++){
		cfitness[i] = population[i].fitness / sum;
	}
	//��Ӧ����ͣ����Ƹ��ʷֲ���0 1 2 3 4 -> 0 1 3 4 10
	for (i = start + 1; i < stop; i++){
		cfitness[i] = cfitness[i - 1] + cfitness[i];
	}
	//ѡ����壬
//#pragma omp parallel for
	for (i = start; i < stop; i++){
		p = rand() % 1000 / 1000.0;//0��1֮��
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
//���������
int random(int randmax){
	return rand() % randmax;
}
//��������
void CrossoverOperator(void){
	//TODO:�����������淶����Ҫ�������
	int i, j;
	int index[POPSIZE];
	int point, temp;
	double p;
	int CityNo;  //����ΪȾɫ����뽻���� �ݴ�BUFF

	for (i = 0; i < PopSize; i++){
		index[i] = i;//TODO:���
	}
	//����i �� point+i �ı��
	for (i = 0; i < PopSize; i++){
		point = random(PopSize - i);  //���ɵ������С��PopSize - i
		temp = index[i];
		index[i] = index[point + i];
		index[point + i] = temp;
	}
	//���� point �� CHROMLENGTH ���ȵ� Ⱦɫ�����
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
//��������
void MutationOperator(){
	int i, j;
	double p;

	for (i = 0; i < PopSize; i++){
		for (j = 1; j < CHROMLENGTH; j++){// Ⱦɫ���һ��Ԫ��Ϊ1�����ܱ���
			p = rand() % 1000 / 1000.0;
			//����޸Ļ���
			if (p < Pm){
				population[i].chrom[j] = random(CITYCOUNT - j) + 1;
			}
		}
	}
}
//���ɳ�ʼȺ��
void GenerateInitialPopulation(){
	int i, j;
	for (i = 0; i < PopSize; i++){
		CitySet[i] = i + 1;
	}
	srand((unsigned)time(NULL));
	for (i = 0; i < PopSize; i++){
		//�������й̶�Ϊ��һ������
		population[i].chrom[0] = 1;
		for (j = 1; j < CITYCOUNT; j++){
			population[i].chrom[j] = random(CITYCOUNT - j) + 1;
			//std::cout << population[i].chrom[j];
		}
	}
}
//������һ��Ⱥ��
void GenerateNextPopulation(){
	SelectionOperator();
	CrossoverOperator();
	MutationOperator();
}
//�ӳ���������ɾȥһ����,ɾ��i����
void DeleteOneCity(int *workset, int position, int n){
	for (int i = position - 1; i < n - 1; i++){
		workset[i] = workset[i + 1];
	}
}
//��Ⱦɫ��Chrom���룬�����Result��
void DecodeChromosome(int *chrom, int *Result){
	int *p;   //ָ��Ҫ���ʵĳ���
	int i = 0;
	int n = CITYCOUNT;

	p = chrom;//��������
	//��ʼ������������
	for (i = 0; i < CITYCOUNT; i++){
		WorkSet[i] = CitySet[i];
	}
	//TODO:
	for (i = 0; i < CITYCOUNT; i++){
		Result[i] = WorkSet[*p - 1];//���������е�*p��Ԫ���Ǽ������ʵĳ���
		DeleteOneCity(WorkSet, *p, n);//��δ���ʵ�n��������ɾȥ�շ��ʵĵ�*p��
		n--;//δ���ʳ��и�����1
		p++;//�������ʳ���ָ����һ��
	}
}
//������
void OutputTextReport(){
	int i = 0;
	DecodeChromosome(currentbest.chrom, TempCitySet);
	printf("Ⱥ�������%d , ���·���� %f \n", generation, currentbest.value);
	printf("������룺[");
	for (i = 0; i < CHROMLENGTH; i++){
		printf(" %d", currentbest.chrom[i]);
	}
	printf("]\n");
	printf("����˳��[");
	for (i = 0; i < CHROMLENGTH; i++){
		printf(" %d", TempCitySet[i]);

	}
	printf("]\n\n");
}
//������Ӧ��
void CalculateFitness(){
	int i, j;
	double TotalDistance = 0.0;
	//��ÿ�����壬����·���ܳ���
	for (i = 0; i < PopSize; i++){
		DecodeChromosome(population[i].chrom, TempCitySet);
		for (j = 1; j < CITYCOUNT; j++){
			TotalDistance = TotalDistance + CityDistance[TempCitySet[j - 1] - 1][TempCitySet[j] - 1];
		}
		TotalDistance = TotalDistance + CityDistance[TempCitySet[CITYCOUNT - 1] - 1][0];
		population[i].value = TotalDistance;
		TotalDistance = 0.0;
	}
	//������Ŀ ����  ����·���ܳ��� = ������Ӧ��
	//·��Խ�̣���Ӧ��Խ��
	for (i = 0; i < PopSize; i++){
		population[i].fitness = CITYCOUNT / population[i].value;
	}
}
//Ѱ�����Ÿ���
void FindBestIndividual(void)
{
	int i;

	bestindividual = population[0];
	//ͨ���Ƚ���Ӧ����ɸѡ��Խ��Խ��
	for (i = 1; i<PopSize; i++)
	{
		if (population[i].fitness>bestindividual.fitness)
			bestindividual = population[i];
	}
	//�Ե�һ��
	if (generation == 0)
	{
		currentbest = bestindividual;
	}
	else
	{
		if (bestindividual.fitness>currentbest.fitness)
		{
			currentbest = bestindividual;
			OutputTextReport();    /* ÿ����һ�ε�ǰ����ֵ�������� */
		
			//�ֱ��������
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
//������㹫ʽ
int CalculatorEuler(int *src1, int *src2){
	return sqrt(pow((src1[X] - src2[X]), 2) + pow((src1[Y] - src2[Y]), 2));

}
//������������֮��ľ���
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
//���ͼ�λ���ʾ����������-��ɫ   ·��-��ɫ  ����-��ɫ 
void ShowResult(){
	//cvZero(tsp_image);
	tsp_image = cv::imread("china.jpg");
	cv::rectangle(
		tsp_image,
		cv::Point(city[0][X] - 5, city[0][Y] - 5),
		//cvPoint(box.x+box.width,box.y+box.height),
		cv::Point(city[0][X] + 5, city[0][Y] + 5),
		cv::Scalar(0, 0,255),											//��ɫ
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
	
	cv::imshow("�Ŵ��㷨����������������", tsp_image);
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