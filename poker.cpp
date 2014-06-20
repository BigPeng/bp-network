#include "poker.h"
#include <Windows.h>


int main(){
	/*****************************初始化********************************************/
	initWeightAndBias();//初始化权值和偏移量	
	vector<vector<string>> data;
	bool tag = readFile(data,FILE_TAG,TRAIN_FILE);//读文件
	if(tag == false)//读文件异常
		return 1;
	vector<vector<inputType>> intdata;
	transToNumVector(data,intdata);//将string类型转为输入神经单元类型	
	/*****************************初始化结束*****************************************/

	/*****************************************训练开始*******************************/	
	int count=0;
	float accuracy = experimentOnModel(intdata);//计算正确率
		cout <<"训练之前:\t"<< accuracy<<endl;
	//saveWeightBias();//保存模型
	//system("spause");
	while(true){
		//saveWeightBias();//保存模型
		if(accuracy > THRESHOD||++count>TRAIN_TIMES)//正确率超过阈值，停止训练
			break;
		for(int index=0;index<intdata.size();index++){//每一条记录		
			trainModel(intdata,index);			
		}	
		accuracy = experimentOnModel(intdata);//计算正确率
		cout <<count<<":\t"<< accuracy<<endl;		
		if(count%20==19)
			saveWeightBias();//保存模型
	}
	/*****************************************训练结束***********************************************/

	saveWeightBias();//保存模型

	/*****************************************测试开始***********************************************/	
	vector<vector<string>> testData;
	tag= readFile(testData,FILE_TAG,TEST_FILE);//读文件
	if(tag == false)//读文件异常
		return 1;
	vector<vector<inputType>> intTestdata;
	transToNumVector(testData,intTestdata);//将string类型转为输入神经单元类型
	accuracy = experimentOnModel(intTestdata);//计算正确率
	writeTestAccuracyResult(accuracy,lables_correct,lables_num);
	cout<<"测试总正确率："<< accuracy<<endl;
	for(int i=0;i<NUM_O;i++){
		cout<<i<<":"<<lables_correct[i]<<","<<lables_num[i]<<","<<(double)lables_correct[i]/lables_num[i]<<endl;
	}
	/*****************************************测试结束***********************************************/
	cout<<"我数到十保存权重咯..."<<endl;
	for(int i=1;i<11;i++){
		Sleep(1000);
		cout<<i<<endl;
	}
	//saveWeightBias();//保存模型
	return 0;
}


/**************返回一个初始化了的输入层*************/
vector<inputType>  initInputLayer(vector<vector<inputType>> &data,int index){
	vector<inputType> currentRecord(data[index]);//根据下标取到记录	
	if(currentRecord.size() != NUM_I+1){
		cout<<"从文件将读取的属性个数与您定义的NUM_I不一致！"<<endl;		
		throw exception("从文件将读取的属性个数与您定义的NUM_I不一致！");
	}	
	return currentRecord;
}

/**************初始化权值weight_ih，weight_ho和偏移量bias_h，bias_o*************/
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
	if(readWeightBias()==true)//从文件中读取权重偏移成功，
		return;//不需要随机生成权重
	for(int i=0;i<NUM_H;i++){//初始化隐藏层
		bias_h[i]=(biasType)(rand()%1024-512)/512;
		for(int j=0;j<NUM_I;j++){
			weight_ih[j][i] = (weightType)(rand()%1024-512)/512;
		}
	}
	for(int i=0;i<NUM_O;i++){//初始化输出层
		bias_o[i]=(biasType)(rand()%1024-512)/512;
		for(int j=0;j<NUM_H;j++){
			weight_ho[j][i] = (weightType)(rand()%1024-512)/512;
		}
	}

}

/***********************根据输入层，权值矩阵和偏移量计算隐藏层每个神经单元************/
vector<hiddenType>  computHiddenUnit(vector<inputType> inputLayer){
	vector<hiddenType> hiddenLayer(NUM_H);	
	for(int i=0;i<NUM_H;i++){//逐个生成隐藏层
		double sum=0.0;
		for(int j=0;j<NUM_I;j++){//对应输入层和权值和			
			sum += inputLayer[j]*weight_ih[j][i];
		}
		sum += bias_h[i];
		hiddenLayer[i]=1/(1+exp(-sum));
	}
	return hiddenLayer;
}

/***********************根据隐藏层，权值矩阵，和偏移量计算输出层每个神经单元************/
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

/**************根据神经单元值和训练集的目标值计算输出层的误差*************/
vector<errorType> computOutputError(vector<outputType> ouputLayer,vector<inputType> lables){
	vector<errorType> outputErrot(NUM_O);
	for(int i=0;i<NUM_O;i++){
		outputErrot[i]=ouputLayer[i]*(1-ouputLayer[i])*((errorType)lables[i]-ouputLayer[i]);
	}
	return outputErrot;
}


/*******************根据输出层的误差和隐藏层的神经单元计算隐藏层的误差***********/
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
/**************更新隐藏层的权值和偏移量*************/
void  updateHidden(vector<errorType> hiddenErrot,vector<inputType> inputLayer){	
	for(int j=0;j<NUM_H;j++){
		for(int i=0;i<NUM_I;i++){
			weight_ih[i][j] += hiddenErrot[j]*(errorType)inputLayer[i]*RATE;
		}
		bias_h[j]+=  hiddenErrot[j]*RATE;
	}

}
/**************更新输出层的权值和偏移量*************/
void updateOutput(vector<errorType> outputErrot,vector<outputType> hiddenLayer){
	for(int j=0;j<NUM_O;j++){
		for(int i=0;i<NUM_H;i++){
			weight_ho[i][j] += outputErrot[j]*hiddenLayer[i]*RATE;
		}
		bias_o[j]+=  outputErrot[j]*RATE;
	}
}
/**************对模型进行训练***************/
void trainModel(vector<vector<inputType>> &intdata,int index){
	if(intdata[index].size() == NUM_I+1){//忽略最后的空行
		vector<inputType> inputLayer = initInputLayer(intdata,index);//计算输入层
		vector<hiddenType> hiddenLayter = computHiddenUnit(inputLayer);//计算隐藏层
		vector<outputType> outputLayer = computOutputUnit(hiddenLayter);//计算输出层
		vector<inputType> lables(NUM_O,0);
		lables[inputLayer[NUM_I]-FIRST_CLASS]=1;
		vector<errorType> outputError = computOutputError(outputLayer,lables);//计算输出层误差
		vector<errorType> hiddenError = computHiddenError(outputError,hiddenLayter);//计算隐藏层误差
		updateHidden(hiddenError,inputLayer);//更新隐藏层的权重和偏移量
		updateOutput(outputError,hiddenLayter);//更新输出层的权重和偏移量
	}
}
/**************对数据进行测试，并计算正确率*************/
float experimentOnModel(vector<vector<inputType>>&intdata){
	lables_num.assign(NUM_O,0);//清零
	lables_correct.assign(NUM_O,0);
	int correctCount =0;
	for(int index=0;index<intdata.size();index++){/*每一条记录*/
		if(intdata[index].size() == NUM_I+1){//忽略最后的空行
			lables_num[intdata[index][NUM_I]-FIRST_CLASS]++;//类标
			vector<inputType> inputLayer = initInputLayer(intdata,index);//计算输入层
			vector<hiddenType> hiddenLayter = computHiddenUnit(inputLayer);//计算隐藏层
			vector<outputType> outputLayer = computOutputUnit(hiddenLayter);//计算输出层
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
*将inputString转化成数值型，利用模板，实现复用
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
	else throw exception("'result'类型异常，transToNum只支持int,float,double三种格式！");
}


/*******将string类型的二维vector转化为数值类型*******/
template <typename T>
void transToNumVector(vector<vector<string> > data,vector<vector<T> > &intdata){
	intdata.clear();//先清空
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
	now = time(NULL);//设置开始时间
	cout<<now<<endl;
	vector<vector<string>> data;
	bool tag = readFile(data,',',filePath);//读文件
	now = time(NULL);//读完时间
	cout<<now<<endl;	
	vector<vector<int>> intdata;
	transToNumVector(data,intdata);
	////////////////遍历intdata，输出数值化的数据////////////////////////////////////////////
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
	//cout<<"开始写文件:"<<endl;
	//cout<<filePath<<endl;
	ofstream fileWriter;
	fileWriter.open(filePath,ios::out);//以方式打开
	if(!fileWriter.good()){//读文件异常
		cout<<filePath<<" 写文件异常！"<<endl;
		return;		
	}
	if(kinds == W_IH){//输入层--隐藏层的权值
		for(int i=0;i<NUM_I;i++){
			for(int j=0;j<NUM_H-1;j++){
				fileWriter<<weight_ih[i][j]<<",";
			}
			fileWriter<<weight_ih[i][NUM_H-1]<<endl;
		}
	}else if(kinds == W_HO){//隐藏层--输出层的权值
		for(int i=0;i<NUM_H;i++){
			for(int j=0;j<NUM_O-1;j++){
				fileWriter<<weight_ho[i][j]<<",";
			}
			fileWriter<<weight_ho[i][NUM_O-1]<<endl;
		}
	}else if(kinds == B_H){//隐藏层偏移量
		for(int i=0;i<NUM_H-1;i++)
			fileWriter<<bias_h[i]<<",";
		fileWriter<<bias_h[NUM_H-1]<<endl;
	}else if(kinds == B_O){//输出层偏移量
		for(int i=0;i<NUM_O-1;i++)
			fileWriter<<bias_o[i]<<",";
		fileWriter<<bias_o[NUM_O-1]<<endl;
	}
	fileWriter.close();
	fileWriter.clear();
	cout<<kinds<<"文件写完！"<<endl;	  
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
	/////////////////////输入-隐藏层/////////////////////
	bool tag1 = readFile(weight_string,',',wih);//
	bool tag2 = readFile(bias_string,',',bh);//读文件	
	if(tag1 == false||false == tag2)
		return false;//读取权重文件出错
	vector<vector<weightType>> weight_data;
	vector<vector<biasType>> bias_data;
	transToNumVector(weight_string,weight_data);//将string类型转为输入神经单元类型
	transToNumVector(bias_string,bias_data);//将string类型转为输入神经单元类型
	for(int i=0;i<NUM_H;i++){//初始化隐藏层
		bias_h[i]=bias_data[0][i];
		for(int j=0;j<NUM_I;j++){
			weight_ih[j][i] = weight_data[j][i];
		}
	}

	/////////////////////隐藏-输出层/////////////////////
	tag1 = readFile(weight_string,',',who);//
	tag2 = readFile(bias_string,',',bo);//读文件	
	if(tag1 == false||false == tag2)
		return false;//读取权重文件出错	
	transToNumVector(weight_string,weight_data);//将string类型转为输入神经单元类型
	transToNumVector(bias_string,bias_data);//将string类型转为输入神经单元类型
	for(int i=0;i<NUM_O;i++){//初始化输出层
		bias_o[i]=bias_data[0][i];
		for(int j=0;j<NUM_H;j++){
			weight_ho[j][i] = weight_data[j][i];
		}
	}
	return true;//正确读出之前保存的权重文件
	
}
/***将各组交替合并
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
	fileWriter.open(fileName,ios::out);//以方式打开
	if(!fileWriter.good()){//读文件异常
		cout<<fileName<<" 写文件异常！"<<endl;
		return;	
	}
	fileWriter<<"total：\t"<< accuracy<<endl;
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