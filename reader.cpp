#include "reader.h"

/*
*读取绝对路径为filePath的文件，文件中每行中的数值以tag字符分开
*返回vector<vector<string>>格式的数据
*/
bool readFile(vector<vector<string>> &data,char tag,string filePath){
	data.clear();//先清空
	cout<<"开始读文件:"<<filePath;	
	ifstream fileReader;	
	fileReader.open(filePath,ios::in);//以只读方式打开
	if(!fileReader.good()){//读文件异常
		cout<<filePath<<" 读文件异常！"<<endl;
		return false;
	}	
	while(!fileReader.eof()){//未到文件末尾    
		string linestring;
		getline(fileReader,linestring);//读取一行
		vector<string> line = split(linestring,tag);//分割每行,并放在line向量中    
		data.push_back(line);
	}
	cout<<" 文件读完！"<<endl;
	return true;    
}
/*
*对字符串inputString按tag字符分割
*返回vector<string>格式的一维向量
*/
vector<string> split(string inputString,char tag){
	int length = inputString.length();
	int start=0;//数值起始下标
	vector<string> line;
	for(int i=0;i<length;i++){
		if(inputString[i] == tag){//遇到tag字符
			string sub = inputString.substr(start,i-start);    //取inputString[start]-inputString[i]子串
			line.push_back(sub);//压入向量中
			start =  i+1;            
		}else if(i==length-1){
			string sub = inputString.substr(start,i-start+1);//最后一个字符没有标点，需单独处理
			line.push_back(sub);//压入向量中
		}
	}
	return line;
}

char * getType(int x){//返回整数类型    
	return "int";    
}
char * getType(double x){//返回double类型    
	return "double";
}
char * getType(float x){//返回float类型    
	return "float";
}

void interInsert(vector<string> &result,vector<vector<string>> &vectors){
	int min_vector = 0;
	int size = -1;
	for(int i=0;i<vectors.size();i++){
		int temp = vectors[i].size();
		if(size<temp){
			size = temp;
			min_vector = i;
		}
	}
	bool tag = false;
	while(tag == false){
		cout<<"interInsert"<<result.size()<<endl;
		vector<vector<string>>::iterator iter = vectors.begin();
		for(;iter != vectors.end();iter++){//从每个中抽取一个		
			if((*iter).size() == 0){
				vectors.erase(iter);//删除空元素
				tag = true;
				break;
			}			
			result.push_back((*iter).back());
			(*iter).pop_back();			
		}
	}
}
 
void randInsert(vector<string> &result,vector<vector<string>> &vectors){
	int size = result.size();
	int count = 0;
	while(count < vectors.size()){//全为空元素时跳出循环
		count = 0;
		cout<<size<<endl;
		vector<vector<string>>::iterator iter = vectors.begin();
		for(;iter != vectors.end();iter++){//从每个中抽取一个			
			if((*iter).size() == 0){
				count++;
				continue;//跳过空
			}					
			int index = rand()%size;
			result.insert(result.begin()+index,(*iter).back());
			size++;
			(*iter).pop_back();	
		}
	}
}
void saveFile(string filePath,vector<string> &result){
	//cout<<"开始写文件:"<<endl;
	//cout<<filePath<<endl;
	ofstream fileWriter;
	fileWriter.open(filePath,ios::out);//以方式打开
	if(!fileWriter.good()){//读文件异常
		cout<<filePath<<" 写文件异常！"<<endl;
		return;		
	}
	vector<string>::iterator iter = result.begin();
	for(;iter != result.end();iter++){
		string cur_vec = *iter;		
		fileWriter<<*iter<<endl;
	}
	fileWriter.close();
	fileWriter.clear();
	cout<<"文件写完！"<<endl;	
}
/*********按行读文件，保存在vector<string>中**********/
bool readAsVecLine(vector<string> &data,char tag,string filePath){
	data.clear();//先清空
	cout<<"开始读文件:"<<endl;
	cout<<filePath<<endl;
	ifstream fileReader;	
	fileReader.open(filePath,ios::in);//以只读方式打开
	if(!fileReader.good()){//读文件异常
		cout<<filePath<<" 读文件异常！"<<endl;
		return false;
	}	
	while(!fileReader.eof()){//未到文件末尾    
		string linestring;
		getline(fileReader,linestring);//读取一行	
		data.push_back(linestring);
	}
	cout<<"文件读完！"<<endl;
	return true;    
}
