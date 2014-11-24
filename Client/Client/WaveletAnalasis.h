#ifndef WAVELETANALASIS_H
#define WAVELETANALASIS_H
#include <iostream>
#include "Matrix.h"

class Filter
{
public:
	//Filter(const CString WaveletName,const CString decrec);

	bool Init(const CString WaveletName,const CString decrec);

public:

	CMatrix lowFilter;
	CMatrix highFilter;
};
/* @brief 边缘延拓
 * @param typeId 延拓数据的类型，1D or 2D
 * @param modeId 延拓方式：对称、周期
 * @param in 输入数据
 * @param filterLen 小波基滤波器长度
 * @param out 返回结果数组
 * @return 返回结果数组长度
 */
int SignalExtension(int typeId,int modeId, CMatrix in,int filterLen,CMatrix &out);

/**
 * @brief 上采样  隔点插0
 * @param data 输入数据
 * @param result 返回结果数组
 * @return 返回结果数组长度
 */
int Upsampling(CMatrix data,CMatrix &result);
/**
 * @brief 下采样  隔点采样
 * @param CMatrix data 输入数据
 * @param result 返回结果
 * @return 返回结果数组长度
 */
int Downsampling(CMatrix data,CMatrix &result);
/**
 * @brief 卷积运算
 * @param shapeId 卷积结果处理方式
 * @param CMatrix inSignal, // 输入信号及其长度
 * @param CMatrix inFilter, // 输入滤波器及其长度
 * @param CMatrix outConv, convLen   // 输出卷积结果及其长度
 * @return
 */
void Conv1(int shapeId,CMatrix inSignal,CMatrix inFilter,CMatrix &outConv,int &convLen);  
//void Conv1(int shapeId,CMatrix inSignal,CMatrix inFilter,CMatrix &outConv,size_t convLen);   
/**
 * @brief 小波变换之分解
 * @param sourceData 源数据
 * @param db 滤波器
 * @param cA 分解后的近似部分序列-低频部分
 * @param cD 分解后的细节部分序列-高频部分
 * @return 正常则返回分解后序列的数据长度，错误则返回-1
 */
int dwt(CMatrix sourceData,const Filter &m_db, CMatrix &cA, CMatrix &cD);
/**
 * @brief 小波变换之重构
 * @param cA 分解后的近似部分序列-低频部分
 * @param cD 分解后的细节部分序列-高频部分
 * @param reclen 输入重构后的原始数据长度
 * @param db 滤波器
 * @param recData 重构后输出的数据
 * @return 正常则返回重构数据长度，错误则返回-1
 */
int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData);
int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData,size_t reclen);
/*
小波多层分解
sourceData源数据
Level 分解层数
wname 小波名称
Coef 小波系数包括细节和近似系数
L 用来指示Coef中的细节和近似系数
*   C      = [app. coef.(Level)|det. coef.(Level)|... |det. coef.(1)]
*   L(1)   = length of app. coef.(Level)
*   L(i)   = length of det. coef.(Level-i+2) for i = 2,...,Level+1
*   L(Level+2) = length(sourceData).
*/
int wavedec(CMatrix sourceData,size_t Level,const CString wname,CMatrix &Coef,CMatrix &L);

/*小波重构
ReconType 重构类型：“a”表示重构近似分量，“d”表示重构细节分量
Coef wavedec得到的系数
L wavedec得到的用来指示Coef中的细节和近似系数
wname 小波名称
Level 与重构相应的层数
ReconstructData 重构的数据
*/
int wrcoef(const CString ReconType,CMatrix &Coef,CMatrix &L,const CString wname,size_t Level ,CMatrix &ReconstructData);

int waverec(CMatrix &Coef,CMatrix &L,const CString wname,CMatrix &RecData);
//获取第Level层的近似系数
int appcoef(CMatrix &Coef,CMatrix &L,const CString wname,size_t Level,CMatrix &A);
//获取第Level层的细节系数
int detcoef(const CMatrix &Coef,const CMatrix &L,const size_t &Level,CMatrix &D);
//小波降噪
//阈值获取函数
//SignalData 信号数据
//tptr 阈值计算方法rigrsure heursure sqtwolog minimaxi 
//返回计算出的阈值
double thselect(const CMatrix &SignalData,const CString &tptr);

//对小波系数进行阈值处理
CMatrix wthresh(const CMatrix &x,const CString &sorh,const double t);

//对Level层细节分量噪声进行估计
double wnoisest(const CMatrix &Coef,const CMatrix &L,const int Level);
//
CMatrix wden(const CMatrix &X,const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname);
#endif