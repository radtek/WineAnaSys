#pragma once
#include <vector>
#include "Matrix.h"
using namespace std;

//const int DIME_NUM=2;        //����ά��Ϊ2��ȫ�ֳ���

//���ݵ�����
class DataPoint
{
private:
    unsigned long dpID;                //���ݵ�ID
	CMatrix pointData;                 //����
	long clusterId;                    //��������ID
public:
	DataPoint();                       //Ĭ�Ϲ��캯��
    unsigned long GetDpId();                //GetDpId����
    void SetDpId(unsigned long dpID);        //SetDpId����
    CMatrix GetPointData() const;                    //GetPointData����   
	void SetPointData(CMatrix dimension);    //SetPointData����
	size_t GetDimension(){return pointData.m_column;};
    long GetClusterId();                    //GetClusterId����
    void SetClusterId(long classId);        //SetClusterId����

	DataPoint& operator=(const DataPoint& A);


//////////////����DBSCAN�����Ա//////////////////
private:
    bool isKey;                        //�Ƿ���Ķ���
    bool visited;                    //�Ƿ��ѷ���
    vector<unsigned long> arrivalPoints;    //�������ݵ�id�б�
public:
	//DataPoint(unsigned long dpID,double* dimension , bool isKey);    //���캯��
	DataPoint(const unsigned long &dpID,const CMatrix &data ,const bool &isKey);    //���캯��
    bool IsKey();                            //GetIsKey����
    void SetKey(bool isKey);                //SetKey����
    bool isVisited();                        //GetIsVisited����
    void SetVisited(bool visited);            //SetIsVisited����
    vector<unsigned long>& GetArrivalPoints();    //GetArrivalPoints����

////////////����K-means�����Ա///////////////
private: // 


public: 
	DataPoint(const unsigned long &dpID,const CMatrix &data);

};

DataPoint operator+(const DataPoint &A,const DataPoint & B);

DataPoint operator/(const DataPoint &A,const double &B);