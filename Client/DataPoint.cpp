#include "StdAfx.h"
#include "DataPoint.h"
//Ĭ�Ϲ��캯��
DataPoint::DataPoint()
{
}

//���캯��
DataPoint::DataPoint(const unsigned long &dpID,const CMatrix &data ,const bool &isKey):isKey(isKey),dpID(dpID)    //���캯��
{
	pointData = data;
}
DataPoint::DataPoint(const unsigned long &dpID,const CMatrix &data):dpID(dpID) 
{
	pointData = data;
}
//����ά������
void DataPoint::SetPointData(CMatrix dimension)
{
	pointData = dimension;
}

//��ȡά������
CMatrix DataPoint::GetPointData()const
{
    return this->pointData;
}

//��ȡ�Ƿ�Ϊ���Ķ���
bool DataPoint::IsKey()
{
    return this->isKey;
}

//���ú��Ķ����־
void DataPoint::SetKey(bool isKey)
{
    this->isKey = isKey;
}

//��ȡDpId����
unsigned long DataPoint::GetDpId()
{
    return this->dpID;
}

//����DpId����
void DataPoint::SetDpId(unsigned long dpID)
{
    this->dpID = dpID;
}

//GetIsVisited����
bool DataPoint::isVisited()
{
    return this->visited;
}


//SetIsVisited����
void DataPoint::SetVisited( bool visited )
{
    this->visited = visited;
}

//GetClusterId����
long DataPoint::GetClusterId()
{
    return this->clusterId;
}

//GetClusterId����
void DataPoint::SetClusterId( long clusterId )
{
    this->clusterId = clusterId;
}

//GetArrivalPoints����
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