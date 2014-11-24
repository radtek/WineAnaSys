#pragma once
#include <vector>
#include "Matrix.h"
using namespace std;

//const int DIME_NUM=2;        //数据维度为2，全局常量

//数据点类型
class DataPoint
{
private:
    unsigned long dpID;                //数据点ID
	CMatrix pointData;                 //数据
	long clusterId;                    //所属聚类ID
public:
	DataPoint();                       //默认构造函数
	//DataPoint(const DataPoint &d);
    unsigned long GetDpId();                //GetDpId方法
    void SetDpId(unsigned long dpID);        //SetDpId方法
    CMatrix GetPointData() const;                    //GetPointData方法   
	void SetPointData(CMatrix dimension);    //SetPointData方法
	size_t GetDimension(){return pointData.m_column;};
    long GetClusterId();                    //GetClusterId方法
    void SetClusterId(long classId);        //SetClusterId方法

	DataPoint& operator=(const DataPoint& A);
	bool operator==(const DataPoint &A)
	{
		return (isKey == A.visited)&&(visited == A.visited)&&(arrivalPoints == A.arrivalPoints)&&(dpID == A.dpID)&&(pointData == A.pointData)&&(clusterId == A.clusterId);
	}
	bool operator!=(const DataPoint &A)
	{
		return !(operator==(A));
	}

//////////////用于DBSCAN聚类成员//////////////////
private:
    bool isKey;                        //是否核心对象
    bool visited;                    //是否已访问
    vector<unsigned long> arrivalPoints;    //领域数据点id列表
public:
	//DataPoint(unsigned long dpID,double* dimension , bool isKey);    //构造函数
	DataPoint(const unsigned long &dpID,const CMatrix &data ,const bool &isKey);    //构造函数
    bool IsKey();                            //GetIsKey方法
    void SetKey(bool isKey);                //SetKey方法
    bool isVisited();                        //GetIsVisited方法
    void SetVisited(bool visited);            //SetIsVisited方法
    vector<unsigned long>& GetArrivalPoints();    //GetArrivalPoints方法

////////////用于K-means聚类成员///////////////
private: // 


public: 
	DataPoint(const unsigned long &dpID,const CMatrix &data);

};

DataPoint operator+(const DataPoint &A,const DataPoint & B);

DataPoint operator/(const DataPoint &A,const double &B);

