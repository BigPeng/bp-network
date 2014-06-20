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

#define NUM_I 784//����㵥Ԫ����
#define NUM_H 20//���ز㵥Ԫ����
#define NUM_O 10//����㵥Ԫ����

static const float THRESHOD =0.99; //��ȷ��
static const float RATE =0.05; //ѧϰ����
static const int TRAIN_TIMES =30; //���ѵ������


static const char FILE_TAG = ',';//�ļ��ָ���
static const int FIRST_CLASS = 0;//��ʼ����

static const int W_IH =0; //�߳�����
static const int W_HO =1; 
static const int B_H =2; 
static const int B_O =3; 

static const string PATH = "..\\Data\\handwrite\\";
static const string TRAIN_FILE = PATH+"train.format";//ѵ���ļ�
static const string TEST_FILE = PATH+"test.format";//�����ļ�
//////////ƫ������Ȩֵ������Ҫѵ������Ϊȫ�ֱ���////////////
weightType weight_ih[NUM_I][NUM_H];//����㵽���ز��Ȩֵ����
weightType weight_ho[NUM_H][NUM_O];//���ز㵽������Ȩֵ����
biasType bias_h[NUM_H];//���ز��ƫ����
biasType bias_o[NUM_O];//������ƫ����

weightType delta_weight_ih[NUM_I][NUM_H];//����㵽���ز��Ȩֵ��������
weightType delta_weight_ho[NUM_H][NUM_O];//���ز㵽������Ȩֵ��������
biasType delta_bias_h[NUM_H];//���ز��ƫ��������
biasType delta_bias_o[NUM_O];//������ƫ��������
/////////////////////////////////////////////////////

vector<inputType> lables_num(NUM_O,0);//ÿ���������ļ�¼
vector<inputType> lables_correct(NUM_O,0);//ÿ������з�����ȷ������

//////////////////////////////////////////////////////
vector<inputType> initInputLayer(vector<vector<inputType>>&,int);//����һ����ʼ���˵������
void initWeightAndBias();//��ʼ��Ȩֵweight_ih��weight_ho��ƫ����bias_h��bias_o
vector<hiddenType> computHiddenUnit(vector<inputType>);//��������㣬Ȩֵ���󣬺�ƫ�����������ز�ÿ���񾭵�Ԫ
vector<outputType> computOutputUnit(vector<hiddenType>);//�������ز㣬Ȩֵ���󣬺�ƫ�������������ÿ���񾭵�Ԫ
vector<errorType>  computOutputError(vector<outputType>,vector<inputType> lables);//�����񾭵�Ԫֵ��ѵ������Ŀ��ֵ�������������
vector<errorType>  computHiddenError(vector<errorType> ,vector<hiddenType>);//�����������������ز���񾭵�Ԫ�������ز�����
void updateHidden(vector<errorType>,vector<inputType>);//�������ز��Ȩֵ��ƫ����
void updateOutput(vector<errorType>,vector<hiddenType>);//����������Ȩֵ��ƫ����
void trainModel(vector<vector<inputType>> &intdata,int index);//��ģ��ѵ��
float experimentOnModel(vector<vector<inputType>>&);//�����ݽ��в��ԣ���������ȷ��
void writeWeightAndBias(int kinds,string filePath);//����Ȩֵ
void saveWeightBias();//����Ȩֵ
bool readWeightBias();//��ȡȨ��
void writeTestAccuracyResult(float accuracy,vector<inputType>lables_correct,vector<inputType>lables_num);//�������׼ȷ��
/////////////////////////////////////////////////////////////
/////////���̣߳�δʹ��/////////

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