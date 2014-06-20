#include "poker.h"
#include <Windows.h>


int main(){
	/*****************************��ʼ��********************************************/
	initWeightAndBias();//��ʼ��Ȩֵ��ƫ����	
	vector<vector<string>> data;
	bool tag = readFile(data,FILE_TAG,TRAIN_FILE);//���ļ�
	if(tag == false)//���ļ��쳣
		return 1;
	vector<vector<inputType>> intdata;
	transToNumVector(data,intdata);//��string����תΪ�����񾭵�Ԫ����	
	/*****************************��ʼ������*****************************************/

	/*****************************************ѵ����ʼ*******************************/	
	int count=0;
	float accuracy = experimentOnModel(intdata);//������ȷ��
		cout <<"ѵ��֮ǰ:\t"<< accuracy<<endl;
	//saveWeightBias();//����ģ��
	//system("spause");
	while(true){
		//saveWeightBias();//����ģ��
		if(accuracy > THRESHOD||++count>TRAIN_TIMES)//��ȷ�ʳ�����ֵ��ֹͣѵ��
			break;
		for(int index=0;index<intdata.size();index++){//ÿһ����¼		
			trainModel(intdata,index);			
		}	
		accuracy = experimentOnModel(intdata);//������ȷ��
		cout <<count<<":\t"<< accuracy<<endl;		
		if(count%20==19)
			saveWeightBias();//����ģ��
	}
	/*****************************************ѵ������***********************************************/

	saveWeightBias();//����ģ��

	/*****************************************���Կ�ʼ***********************************************/	
	vector<vector<string>> testData;
	tag= readFile(testData,FILE_TAG,TEST_FILE);//���ļ�
	if(tag == false)//���ļ��쳣
		return 1;
	vector<vector<inputType>> intTestdata;
	transToNumVector(testData,intTestdata);//��string����תΪ�����񾭵�Ԫ����
	accuracy = experimentOnModel(intTestdata);//������ȷ��
	writeTestAccuracyResult(accuracy,lables_correct,lables_num);
	cout<<"��������ȷ�ʣ�"<< accuracy<<endl;
	for(int i=0;i<NUM_O;i++){
		cout<<i<<":"<<lables_correct[i]<<","<<lables_num[i]<<","<<(double)lables_correct[i]/lables_num[i]<<endl;
	}
	/*****************************************���Խ���***********************************************/
	cout<<"������ʮ����Ȩ�ؿ�..."<<endl;
	for(int i=1;i<11;i++){
		Sleep(1000);
		cout<<i<<endl;
	}
	//saveWeightBias();//����ģ��
	return 0;
}


/**************����һ����ʼ���˵������*************/
vector<inputType>  initInputLayer(vector<vector<inputType>> &data,int index){
	vector<inputType> currentRecord(data[index]);//�����±�ȡ����¼	
	if(currentRecord.size() != NUM_I+1){
		cout<<"���ļ�����ȡ�����Ը������������NUM_I��һ�£�"<<endl;		
		throw exception("���ļ�����ȡ�����Ը������������NUM_I��һ�£�");
	}	
	return currentRecord;
}

/**************��ʼ��Ȩֵweight_ih��weight_ho��ƫ����bias_h��bias_o*************/
void initWeightAndBias(){
	/*	
	bias_h[0]=-0.4;
	bias_h[1]=0.2;
	bias_o[0]=0.1;
	weight_ih[0][0]=0.2;
	weight_ih[0][1]=-0.3;
	weight_ih[1][0]=0.4;
	weight_ih[1][1]=0.1;
	weight_ih[2][0]=-0.5;
	weight_ih[2][1]=0.2;
	weight_ho[0][0]=-0.3;
	weight_ho[1][0]=-0.2;
	*/
	if(readWeightBias()==true)//���ļ��ж�ȡȨ��ƫ�Ƴɹ���
		return;//����Ҫ�������Ȩ��
	for(int i=0;i<NUM_H;i++){//��ʼ�����ز�
		bias_h[i]=(biasType)(rand()%1024-512)/512;
		for(int j=0;j<NUM_I;j++){
			weight_ih[j][i] = (weightType)(rand()%1024-512)/512;
		}
	}
	for(int i=0;i<NUM_O;i++){//��ʼ�������
		bias_o[i]=(biasType)(rand()%1024-512)/512;
		for(int j=0;j<NUM_H;j++){
			weight_ho[j][i] = (weightType)(rand()%1024-512)/512;
		}
	}

}

/***********************��������㣬Ȩֵ�����ƫ�����������ز�ÿ���񾭵�Ԫ************/
vector<hiddenType>  computHiddenUnit(vector<inputType> inputLayer){
	vector<hiddenType> hiddenLayer(NUM_H);	
	for(int i=0;i<NUM_H;i++){//����������ز�
		double sum=0.0;
		for(int j=0;j<NUM_I;j++){//��Ӧ������Ȩֵ��			
			sum += inputLayer[j]*weight_ih[j][i];
		}
		sum += bias_h[i];
		hiddenLayer[i]=1/(1+exp(-sum));
	}
	return hiddenLayer;
}

/***********************�������ز㣬Ȩֵ���󣬺�ƫ�������������ÿ���񾭵�Ԫ************/
vector<outputType> computOutputUnit(vector<hiddenType> hiddenLayer){
	vector<outputType> ouputLayer(NUM_O);
	for(int i=0;i<NUM_O;i++){
		double sum=0.0;
		for(int j=0;j<NUM_H;j++){
			sum += hiddenLayer[j]*weight_ho[j][i];
		}
		sum += bias_o[i];
		ouputLayer[i]=1/(1+exp(-sum));
	}
	
	return ouputLayer;
}

/**************�����񾭵�Ԫֵ��ѵ������Ŀ��ֵ�������������*************/
vector<errorType> computOutputError(vector<outputType> ouputLayer,vector<inputType> lables){
	vector<errorType> outputErrot(NUM_O);
	for(int i=0;i<NUM_O;i++){
		outputErrot[i]=ouputLayer[i]*(1-ouputLayer[i])*((errorType)lables[i]-ouputLayer[i]);
	}
	return outputErrot;
}


/*******************�����������������ز���񾭵�Ԫ�������ز�����***********/
vector<errorType> computHiddenError(vector<errorType> outputErrot,vector<hiddenType> hiddenLayer){
	vector<errorType> hiddenErrot(NUM_H);
	for(int i=0;i<NUM_H;i++){
		errorType sum = 0.0;
		for(int j=0;j<NUM_O;j++){
			sum+=outputErrot[j]*weight_ho[i][j];
		}
		hiddenErrot[i]=hiddenLayer[i]*(1-hiddenLayer[i])*sum;
	}
	return hiddenErrot;
}
/**************�������ز��Ȩֵ��ƫ����*************/
void  updateHidden(vector<errorType> hiddenErrot,vector<inputType> inputLayer){	
	for(int j=0;j<NUM_H;j++){
		for(int i=0;i<NUM_I;i++){
			weight_ih[i][j] += hiddenErrot[j]*(errorType)inputLayer[i]*RATE;
		}
		bias_h[j]+=  hiddenErrot[j]*RATE;
	}

}
/**************����������Ȩֵ��ƫ����*************/
void updateOutput(vector<errorType> outputErrot,vector<outputType> hiddenLayer){
	for(int j=0;j<NUM_O;j++){
		for(int i=0;i<NUM_H;i++){
			weight_ho[i][j] += outputErrot[j]*hiddenLayer[i]*RATE;
		}
		bias_o[j]+=  outputErrot[j]*RATE;
	}
}
/**************��ģ�ͽ���ѵ��***************/
void trainModel(vector<vector<inputType>> &intdata,int index){
	if(intdata[index].size() == NUM_I+1){//�������Ŀ���
		vector<inputType> inputLayer = initInputLayer(intdata,index);//���������
		vector<hiddenType> hiddenLayter = computHiddenUnit(inputLayer);//�������ز�
		vector<outputType> outputLayer = computOutputUnit(hiddenLayter);//���������
		vector<inputType> lables(NUM_O,0);
		lables[inputLayer[NUM_I]-FIRST_CLASS]=1;
		vector<errorType> outputError = computOutputError(outputLayer,lables);//������������
		vector<errorType> hiddenError = computHiddenError(outputError,hiddenLayter);//�������ز����
		updateHidden(hiddenError,inputLayer);//�������ز��Ȩ�غ�ƫ����
		updateOutput(outputError,hiddenLayter);//����������Ȩ�غ�ƫ����
	}
}
/**************�����ݽ��в��ԣ���������ȷ��*************/
float experimentOnModel(vector<vector<inputType>>&intdata){
	lables_num.assign(NUM_O,0);//����
	lables_correct.assign(NUM_O,0);
	int correctCount =0;
	for(int index=0;index<intdata.size();index++){/*ÿһ����¼*/
		if(intdata[index].size() == NUM_I+1){//�������Ŀ���
			lables_num[intdata[index][NUM_I]-FIRST_CLASS]++;//���
			vector<inputType> inputLayer = initInputLayer(intdata,index);//���������
			vector<hiddenType> hiddenLayter = computHiddenUnit(inputLayer);//�������ز�
			vector<outputType> outputLayer = computOutputUnit(hiddenLayter);//���������
			outputType max = -2;	
			int lable = NUM_O-1;
			for(int i=0;i<NUM_O;i++){
				if(outputLayer[i]>max){
					max = outputLayer[i];
					lable = i;
				}
			}
			if(inputLayer[NUM_I]-FIRST_CLASS==lable){
				correctCount++;
				lables_correct[lable]++;

				
				if(lable >= 16){
					for(vector<inputType>::iterator it = inputLayer.begin();it!=inputLayer.end();it++)
						cout<<*it<<" ";
					cout<<endl;
					
				}
			}
		}
	}
	
	return (float)correctCount/intdata.size();
}


/*
*��inputStringת������ֵ�ͣ�����ģ�壬ʵ�ָ���
*/
template <typename T>
void transToNum(string inputString,T &result){    
	const char *p = inputString.c_str();
	char * type = getType(result);
	if(type== "int")
		result =  atoi(p);
	else if(type== "double")
		result =   atof(p);
	else if(type== "float")
		result =   atof(p);
	else throw exception("'result'�����쳣��transToNumֻ֧��int,float,double���ָ�ʽ��");
}


/*******��string���͵Ķ�άvectorת��Ϊ��ֵ����*******/
template <typename T>
void transToNumVector(vector<vector<string> > data,vector<vector<T> > &intdata){
	intdata.clear();//�����
	vector<vector<string>>::iterator iter =  data.begin();
	for(;iter != data.end();iter++){
		vector<string> line = *iter;
		vector<string>::iterator lineiter =  line.begin();
		vector<T> intline;
		for(;lineiter != line.end();lineiter++){
			T result;
			transToNum(*lineiter,result);
			intline.push_back(result);    
		}
		intdata.push_back(intline);
	}	
}

void testRead(){
	string filePath ="E:\\VisualStudioWordspace\\Poker\\Data\\train.data";
	time_t now;
	now = time(NULL);//���ÿ�ʼʱ��
	cout<<now<<endl;
	vector<vector<string>> data;
	bool tag = readFile(data,',',filePath);//���ļ�
	now = time(NULL);//����ʱ��
	cout<<now<<endl;	
	vector<vector<int>> intdata;
	transToNumVector(data,intdata);
	////////////////����intdata�������ֵ��������////////////////////////////////////////////
	vector<vector<int>>::iterator intIter =  intdata.begin();
	for(;intIter != intdata.end();intIter++){
		vector<int> line = *intIter;
		vector<int>::iterator lineiter =  line.begin();        
		for(;lineiter != line.end();lineiter++){            
			cout<<*lineiter<<" ";            
		}        
		cout<<endl;
	}    
}

void writeWeightAndBias(int kinds,string filePath){
	//cout<<"��ʼд�ļ�:"<<endl;
	//cout<<filePath<<endl;
	ofstream fileWriter;
	fileWriter.open(filePath,ios::out);//�Է�ʽ��
	if(!fileWriter.good()){//���ļ��쳣
		cout<<filePath<<" д�ļ��쳣��"<<endl;
		return;		
	}
	if(kinds == W_IH){//�����--���ز��Ȩֵ
		for(int i=0;i<NUM_I;i++){
			for(int j=0;j<NUM_H-1;j++){
				fileWriter<<weight_ih[i][j]<<",";
			}
			fileWriter<<weight_ih[i][NUM_H-1]<<endl;
		}
	}else if(kinds == W_HO){//���ز�--������Ȩֵ
		for(int i=0;i<NUM_H;i++){
			for(int j=0;j<NUM_O-1;j++){
				fileWriter<<weight_ho[i][j]<<",";
			}
			fileWriter<<weight_ho[i][NUM_O-1]<<endl;
		}
	}else if(kinds == B_H){//���ز�ƫ����
		for(int i=0;i<NUM_H-1;i++)
			fileWriter<<bias_h[i]<<",";
		fileWriter<<bias_h[NUM_H-1]<<endl;
	}else if(kinds == B_O){//�����ƫ����
		for(int i=0;i<NUM_O-1;i++)
			fileWriter<<bias_o[i]<<",";
		fileWriter<<bias_o[NUM_O-1]<<endl;
	}
	fileWriter.close();
	fileWriter.clear();
	cout<<kinds<<"�ļ�д�꣡"<<endl;	  
}
void saveWeightBias(){
	string fileMain = PATH;
	char  ch[10];
	itoa(NUM_H,ch,10);
	string num_s(ch);	
	string wih = fileMain+"weight_ih_"+num_s+".txt";
	string who = fileMain+"weight_ho_"+num_s+".txt";
	string bh = fileMain+"bias_h_"+num_s+".txt";
	string bo = fileMain+"bias_o_"+num_s+".txt";
	writeWeightAndBias(W_IH,wih);
	writeWeightAndBias(W_HO,who);
	writeWeightAndBias(B_H,bh);
	writeWeightAndBias(B_O,bo);
}
bool readWeightBias(){
	string fileMain =PATH;
	char  ch[10];
	itoa(NUM_H,ch,10);
	string num_s(ch);	
	string wih = fileMain+"weight_ih_"+num_s+".txt";
	string who = fileMain+"weight_ho_"+num_s+".txt";
	string bh = fileMain+"bias_h_"+num_s+".txt";
	string bo = fileMain+"bias_o_"+num_s+".txt";
	vector<vector<string>> weight_string;	
	vector<vector<string>> bias_string;
	/////////////////////����-���ز�/////////////////////
	bool tag1 = readFile(weight_string,',',wih);//
	bool tag2 = readFile(bias_string,',',bh);//���ļ�	
	if(tag1 == false||false == tag2)
		return false;//��ȡȨ���ļ�����
	vector<vector<weightType>> weight_data;
	vector<vector<biasType>> bias_data;
	transToNumVector(weight_string,weight_data);//��string����תΪ�����񾭵�Ԫ����
	transToNumVector(bias_string,bias_data);//��string����תΪ�����񾭵�Ԫ����
	for(int i=0;i<NUM_H;i++){//��ʼ�����ز�
		bias_h[i]=bias_data[0][i];
		for(int j=0;j<NUM_I;j++){
			weight_ih[j][i] = weight_data[j][i];
		}
	}

	/////////////////////����-�����/////////////////////
	tag1 = readFile(weight_string,',',who);//
	tag2 = readFile(bias_string,',',bo);//���ļ�	
	if(tag1 == false||false == tag2)
		return false;//��ȡȨ���ļ�����	
	transToNumVector(weight_string,weight_data);//��string����תΪ�����񾭵�Ԫ����
	transToNumVector(bias_string,bias_data);//��string����תΪ�����񾭵�Ԫ����
	for(int i=0;i<NUM_O;i++){//��ʼ�������
		bias_o[i]=bias_data[0][i];
		for(int j=0;j<NUM_H;j++){
			weight_ho[j][i] = weight_data[j][i];
		}
	}
	return true;//��ȷ����֮ǰ�����Ȩ���ļ�
	
}
/***�����齻��ϲ�
void inter(){
	string path ="..\\Data\\poker-order\\";
	vector<vector<string>> vectors;
	for(int i=0;i<10;i++){
		char  ch[10];
		itoa(i,ch,10);
		string num_s(ch);
		vector<string> data;
		if(false == readAsVecLine(data,'\t',path+num_s+".txt")){
			cout<<"Error!";
			return 1;
		}		
		vectors.push_back(data);
	}
	vector<string> result;
	interInsert(result,vectors);
	randInsert(result,vectors);
	saveFile(path+"meger.txt",result);

}********/

void writeTestAccuracyResult(float accuracy,vector<inputType>lables_correct,vector<inputType>lables_num){
	char  ch[10];
	itoa(NUM_H,ch,10);
	string num_s(ch);
	string fileName = PATH+"accuracy_result"+num_s+".txt";
	ofstream fileWriter;
	fileWriter.open(fileName,ios::out);//�Է�ʽ��
	if(!fileWriter.good()){//���ļ��쳣
		cout<<fileName<<" д�ļ��쳣��"<<endl;
		return;	
	}
	fileWriter<<"total��\t"<< accuracy<<endl;
	double sum = 0.0;
	for(int i=0;i<NUM_O;i++){
		double acc = (double)lables_correct[i]/lables_num[i];
		sum += acc;
		fileWriter<<i<<":"<<lables_correct[i]<<"\t"<<lables_num[i]<<"\t"<<acc<<endl;
	}
	fileWriter<<"avrg:\t"<<(double)sum/NUM_O;
	fileWriter.flush();
	fileWriter.close();
}