#pragma once
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "DataPoint.h"
#include "Matrix.h"
using namespace std;

//聚类分析类型
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;    //数据集合
	unsigned int dimNum;           //维度
	size_t TypeNum;                //聚类类别
public:
	ClusterAnalysis(){};           //默认构造函数
	ClusterAnalysis(const ClusterAnalysis &c)
	{
		*this = c;
	}
	int GetPointID(int index);     //获取第index个样本的ID
	inline int GetTypeNum() {return TypeNum;};
	void ShowResult();

// 进行DBSCAN的成员
public:   
	bool Init(const CMatrix  &A, double radius, int minPTs);    //初始化操作
    bool DoDBSCANRecursive();            //DBSCAN递归算法
private: 
    double radius;                    //半径
    unsigned int dataNum;            //数据数量
    unsigned int minPTs;            //邻域最小数据个数
    double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //距离函数

	double GetNearestDistance(DataPoint& dp1, vector<DataPoint>& datasetpt);  //获取较近的距离

    void SetArrivalPoints(DataPoint& dp);                                //设置数据点的领域点列表
    void KeyPointCluster( unsigned long i, unsigned long clusterId );    //对数据点领域内的点执行聚类操作
//进行K-means的成员
public:
	typedef vector<int> ClusterIDVector;
	bool Init(const CMatrix  &Dataset,const int &TypeNum,int MaxIter = 100);    //初始化操作
	bool DoKmeans();
private:
	vector<DataPoint> m_MeanSets; //中心点集合
	int m_MaxIter;
	ClusterIDVector m_OldClusterIDVector;                 //之前聚类结果,用于判断迭代是否终止
	bool KmeansUpdateID();  //根据新的中心点更新聚类结果
	void KmeansUpdateCenter(); //根据新的聚类结果更新中心点
public:

};