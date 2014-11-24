#include "stdafx.h"
#include "WaveletAnalasis.h"
using namespace std;


bool Filter::Init(const CString WaveletName,const CString decrec)
{
	if(decrec == L"d") //滤波器为分解
	{
		if(WaveletName == L"haar"||WaveletName == L"db1")
		{
			CMatrix Lo_D(1,2);
			Lo_D.SetItem(1,1,0.707106781186548);
			Lo_D.SetItem(1,2,0.707106781186548);

			lowFilter = Lo_D;
				

			CMatrix Hi_D(1,2);
			Hi_D.SetItem(1,1,-0.707106781186548);
			Hi_D.SetItem(1,2,0.707106781186548);

			highFilter = Hi_D;
			return true;
		}
		else if(WaveletName == L"db2")
		{
			CMatrix Lo_D(1,4);
			Lo_D.SetItem(1,1,-0.129409522550921);
			Lo_D.SetItem(1,2,0.224143868041857);
			Lo_D.SetItem(1,3,0.836516303737469);
			Lo_D.SetItem(1,4,0.482962913144690);

			lowFilter = Lo_D;
				
			CMatrix Hi_D(1,4);
			Hi_D.SetItem(1,1,-0.482962913144690);
			Hi_D.SetItem(1,2,0.836516303737469);
			Hi_D.SetItem(1,3,-0.224143868041857);
			Hi_D.SetItem(1,4,-0.129409522550921);
			highFilter = Hi_D;
			return true;

		}
		else if(WaveletName == L"db3")
		{
			CMatrix Lo_D(1,6);
			Lo_D.SetItem(1,1,0.0352262918821007);
			Lo_D.SetItem(1,2,-0.0854412738822415);
			Lo_D.SetItem(1,3,-0.135011020010391);
			Lo_D.SetItem(1,4,0.459877502119331);
			Lo_D.SetItem(1,5,0.806891509313339);
			Lo_D.SetItem(1,6,0.332670552950957);

			lowFilter = Lo_D;
								
			CMatrix Hi_D(1,6);
			Hi_D.SetItem(1,1,-0.332670552950957);
			Hi_D.SetItem(1,2,0.806891509313339);
			Hi_D.SetItem(1,3,-0.459877502119331);
			Hi_D.SetItem(1,4,-0.135011020010391);
			Hi_D.SetItem(1,5,0.0854412738822415);
			Hi_D.SetItem(1,6,0.0352262918821007);
			highFilter = Hi_D;
			return true;
		}
		else if(WaveletName == L"db4")
		{
			CMatrix Lo_D(1,8);
			Lo_D.SetItem(1,1,-0.0105974017849970);
			Lo_D.SetItem(1,2,0.0328830116669829);
			Lo_D.SetItem(1,3,0.0308413818359870);
			Lo_D.SetItem(1,4,-0.187034811718881);
			Lo_D.SetItem(1,5,-0.0279837694169839);
			Lo_D.SetItem(1,6,0.630880767929590);
			Lo_D.SetItem(1,7,0.714846570552542);
			Lo_D.SetItem(1,8,0.230377813308855);

			lowFilter = Lo_D;
										
			CMatrix Hi_D(1,8);
			Hi_D.SetItem(1,1,-0.230377813308855);
			Hi_D.SetItem(1,2,0.714846570552542);
			Hi_D.SetItem(1,3,-0.630880767929590);
			Hi_D.SetItem(1,4,-0.0279837694169839);
			Hi_D.SetItem(1,5,0.187034811718881);
			Hi_D.SetItem(1,6,0.0308413818359870);
			Hi_D.SetItem(1,7,-0.0328830116669829);
			Hi_D.SetItem(1,8,-0.0105974017849973);
			highFilter = Hi_D;		
			return true;
		}
		else if(WaveletName == L"db5")
		{									
			CMatrix Lo_D(1,8);
			Lo_D.SetItem(1,1,0.00333572528500155);
			Lo_D.SetItem(1,2,-0.0125807519990155);
			Lo_D.SetItem(1,3,-0.00624149021301171);
			Lo_D.SetItem(1,4,0.0775714938400652);
			Lo_D.SetItem(1,5,-0.0322448695850295);
			Lo_D.SetItem(1,6,-0.242294887066190);
			Lo_D.SetItem(1,7,0.138428145901103);
			Lo_D.SetItem(1,8,0.724308528438574);
			Lo_D.SetItem(1,9,0.603829269797473);
			Lo_D.SetItem(1,10,0.160102397974125);
			lowFilter = Lo_D;
																
			CMatrix Hi_D(1,8);
			Hi_D.SetItem(1,1,-0.160102397974125);
			Hi_D.SetItem(1,2,0.603829269797473);
			Hi_D.SetItem(1,3,-0.724308528438574);
			Hi_D.SetItem(1,4,0.138428145901103);
			Hi_D.SetItem(1,5,0.242294887066190);
			Hi_D.SetItem(1,6,-0.0322448695850295);
			Hi_D.SetItem(1,7,-0.0775714938400652);
			Hi_D.SetItem(1,8,-0.00624149021301171);
			Hi_D.SetItem(1,9,0.0125807519990155);
			Hi_D.SetItem(1,10,0.00333572528500155);

			highFilter = Hi_D;		
			return true;
		}
		else if(WaveletName == L"db6")
		{

		}
		else if(WaveletName == L"db7")
		{

		}
		else if(WaveletName == L"db8")
		{

		}
		else if(WaveletName == L"db9")
		{

		}
		else if(WaveletName == L"db10")
		{

		}
		else if(WaveletName == L"sym2")
		{					
			CMatrix Lo_D(1,4);
			Lo_D.SetItem(1,1,-0.129409522550921);
			Lo_D.SetItem(1,2,0.224143868041857);
			Lo_D.SetItem(1,3,0.836516303737469);
			Lo_D.SetItem(1,4,0.482962913144690);
			lowFilter = Lo_D;
		
			CMatrix Hi_D(1,4);
			Hi_D.SetItem(1,1,-0.482962913144690);
			Hi_D.SetItem(1,2,0.836516303737469);
			Hi_D.SetItem(1,3,-0.224143868041857);
			Hi_D.SetItem(1,4,-0.129409522550921);
			highFilter = Hi_D;			
			return true;
		}
		else if(WaveletName == L"sym3")
		{							
			CMatrix Lo_D(1,6);
			Lo_D.SetItem(1,1,0.0352262918821007);
			Lo_D.SetItem(1,2,-0.0854412738822415);
			Lo_D.SetItem(1,3,-0.135011020010391);
			Lo_D.SetItem(1,4,0.459877502119331);
			Lo_D.SetItem(1,5,0.806891509313339);
			Lo_D.SetItem(1,6,0.332670552950957);
							
			lowFilter = Lo_D;								
			CMatrix Hi_D(1,6);
			Hi_D.SetItem(1,1,-0.332670552950957);
			Hi_D.SetItem(1,2,0.806891509313339);
			Hi_D.SetItem(1,3,-0.459877502119331);
			Hi_D.SetItem(1,4,-0.135011020010391);
			Hi_D.SetItem(1,5,0.0854412738822415);
			Hi_D.SetItem(1,6,0.0352262918821007);
			highFilter = Hi_D;			
			return true;
		}
		else if(WaveletName == L"sym4")
		{								
			CMatrix Lo_D(1,8);
			Lo_D.SetItem(1,1,-0.0757657147892733);
			Lo_D.SetItem(1,2,-0.0296355276459985);
			Lo_D.SetItem(1,3,0.497618667632015);
			Lo_D.SetItem(1,4,0.803738751805916);
			Lo_D.SetItem(1,5,0.297857795605277);
			Lo_D.SetItem(1,6,-0.0992195435768472);
			Lo_D.SetItem(1,7,-0.0126039672620378);
			Lo_D.SetItem(1,8,0.0322231006040427);
			lowFilter = Lo_D;	
												
			CMatrix Hi_D(1,8);
			Hi_D.SetItem(1,1,-0.0322231006040427);
			Hi_D.SetItem(1,2,-0.0126039672620378);
			Hi_D.SetItem(1,3,0.0992195435768472);
			Hi_D.SetItem(1,4,0.297857795605277);
			Hi_D.SetItem(1,5,-0.803738751805916);
			Hi_D.SetItem(1,6,0.497618667632015);
			Hi_D.SetItem(1,7,0.0296355276459985);
			Hi_D.SetItem(1,8,-0.0757657147892733);
			highFilter = Hi_D;			
			return true;
		}
		else if(WaveletName == L"sym5")
		{

		}
		else if(WaveletName == L"sym6")
		{

		}
		else if(WaveletName == L"sym7")
		{

		}
		else if(WaveletName == L"sym8")
		{

		}

	}
	else if(decrec == L"r") //滤波器为重构
	{
		if(WaveletName == L"haar"||WaveletName == L"db1")
		{
			CMatrix Lo_R(1,2);
			Lo_R.SetItem(1,1,0.707106781186548);
			Lo_R.SetItem(1,2,0.707106781186548);
			lowFilter = Lo_R;

			CMatrix Hi_R(1,2);
			Hi_R.SetItem(1,1,0.707106781186548);
			Hi_R.SetItem(1,2,-0.707106781186548);
			highFilter = Hi_R;
			return true;
		}
		else if(WaveletName == L"db2")
		{
						
			CMatrix Lo_R(1,4);
			Lo_R.SetItem(1,1,0.482962913144690);
			Lo_R.SetItem(1,2,0.836516303737469);
			Lo_R.SetItem(1,3,0.224143868041857);
			Lo_R.SetItem(1,4,-0.129409522550921);
			lowFilter = Lo_R;
			
			CMatrix Hi_R(1,4);
			Hi_R.SetItem(1,1,-0.129409522550921);
			Hi_R.SetItem(1,2,-0.224143868041857);
			Hi_R.SetItem(1,3,0.836516303737469);
			Hi_R.SetItem(1,4,-0.482962913144690);
			highFilter = Hi_R;
			return true;
		}
		else if(WaveletName == L"db3")
		{
								
			CMatrix Lo_R(1,6);
			Lo_R.SetItem(1,1,0.332670552950957);
			Lo_R.SetItem(1,2,0.806891509313339);
			Lo_R.SetItem(1,3,0.459877502119331);
			Lo_R.SetItem(1,4,-0.135011020010391);
			Lo_R.SetItem(1,5,-0.0854412738822415);
			Lo_R.SetItem(1,6,0.0352262918821007);
			lowFilter = Lo_R;
							
			CMatrix Hi_R(1,6);
			Hi_R.SetItem(1,1,0.0352262918821007);
			Hi_R.SetItem(1,2,0.0854412738822415);
			Hi_R.SetItem(1,3,-0.135011020010391);
			Hi_R.SetItem(1,4,-0.459877502119331);
			Hi_R.SetItem(1,5,0.806891509313339);
			Hi_R.SetItem(1,6,-0.332670552950957);
			highFilter = Hi_R;
			return true;

		}
		else if(WaveletName == L"db4")
		{
			CMatrix Lo_D(1,8);
			Lo_D.SetItem(1,1,0.230377813308855);
			Lo_D.SetItem(1,2,0.714846570552542);
			Lo_D.SetItem(1,3,0.630880767929590);
			Lo_D.SetItem(1,4,-0.0279837694169839);
			Lo_D.SetItem(1,5,-0.187034811718881);
			Lo_D.SetItem(1,6,0.0308413818359870);
			Lo_D.SetItem(1,7,0.0328830116669829);
			Lo_D.SetItem(1,8,-0.0105974017849973);

			lowFilter = Lo_D;
										
							
			CMatrix Hi_D(1,8);
			Hi_D.SetItem(1,1,-0.0105974017849973);
			Hi_D.SetItem(1,2,-0.0328830116669829);
			Hi_D.SetItem(1,3,0.0308413818359870);
			Hi_D.SetItem(1,4,0.187034811718881);
			Hi_D.SetItem(1,5,-0.0279837694169839);
			Hi_D.SetItem(1,6,-0.630880767929590);
			Hi_D.SetItem(1,7,0.714846570552542);
			Hi_D.SetItem(1,8,-0.230377813308855);
			highFilter = Hi_D;			

			return true;
		}
		else if(WaveletName == L"db5")
		{										
			CMatrix Lo_D(1,10);
			Lo_D.SetItem(1,1,0.160102397974125);
			Lo_D.SetItem(1,2,0.603829269797473);
			Lo_D.SetItem(1,3,0.724308528438574);
			Lo_D.SetItem(1,4,0.138428145901103);
			Lo_D.SetItem(1,5,-0.242294887066190);
			Lo_D.SetItem(1,6,-0.0322448695850295);
			Lo_D.SetItem(1,7,0.0775714938400652);
			Lo_D.SetItem(1,8,-0.00624149021301171);
			Lo_D.SetItem(1,9,-0.0125807519990155);
			Lo_D.SetItem(1,10,0.00333572528500155);
			lowFilter = Lo_D;
																						
			CMatrix Hi_D(1,10);
			Hi_D.SetItem(1,1,0.00333572528500155);
			Hi_D.SetItem(1,2,0.0125807519990155);
			Hi_D.SetItem(1,3,-0.00624149021301171);
			Hi_D.SetItem(1,4,-0.0775714938400652);
			Hi_D.SetItem(1,5,-0.0322448695850295);
			Hi_D.SetItem(1,6,0.242294887066190);
			Hi_D.SetItem(1,7,0.138428145901103);
			Hi_D.SetItem(1,8,-0.724308528438574);
			Hi_D.SetItem(1,9,0.603829269797473);
			Hi_D.SetItem(1,10,-0.160102397974125);
			highFilter = Hi_D;			

			return true;
		}
		else if(WaveletName == L"db6")
		{

		}
		else if(WaveletName == L"db7")
		{

		}
		else if(WaveletName == L"db8")
		{

		}
		else if(WaveletName == L"db9")
		{

		}
		else if(WaveletName == L"db10")
		{

		}
		else if(WaveletName == L"sym2")
		{			
			CMatrix Lo_D(1,4);
			Lo_D.SetItem(1,1,0.482962913144690);
			Lo_D.SetItem(1,2,0.836516303737469);
			Lo_D.SetItem(1,3,0.224143868041857);
			Lo_D.SetItem(1,4,-0.129409522550921);
			lowFilter = Lo_D;
								
			CMatrix Hi_D(1,4);
			Hi_D.SetItem(1,1,-0.129409522550921);
			Hi_D.SetItem(1,2,-0.224143868041857);
			Hi_D.SetItem(1,3,0.836516303737469);
			Hi_D.SetItem(1,4,-0.482962913144690);
			highFilter = Hi_D;			
			return true;

		}
		else if(WaveletName == L"sym3")
		{
								
			CMatrix Lo_D(1,6);
			Lo_D.SetItem(1,1,0.332670552950957);
			Lo_D.SetItem(1,2,0.806891509313339);
			Lo_D.SetItem(1,3,0.459877502119331);
			Lo_D.SetItem(1,4,-0.135011020010391);
			Lo_D.SetItem(1,5,-0.0854412738822415);
			Lo_D.SetItem(1,6,0.0352262918821007);

			lowFilter = Lo_D;								
			CMatrix Hi_D(1,6);
			Hi_D.SetItem(1,1,0.0352262918821007);
			Hi_D.SetItem(1,2,0.0854412738822415);
			Hi_D.SetItem(1,3,-0.135011020010391);
			Hi_D.SetItem(1,4,-0.459877502119331);
			Hi_D.SetItem(1,5,0.806891509313339);
			Hi_D.SetItem(1,6,-0.332670552950957);
			highFilter = Hi_D;			
			return true;

		}
		else if(WaveletName == L"sym4")
		{								
			CMatrix Lo_D(1,8);
			Lo_D.SetItem(1,1,0.0322231006040427);
			Lo_D.SetItem(1,2,-0.0126039672620378);
			Lo_D.SetItem(1,3,-0.0992195435768472);
			Lo_D.SetItem(1,4,0.297857795605277);
			Lo_D.SetItem(1,5,0.803738751805916);
			Lo_D.SetItem(1,6,0.497618667632015);
			Lo_D.SetItem(1,7,-0.0296355276459985);
			Lo_D.SetItem(1,8,-0.0757657147892733);
			lowFilter = Lo_D;	
									
			CMatrix Hi_D(1,8);
			Hi_D.SetItem(1,1,-0.0757657147892733);
			Hi_D.SetItem(1,2,0.0296355276459985);
			Hi_D.SetItem(1,3,0.497618667632015);
			Hi_D.SetItem(1,4,-0.803738751805916);
			Hi_D.SetItem(1,5,0.297857795605277);
			Hi_D.SetItem(1,6,0.0992195435768472);
			Hi_D.SetItem(1,7,-0.0126039672620378);
			Hi_D.SetItem(1,8,-0.0322231006040427);
			highFilter = Hi_D;			
			return true;
		}
		else if(WaveletName == L"sym5")
		{

		}
		else if(WaveletName == L"sym6")
		{

		}
		else if(WaveletName == L"sym7")
		{

		}
		else if(WaveletName == L"sym8")
		{

		}

	}
	return false;
}

int SignalExtension(int typeId,int modeId, CMatrix in,int filterLen,CMatrix &out)
{
    //if((NULL == in)||(NULL == out))
    //    return -1;
	int inLen = in.m_column;
    if(0 != typeId) // 目前只支持一种模型
    	return -1;
    //if(0 != modeId) // 目前只支持一种模型，信号对称拓延  'sym' or 'symh'  	Symmetric-padding (half-point): boundary value symmetric replication
    //	return -1;
	if( inLen < filterLen ) // inLen should lager than or equal extendLen, otherwise no extension
    	return -1;


    int i;
    int extendLen = filterLen - 1;

	CMatrix Out(1,inLen + 2*extendLen);


    if(0 == modeId) // 信号对称拓延
    {
        for(i=0; i<inLen; i++)
        {
			Out.SetItem(1,extendLen+i+1,in(1,i+1));
        }
        for(i=0; i<extendLen; i++)
        {
			Out.SetItem(1,i+1,Out(1,2*extendLen - i));
        	//Out[i]                     = Out[2*extendLen - i - 1];       // 左边沿对称延拓
			Out.SetItem(1,inLen + extendLen + i+1,Out(1,extendLen + inLen  - i));
        	//Out[inLen + extendLen + i] = Out[extendLen + inLen - i - 1]; // 右边沿对称延拓
        }
		out = Out;
        return inLen + 2*extendLen;
    }
    else if(1 == modeId) // 信号周期拓延
    {

		for( i = 0; i < extendLen; i++ )
			Out.SetItem(1,i+1,in(1,inLen-extendLen+i+1));
			//out[i] = in[inLen-extendLen+i];
		for ( i = 0; i < inLen; i++ )
			Out.SetItem(1,extendLen+i+1,in(1,i+1));
			//out[extendLen+i] = in[i];
		out = Out;
        return inLen + extendLen;
    }
	return -1;

}

int Upsampling(CMatrix data,CMatrix &result)
{
	int i;

	CMatrix Result(1,2*data.m_column);
	for( i = 0; i < data.m_column; i++ )
	{
		Result.SetItem(1,2*i+1,data(1,i+1));
		Result.SetItem(1,2*i+2,0);
		//result[2*i+1] = 0;
	}
	result = Result;
	return( 2*data.m_column );
}

int Downsampling(CMatrix data,CMatrix &result)
{
	int i, m;

	m = data.m_column/2;
	CMatrix Result(1,m);
	for( i = 0; i < m; i++ )
		Result.SetItem(1,i+1,data(1,i*2 + 2));
		//result[i] = data[i*2 + 1];
	result = Result;
	return( m );
}


void Conv1(int shapeId,CMatrix inSignal,CMatrix inFilter,CMatrix &outConv, int &convLen)   
{
	if((NULL == inSignal.m_Matrix)||(NULL == inFilter.m_Matrix))
        return;

    int n,k,kmin,kmax,p;
	int signalLen = inSignal.m_column;
	int filterLen = inFilter.m_column;

    if(0 == shapeId)      // 对于MATLAB conv(...,'shape')  -----full
    {
    	convLen = signalLen + filterLen - 1;
		CMatrix OutConv(1,convLen);
    	for (n = 0; n < convLen; n++)
    	{
    		//outConv[n] = 0;
			
    	    kmin = (n >= filterLen - 1) ? n - (filterLen - 1) : 0;
    	    kmax = (n < signalLen - 1) ? n : signalLen - 1;

			double temp = 0;
    	    for (k = kmin; k <= kmax; k++)
    	    {
    	    	//outConv[n] += inSignal[k] * inFilter[n - k];
				temp+= inSignal(1,k+1)*inFilter(1,n-k+1);
    	    }
			OutConv.SetItem(1,n+1,temp);
    	}
		outConv = OutConv;
    }
    else if(1 == shapeId) // 对于MATLAB conv(...,'shape')  -----valid
    {
    	convLen = signalLen - filterLen + 1;
		CMatrix OutConv(1,convLen);
    	for (n = filterLen - 1; n < signalLen; n++)
    	{
    		p = n - filterLen + 1;
    		//outConv[p] = 0;

    	    kmin = (n >= filterLen - 1) ? n - (filterLen - 1) : 0;
    	    kmax = (n < signalLen - 1) ? n : signalLen - 1;
			double temp = 0;
    	    for (k = kmin; k <= kmax; k++)
    	    {
    	    	//outConv[p] += inSignal[k] * inFilter[n - k];
				temp += inSignal(1,k+1)*inFilter(1,n-k+1);
    	    }
			OutConv.SetItem(1,p+1,temp);
    	}
		outConv = OutConv;
    }
    else
    	return ;
	
}


int dwt(CMatrix sourceData, const Filter &m_db, CMatrix &cA, CMatrix &cD)
{
	int dataLen = sourceData.m_column;

    if(dataLen < 2)
        return -1;
	if((NULL == sourceData.m_Matrix))
        return -1;

    //m_db = db;

	int filterLen = m_db.lowFilter.m_column;
    //int i, n;
    int decLen = (dataLen+filterLen-1)/2;
    int convLen = 0;
    CMatrix extendData(1,dataLen+2*filterLen-2);
    CMatrix convDataLow(1,dataLen+filterLen-1);
    CMatrix convDataHigh(1,dataLen+filterLen-1);

    // 1.边缘延拓
    SignalExtension(0, 0 , sourceData, filterLen, extendData);

    // 2.卷积运算
    Conv1(1, extendData,m_db.lowFilter,convDataLow,convLen);
    Conv1(1, extendData,m_db.highFilter,convDataHigh,convLen);

    // 3.下采样
    Downsampling(convDataLow,cA);
    Downsampling(convDataHigh,cD);

    return decLen;
}

int wavedec(CMatrix sourceData,size_t Level,const CString wname,CMatrix &Coef,CMatrix &L)
{
	if(sourceData.m_Matrix == NULL)
		return false;

	Filter filter;
	if(filter.Init(wname,L"d") == 0) //初始化小波分解滤波器组
		return false;

	int sourceDataLen = sourceData.m_column;
	CMatrix L_temp(1,Level+2);
	L_temp.SetItem(1,Level+2,sourceDataLen);
	CMatrix Coef_temp;
	CMatrix x = sourceData;
	CMatrix cA,cD;
	CMatrix tempc;
	for (size_t k = 0;k< Level;++k)
	{	
		dwt(x,filter,cA,cD);
		tempc = Coef_temp;
		Coef_temp = cD.Transposition();
		Coef_temp.AddRows(tempc);
		L_temp.SetItem(1,Level+2-k-1,cD.m_column);
		x = cA;
	}	
	tempc = Coef_temp;
	Coef_temp = cA.Transposition();
	Coef_temp.AddRows(tempc);	
	L_temp.SetItem(1,1,cA.m_column);
	Coef = Coef_temp.Transposition();
	L = L_temp;
	return true;
}
int waverec(CMatrix &Coef,CMatrix &L,const CString wname,CMatrix &RecData)
{
	return appcoef(Coef,L,wname,0,RecData);
}


int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData)
{
	if((NULL == cA.m_Matrix)||(NULL == cD.m_Matrix))
        return -1;

    //m_db = db;
	int filterLen = db.highFilter.m_column;
	int cALength = cA.m_column;

    int i,k;
   // int k;
    int recLen = 2*cALength-filterLen+2;

	CMatrix RecData(1,recLen);

    int convLen = 0;
    CMatrix convDataLow(1,recLen+filterLen-1);
    CMatrix convDataHigh(1,recLen+filterLen-1);

    CMatrix cATemp(1,2*cALength);
    CMatrix cDTemp(1,2*cALength);

   // memset(convDataLow, 0, (recLen+filterLen-1)*sizeof(double)); // 清0
   // memset(convDataHigh, 0, (recLen+filterLen-1)*sizeof(double)); // 清0
   // memset(cATemp, 0, 2*cALength*sizeof(double)); // 清0
  //  memset(cDTemp, 0, 2*cALength*sizeof(double)); // 清0

    // 1.隔点插0
    Upsampling(cA, cATemp);
    Upsampling(cD, cDTemp);

    // 2.卷积运算
    Conv1(0, cATemp, db.lowFilter ,convDataLow, convLen);
    convLen = 0;
    Conv1(0, cDTemp, db.highFilter ,convDataHigh, convLen);

    // 3.抽取结果及求和——实现类似MATLAB中的wkeep1(s,len,'c')的功能
    k = (convLen - recLen)/2;
    for(i=0; i<recLen; i++)
    {
		RecData.SetItem(1,i+1,convDataLow(1,i+k+1)+convDataHigh(1,i+k+1));
    	//recData[i] = convDataLow[i + k] + convDataHigh[i + k];
    }	
	recData = RecData;

    return recLen;
}

int idwt(const CMatrix &cA,const CMatrix &cD, Filter &db, CMatrix &recData,size_t reclen)
{
	if((NULL == cA.m_Matrix)||(NULL == cD.m_Matrix))
        return -1;

    //m_db = db;
	int filterLen = db.highFilter.m_column;
	int cALength = cA.m_column;

    int i;
    int k;


    int recLen = 2*cALength-filterLen+2;

	CMatrix RecData(1,recLen);

    int convLen = 0;
    CMatrix convDataLow(1,recLen+filterLen-1);
    CMatrix convDataHigh(1,recLen+filterLen-1);

    CMatrix cATemp(1,2*cALength);
    CMatrix cDTemp(1,2*cALength);

    // 1.隔点插0
    Upsampling(cA, cATemp);
    Upsampling(cD, cDTemp);

    // 2.卷积运算
    Conv1(0, cATemp, db.lowFilter ,convDataLow, convLen);
    convLen = 0;
    Conv1(0, cDTemp, db.highFilter ,convDataHigh, convLen);

    // 3.抽取结果及求和——实现类似MATLAB中的wkeep1(s,len,'c')的功能
    k = (convLen - recLen)/2;
    for(i=0; i<recLen; i++)
    {
		RecData.SetItem(1,i+1,convDataLow(1,i+k+1)+convDataHigh(1,i+k+1));
    	//recData[i] = convDataLow[i + k] + convDataHigh[i + k];
    }	
	recData = RecData;
	recData = recData.GetBlock(1,1,reclen,1);
    return recLen;
}


int wrcoef(const CString ReconType,CMatrix &Coef,CMatrix &L,const CString wname,size_t Level ,CMatrix &ReconstructData)
{
	Filter filter;

	//size_t maxLevel = L.m_column - 2;//最大层数
	size_t rmax = L.m_column ;
	size_t nmax = rmax-2;

	if(filter.Init(wname,L"r") == false)
		return -1;
	if(Level>nmax||Level<=0)  
		return -1;
	//Initialization
	int imin = rmax-Level;
	CMatrix Coef_tmp;
	CMatrix filter1;
	if(ReconType == L"a")
	{
		appcoef(Coef,L,wname,Level,Coef_tmp);
		filter1 = filter.lowFilter;
	}
	else if(ReconType == L"d")
	{
		detcoef(Coef,L,Level,Coef_tmp);
		filter1 = filter.highFilter;
	}
	else
		return -1;

	CMatrix ReconsData_tmp ;
    int convLen = 0;

   // CMatrix convDataHigh(1,recLen+filterLen-1);

	CMatrix cTemp(1,2*filter1.m_column);
    // 1.隔点插0
    Upsampling(Coef_tmp, cTemp);
    // 2.卷积运算
    Conv1(0, cTemp, filter1 ,ReconsData_tmp, convLen);

	ReconsData_tmp = ReconsData_tmp.GetBlock(1,1,L(1,imin+1),1);
	for(int k = 2; k<= Level;++k)
	{
		//cTemp = 
		// 1.隔点插0
		Upsampling(ReconsData_tmp, cTemp);
		// 2.卷积运算
		Conv1(0, cTemp, filter.lowFilter ,ReconsData_tmp, convLen);
		ReconsData_tmp = ReconsData_tmp.GetBlock(1,1,L(1,imin+k),1);
	}
	/*imin = rmax-n;
x  = upsconv1(x,F1,l(imin+1),dwtATTR);
for k=2:n , x = upsconv1(x,Lo_R,l(imin+k),dwtATTR); end*/
	ReconstructData = ReconsData_tmp;

	return 0;
}
int appcoef(CMatrix &Coef,CMatrix &L,const CString wname,size_t Level,CMatrix &A)
{
	Filter filter;

	//size_t maxLevel = L.m_column - 2;//最大层数
	int rmax = L.m_column ;
	int nmax = rmax-2;

	if(filter.Init(wname,L"r") == false)
		return -1;
	if(Level>nmax||Level<0)  
		return -1;
	//Initialization
	CMatrix A_tmp = Coef.GetBlock(1,1,L(1,1),1);


	int imax = rmax+1;

	CMatrix D_tmp;
	for(int p = nmax;p>=Level+1;--p)
	{
		CMatrix A_tmp2;
		detcoef(Coef,L,p,D_tmp);	
		idwt(A_tmp,D_tmp,filter,A_tmp2,L(1,imax-p));
		A_tmp = A_tmp2;
	}
	A = A_tmp;
	return 0;
}

int detcoef(const CMatrix &Coef,const CMatrix &L,const size_t &Level,CMatrix &D)
{
	size_t maxLevel = L.m_column - 2;

	if(Level>maxLevel||Level<=0)
		return -1;
	//L(1,1);
	size_t last = L.Sum() - L(1,L.m_column);
	int i = 0;
	for(;i<Level-1;++i)
	{
		last-=L(1,L.m_column-1-i);
	}
	int first = last-(int)L(1,L.m_column-1-i)+1;	
	//
	CMatrix tmp =  Coef.GetBlock(first,1,last,1);
	D = tmp;
	return 0;
}

double thselect(const CMatrix &SignalData,const CString &tptr)
{
/*    case 'rigrsure'
        sx2 = sort(abs(x)).^2;
        risks = (n-(2*(1:n))+(cumsum(sx2)+(n-1:-1:0).*sx2))/n;
        [~,best] = min(risks);
        thr = sqrt(sx2(best));
*/

	double n = SignalData.m_column;
	if(tptr == L"rigrsure")
	{
		CMatrix SignalDataabs = Abs(SignalData);
		CMatrix SignalDataabssort = Sort(SignalDataabs,1,L"ascend");
		CMatrix sx2(SignalDataabssort);
		for(int i =0;i<SignalDataabssort.m_column;++i)
		{
			sx2.SetItem(1,i+1,sx2(1,i+1)*sx2(1,i+1));
		}
		CMatrix one2n(1,n);
		for(int i = 0; i<n; ++i)
		{
			one2n.SetItem(1,i+1,i+1);
		}

		CMatrix n_12zero(1,n);
		for(int i = 0; i<n; ++i)
		{
			n_12zero.SetItem(1,i+1,n-i-1);
		}

		CMatrix th3d(1,n);
		for(int i = 0; i<n; ++i)
		{
			th3d.SetItem(1,i+1,n_12zero(1,i+1)*sx2(1,i+1));
		}

		CMatrix risks = (CMatrix(1,sx2.m_column,n) - 2*one2n+ ( sx2.cumsum(1)+th3d))/n;

		CMatrixIndex minIndex;
		risks.FindMin(minIndex);
		//threshold =

		return  sqrt(sx2(minIndex));
	}
	else if(tptr == L"heursure")
	{
		
		double hthr = sqrt(log(n));
		double eta = (SignalData.Square()-n)/n;
		double crit = pow((log(n)/log(2.0)),1.5)/sqrt(n);
		if(eta<crit)
			return  hthr;
		else
			return min(thselect(SignalData,L"rigrsure"),hthr);
	}
	else if(tptr == L"sqtwolog")
	{		
		return sqrt(2*log(n));
	}
	else if(tptr == L"minimaxi")
	{
		if(n<=32)
			return  0;
		else
			return 0.3936 + 0.1829*(log(n)/log(2.0));
	}
	else
	{
		throw invalid_argument("阈值类型错误");
	}
}


CMatrix wthresh(const CMatrix &x,const CString &sorh,const double t) //注意x为行向量
{
	CMatrix Y(x.m_row,x.m_column);
	if(sorh == L"s") //软阈值降噪
	{
		for(int i = 0 ;i<x.m_column;++i)
		{
			double tmp = abs(x(1,i+1))-t;
			tmp = (tmp+abs(tmp))/2;
			Y.SetItem(1,i+1,sign(x(1,i+1))*tmp);
		}
	}
	else if(sorh == L"h") //硬阈值降噪
	{
		for(int i = 0 ;i<x.m_column;++i)
		{
			Y.SetItem(1,i+1,x(1,i+1)*(abs(x(1,i+1)>t)));
		}
	}
	else
		throw invalid_argument("降噪类型错误");
	return Y;
}

double wnoisest(const CMatrix &Coef,const CMatrix &L,int Level)
{
	CMatrix D_Level;
	detcoef(Coef,L,Level,D_Level);
	double med = median(Abs(D_Level));
	return median(Abs(D_Level))/0.6745;
}

CMatrix wden(const CMatrix &X,const CString &tptr,const CString &sorh,const CString & scal,const size_t &n,const CString & wname)
{
	//int n = Level;
	CMatrix s;

	CMatrix Coef;
	CMatrix L;
	wavedec(X,n,wname,Coef,L);

	if(scal == L"one")
	{
		s = CMatrix(1,n,1);
	}
	else if(scal == L"sln")
	{
		s = CMatrix(1,n,1)*wnoisest(Coef,L,1);
	}
	else if(scal == L"mln")
	{
		s = CMatrix(1,n);
		for(size_t i =0;i<n;++i)
		{
			s.SetItem(1,i+1,wnoisest(Coef,L,i+1));
		}

	}
	else
	{
		throw invalid_argument("错误的输入参数");
	}

	CMatrix first = L.cumsum(2)+CMatrix(1,L.m_column,1);
	//first = first(end-2:-1:1);
	CMatrix first_tmp(1,L.m_column-2);
	for(int i =0 ;i<first_tmp.m_column;++i)
	{
		first_tmp.SetItem(1,i+1,first(1,L.m_column-2-i));
	}
	first = first_tmp;
	CMatrix ld(1,L.m_column-2);
	for(int i = 1;i<L.m_column-1;++i)
	{
		ld.SetItem(1,i,L(1,L.m_column-i));
	}
	CMatrix last = first+ld-CMatrix(1,L.m_column-2,1);

	CMatrix  cxd = Coef;
	CMatrix lxd = L;	
	for(size_t k = 0;k<n;++k)
	{
		int first_tmp = first(1,k+1);
		int last_tmp = last(1,k+1);
		double thr;
		if(tptr == L"sqtwolog" || tptr == L"minimaxi")
			thr = thselect(Coef,tptr);
		else
			if(s(1,k+1) < 1.4901e-08*(Coef.GetBlock(first_tmp,1,last_tmp,1).GetMax()))
				thr = 0;
			else
				thr = thselect(Coef.GetBlock(first_tmp,1,last_tmp,1)/s(1,k+1),tptr);
		thr = thr * s(1,k+1);     	
		CMatrix cxd_tmp = wthresh(Coef.GetBlock(first_tmp,1,last_tmp,1),sorh,thr);
		for(int l =first_tmp;l<=last_tmp;++l)
		{
			cxd.SetItem(1,l,cxd_tmp(1,l-first_tmp+1));
		}
	}
	CMatrix Xden;
	waverec(cxd,lxd,wname,Xden);
	return Xden;
}
