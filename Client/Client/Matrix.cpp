#include "StdAfx.h"
#include "WaveletAnalasis.h"
#include "Matrix.h"
#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <list>
#include <algorithm>
using namespace std;
using std::runtime_error;
CMatrix::CMatrix()
{
	m_row =0;
	m_column =0;
	m_Matrix = NULL;
}
CMatrix::CMatrix(const CMatrix &A)
{
	m_row=A.m_row;
	m_column=A.m_column;
	m_Matrix=(double*)malloc(sizeof(double)*m_row*m_column);
	for(int i=0;i<m_row*m_column;i++)
	{
		m_Matrix[i]=A.m_Matrix[i];
	}
}
CMatrix::CMatrix(int row,int column)
{
	m_row=row;
	m_column=column;
	m_Matrix=(double*)malloc(sizeof(double)*row*column);
}
CMatrix::CMatrix(int row,int column,const double &data)
{
	m_row=row;
	m_column=column;
	m_Matrix=(double*)malloc(sizeof(double)*row*column);
	for(int i=0;i<m_row*m_column;i++)
	{
		m_Matrix[i]=data;
	}
}
CMatrix::CMatrix(double** A,int row,int column)
{
	m_row=row;
	m_column=column;
	m_Matrix=(double*)malloc(sizeof(double)*row*column);
	for(int i=0;i<m_row;i++)
	{
		for(int j=0;j<m_column;j++)
		{
			m_Matrix[i*m_column+j]=A[i][j];
		}		
	}
}
CMatrix::CMatrix(double * A,int row,int column)
{
	m_row=row;
	m_column=column;
	m_Matrix=(double*)malloc(sizeof(double)*row*column);
	for(int i=0;i<m_row;i++)
	{
		for(int j=0;j<m_column;j++)
		{
			m_Matrix[i*m_column+j]=A[i*m_column+j];
		}		
	}
}

CMatrix::~CMatrix(void)
{
	if(m_Matrix)
	{
		free(m_Matrix);
		m_Matrix=NULL;
	}
}
	//CMatrix&  operator=(const CMatrix&); 
	//CMatrix&  operator+=(const CMatrix&); 
	//CMatrix&  operator-=(const CMatrix&); 
//矩阵索引
const double CMatrix::operator()(int row,int column) const
{
	return m_Matrix[(row-1)*m_column+(column-1)];
}
const double CMatrix::operator()(CMatrixIndex MatrixIndex) const
{
	return (*this)(MatrixIndex.row,MatrixIndex.column);
}

bool CMatrix::SetItem(int row,int column,const double &data)
{

	m_Matrix[(row-1)*m_column+(column-1)]=data;
	return 1;
}


CMatrix CMatrix::GetBlock(int left,int top,int right,int bottom) const
{
	if(left<=0||top<=0||right>m_column||bottom>m_row)
	{
		throw runtime_error("超出矩阵范围");
	}
	CMatrix C(bottom-top+1,right-left+1);
	for(int i=top;i<=bottom;i++)
	{
		for(int j=left;j<=right;j++)
		{
			C.SetItem(i-top+1,j-left+1,(*this)(i,j));
		}
	}
	return C;

}

CMatrix CMatrix::GetRow(int row) const
{
	if(row>m_row)
	{
		throw out_of_range("超出矩阵范围");
	}
	CMatrix C(1,m_column);
	for(int i=0;i<m_column;i++)
	{
		C.m_Matrix[i]=m_Matrix[(row-1)*m_column+i];
	}
	return C;
}
CMatrix CMatrix::GetRows(int beginrow,int endrow) const
{
	if(beginrow<1||endrow>m_row||beginrow>endrow)
	{
		throw out_of_range("超出矩阵范围");
	}
	CMatrix C(endrow-beginrow+1,m_column);
	for(int i=beginrow;i<=endrow;i++)
	{
		C.SetRow(i-beginrow+1,this->GetRow(i));
	}
	return C;
}


CMatrix CMatrix::GetColumn(int column) const
{
	if(column>m_column)
	{
		throw out_of_range("超出矩阵范围");
	}
	CMatrix C(m_row,1);
	for(int i=0;i<m_row;i++)
	{
		C.m_Matrix[i]=m_Matrix[i*m_column+column-1];
	}
	return C;
}
bool CMatrix::SetColumn(int column,CMatrix &B)
{
	if(m_row!=B.m_row)
	{
		throw out_of_range("矩阵不匹配");
	}
	for(int i=0;i<m_row;i++)
	{
		SetItem(i+1,column,B(i+1,1));
	}
	return 1;
}

bool CMatrix::SetRow(int row,CMatrix &B)
{
	if(m_column!=B.m_column)
	{
		throw out_of_range("矩阵不匹配");
	}
	for(int i=0;i<m_column;i++)
	{
		SetItem(row,i+1,B(1,i+1));
	}
	return 1;
}
bool CMatrix::AddRows(const CMatrix &matrix)
{
	if(this->m_Matrix == NULL)
	{
		*this = matrix;
		return true;
	}
	else
	{
		if(this->m_column != matrix.m_column)
		{
			return false;
		}	
		this->m_Matrix = (double*)realloc(m_Matrix,(m_row+matrix.m_row)*m_column*sizeof(double));
		int temp = this->m_row;
		this->m_row += matrix.m_row;
		for(int i=0;i<matrix.m_row;++i)
		{
			this->SetRow(temp+i+1,matrix.GetRow(i+1));
		}

		return true;
	}
}



CMatrix&  CMatrix::operator=(const CMatrix &A)
{	
	if(&A!=this)
	{
		if(m_Matrix)
		{
			free(m_Matrix);
		}
		m_row = A.m_row;
		m_column = A.m_column;
		m_Matrix = (double*)malloc(sizeof(double)*m_row*m_column);		

		for(int i=0;i<m_row*m_column;i++)
		{
			m_Matrix[i]=A.m_Matrix[i];
		}
	}
	return *this;
}
CMatrix&  CMatrix::operator+=(const CMatrix &A)
{
	if(m_row!=A.m_row||m_column!=A.m_column)
	{
		throw out_of_range("矩阵行列不匹配");
	}
	for(int i=0;i<m_row*m_column;i++)
	{
		m_Matrix[i]+=A.m_Matrix[i];
	}
	return *this;
}
CMatrix&  CMatrix::operator-=(const CMatrix &A)
{
	if(m_row!=A.m_row||m_column!=A.m_column)
	{
		throw out_of_range("矩阵行列不匹配");
	}
	for(int i=0;i<m_row*m_column;i++)
	{
		m_Matrix[i]-=A.m_Matrix[i];
	}
	return *this;
}


CMatrix   operator+(const CMatrix &A,const CMatrix &B)
{
	if(A.m_row!=B.m_row||A.m_column!=B.m_column)
	{
		throw out_of_range("矩阵行列不匹配");
	}
	CMatrix C(A);
	C+=B;
	return C;
}
CMatrix  operator-(const CMatrix &A,const CMatrix &B)
{
	if(A.m_row!=B.m_row||A.m_column!=B.m_column)
	{
		throw out_of_range("矩阵行列不匹配");
	}
	CMatrix C(A);
	C-=B;
	return C;
}

CMatrix   operator*(const CMatrix &A,const CMatrix &B)
{
	if(A.m_column!=B.m_row)
	{
		throw out_of_range("矩阵行列不匹配");
	}
	CMatrix C(A.m_row,B.m_column);
	for(int i=0;i<C.m_row;i++)
	{
		for(int j=0;j<C.m_column;j++)
		{
			C.m_Matrix[i*C.m_column+j]=0;
			for(int k=0;k<A.m_column;k++)
			{
				C.m_Matrix[i*C.m_column+j]+=A.m_Matrix[i*A.m_column+k]*B.m_Matrix[k*B.m_column+j];
			}
		}
	}
	return C;
}
CMatrix   operator*(const CMatrix &A,const double &B)
{
	
	if(1==B)
	{
		CMatrix *C=new CMatrix(A);
		return *C;
	}
	else if(0==B)
	{
		CMatrix *C=new CMatrix(A.m_row,A.m_column,B);
		return *C;
	}
	else
	{
		CMatrix C(A);

		for(int i=0;i<C.m_row*C.m_column;i++)
		{
			C.m_Matrix[i]*=B;   
		}
		return C;
	}	

}
CMatrix   operator*(const double &B,const CMatrix &A)
{	
	if(1==B)
	{
		CMatrix C(A);
		return C;
	}
	else if(0==B)
	{
		CMatrix C(A.m_row,A.m_column,B);
		return C;
	}
	else
	{
		CMatrix C(A);

		for(int i=0;i<C.m_row*C.m_column;i++)
		{
			C.m_Matrix[i]*=B;   
		}
		return C;
	}	
}
CMatrix  operator/(const CMatrix &A,const double &B)
{
	
	if(1==B)
	{
		CMatrix C(A);
		return C;
	}
	else if(0==B)
	{
		throw overflow_error("除数不能为零！");
	}
	else
	{
		CMatrix C(A);

		for(int i=0;i<C.m_row*C.m_column;i++)
		{
			C.m_Matrix[i]/=B;   
		}
		return C;
	}	
}
CMatrix   operator/(const CMatrix &A,const CMatrix &B)   //矩阵除法 ,目前只能做矩阵除以行向量
{
	CMatrix result(A.m_row,1);
	if(A.m_column!=B.m_column)
		throw out_of_range("行列不匹配");
	double B_square = B.Square();
	if(B.m_row == 1)
	{
		for(int i = 0;i<A.m_row;++i)
		{
			double temp = 0;
			for(int j = 0;j<A.m_column;++j)
			{
				temp += A(i+1,j+1)*B(1,j+1);
			}
			temp/=B_square;
			result.SetItem(i+1,1,temp);
		}
	}
	return result;
}

//求逆
void swap(double *a,double *b)
{  
	double c;
	c=*a;
	*a=*b;
	*b=c;
}
bool CMatrix::Inv()
{
	if(m_row!=m_column)
	{
		throw runtime_error("矩阵没有逆矩阵");
	}
	int *is,*js,i,j,k;
	double temp,fmax;
	is=(int *)malloc(m_row*sizeof(int));
	js=(int *)malloc(m_row*sizeof(int));
	for(k=0;k<m_row;k++)
	{
		fmax=0.0;
		for(i=k;i<m_row;i++)
			for(j=k;j<m_row;j++)
			{ 
				temp=fabs(*(m_Matrix+i*m_row+j));//找最大值
		
				if(temp>fmax)
				{ 
					fmax=temp;
					is[k]=i;
					js[k]=j;
				}
			}
		if((fmax+1.0)==1.0)
		{
			free(is);free(js);
			return 0;
		}
		if((i=is[k])!=k)
			for(j=0;j<m_row;j++)
				swap(m_Matrix+k*m_row+j,m_Matrix+i*m_row+j);//交换指针
		if((j=js[k])!=k)
			for(i=0;i<m_row;i++)
				swap(m_Matrix+i*m_row+k,m_Matrix+i*m_row+j);  //交换指针
		m_Matrix[k*m_row+k]=1.0/m_Matrix[k*m_row+k];
		for(j=0;j<m_row;j++)
			if(j!=k)
				m_Matrix[k*m_row+j]*=m_Matrix[k*m_row+k];
		for(i=0;i<m_row;i++)
			if(i!=k)
				for(j=0;j<m_row;j++)
					if(j!=k)
						m_Matrix[i*m_row+j]=m_Matrix[i*m_row+j]-m_Matrix[i*m_row+k]*m_Matrix[k*m_row+j];
		for(i=0;i<m_row;i++)
			if(i!=k)
				m_Matrix[i*m_row+k]*=-m_Matrix[k*m_row+k];
	}
	for(k=m_row-1;k>=0;k--)
	{
		if((j=js[k])!=k)
			for(i=0;i<m_row;i++)
				swap((m_Matrix+j*m_row+i),(m_Matrix+k*m_row+i));
		if((i=is[k])!=k)
			for(j=0;j<m_row;j++)
				swap((m_Matrix+j*m_row+i),(m_Matrix+j*m_row+k));
	}
	free(is);
	free(js);
	return 1;
}
CMatrix CMatrix::Transposition() const
{
	CMatrix C(m_column,m_row);
	for(int i=0;i<m_column;i++)
	{
		for(int j=0;j<m_row;j++)
		{
			C.SetItem(i+1,j+1,(*this)(j+1,i+1));
		}
	}
	return C;
}
double CMatrix::Square() const 
{
	double square=0;
	for(int i=0;i<m_row*m_column;i++)
	{
		square+=m_Matrix[i]*m_Matrix[i];
	}
	return square;
}
double  CMatrix::Norm() const
{
	double square=this->Square();
	return sqrt(square);
}
bool CMatrix::FindMax(CMatrixIndex &MatirxIndex) const
{
	 MatirxIndex.row=1;
	 MatirxIndex.column=1;
	 for(int i=1;i<=m_row;i++)
	 {
		 for(int j=1;j<=m_column;j++)
		 {
			 if((*this)(MatirxIndex.row,MatirxIndex.column)<(*this)(i,j))
			 {
				 MatirxIndex.row=i;
				 MatirxIndex.column=j;
			 }
		 }
	 }
	 return 1;
}
double CMatrix::GetMax()const
{
	double Max=(*this)(1,1);
	 for(int i=1;i<=m_row;i++)
	 {
		 for(int j=1;j<=m_column;j++)
		 {
			 if(Max<(*this)(i,j))
			 {
				 Max=(*this)(i,j);
			 }
		 }
	 }
	 return Max;
}

bool CMatrix::FindMin(CMatrixIndex &MatirxIndex) const
{
	 MatirxIndex.row=1;
	 MatirxIndex.column=1;
	 for(int i=1;i<=m_row;i++)
	 {
		 for(int j=1;j<=m_column;j++)
		 {
			 if((*this)(MatirxIndex.row,MatirxIndex.column)>(*this)(i,j))
			 {
				 MatirxIndex.row=i;
				 MatirxIndex.column=j;
			 }
		 }
	 }
	 return 1;
}
double CMatrix::GetMin() const
{
	double Min=(*this)(1,1);
	 for(int i=1;i<=m_row;i++)
	 {
		 for(int j=1;j<=m_column;j++)
		 {
			 if(Min>(*this)(i,j))
			 {
				 Min=(*this)(i,j);
			 }
		 }
	 }
	 return Min;
}

CMatrix CMatrix::Mean(const int &dim) const
{
	if(1 == dim)
	{
		CMatrix C(1,m_column);
		for(int i=1;i<=m_column;i++)
		{
			C.SetItem(1,i,(this->GetColumn(i).Sum())/m_row);
		}
		return C;
	}
	else if (2 == dim)
	{
		CMatrix C(m_row,1);
		for(int i=1;i<=m_row;i++)
		{
			C.SetItem(i,1,(this->GetRow(i).Sum())/m_column);
		}
		return C;
	}
	else
		throw invalid_argument("输入参数有误");
	
}

double CMatrix::Sum() const
{
	double sum=0;
	for(int i=0;i<m_row*m_column;i++)
	{
		sum+=m_Matrix[i];
	}
	return sum;
}

CMatrix CMatrix::cumsum(int type) const
{
	CMatrix consum(this->m_row,this->m_column);

	if(type == 2) // 对列进行累加
	{
		CMatrix temp;
		for (int i = 0;i<this->m_column;++i)
		{
			temp = this->GetColumn(1);
			for(int j = 1;j<=i;++j)
			{
				temp+=this->GetColumn(j+1);
			}
			consum.SetColumn(i+1,temp);
		}
	}
	else if(type == 1)//对行进行累加
	{
		CMatrix temp;
		for (int i = 0;i<this->m_row;++i)
		{
			temp = this->GetRow(1);
			for(int j = 1;j<=i;++j)
			{
				temp+=this->GetRow(j+1);
			}
			consum.SetRow(i+1,temp);
		}
	}
	return consum;
}


double CMatrix::GetMean() const
{
	double sum=0;
	for(int i=0;i<m_row*m_column;i++)
	{
		sum+=m_Matrix[i];
	}
	return sum/(m_row*m_column);
}
void CMatrix::Smooth(const int &N)
{
	CMatrix Smoothed;
	CMatrix temp(1,m_column);
	int halfwidth = (N-1)/2;
	int tempHalfwidth = 0;
	for(int i = 0;i<m_row;++i)
	{				
		for(int j = 0;j<m_column;++j)
		{
			double mean = 0;
			if(j < halfwidth)
			{
				tempHalfwidth = j;
				mean = (*this).GetBlock(j+1-tempHalfwidth,i+1,j+1+tempHalfwidth,i+1).Sum()/(2*tempHalfwidth+1);	
			}
			else if(j >= m_column - halfwidth)
			{
				tempHalfwidth = m_column - j-1;
				mean = (*this).GetBlock(j+1-tempHalfwidth,i+1,j+1+tempHalfwidth,i+1).Sum()/(2*tempHalfwidth+1);	
			}
			else
			{	
				tempHalfwidth = halfwidth;
				mean = (*this).GetBlock(j+1-tempHalfwidth,i+1,j+1+tempHalfwidth,i+1).Sum()/(2*tempHalfwidth+1);	
			}
			temp.SetItem(1,j+1,mean);
		}		
		Smoothed.AddRows(temp);
	}
	*this = Smoothed;
}
void CMatrix::Wden(const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname)
{
	CMatrix temp;

	for(int i = 0;i<m_row;++i)
	{
		temp = this->GetRow(i+1);
		temp = wden(temp,tptr,sorh,scal,n,wname);
		this->SetRow(i+1,temp);
	}
}

void CMatrix::MSC(CMatrix &ref,CMatrix &beta,CMatrix &alpha,bool mc)//对矩阵做多元散射校正（MSC）
{
	if(mc)  //进行中心化
	{
		ref = this->Mean(1);
		//对原始数据进行中心化
		alpha = this->Mean(2);
		for(int i =0;i<m_row;++i)
		{
			(*this).SetRow(i+1,((*this).GetRow(i+1))-CMatrix(1,m_column,alpha(i+1,1)));
		}	
		//对参考图谱进行中心化
		double mx = ref.GetMean();
		ref -= CMatrix(1,m_column,mx);
		//计算斜率
		beta = (*this)/ref;
		//计算偏置
		alpha -= mx*beta;

		for(int i =0 ;i<m_row;++i)
		{
			CMatrix tmp = ((*this).GetRow(i+1) - CMatrix(1,m_column,alpha(i+1,1)))/beta(i+1,1);
			(*this).SetRow(i+1,tmp);
		}

	}
	else //不进行中心化
	{
		ref = this->Mean(1);
		alpha = CMatrix(1,m_column,0);
		beta = (*this)/ref;
		for(int i =0;i<m_row;++i)
		{
			(*this).SetRow(i+1,((*this).GetRow(i+1))/beta(i+1,1));
		}		
	}
}
void CMatrix::Derivative(const int &N) //对矩阵每一行做N阶导
{
	if(N<0||N>m_column)
		throw invalid_argument("错误的导数阶次");
	CMatrix Deri = *this;
	for(int i = 0;i<N;++i)
	{
		Deri = Deri.Diff();
	}
	return;
}
CMatrix CMatrix::Diff() const
{

	CMatrix diff(m_row,m_column-1);
	for(int i = 0;i<m_row;++i)
	{
		for(int j = 1;j<m_column;++j)
		{
			diff.SetItem(i+1,j,(*this)(i+1,j+1)-(*this)(i+1,j));
		}
	}
	return diff;
}

bool CMatrix::Matrix2CString(CString &cstr,const CString &Sep) const
{
	cstr="";
	CString temp;
	int count=0;
	for(int i=1;i<=m_row;i++)
	{
		for(int j=1;j<=m_column;j++)
		{
			if(i==m_row&&j==m_column)
			{
				temp.Format(L"%lf",(*this)(i,j));
				cstr+=temp;
				count++;
			}
			else
			{
				temp.Format(L"%lf",(*this)(i,j));
				cstr+=temp;
				cstr+=Sep;
				count++;
			}
		}
	}
	return 1;
}

CMatrix CMatrix::covariance()
{
	if(m_row==1)
	{
		throw runtime_error("error");
	}
	else
	{
		CMatrix M(m_row,m_column);
		for(int i=1;i<=m_row;i++)
		{
			M.SetRow(i,this->Mean(1));
		}
		CMatrix X=(*this)-M;
		CMatrix COV(m_column,m_column);
		COV=(X.Transposition()*X)/(m_row-1);
		return COV;
	}
}

///PCA
void ppp( double a[], double e[], double s[], 
		  double v[], int m, int n )
{ 
	int i,j,p,q;
	double d;
	
	if ( m >= n ) i = n;
	else i = m;
	for ( j = 1; j <= i-1; j++ )
	{ 
		a[(j-1)*n+j-1] = s[j-1];
		a[(j-1)*n+j] = e[j-1];
	}
	a[(i-1)*n+i-1] = s[i-1];
	if ( m < n ) a[(i-1)*n+i] = e[i-1];
	for ( i = 1; i <= n-1; i++ )
		for ( j = i+1; j <= n; j++ )
		{ 
			p = (i-1)*n+j-1; q = (j-1)*n+i-1;
			d = v[p]; v[p] = v[q]; v[q] = d;
		}
		return;
}

void sss( double fg[2], double cs[2] )
{
	double r, d;
    if ( ( fabs(fg[0]) + fabs(fg[1] ) ) == 0.0 )
	{ 
		cs[0] = 1.0; cs[1] = 0.0; d = 0.0;
	}
    else
	{ 
		d = sqrt( fg[0]*fg[0]+fg[1]*fg[1] );
		if ( fabs( fg[0] ) > fabs( fg[1] ) )
		{
			d = fabs(d);
			if ( fg[0] < 0.0 ) d = -d;
		}
        if ( fabs( fg[1] ) >= fabs( fg[0] ) )
        { 
			d = fabs(d);
            if ( fg[1] < 0.0 ) d = -d;
		}
        cs[0] = fg[0]/d; cs[1] = fg[1]/d;
    }
    r = 1.0;
    if ( fabs( fg[0] ) > fabs( fg[1] ) ) 
		r = cs[1];
    else
		if ( cs[0] != 0.0 ) r = 1.0/cs[0];
		fg[0] = d; fg[1] = r;
		
		return;
}

/*
   一般实矩阵奇异值分解
   徐士良. 常用算法程序集（C语言描述），第3版. 清华大学出版社. 2004

   double a[m][n] --- 存放mxn维实矩阵A。返回时其对角线给出奇异值（以非递增次序排列）
                      其余元素均为0
   int m, int n   --- 实矩阵A的行数和列数
   double u[m][m] --- 返回左奇异向量U
   double v[n][n] --- 返回右奇异向量V'
   double eps     --- 给定的精度要求
   int ka         --- 其值为max(m, n) + 1
   返回值：
           若返回标志值小于0，则表示程序工作失败；
		   若返回标志值大于0，则表示正常返回   
*/
int svd( double a[], int m, int n, double u[], double v[],
	     double eps, int ka )
{ 
	int i, j, k, l, it, ll, kk, ix, iy, mm, nn, iz, m1, ks;
    double d, dd, t, sm, sm1, em1, sk, ek, b, c, shh, fg[2], cs[2];
    double * s, * e, * w;

    s = ( double * )malloc( ka * sizeof(double) );
    e = ( double * )malloc( ka * sizeof(double) );
    w = ( double * )malloc( ka * sizeof(double) );
    it = 60; k = n;
	if ( m-1 < n ) k = m-1;
	l = m;
	if ( n-2 < m ) l = n-2;
	if ( l < 0 ) l = 0;
	ll = k;
	if ( l > k ) ll = l;
	if ( ll >= 1 )
	{ 
		for ( kk = 1; kk <= ll; kk++ )
		{ 
			if ( kk <= k )
			{ 
				d = 0.0;
				for ( i = kk; i <= m; i++ )
				{ 
					ix = (i-1)*n+kk-1; d = d+a[ix]*a[ix]; 
				}
                s[kk-1] = sqrt(d);
                if ( s[kk-1] != 0.0 )
				{ 
					ix = (kk-1)*n+kk-1;
                    if ( a[ix] != 0.0 )
					{ 
						s[kk-1] = fabs( s[kk-1] );
                        if ( a[ix] < 0.0 ) s[kk-1] = -s[kk-1];
                    }
                    for ( i = kk; i <= m; i++ )
					{  
						iy = (i-1)*n+kk-1;
                        a[iy] = a[iy]/s[kk-1];
                    }
                    a[ix] = 1.0+a[ix];
                }
                s[kk-1] = -s[kk-1];
            }
            if ( n >= kk+1 )
            { 
				for ( j = kk+1; j <= n; j++ )
                { 
					if (( kk <= k ) && ( s[kk-1] != 0.0 ) )
                    { 
						d = 0.0;
                        for ( i = kk; i <= m; i++ )
                        { 
							ix = (i-1)*n+kk-1;
                            iy = (i-1)*n+j-1;
                            d = d+a[ix]*a[iy];
                        }
                        d = -d/a[(kk-1)*n+kk-1];
                        for ( i = kk; i <= m; i++ )
                        { 
							ix = (i-1)*n+j-1;
                            iy = (i-1)*n+kk-1;
                            a[ix] = a[ix]+d*a[iy];
                        }
                    }
                    e[j-1] = a[(kk-1)*n+j-1];
                }
            }
            if ( kk <= k )
            { 
				for ( i = kk; i <= m; i++ )
                { 
					ix = (i-1)*m+kk-1; 
					iy = (i-1)*n+kk-1;
                    u[ix] = a[iy];
                }
            }
            if ( kk <= l )
            { 
				d = 0.0;
                for ( i = kk+1; i <= n; i++ )
					d = d + e[i-1]*e[i-1];
                e[kk-1] = sqrt(d);
                if ( e[kk-1] != 0.0 )
                { 
					if ( e[kk] != 0.0 )
                    { 
						e[kk-1] = fabs(e[kk-1]);
                        if ( e[kk] < 0.0 ) e[kk-1] = -e[kk-1];
                    }
                    for ( i = kk+1; i <= n; i++ )
						e[i-1] = e[i-1]/e[kk-1];
                    e[kk] = 1.0+e[kk];
                }
                e[kk-1] = -e[kk-1];
                if (( kk+1 <= m ) && ( e[kk-1] != 0.0 ) )
                { 
					for ( i = kk+1; i <= m; i++ ) w[i-1] = 0.0;
                    for ( j = kk+1; j <= n; j++)
						for ( i = kk+1; i <= m; i++ )
							w[i-1] = w[i-1]+e[j-1]*a[(i-1)*n+j-1];
                    for ( j = kk+1; j <= n; j++ )
                        for ( i = kk+1; i <= m; i++ )
                        { 
							ix = (i-1)*n+j-1;
                            a[ix] = a[ix]-w[i-1]*e[j-1]/e[kk];
                        }
                }
                for ( i = kk+1; i <= n; i++ )
					v[(i-1)*n+kk-1] = e[i-1];
            }
        }
	}
    mm = n;
    if ( m+1 < n ) mm = m+1;
    if ( k < n ) s[k] = a[k*n+k];
    if ( m < mm ) s[mm-1] = 0.0;
    if ( l+1 < mm) e[l] = a[l*n+mm-1];
    e[mm-1] = 0.0;
    nn = m;
    if ( m > n ) nn = n;
    if ( nn >= k+1 )
    { 
		for ( j = k+1; j <= nn; j++ )
        { 
			for ( i = 1; i <= m; i++ )
				u[(i-1)*m+j-1] = 0.0;
            u[(j-1)*m+j-1] = 1.0;
        }
    }
    if ( k >= 1)
    { 
		for (ll = 1; ll <= k; ll++ )
        { 
			kk = k-ll+1; iz = (kk-1)*m+kk-1;
            if ( s[kk-1] != 0.0 )
            {
				if ( nn >= kk+1 )
                  for ( j = kk+1; j <= nn; j++ )
                  { 
					  d = 0.0;
                      for ( i = kk; i <= m; i++ )
                      {   
						  ix = (i-1)*m+kk-1;
                          iy = (i-1)*m+j-1;
                          d = d+u[ix]*u[iy]/u[iz];
                      }
                      d = -d;
                      for ( i = kk; i <= m; i++ )
                      { 
						  ix = (i-1)*m+j-1;
                          iy = (i-1)*m+kk-1;
                          u[ix] = u[ix]+d*u[iy];
                      }
                  }
                  for ( i = kk; i <= m; i++ )
                  { 
					  ix = (i-1)*m+kk-1; u[ix] = -u[ix];
				  }
                  u[iz] = 1.0+u[iz];
                  if ( kk-1 >= 1 )
					  for ( i = 1; i <= kk-1; i++ )
						  u[(i-1)*m+kk-1] = 0.0;
            }
            else
            { 
				for ( i = 1; i <= m; i++ )
					u[(i-1)*m+kk-1] = 0.0;
                u[(kk-1)*m+kk-1] = 1.0;
            }
		}
    }
    for ( ll = 1; ll <= n; ll++ )
	{ 
		kk = n-ll+1; iz = kk*n+kk-1;
        if ( (kk<=l) && (e[kk-1] != 0.0) )
        { 
			for ( j = kk+1; j <= n; j++ )
            { 
				d = 0.0;
                for ( i = kk+1; i <= n; i++ )
                { 
					ix = (i-1)*n+kk-1; iy = (i-1)*n+j-1;
                    d = d+v[ix]*v[iy]/v[iz];
                }
                d = -d;
                for ( i = kk+1; i <= n; i++ )
                { 
					ix = (i-1)*n+j-1; iy = (i-1)*n+kk-1;
                    v[ix] = v[ix]+d*v[iy];
                }
            }
        }
        for ( i = 1; i <= n; i++ )
			v[(i-1)*n+kk-1] = 0.0;
        v[iz-n] = 1.0;
    }
    for ( i = 1; i <= m; i++ )
    for ( j = 1; j <= n; j++ )
		a[(i-1)*n+j-1] = 0.0;
    m1 = mm; it = 60;
    while ( 1==1 )
    {
		if ( mm == 0 )
        { 
			ppp( a, e, s, v, m, n );
            free( s ); 
			free( e ); 
			free( w ); 
			return( 1 );
        }
        if ( it == 0 )
        { 
			ppp( a, e, s, v, m, n );
            free( s ); 
			free( e ); 
			free( w ); 
			return( -1 );
        }
        kk = mm-1;
		while ( (kk != 0) && (fabs(e[kk-1]) != 0.0) )
		{ 
			d = fabs(s[kk-1])+fabs(s[kk]);
			dd = fabs(e[kk-1]);
			if ( dd > eps*d ) 
				kk = kk-1;
			else 
				e[kk-1] = 0.0;
		}
		if ( kk == mm-1 )
		{ 
			kk = kk+1;
			if ( s[kk-1] < 0.0 )
			{ 
				s[kk-1] = -s[kk-1];
				for ( i = 1; i <= n; i++ )
				{ 
					ix = (i-1)*n+kk-1; 
					v[ix] = -v[ix];
				}
			}
			while ( (kk!=m1) && (s[kk-1] < s[kk]) )
			{ 
				d = s[kk-1]; 
				s[kk-1] = s[kk]; 
				s[kk] = d;
				if ( kk < n )
					for (i=1; i<=n; i++)
					{ 
						ix = (i-1)*n+kk-1; iy = (i-1)*n+kk;
						d = v[ix]; v[ix] = v[iy]; v[iy] = d;
					}
					if ( kk < m )
						for ( i = 1; i <= m; i++ )
						{ 
							ix = (i-1)*m+kk-1; iy = (i-1)*m+kk;
							d = u[ix]; u[ix] = u[iy]; u[iy] = d;
						}
					kk = kk+1;
			}
			it = 60;
			mm = mm-1;
		}
		else
		{ 
			ks = mm;
			while ( (ks>kk) && (fabs(s[ks-1]) != 0.0 ) )
			{ 
				d = 0.0;
				if ( ks != mm ) d = d+fabs(e[ks-1]);
				if ( ks != kk+1 ) d = d+fabs(e[ks-2]);
				dd = fabs(s[ks-1]);
				if ( dd > eps*d ) ks = ks-1;
				else s[ks-1] = 0.0;
			}
			if ( ks == kk )
			{ 
				kk = kk+1;
				d = fabs(s[mm-1]);
				t = fabs(s[mm-2]);
				if ( t > d ) d = t;
				t = fabs( e[mm-2] );
				if ( t > d ) d = t;
				t = fabs( s[kk-1] );
				if ( t > d ) d = t;
				t = fabs( e[kk-1] );
				if ( t > d ) d = t;
				sm = s[mm-1]/d; sm1 = s[mm-2]/d;
				em1 = e[mm-2]/d;
				sk = s[kk-1]/d; ek = e[kk-1]/d;
				b = ((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
				c = sm*em1; c = c*c; shh = 0.0;
				if ( (b!=0.0) || (c!=0.0) )
				{ 
					shh = sqrt( b*b+c );
					if ( b < 0.0 ) shh = -shh;
					shh = c/(b+shh);
				}
				fg[0] = (sk+sm)*(sk-sm)-shh;
				fg[1] = sk*ek;
				for ( i = kk; i <= mm-1; i++ )
				{ 
					sss( fg, cs );
					if ( i != kk ) e[i-2] = fg[0];
					fg[0] = cs[0]*s[i-1]+cs[1]*e[i-1];
					e[i-1] = cs[0]*e[i-1]-cs[1]*s[i-1];
					fg[1] = cs[1]*s[i];
					s[i] = cs[0]*s[i];
					if ( (cs[0] != 1.0) || (cs[1] != 0.0) )
						for ( j = 1; j <= n; j++ )
						{ 
							ix = (j-1)*n+i-1;
							iy = (j-1)*n+i;
							d = cs[0]*v[ix]+cs[1]*v[iy];
							v[iy] = -cs[1]*v[ix]+cs[0]*v[iy];
							v[ix] = d;
						}
						sss( fg, cs );
						s[i-1] = fg[0];
						fg[0] = cs[0]*e[i-1]+cs[1]*s[i];
						s[i] = -cs[1]*e[i-1]+cs[0]*s[i];
						fg[1] = cs[1]*e[i];
						e[i] = cs[0]*e[i];
						if ( i < m )
							if ( ( cs[0] != 1.0 ) || ( cs[1] != 0.0 ) )
								for ( j = 1; j <= m; j++ )
								{ 
									ix = (j-1)*m+i-1;
									iy = (j-1)*m+i;
									d = cs[0]*u[ix]+cs[1]*u[iy];
									u[iy] = -cs[1]*u[ix]+cs[0]*u[iy];
									u[ix] = d;
								}
				}
				e[mm-2] = fg[0];
				it = it-1;
			}
			else
			{ 
				if ( ks == mm )
				{ 
					kk = kk+1;
					fg[1] = e[mm-2]; e[mm-2] = 0.0;
	                for ( ll = kk; ll <= mm-1; ll++ )
		            { 
						i = mm+kk-ll-1;
				        fg[0] = s[i-1];
					    sss( fg, cs );
						s[i-1] = fg[0];
					    if ( i != kk )
						{ 
							fg[1] = -cs[1]*e[i-2];
							e[i-2] = cs[0]*e[i-2];
						}
						if ( (cs[0]!=1.0)||(cs[1]!=0.0) )
							for ( j = 1; j <= n; j++ )
							{ 
								ix = (j-1)*n+i-1;
								iy = (j-1)*n+mm-1;
								d = cs[0]*v[ix]+cs[1]*v[iy];
								v[iy] = -cs[1]*v[ix]+cs[0]*v[iy];
								v[ix] = d;
							}
					}
				}
				else
				{ 
					kk = ks+1;
					fg[1] = e[kk-2];
					e[kk-2] = 0.0;
					for ( i = kk; i <= mm; i++ )
					{ 
						fg[0] = s[i-1];
						sss( fg, cs );
						s[i-1] = fg[0];
						fg[1] = -cs[1]*e[i-1];
						e[i-1] = cs[0]*e[i-1];
						if ( (cs[0]!=1.0)||(cs[1]!=0.0) )
							for (j=1; j<=m; j++)
							{ 
								ix = (j-1)*m+i-1;
								iy = (j-1)*m+kk-2;
								d = cs[0]*u[ix]+cs[1]*u[iy];
								u[iy] = -cs[1]*u[ix]+cs[0]*u[iy];
								u[ix] = d;
							}
					}
				}
			}
		}
    }

    return( 1 );
}

# define EPS  0.000001

/*
  PCA2: Perform PCA using SVD.
  data     --- MxN matrix of input data ( M dimensions, N trials )
  signals  --- MxN matrix of projected data 
  PC       --- each column is a PC
  V        --- Mx1 matrix of variances
  row = M dimensions, col = N trials 
*/
int pca2( double * data, int row, int col,
	      double * signals, double * PC, double * V )
{
	int x, y, i, ka, rvalue;
	double * mean;
	double * u;
    double * d;
    double * v;
	double * sd;
	double * data1;
	double sqrt_col_1;
	int col1, row1;

	if ( row <= 1 || col <= 1 ) return( -1 );

	/* subtract off the mean for each dimension */
	mean = ( double * )malloc( sizeof( double )*row );
	data1 = ( double * ) malloc( sizeof( double )*row*col );
	for ( y = 0; y < row; y++ ) /* calculate mean */
	{
		mean[y] = 0;
		for ( x = 0; x < col; x++ )
			mean[y] += data[y*col+x];
	}
	for ( y = 0; y < row; y++ ) mean[y] = mean[y]/col;
	for ( y = 0; y < row; y++ ) /* subtract mean */
		for ( x = 0; x < col; x++ )
		{
			data1[y*col+x] = data[y*col+x]-mean[y];
		}
	free( mean );

	/* construct the matrix Y: Y = data' / sqrt(col-1); */

	/* construct the matrix Y: Y = data' / sqrt(col-1); */
	sqrt_col_1 = sqrt(col-1.0);
	row1 = col;
	col1 = row;
	sd = (double *)malloc( sizeof(double)*col1*row1 );
	u = (double *)malloc( sizeof(double)*row1*row1 );
	v = (double *)malloc( sizeof(double)*col1*col1 );
	for ( y = 0; y < row1; y++ )
		for ( x = 0; x < col1; x++ )
		{
			sd[y*col1+x] = data1[x*row1+y]/sqrt_col_1;
		}

	/* SVD does it all: [u, S, PC] = svd( Y ); */
	if ( row1 >= col1 ) 
		ka = row1+1;
	else 
		ka = col1+1;
	rvalue = svd( sd, row1, col1, u, v, EPS, ka ); /* svd decomposition */
	d = (double *)malloc( sizeof(double) * col1 );
	for ( i = 0; i < col1; i++ ) d[i] = 0;
	if ( row1 <= col1 )
	{
		for ( i = 0; i < row1; i++ )
			d[i] = sd[i*col1+i];
	}
	else
	{
		for ( i = 0; i < col1; i++ )
			d[i] = sd[i*col1+i];
	}

	/* calculate the variances: S = diag( S ); V = S .* S; */
	for ( x = 0; x < col1; x++ )
		V[x] = d[x] * d[x];
	/* get PC */
	for ( y = 0; y < col1; y++ )
		for ( x = 0; x < col1; x++ )
		{
			PC[y*col1+x] = v[x*col1+y];
		}

	/* project the original data: signals = PC' * data; */
	for ( y = 0; y < row; y++ )
		for ( x = 0; x < col; x++ )
		{
			signals[y*col+x] = 0;
			for ( i = 0; i < row; i++ )
				signals[y*col+x] += PC[i*row+y] * data1[i*col+x];
		}

	free( u );
	free( v );
	free( sd );
	free( d );
	free( data1 );

	return 1;
}
bool CMatrix::PCA(CMatrix &signals,CMatrix &PC,CMatrix &V) const
{

	CMatrix X_T=this->Transposition();
	CMatrix signals_temp=signals.Transposition();
	//CMatrix PC_temp=PC.Transposition();
	int flag=pca2(X_T.m_Matrix,X_T.m_row,X_T.m_column,signals_temp.m_Matrix,PC.m_Matrix,V.m_Matrix);
	signals=signals_temp.Transposition();
	//PC=PC_temp.Transposition();
	if(flag)return 1;
	else return 0;
}

int CMatrix::Count(double a) const
{
	int count=0;
	for(int i=0;i<m_row*m_column;i++)
	{
		if(m_Matrix[i]==a)
		{
			count++;
		}
	}
	return count;
}

bool CMatrix::FindFirst(double a,CMatrixIndex &index) const
{
	for(int i=1;i<=m_row;i++)
	{
		for(int j=1;j<=m_column;j++)
		{
			if((*this)(i,j)==a)
			{
				index.row=i;
				index.column=j;
				return 1;
			}
		}
	}
	return 0;
}

bool CMatrix::Have(double a) const
{
	for(int i=1;i<=m_row;i++)
	{
		for(int j=1;j<=m_column;j++)
		{
			if((*this)(i,j)==a)
			{
				return 1;
			}
		}
	}
	return 0;
}
CMatrix CMatrix::GetDistinct() const 
{
	list<double> Distinct;
	for(int i=0;i<m_row*m_column;i++)
	{	
		if(Distinct.end()==find(Distinct.begin(),Distinct.end(),m_Matrix[i]))
		{
			Distinct.push_back(m_Matrix[i]);
		}
	}
	int count=0;
	list<double>::iterator Distinctiterator=Distinct.begin();
	for(;Distinctiterator!=Distinct.end();Distinctiterator++,count++);

	CMatrix DistinctMatrix(1,count);

	int i=1;
	for(Distinctiterator=Distinct.begin();Distinctiterator!=Distinct.end();Distinctiterator++,i++)
	{
		DistinctMatrix.SetItem(1,i,*Distinctiterator);
	}
	return DistinctMatrix;
}

double Max(double a,double b)
{
	return a>b?a:b;
}

CMatrix Abs(const CMatrix &A)
{
	CMatrix Aabs(A.m_row,A.m_column);

	for(int i = 0;i<A.m_row;++i)
	{
		for(int j = 0;j<A.m_column;++j)
		{
			Aabs.SetItem(i+1,j+1,abs(A(i+1,j+1)));
		}
	}
	return Aabs;
}

CMatrix Sort(const CMatrix &A,const int &dim,const CString Type)
{
	CMatrix A_sorted(A);
	if(Type == L"ascend") //按升序排列
	{
		if(dim == 1) //按行排序
		{
			CMatrix tmp;
			for(int i = 0;i<A.m_row;++i)
			{
				tmp = A.GetRow(i+1);
				QuickSort(tmp.m_Matrix,0,tmp.m_column-1);
				A_sorted.SetRow(i+1,tmp);
			}
		}
		else if(dim == 2) //按列排序
		{
			CMatrix tmp;
			for(int i = 0;i<A.m_row;++i)
			{
				tmp = A.GetColumn(i+1);
				QuickSort(tmp.m_Matrix,0,tmp.m_column-1);
				A_sorted.SetColumn(i+1,tmp);
			}
		}
	}
	else if(Type == L"descend") //按降序排列
	{
		if(dim == 1) //按行排序
		{

		}
		else if(dim == 2) //按列排序
		{

		}
	}
	return A_sorted;
}

void QuickSort(double *a,int low,int high)
{
	if(low>=high)
	{
		return;
	}
	int first=low;
	int last=high;
	double key=a[first];//用第一个记录作为关键字
	while(first<last)
	{
		while(first<last&&a[last]>=key)--last;
		a[first]=a[last];//将比第一个小的移到低端
		while(first<last&&a[first]<=key)++first;
		a[last]=a[first];//将比第一个大的移到高端
	}
	a[first]=key;//枢轴记录到位
	QuickSort(a,low,first-1);
	QuickSort(a,last+1,high);
}

int sign(double x)
{
	if(x>0)
		return 1;
	else if(x == 0)
		return 0;
	else
		return -1;
}

double median(const CMatrix &A)
{
	CMatrix A_sorted = Sort(A,1,L"ascend");
	//double x;
	if(A_sorted.m_column%2 == 0)
	{
		return (A_sorted(1,A_sorted.m_column/2)+A_sorted(1,A_sorted.m_column/2+1))/2;
	}
	else
	{
		return A_sorted(1,A_sorted.m_column/2+1);
	}
}
