#pragma once
#ifndef MATRIX_H
#define MATRIX_H
#include <cstring>
#include <afx.h>
#include <math.h>
#include <cmath>

//#define _AFXDLL
double Max(double a,double b);
int sign(double x);
struct CMatrixIndex
{
	int row;
	int column;
};

class CMatrix
{
public:
	CMatrix();
	CMatrix(const CMatrix &A);
	CMatrix(int row,int column);
	CMatrix(int row,int column,const double &data);

	CMatrix(double** A,int row,int column);

	CMatrix(double *A,int row,int column);

	virtual ~CMatrix(void);
	CMatrix&  operator=(const CMatrix&A); 
	CMatrix&  operator+=(const CMatrix&A); 
	CMatrix&  operator-=(const CMatrix&A); 

	//operator double()const { return m_Matrix[0];}



	
	bool SetItem(int row,int column,const double &data); //����
	bool SetColumn(int column,CMatrix &B); //����
	bool SetRow(int row,CMatrix &B);

	//�����
	//bool AddRow(const CMatrix &matrix); 
	bool AddRows(const CMatrix &matrix);


	CMatrix Transposition() const;


	CMatrix GetBlock(int left,int top,int right,int bottom) const;
	CMatrix GetRow(int row) const;
	CMatrix GetRows(int beginrow,int endrow) const;
	CMatrix GetColumn(int column) const;
	CMatrix GetDistinct() const;

	//��ѧ����
	bool Inv(void);  //����
	double Square() const; 
	double Norm() const; //2����
	CMatrix Mean(const int &dim) const;
	CMatrix cumsum(int type) const;

	void Smooth(const int &N); //�Ծ���ÿһ�н���ƽ������
	void Wden(const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname);   //�Ծ���ÿһ�н���С�����봦��
	void MSC(CMatrix &ref,CMatrix &beta,CMatrix &alpha,bool mc);//�Ծ�������Ԫɢ��У����MSC��ref Ϊ�ο� betaб�� alphaƫ�ã�mcΪ�Ƿ�������Ļ�
	void Derivative(const int &N); //�Ծ���ÿһ����N�׵�
	CMatrix Diff() const;

	double Sum() const;
	double GetMean() const;
	bool Have(double a) const;
	int Count(double a) const;
	CMatrix covariance();


	//
	bool FindFirst(double a,CMatrixIndex &index) const;
	bool FindMax(CMatrixIndex &MatirxIndex) const;
	double GetMax() const;
	bool FindMin(CMatrixIndex &MatirxIndex) const;
	double GetMin() const;
	

	bool PCA(CMatrix &signals,CMatrix &PC,CMatrix &V) const;    //signals���ɷ� PC�������� V����ֵ
	
	bool Matrix2CString(CString &cstr,const CString &Sep) const;

	const double operator()(int row,int column) const; 
	const double operator()(CMatrixIndex MatrixIndex) const;


public:
	int m_row;
	int m_column;
	double *m_Matrix;

};

CMatrix   operator+(const CMatrix &A,const CMatrix &B);//���ؾ���ӷ� 
CMatrix   operator-(const CMatrix &A,const CMatrix &B);   //������� 

CMatrix   operator*(const CMatrix &A,const CMatrix &B);   //����˷� 
CMatrix   operator*(const CMatrix &A,const double &B);   //����˷� 
CMatrix   operator*(const double &B,const CMatrix &A);  

CMatrix   operator/(const CMatrix &A,const double &B);   //������� 

CMatrix   operator/(const CMatrix &A,const CMatrix &B);   //������� 

CMatrix Abs(const CMatrix &A);

CMatrix Sort(const CMatrix &A,const int &dim,const CString Type);
void QuickSort(double *a,int low,int high);

double median(const CMatrix &A);

#endif