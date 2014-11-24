#include "StdAfx.h"
#include "PLS.h"
#include <map>
#include <algorithm>
using std::map;

CMatrix Pls_nipals(CMatrix &X,CMatrix &Y,CMatrix &W,CMatrix &T,CMatrix &P,CMatrix &Q,const int &A)
{
	
	CMatrix W_column(X.m_column,1);
	CMatrix T_column(X.m_row,1);
	CMatrix P_column(X.m_column,1);

	CMatrix X_remain=X;
	CMatrix Y_remain=Y;


	for(int i=0;i<A;i++)
	{
		CMatrix X_remain_T=X_remain.Transposition();

		//CMatrix XT_Y=

		W_column=X_remain_T*Y_remain;
		double norm=W_column.Norm();
		W_column=W_column/(W_column.Norm());
		T_column=X_remain*W_column;

		CMatrix T_column_T=T_column.Transposition();
		CMatrix T_2=T_column_T*T_column;
		CMatrix Y_remain_T=Y_remain.Transposition();
		P_column=(X_remain_T*T_column)/T_2(1,1);

		CMatrix Q_temp=Y_remain_T*T_column/T_2(1,1);
		

		CMatrix P_column_T=P_column.Transposition();

		CMatrix TP=T_column*P_column_T;
		X_remain-=TP;

		CMatrix TQ=T_column*Q_temp;
		Y_remain-=TQ;

		W.SetColumn(i+1,W_column);
		T.SetColumn(i+1,T_column);
		P.SetColumn(i+1,P_column);
		Q.SetItem(1,i+1,Q_temp(1,1));
	}

	CMatrix P_T=P.Transposition();
	CMatrix PTW=P_T*W;
	PTW.Inv();
	W=W*PTW;
	
	CMatrix B(W*Q);
	return B;
}

//暂时只做中心化
bool Pretreat(CMatrix &X,const char* method,CMatrix &para1,CMatrix &para2)
{
	if(method=="center")
	{
		para1=X.Mean(1);
		para2=CMatrix(1,X.m_column,1);
		for(int k=1;k<=X.m_row;k++)
		{
			X.SetRow(k,X.GetRow(k)-para1);
		}		
		return 0;
	}
	return 0;
}

//交叉验证法选择PLS最佳主成分数（只做了留一法）

CMatrix Plscvfold(CMatrix X,CMatrix y,int K_fold,const char *method,int Amax)
{
	CMatrix CV(1,Amax,0);

	if(X.m_row==K_fold)
	{
		CMatrix X_n_1(X.m_row-1,X.m_column);
		CMatrix y_n_1(y.m_row-1,y.m_column);

		CMatrix W(X_n_1.m_column,Amax);
		CMatrix T(X_n_1.m_row,Amax);
		CMatrix P(X_n_1.m_column,Amax);
		CMatrix Q(Amax,1);

		CMatrix X_para1(1,X.m_column);
		CMatrix X_para2(1,X.m_column);
		CMatrix y_para1(1,y.m_column);
		CMatrix y_para2(1,y.m_column);

		for(int i=1;i<=K_fold;i++)
		{
			//得到交叉验证训练集
			int row_count=1;
			for(int j=1;j<=K_fold;j++)
			{			
				if(j!=i)
				{
					X_n_1.SetRow(row_count,X.GetRow(j));
					y_n_1.SetRow(row_count,y.GetRow(j));
					row_count++;
				}
			}
			Pretreat(X_n_1,"center",X_para1,X_para2);
			Pretreat(y_n_1,"center",y_para1,y_para2);
			CMatrix B(X.m_column,1,0);

			B=Pls_nipals(X_n_1,y_n_1,W,T,P,Q,Amax);

			for(int k=1;k<=Amax;k++)
			{	
				//计算主成分为k时的B=W(:,1:k)*Q(1:k);
				double Offset;
				double pred;
				CMatrix Wk(W.m_row,k);
				CMatrix Qk(k,1);
				for(int l=1;l<=k;l++)
				{
					Wk.SetColumn(l,W.GetColumn(l));
					Qk.SetRow(l,Q.GetRow(l));
				}
				CMatrix Bk=Wk*Qk;

				Offset=(y_para1-X_para1*Bk)(1,1);
				pred=Offset+(X.GetRow(i)*Bk)(1,1);
				double tempE_2=pow(pred-y(i,1),2);
				CV.SetItem(1,k,CV(1,k)+tempE_2);	
			}
			
		}
		for(int m=1;m<=Amax;m++)
		{
			CV.SetItem(1,m,sqrt((CV(1,m))/X.m_row));
		}
		return CV;
	}
}

CMatrix BuildMapLabel(CMatrix X)
{
	if(X.m_column!=1)
	{
		return X;
	}
	CMatrix Label(X);
	map<int,int> Label_map;
	int label=1;
	Label_map[X(1,1)]=label;
	for(int i=2;i<=X.m_row;i++)
	{
		if(Label_map.end()==Label_map.find(X(i,1)))
		{
			label++;
			Label.SetItem(i,1,label);
			Label_map[X(1,i)]=label;
		}
		else
		{
			Label.SetItem(i,1,Label_map[X(1,i)]);
		}
	}
	return Label;

}