#include "StdAfx.h"
#include "ClusterAnalysis.h"
#include <fstream>
#include <iosfwd>
#include <math.h>

/*
函数：聚类初始化操作
说明：将数据文件名，半径，领域最小数据个数信息写入聚类算法类，读取文件，把数据信息读入写进算法类数据集合中
参数：
CMatrix dataset    //数据矩阵，每一行都是一个样本数据
double radius;    //半径
int minPTs;        //领域最小数据个数  
返回值： true;    */
bool ClusterAnalysis::Init(const CMatrix &Dataset, double radius, int minPTs)    //初始化操作
{
	dadaSets.clear();
    this->radius = radius;        //设置半径
    this->minPTs = minPTs;        //设置领域最小数据个数
	this->dimNum = Dataset.m_column;    //设置数据维度
	dataNum =Dataset.m_row;            //设置数据对象集合大小
	for(int i=0;i<dataNum;++i)
	{
		DataPoint tempDP;                //临时数据点对象
		tempDP.SetPointData(Dataset.GetRow(i+1));
        tempDP.SetDpId(i);                    //将数据点对象ID设置为i
        tempDP.SetVisited(false);            //数据点对象isVisited设置为false
        tempDP.SetClusterId(0);            //设置默认簇ID为0

		this->dadaSets.push_back(tempDP); 
	}	
    for(unsigned long i=0; i<dataNum;i++)
    {
        SetArrivalPoints(dadaSets[i]);            //计算数据点领域内对象
    }
	TypeNum = 0;
    return true;    //返回
}


/*
函数：将已经过聚类算法处理的数据集合写回文件
说明：将已经过聚类结果写回文件
参数：
char* fileName;    //要写入的文件名
返回值： true    */
/*bool ClusterAnalysis::WriteToFile(char* fileName )
{
    ofstream of1(fileName);                                //初始化文件输出流
    for(unsigned long i=0; i<dataNum;i++)                //对处理过的每个数据点写入文件
    {
        for(int d=0; d<DIME_NUM ; d++)                    //将维度信息写入文件
            of1<<dadaSets[i].GetPointData()[d]<<'\t';
        of1 << dadaSets[i].GetClusterId() <<endl;        //将所属簇ID写入文件
    }
    of1.close();    //关闭输出文件流
    return true;    //返回
}*/

/*
函数：设置数据点的领域点列表
说明：设置数据点的领域点列表
参数：
返回值： true;    */
void ClusterAnalysis::SetArrivalPoints(DataPoint& dp)
{
    for(unsigned long i=0; i<dataNum; i++)                //对每个数据点执行
    {
        double distance =GetDistance(dadaSets[i], dp);    //获取与特定点之间的距离
        if(distance <= radius && i!=dp.GetDpId())        //若距离小于半径，并且特定点的id与dp的id不同执行
            dp.GetArrivalPoints().push_back(i);            //将特定点id压力dp的领域列表中
    }
    if(dp.GetArrivalPoints().size() >= minPTs)            //若dp领域内数据点数据量> minPTs执行
    {
        dp.SetKey(true);    //将dp核心对象标志位设为true
        return;                //返回
    }
    dp.SetKey(false);    //若非核心对象，则将dp核心对象标志位设为false
}


/*
函数：执行聚类操作
说明：执行聚类操作
参数：
返回值： true;    */
bool ClusterAnalysis::DoDBSCANRecursive()
{
    unsigned long clusterId=1;                        //聚类id计数，初始化为1
    for(unsigned long i=0; i<dataNum;i++)            //对每一个数据点执行
    {
        DataPoint& dp=dadaSets[i];                    //取到第i个数据点对象
        if(!dp.isVisited() && dp.IsKey())            //若对象没被访问过，并且是核心对象执行
        {
            dp.SetClusterId(clusterId);                //设置该对象所属簇ID为clusterId
            dp.SetVisited(true);                    //设置该对象已被访问过
            KeyPointCluster(i,clusterId);            //对该对象领域内点进行聚类
            clusterId++;                            //clusterId自增1
        }
        //cout << "孤立点\T" << i << endl;
    }
	TypeNum = clusterId-1;
    //cout <<"共聚类" <<clusterId-1<<"个"<< endl;        //算法完成后，输出聚类个数
    return true;    //返回
}

/*
函数：对数据点领域内的点执行聚类操作
说明：采用递归的方法，深度优先聚类数据
参数：
unsigned long dpID;            //数据点id
unsigned long clusterId;    //数据点所属簇id
返回值： void;    */
void ClusterAnalysis::KeyPointCluster(unsigned long dpID, unsigned long clusterId )
{
    DataPoint& srcDp = dadaSets[dpID];        //获取数据点对象
    if(!srcDp.IsKey())    return;
    vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //获取对象领域内点ID列表
    for(unsigned long i=0; i<arrvalPoints.size(); i++)
    {
        DataPoint& desDp = dadaSets[arrvalPoints[i]];    //获取领域内点数据点
        if(!desDp.isVisited())                            //若该对象没有被访问过执行
        {
            //cout << "数据点\t"<< desDp.GetDpId()<<"聚类ID为\t" <<clusterId << endl;
            desDp.SetClusterId(clusterId);        //设置该对象所属簇的ID为clusterId，即将该对象吸入簇中
            desDp.SetVisited(true);                //设置该对象已被访问
            if(desDp.IsKey())                    //若该对象是核心对象
            {
                KeyPointCluster(desDp.GetDpId(),clusterId);    //递归地对该领域点数据的领域内的点执行聚类操作，采用深度优先方法
            }
        }
    }
}

//两数据点之间距离
/*
函数：获取两数据点之间距离
说明：获取两数据点之间的欧式距离
参数：
DataPoint& dp1;        //数据点1
DataPoint& dp2;        //数据点2
返回值： double;    //两点之间的距离        */
double ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance = ((dp1.GetPointData()-dp2.GetPointData())*((dp1.GetPointData()-dp2.GetPointData()).Transposition()))(1,1);
    return pow(distance,0.5);        //开方并返回距离
}
int ClusterAnalysis::GetPointID(int index)
{
	if((index>=0)&&index<dadaSets.size())
	{
		return dadaSets[index].GetClusterId();
	}
	else
		return -1;
}//获取第index个样本的ID
void ClusterAnalysis::ShowResult()
{
	for(int i=0;i<dataNum;i++)
		std::cout<<dadaSets[i].GetClusterId()<<std::endl;
}
//
bool ClusterAnalysis::Init(const CMatrix  &Dataset,const int &TypeNum,int MaxIter /* = 100*/)    //初始化操作
{
	//先进行
	this->dimNum = Dataset.m_column;    //设置数据维度
	dataNum =Dataset.m_row;             //设置数据对象集合大小
	this->TypeNum = TypeNum;            //设置簇个数
	m_MaxIter = MaxIter;                //最大迭代次数
	for(int i=0;i<dataNum;++i)
	{
		DataPoint tempDP;               //临时数据点对象
		tempDP.SetPointData(Dataset.GetRow(i+1));
        tempDP.SetDpId(i);              //将数据点对象ID设置为i
        tempDP.SetVisited(false);       //数据点对象isVisited设置为false
        tempDP.SetClusterId(0);         //设置默认簇ID为0
		this->dadaSets.push_back(tempDP); 
	}
	//初始化中心，采用的策略是选择彼此距离尽可能远的TypeNum个点
	//首先将第一个点作为中心
	m_MeanSets.push_back(dadaSets[0]);
	CMatrix dist(1,dataNum);

	for(int j =1;j<TypeNum;++j)
	{
		//获取所有数据点中距离已添加的中心点中较近的点的距离
		for(int k =0;k<dataNum;++k)
		{
			dist.SetItem(1,k+1,GetNearestDistance(dadaSets[k],m_MeanSets));
		}
		CMatrixIndex maxIndex;
		dist.FindMax(maxIndex);
		m_MeanSets.push_back(dadaSets[maxIndex.column - 1]);
	}
	//初始化聚类结果
	KmeansUpdateID();
	return true;
}

double ClusterAnalysis::GetNearestDistance(DataPoint& dp1, vector<DataPoint>& datasetpt)  //获取较近的距离
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
    return pow(distance,0.5);        //开方并返回距离
}
bool ClusterAnalysis::DoKmeans()
{
	int itercount = 0;
	while(itercount<m_MaxIter)
	{
		KmeansUpdateCenter();
		if(false == KmeansUpdateID())//如果前后聚类结果一致，则终止迭代
			break;
		++itercount;
	}
	return true;

}
bool ClusterAnalysis::KmeansUpdateID()  //根据新的中心点更新聚类结果
{
	vector<int> NewClusterIDVector;
	for(int i = 0;i<dadaSets.size();++i)
	{
		//获取每个点的聚类ID
		double minDist = GetDistance(dadaSets[i],m_MeanSets[0]);
		int PtId = 1;

		for(int j = 1;j<TypeNum;++j)
		{
			double TmpDist = GetDistance(dadaSets[i],m_MeanSets[j]);
			if(minDist>TmpDist) //更新聚类ID
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
		return false; //如果前后聚类结果一致
	}
	else
	{
		m_OldClusterIDVector = NewClusterIDVector;
		return true; //如果前后聚类结果不一致
	}


}
void ClusterAnalysis::KmeansUpdateCenter() //根据新的聚类结果更新中心点
{

	int CountEachCluster = 0;
	vector<int> CountEachClusterVector(TypeNum,0);          //统计每一类的个数
	//vector<DataPoint> SumDataEachClusterVector(TypeNum,0);  //计算每一类的点的求和值
	DataPoint tmpDataPoint;
	tmpDataPoint.SetPointData(CMatrix(1,dimNum,0));
	m_MeanSets = vector<DataPoint>(TypeNum,tmpDataPoint);


	for(int i =0;i<dadaSets.size();++i)
	{
		++CountEachClusterVector[dadaSets[i].GetClusterId()-1];
		m_MeanSets[dadaSets[i].GetClusterId()-1] = m_MeanSets[dadaSets[i].GetClusterId()-1] + dadaSets[i];
	}
	//计算中心点
	for(int k =0;k<TypeNum;++k)
	{
		if(CountEachClusterVector[k]!=0)
			m_MeanSets[k] = m_MeanSets[k]/CountEachClusterVector[k];
	}
}