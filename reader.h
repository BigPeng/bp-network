#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;
bool readFile(vector<vector<string>> &data,char tag,string filePath);
bool readAsVecLine(vector<string> &data,char tag,string filePath);
vector<string> split(string inputString,char tag);
template <typename T>
void transToNum(string inputString,T &result);
void saveFile(string filePath,vector<string> &result);
void randInsert(vector<string> &result,vector<vector<string>> &vectors);
void interInsert(vector<string> &result,vector<vector<string>> &vectors);
template <typename T>
void transToNumVector(vector<vector<string> > data,vector<vector<T> > &numResult);
char * getType(int x);
char * getType(double x);
char * getType(float x);