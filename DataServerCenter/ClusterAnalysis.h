#pragma once
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "DataPoint.h"
#include "Matrix.h"
using namespace std;

//�����������
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;    //���ݼ���
	unsigned int dimNum;           //ά��
	size_t TypeNum;                //�������
public:
	ClusterAnalysis(){};                    //Ĭ�Ϲ��캯��
	int GetPointID(int index);//��ȡ��index��������ID
	inline int GetTypeNum() {return TypeNum;};
	void ShowResult();

// ����DBSCAN�ĳ�Ա
public:   
   // bool Init(char* fileName, double radius, int minPTs);    //��ʼ������
	bool Init(const CMatrix  &A, double radius, int minPTs);    //��ʼ������
    bool DoDBSCANRecursive();            //DBSCAN�ݹ��㷨
   // bool WriteToFile(char* fileName);    //��������д���ļ�
private: 
    double radius;                    //�뾶
    unsigned int dataNum;            //��������
    unsigned int minPTs;            //������С���ݸ���
    double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //���뺯��

	double GetNearestDistance(DataPoint& dp1, vector<DataPoint>& datasetpt);  //��ȡ�Ͻ��ľ���

    void SetArrivalPoints(DataPoint& dp);                                //�������ݵ��������б�
    void KeyPointCluster( unsigned long i, unsigned long clusterId );    //�����ݵ������ڵĵ�ִ�о������
//����K-means�ĳ�Ա
public:
	typedef vector<int> ClusterIDVector;
	bool Init(const CMatrix  &Dataset,const int &TypeNum,int MaxIter = 100);    //��ʼ������
	bool DoKmeans();
private:
	vector<DataPoint> m_MeanSets; //���ĵ㼯��
	int m_MaxIter;
	ClusterIDVector m_OldClusterIDVector;                 //֮ǰ������,�����жϵ����Ƿ���ֹ
	bool KmeansUpdateID();  //�����µ����ĵ���¾�����
	void KmeansUpdateCenter(); //�����µľ������������ĵ�
};

