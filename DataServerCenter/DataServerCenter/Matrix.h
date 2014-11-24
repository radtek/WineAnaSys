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



	
	bool SetItem(int row,int column,const double &data); //设置
	bool SetColumn(int column,CMatrix &B); //设置
	bool SetRow(int row,CMatrix &B);

	//添加行
	//bool AddRow(const CMatrix &matrix); 
	bool AddRows(const CMatrix &matrix);


	CMatrix Transposition() const;


	CMatrix GetBlock(int left,int top,int right,int bottom) const;
	CMatrix GetRow(int row) const;
	CMatrix GetRows(int beginrow,int endrow) const;
	CMatrix GetColumn(int column) const;
	CMatrix GetDistinct() const;

	//数学计算
	bool Inv(void);  //求逆
	double Square() const; 
	double Norm() const; //2范数
	CMatrix Mean(const int &dim) const;
	CMatrix cumsum(int type) const;

	void Smooth(const int &N); //对矩阵每一行进行平滑处理
	void Wden(const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname);   //对矩阵每一行进行小波降噪处理
	void MSC(CMatrix &ref,CMatrix &beta,CMatrix &alpha,bool mc);//对矩阵做多元散射校正（MSC）ref 为参考 beta斜率 alpha偏置，mc为是否进行中心化
	void Derivative(const int &N); //对矩阵每一行做N阶导
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
	

	bool PCA(CMatrix &signals,CMatrix &PC,CMatrix &V) const;    //signals主成分 PC特征向量 V特征值
	
	bool Matrix2CString(CString &cstr,const CString &Sep) const;

	const double operator()(int row,int column) const; 
	const double operator()(CMatrixIndex MatrixIndex) const;
	bool operator==(const CMatrix& A);

public:
	int m_row;
	int m_column;
	double *m_Matrix;

};

CMatrix   operator+(const CMatrix &A,const CMatrix &B);//重载矩阵加法 
CMatrix   operator-(const CMatrix &A,const CMatrix &B);   //矩阵减法 

CMatrix   operator*(const CMatrix &A,const CMatrix &B);   //矩阵乘法 
CMatrix   operator*(const CMatrix &A,const double &B);   //矩阵乘法 
CMatrix   operator*(const double &B,const CMatrix &A);  

CMatrix   operator/(const CMatrix &A,const double &B);   //矩阵除法 

CMatrix   operator/(const CMatrix &A,const CMatrix &B);   //矩阵除法 

CMatrix Abs(const CMatrix &A);

CMatrix Sort(const CMatrix &A,const int &dim,const CString Type);
void QuickSort(double *a,int low,int high);

double median(const CMatrix &A);

#endif