#include "reader.h"

/*
*��ȡ����·��ΪfilePath���ļ����ļ���ÿ���е���ֵ��tag�ַ��ֿ�
*����vector<vector<string>>��ʽ������
*/
bool readFile(vector<vector<string>> &data,char tag,string filePath){
	data.clear();//�����
	cout<<"��ʼ���ļ�:"<<filePath;	
	ifstream fileReader;	
	fileReader.open(filePath,ios::in);//��ֻ����ʽ��
	if(!fileReader.good()){//���ļ��쳣
		cout<<filePath<<" ���ļ��쳣��"<<endl;
		return false;
	}	
	while(!fileReader.eof()){//δ���ļ�ĩβ    
		string linestring;
		getline(fileReader,linestring);//��ȡһ��
		vector<string> line = split(linestring,tag);//�ָ�ÿ��,������line������    
		data.push_back(line);
	}
	cout<<" �ļ����꣡"<<endl;
	return true;    
}
/*
*���ַ���inputString��tag�ַ��ָ�
*����vector<string>��ʽ��һά����
*/
vector<string> split(string inputString,char tag){
	int length = inputString.length();
	int start=0;//��ֵ��ʼ�±�
	vector<string> line;
	for(int i=0;i<length;i++){
		if(inputString[i] == tag){//����tag�ַ�
			string sub = inputString.substr(start,i-start);    //ȡinputString[start]-inputString[i]�Ӵ�
			line.push_back(sub);//ѹ��������
			start =  i+1;            
		}else if(i==length-1){
			string sub = inputString.substr(start,i-start+1);//���һ���ַ�û�б�㣬�赥������
			line.push_back(sub);//ѹ��������
		}
	}
	return line;
}

char * getType(int x){//������������    
	return "int";    
}
char * getType(double x){//����double����    
	return "double";
}
char * getType(float x){//����float����    
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
		for(;iter != vectors.end();iter++){//��ÿ���г�ȡһ��		
			if((*iter).size() == 0){
				vectors.erase(iter);//ɾ����Ԫ��
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
	while(count < vectors.size()){//ȫΪ��Ԫ��ʱ����ѭ��
		count = 0;
		cout<<size<<endl;
		vector<vector<string>>::iterator iter = vectors.begin();
		for(;iter != vectors.end();iter++){//��ÿ���г�ȡһ��			
			if((*iter).size() == 0){
				count++;
				continue;//������
			}					
			int index = rand()%size;
			result.insert(result.begin()+index,(*iter).back());
			size++;
			(*iter).pop_back();	
		}
	}
}
void saveFile(string filePath,vector<string> &result){
	//cout<<"��ʼд�ļ�:"<<endl;
	//cout<<filePath<<endl;
	ofstream fileWriter;
	fileWriter.open(filePath,ios::out);//�Է�ʽ��
	if(!fileWriter.good()){//���ļ��쳣
		cout<<filePath<<" д�ļ��쳣��"<<endl;
		return;		
	}
	vector<string>::iterator iter = result.begin();
	for(;iter != result.end();iter++){
		string cur_vec = *iter;		
		fileWriter<<*iter<<endl;
	}
	fileWriter.close();
	fileWriter.clear();
	cout<<"�ļ�д�꣡"<<endl;	
}
/*********���ж��ļ���������vector<string>��**********/
bool readAsVecLine(vector<string> &data,char tag,string filePath){
	data.clear();//�����
	cout<<"��ʼ���ļ�:"<<endl;
	cout<<filePath<<endl;
	ifstream fileReader;	
	fileReader.open(filePath,ios::in);//��ֻ����ʽ��
	if(!fileReader.good()){//���ļ��쳣
		cout<<filePath<<" ���ļ��쳣��"<<endl;
		return false;
	}	
	while(!fileReader.eof()){//δ���ļ�ĩβ    
		string linestring;
		getline(fileReader,linestring);//��ȡһ��	
		data.push_back(linestring);
	}
	cout<<"�ļ����꣡"<<endl;
	return true;    
}
