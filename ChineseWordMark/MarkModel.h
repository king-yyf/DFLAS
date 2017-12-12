#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "map"
#include "regex"
using namespace std;
class MarkModel
{
public:
	MarkModel();
	~MarkModel();
	map<string , int> getStateMap();
	map<string , int> getWordMap();
	//��ѵ����Ԥ�����õ����յ�һ������һ����ǩ���ļ�
	void filePreprocess(const char* inputfile, const char* outputfile);
	//inputfile : ����Ԥ����֮��Ĵ���ǩ�ļ�markfile.txt outputfile : ��ȡ֮���״̬�ļ�statefile.txt
	//ͬʱ��ȡ��statemap��wordmap���ݱ�
	void getMapData(const char* inputfile, const char* outputfile);
	void initialModel(); //��ʼ��ģ�Ͳ���
	void getIniPro(const char* inputfile);//����ʼ���ʾ���
	void getTranPro(const char* inputfile);// ���ת�Ƹ��ʾ���
	void getConPro(const char* inputfile);//���������ʾ���
	void saveMapData(const char* outputfile, map<string, int> MapData);
	void saveModelData(const char* outputfile);

private:
	map<string , int> statemap;	//���Ա�ǩ��map��
	map<string , int> wordmap;	//���ʵ�map���ݱ�
	int M;						//����״̬������ǩ����
	int N;						//�۲�״̬�������ʸ���
	double* Pi;					// M*1
	double** TranMatrix;		// M*M
	double** ConMatrix;			// M*N
	
};
