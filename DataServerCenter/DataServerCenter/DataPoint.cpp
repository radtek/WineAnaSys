#include "StdAfx.h"
#include "DataPoint.h"
//默认构造函数
DataPoint::DataPoint()
{
}

//构造函数
DataPoint::DataPoint(const unsigned long &dpID,const CMatrix &data ,const bool &isKey):isKey(isKey),dpID(dpID)    //构造函数
{
	pointData = data;
}
DataPoint::DataPoint(const unsigned long &dpID,const CMatrix &data):dpID(dpID) 
{
	pointData = data;
}
//设置维度数据
void DataPoint::SetPointData(CMatrix dimension)
{
	pointData = dimension;
}

//获取维度数据
CMatrix DataPoint::GetPointData()const
{
    return this->pointData;
}

//获取是否为核心对象
bool DataPoint::IsKey()
{
    return this->isKey;
}

//设置核心对象标志
void DataPoint::SetKey(bool isKey)
{
    this->isKey = isKey;
}

//获取DpId方法
unsigned long DataPoint::GetDpId()
{
    return this->dpID;
}

//设置DpId方法
void DataPoint::SetDpId(unsigned long dpID)
{
    this->dpID = dpID;
}

//GetIsVisited方法
bool DataPoint::isVisited()
{
    return this->visited;
}


//SetIsVisited方法
void DataPoint::SetVisited( bool visited )
{
    this->visited = visited;
}

//GetClusterId方法
long DataPoint::GetClusterId()
{
    return this->clusterId;
}

//GetClusterId方法
void DataPoint::SetClusterId( long clusterId )
{
    this->clusterId = clusterId;
}

//GetArrivalPoints方法
vector<unsigned long>& DataPoint::GetArrivalPoints()
{
    return arrivalPoints;
}

DataPoint& DataPoint::operator=(const DataPoint& A)
{
	this->pointData = A.GetPointData();
	return *this;
}

DataPoint operator+(const DataPoint &A,const DataPoint & B)
{
	DataPoint tmp;
	tmp.SetPointData(A.GetPointData()+ B.GetPointData());
	return tmp;
}

DataPoint operator/(const DataPoint &A,const double &B)
{
	DataPoint tmp;
	tmp.SetPointData(A.GetPointData()/B);
	return tmp;
}