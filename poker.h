#ifndef POKER_H
#define POKER_H
#include <cmath>
#include "reader.h"

typedef int inputType;
typedef double hiddenType;
typedef double outputType;
typedef double weightType;
typedef float biasType;
typedef double errorType;

#define NUM_I 784//输入层单元个数
#define NUM_H 20//隐藏层单元个数
#define NUM_O 10//输出层单元个数

static const float THRESHOD =0.99; //正确率
static const float RATE =0.05; //学习速率
static const int TRAIN_TIMES =30; //最大训练次数


static const char FILE_TAG = ',';//文件分隔符
static const int FIRST_CLASS = 0;//起始类标号

static const int W_IH =0; //线程数量
static const int W_HO =1; 
static const int B_H =2; 
static const int B_O =3; 

static const string PATH = "..\\Data\\handwrite\\";
static const string TRAIN_FILE = PATH+"train.format";//训练文件
static const string TEST_FILE = PATH+"test.format";//测试文件
//////////偏移量和权值矩阵需要训练，作为全局变量////////////
weightType weight_ih[NUM_I][NUM_H];//输入层到隐藏层的权值矩阵
weightType weight_ho[NUM_H][NUM_O];//隐藏层到输出层的权值矩阵
biasType bias_h[NUM_H];//隐藏层的偏移量
biasType bias_o[NUM_O];//输出层的偏移量

weightType delta_weight_ih[NUM_I][NUM_H];//输入层到隐藏层的权值增量矩阵
weightType delta_weight_ho[NUM_H][NUM_O];//隐藏层到输出层的权值增量矩阵
biasType delta_bias_h[NUM_H];//隐藏层的偏移量增量
biasType delta_bias_o[NUM_O];//输出层的偏移量增量
/////////////////////////////////////////////////////

vector<inputType> lables_num(NUM_O,0);//每个类表包含的记录
vector<inputType> lables_correct(NUM_O,0);//每个类表中分类正确的数据

//////////////////////////////////////////////////////
vector<inputType> initInputLayer(vector<vector<inputType>>&,int);//返回一个初始化了的输入层
void initWeightAndBias();//初始化权值weight_ih，weight_ho和偏移量bias_h，bias_o
vector<hiddenType> computHiddenUnit(vector<inputType>);//根据输入层，权值矩阵，和偏移量计算隐藏层每个神经单元
vector<outputType> computOutputUnit(vector<hiddenType>);//根据隐藏层，权值矩阵，和偏移量计算输出层每个神经单元
vector<errorType>  computOutputError(vector<outputType>,vector<inputType> lables);//根据神经单元值和训练集的目标值计算输出层的误差
vector<errorType>  computHiddenError(vector<errorType> ,vector<hiddenType>);//根据输出层的误差和隐藏层的神经单元计算隐藏层的误差
void updateHidden(vector<errorType>,vector<inputType>);//更新隐藏层的权值和偏移量
void updateOutput(vector<errorType>,vector<hiddenType>);//更新输出层的权值和偏移量
void trainModel(vector<vector<inputType>> &intdata,int index);//对模型训练
float experimentOnModel(vector<vector<inputType>>&);//对数据进行测试，并计算正确率
void writeWeightAndBias(int kinds,string filePath);//保存权值
void saveWeightBias();//保存权值
bool readWeightBias();//读取权重
void writeTestAccuracyResult(float accuracy,vector<inputType>lables_correct,vector<inputType>lables_num);//保存测试准确率
/////////////////////////////////////////////////////////////
/////////多线程，未使用/////////

#include <process.h>
#include "reader.h"
#include <windows.h>
typedef void *HANDLE;
class Thread
{
public:
	void start();
	virtual void run();
	HANDLE getThread();
	Thread(int b,int e,vector<vector<inputType>> &data):begin(b),end(e),intdata(data){};
private:
	int begin;
	int end;
	vector<vector<inputType>> intdata;
	HANDLE hThread;
	static void agent(void *p);
};

void Thread::start()
{
	hThread =(HANDLE)_beginthread(agent, 0, (void *)this);
}
void Thread::run()
{
	
	for(int i=begin;i<end;i++){
		//Sleep(200);
		trainModel(intdata,i);
	}
}
void Thread::agent(void *p)
{
	Thread *agt = (Thread *)p;
	agt->run();
}
HANDLE Thread::getThread()
{
	return hThread;
}


#endif