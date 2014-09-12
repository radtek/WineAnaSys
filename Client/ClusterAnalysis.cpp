#include "StdAfx.h"
#include "ClusterAnalysis.h"
#include <fstream>
#include <iosfwd>
#include <math.h>

/*
�����������ʼ������
˵�����������ļ������뾶��������С���ݸ�����Ϣд������㷨�࣬��ȡ�ļ�����������Ϣ����д���㷨�����ݼ�����
������
CMatrix dataset    //���ݾ���ÿһ�ж���һ����������
double radius;    //�뾶
int minPTs;        //������С���ݸ���  
����ֵ�� true;    */
bool ClusterAnalysis::Init(const CMatrix &Dataset, double radius, int minPTs)    //��ʼ������
{
    this->radius = radius;        //���ð뾶
    this->minPTs = minPTs;        //����������С���ݸ���
	this->dimNum = Dataset.m_column;    //��������ά��
	dataNum =Dataset.m_row;            //�������ݶ��󼯺ϴ�С
	for(int i=0;i<dataNum;++i)
	{
		DataPoint tempDP;                //��ʱ���ݵ����
		tempDP.SetPointData(Dataset.GetRow(i+1));
        tempDP.SetDpId(i);                    //�����ݵ����ID����Ϊi
        tempDP.SetVisited(false);            //���ݵ����isVisited����Ϊfalse
        tempDP.SetClusterId(0);            //����Ĭ�ϴ�IDΪ0

		this->dadaSets.push_back(tempDP); 
	}	
    for(unsigned long i=0; i<dataNum;i++)
    {
        SetArrivalPoints(dadaSets[i]);            //�������ݵ������ڶ���
    }
	TypeNum = 0;
    return true;    //����
}


/*
���������Ѿ��������㷨��������ݼ���д���ļ�
˵�������Ѿ���������д���ļ�
������
char* fileName;    //Ҫд����ļ���
����ֵ�� true    */
/*bool ClusterAnalysis::WriteToFile(char* fileName )
{
    ofstream of1(fileName);                                //��ʼ���ļ������
    for(unsigned long i=0; i<dataNum;i++)                //�Դ������ÿ�����ݵ�д���ļ�
    {
        for(int d=0; d<DIME_NUM ; d++)                    //��ά����Ϣд���ļ�
            of1<<dadaSets[i].GetPointData()[d]<<'\t';
        of1 << dadaSets[i].GetClusterId() <<endl;        //��������IDд���ļ�
    }
    of1.close();    //�ر�����ļ���
    return true;    //����
}*/

/*
�������������ݵ��������б�
˵�����������ݵ��������б�
������
����ֵ�� true;    */
void ClusterAnalysis::SetArrivalPoints(DataPoint& dp)
{
    for(unsigned long i=0; i<dataNum; i++)                //��ÿ�����ݵ�ִ��
    {
        double distance =GetDistance(dadaSets[i], dp);    //��ȡ���ض���֮��ľ���
        if(distance <= radius && i!=dp.GetDpId())        //������С�ڰ뾶�������ض����id��dp��id��ִͬ��
            dp.GetArrivalPoints().push_back(i);            //���ض���idѹ��dp�������б���
    }
    if(dp.GetArrivalPoints().size() >= minPTs)            //��dp���������ݵ�������> minPTsִ��
    {
        dp.SetKey(true);    //��dp���Ķ����־λ��Ϊtrue
        return;                //����
    }
    dp.SetKey(false);    //���Ǻ��Ķ�����dp���Ķ����־λ��Ϊfalse
}


/*
������ִ�о������
˵����ִ�о������
������
����ֵ�� true;    */
bool ClusterAnalysis::DoDBSCANRecursive()
{
    unsigned long clusterId=1;                        //����id��������ʼ��Ϊ1
    for(unsigned long i=0; i<dataNum;i++)            //��ÿһ�����ݵ�ִ��
    {
        DataPoint& dp=dadaSets[i];                    //ȡ����i�����ݵ����
        if(!dp.isVisited() && dp.IsKey())            //������û�����ʹ��������Ǻ��Ķ���ִ��
        {
            dp.SetClusterId(clusterId);                //���øö���������IDΪclusterId
            dp.SetVisited(true);                    //���øö����ѱ����ʹ�
            KeyPointCluster(i,clusterId);            //�Ըö��������ڵ���о���
            clusterId++;                            //clusterId����1
        }
        //cout << "������\T" << i << endl;
    }
	TypeNum = clusterId-1;
    //cout <<"������" <<clusterId-1<<"��"<< endl;        //�㷨��ɺ�����������
    return true;    //����
}

/*
�����������ݵ������ڵĵ�ִ�о������
˵�������õݹ�ķ�����������Ⱦ�������
������
unsigned long dpID;            //���ݵ�id
unsigned long clusterId;    //���ݵ�������id
����ֵ�� void;    */
void ClusterAnalysis::KeyPointCluster(unsigned long dpID, unsigned long clusterId )
{
    DataPoint& srcDp = dadaSets[dpID];        //��ȡ���ݵ����
    if(!srcDp.IsKey())    return;
    vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //��ȡ���������ڵ�ID�б�
    for(unsigned long i=0; i<arrvalPoints.size(); i++)
    {
        DataPoint& desDp = dadaSets[arrvalPoints[i]];    //��ȡ�����ڵ����ݵ�
        if(!desDp.isVisited())                            //���ö���û�б����ʹ�ִ��
        {
            //cout << "���ݵ�\t"<< desDp.GetDpId()<<"����IDΪ\t" <<clusterId << endl;
            desDp.SetClusterId(clusterId);        //���øö��������ص�IDΪclusterId�������ö����������
            desDp.SetVisited(true);                //���øö����ѱ�����
            if(desDp.IsKey())                    //���ö����Ǻ��Ķ���
            {
                KeyPointCluster(desDp.GetDpId(),clusterId);    //�ݹ�ضԸ���������ݵ������ڵĵ�ִ�о������������������ȷ���
            }
        }
    }
}

//�����ݵ�֮�����
/*
��������ȡ�����ݵ�֮�����
˵������ȡ�����ݵ�֮���ŷʽ����
������
DataPoint& dp1;        //���ݵ�1
DataPoint& dp2;        //���ݵ�2
����ֵ�� double;    //����֮��ľ���        */
double ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance = ((dp1.GetPointData()-dp2.GetPointData())*((dp1.GetPointData()-dp2.GetPointData()).Transposition()))(1,1);
    return pow(distance,0.5);        //���������ؾ���
}
int ClusterAnalysis::GetPointID(int index)
{
	if((index>=0)&&index<dadaSets.size())
	{
		return dadaSets[index].GetClusterId();
	}
	else
		return -1;
}//��ȡ��index��������ID
void ClusterAnalysis::ShowResult()
{
	for(int i=0;i<dataNum;i++)
		std::cout<<dadaSets[i].GetClusterId()<<std::endl;
}
//
bool ClusterAnalysis::Init(const CMatrix  &Dataset,const int &TypeNum,int MaxIter /* = 100*/)    //��ʼ������
{
	//�Ƚ���
	this->dimNum = Dataset.m_column;    //��������ά��
	dataNum =Dataset.m_row;             //�������ݶ��󼯺ϴ�С
	this->TypeNum = TypeNum;            //���ôظ���
	m_MaxIter = MaxIter;                //����������
	for(int i=0;i<dataNum;++i)
	{
		DataPoint tempDP;               //��ʱ���ݵ����
		tempDP.SetPointData(Dataset.GetRow(i+1));
        tempDP.SetDpId(i);              //�����ݵ����ID����Ϊi
        tempDP.SetVisited(false);       //���ݵ����isVisited����Ϊfalse
        tempDP.SetClusterId(0);         //����Ĭ�ϴ�IDΪ0
		this->dadaSets.push_back(tempDP); 
	}
	//��ʼ�����ģ����õĲ�����ѡ��˴˾��뾡����Զ��TypeNum����
	//���Ƚ���һ������Ϊ����
	m_MeanSets.push_back(dadaSets[0]);
	CMatrix dist(1,dataNum);

	for(int j =1;j<TypeNum;++j)
	{
		//��ȡ�������ݵ��о�������ӵ����ĵ��нϽ��ĵ�ľ���
		for(int k =0;k<dataNum;++k)
		{
			dist.SetItem(1,k+1,GetNearestDistance(dadaSets[k],m_MeanSets));
		}
		CMatrixIndex maxIndex;
		dist.FindMax(maxIndex);
		m_MeanSets.push_back(dadaSets[maxIndex.column - 1]);
	}
	//��ʼ��������
	KmeansUpdateID();
	return true;
}

double ClusterAnalysis::GetNearestDistance(DataPoint& dp1, vector<DataPoint>& datasetpt)  //��ȡ�Ͻ��ľ���
{
	DataPoint dp2 = datasetpt[0];
	double distance = ((dp1.GetPointData()-dp2.GetPointData())*((dp1.GetPointData()-dp2.GetPointData()).Transposition()))(1,1);
	for(int i = 1;i<datasetpt.size();++i)
	{
		dp2 = datasetpt[i];
		double tmpdistance = ((dp1.GetPointData()-dp2.GetPointData())*((dp1.GetPointData()-dp2.GetPointData()).Transposition()))(1,1);
		if(distance>tmpdistance)
		{
			distance = tmpdistance;
		}
	}
    return pow(distance,0.5);        //���������ؾ���
}
bool ClusterAnalysis::DoKmeans()
{
	int itercount = 0;
	while(itercount<m_MaxIter)
	{
		KmeansUpdateCenter();
		if(false == KmeansUpdateID())//���ǰ�������һ�£�����ֹ����
			break;
		++itercount;
	}
	return true;

}
bool ClusterAnalysis::KmeansUpdateID()  //�����µ����ĵ���¾�����
{
	vector<int> NewClusterIDVector;
	for(int i = 0;i<dadaSets.size();++i)
	{
		//��ȡÿ����ľ���ID
		double minDist = GetDistance(dadaSets[i],m_MeanSets[0]);
		int PtId = 1;

		for(int j = 1;j<TypeNum;++j)
		{
			double TmpDist = GetDistance(dadaSets[i],m_MeanSets[j]);
			if(minDist>TmpDist) //���¾���ID
			{
				minDist = TmpDist;
				PtId = j+1;
			}
		}
		dadaSets[i].SetClusterId(PtId);
		NewClusterIDVector.push_back(PtId);
	}
	if(NewClusterIDVector == m_OldClusterIDVector)
	{
		return false; //���ǰ�������һ��
	}
	else
	{
		m_OldClusterIDVector = NewClusterIDVector;
		return true; //���ǰ���������һ��
	}


}
void ClusterAnalysis::KmeansUpdateCenter() //�����µľ������������ĵ�
{

	int CountEachCluster = 0;
	vector<int> CountEachClusterVector(TypeNum,0);          //ͳ��ÿһ��ĸ���
	//vector<DataPoint> SumDataEachClusterVector(TypeNum,0);  //����ÿһ��ĵ�����ֵ
	DataPoint tmpDataPoint;
	tmpDataPoint.SetPointData(CMatrix(1,dimNum,0));
	m_MeanSets = vector<DataPoint>(TypeNum,tmpDataPoint);


	for(int i =0;i<dadaSets.size();++i)
	{
		++CountEachClusterVector[dadaSets[i].GetClusterId()-1];
		m_MeanSets[dadaSets[i].GetClusterId()-1] = m_MeanSets[dadaSets[i].GetClusterId()-1] + dadaSets[i];
	}
	//�������ĵ�
	for(int k =0;k<TypeNum;++k)
	{
		if(CountEachClusterVector[k]!=0)
			m_MeanSets[k] = m_MeanSets[k]/CountEachClusterVector[k];
	}
}