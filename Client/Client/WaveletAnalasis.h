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
/* @brief ��Ե����
 * @param typeId �������ݵ����ͣ�1D or 2D
 * @param modeId ���ط�ʽ���Գơ�����
 * @param in ��������
 * @param filterLen С�����˲�������
 * @param out ���ؽ������
 * @return ���ؽ�����鳤��
 */
int SignalExtension(int typeId,int modeId, CMatrix in,int filterLen,CMatrix &out);

/**
 * @brief �ϲ���  �����0
 * @param data ��������
 * @param result ���ؽ������
 * @return ���ؽ�����鳤��
 */
int Upsampling(CMatrix data,CMatrix &result);
/**
 * @brief �²���  �������
 * @param CMatrix data ��������
 * @param result ���ؽ��
 * @return ���ؽ�����鳤��
 */
int Downsampling(CMatrix data,CMatrix &result);
/**
 * @brief �������
 * @param shapeId ����������ʽ
 * @param CMatrix inSignal, // �����źż��䳤��
 * @param CMatrix inFilter, // �����˲������䳤��
 * @param CMatrix outConv, convLen   // ������������䳤��
 * @return
 */
void Conv1(int shapeId,CMatrix inSignal,CMatrix inFilter,CMatrix &outConv,int &convLen);  
//void Conv1(int shapeId,CMatrix inSignal,CMatrix inFilter,CMatrix &outConv,size_t convLen);   
/**
 * @brief С���任֮�ֽ�
 * @param sourceData Դ����
 * @param db �˲���
 * @param cA �ֽ��Ľ��Ʋ�������-��Ƶ����
 * @param cD �ֽ���ϸ�ڲ�������-��Ƶ����
 * @return �����򷵻طֽ�����е����ݳ��ȣ������򷵻�-1
 */
int dwt(CMatrix sourceData,const Filter &m_db, CMatrix &cA, CMatrix &cD);
/**
 * @brief С���任֮�ع�
 * @param cA �ֽ��Ľ��Ʋ�������-��Ƶ����
 * @param cD �ֽ���ϸ�ڲ�������-��Ƶ����
 * @param reclen �����ع����ԭʼ���ݳ���
 * @param db �˲���
 * @param recData �ع������������
 * @return �����򷵻��ع����ݳ��ȣ������򷵻�-1
 */
int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData);
int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData,size_t reclen);
/*
С�����ֽ�
sourceDataԴ����
Level �ֽ����
wname С������
Coef С��ϵ������ϸ�ںͽ���ϵ��
L ����ָʾCoef�е�ϸ�ںͽ���ϵ��
*   C      = [app. coef.(Level)|det. coef.(Level)|... |det. coef.(1)]
*   L(1)   = length of app. coef.(Level)
*   L(i)   = length of det. coef.(Level-i+2) for i = 2,...,Level+1
*   L(Level+2) = length(sourceData).
*/
int wavedec(CMatrix sourceData,size_t Level,const CString wname,CMatrix &Coef,CMatrix &L);

/*С���ع�
ReconType �ع����ͣ���a����ʾ�ع����Ʒ�������d����ʾ�ع�ϸ�ڷ���
Coef wavedec�õ���ϵ��
L wavedec�õ�������ָʾCoef�е�ϸ�ںͽ���ϵ��
wname С������
Level ���ع���Ӧ�Ĳ���
ReconstructData �ع�������
*/
int wrcoef(const CString ReconType,CMatrix &Coef,CMatrix &L,const CString wname,size_t Level ,CMatrix &ReconstructData);

int waverec(CMatrix &Coef,CMatrix &L,const CString wname,CMatrix &RecData);
//��ȡ��Level��Ľ���ϵ��
int appcoef(CMatrix &Coef,CMatrix &L,const CString wname,size_t Level,CMatrix &A);
//��ȡ��Level���ϸ��ϵ��
int detcoef(const CMatrix &Coef,const CMatrix &L,const size_t &Level,CMatrix &D);
//С������
//��ֵ��ȡ����
//SignalData �ź�����
//tptr ��ֵ���㷽��rigrsure heursure sqtwolog minimaxi 
//���ؼ��������ֵ
double thselect(const CMatrix &SignalData,const CString &tptr);

//��С��ϵ��������ֵ����
CMatrix wthresh(const CMatrix &x,const CString &sorh,const double t);

//��Level��ϸ�ڷ����������й���
double wnoisest(const CMatrix &Coef,const CMatrix &L,const int Level);
//
CMatrix wden(const CMatrix &X,const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname);
#endif