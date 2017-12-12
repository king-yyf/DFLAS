#include "iostream"
#include "fstream"
#include "string"
#include "map"
#include "vector"
#include "sstream"

using namespace std;
class WordMark
{
public:
	WordMark();
	~WordMark();
	int getNum();
	int* getT();
	int** getO();
	vector<string> getVStr();
	void loadWMapData(const char* inputfile);//��������map��
	void loadSMapData(const char* inputfile);//��������map��
	//���۲��ı���ÿһ�������ɹ۲�����
	void getOSequence(const char* inputfile, const char* outputfile);
	//�����й۲������Ƴ�����洢int* T, int** O
	void getOSdata(const char* inputfile);
	//��ÿһ���ַ���push��vector��
	void file2Vector(const char* inputfile);
	//����viterbi����֮������У��õ���ע���
	void getMarkResult(int* path, string& str);
private:
	map<string, int> wordmap;
	map<int, string> restatemap; //�����statemap�����ڽ������pathת��Ϊ���Ա��
	vector<string> vstr;
	int num;
	int* T;
	int** O;

};