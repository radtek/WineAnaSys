#include "stdafx.h"
#include "ImplementFunc.h"
#include "Matrix.h"
#include "WineTypeIndex.h"
BOOL DisplayPic(CString strFileName, HWND hWnd, int nScrWidth, int nScrHeight)
{
	HDC hDC=::GetDC(hWnd);

	IPicture *pPicture;
	IStream *pStream;

	HRESULT hResult;

	HANDLE hFile = NULL;
	DWORD dwFileSize,dwByteRead;

	hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	dwFileSize = GetFileSize(hFile, NULL);

	HGLOBAL hGlobal = ::GlobalAlloc(GHND, dwFileSize);
	if(hGlobal == NULL)
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}

	LPVOID lpData = NULL;

	lpData = ::GlobalLock(hGlobal);

	ReadFile(hFile, lpData, dwFileSize, &dwByteRead, NULL);
	::GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{
		AfxMessageBox(L"流初始化失败！");
		return E_FAIL;
	}

	hResult = OleLoadPicture(pStream, dwFileSize, TRUE, IID_IPicture, (LPVOID*) &pPicture);

	if(FAILED(hResult))
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度
	OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度
	pPicture->get_Width(&hmWidth);
	pPicture->get_Height(&hmHeight);

	//将图形输出到屏幕上
	hResult = pPicture->Render(hDC, 0, 0, nScrWidth, nScrHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);

	CloseHandle(hFile);//关闭打开的文件

	pPicture->Release();
	// Free memory.
	::GlobalFree(hGlobal);

	if (SUCCEEDED(hResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(L"图形文件装载出错！");
		return E_FAIL;
	}
}
BOOL GetDirTime(CString DirName, SYSTEMTIME &stime)
{

	// 打开文件夹
	HANDLE hDir = CreateFile (DirName, GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE, 
		  NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL); 
	FILETIME lpCreationTime; // 文件夹的创建时间
	FILETIME lpLastAccessTime; // 对文件夹的最近访问时间 
	FILETIME lpLastWriteTime; // 文件夹的最近修改时间 
	// 获取文件夹时间属性信息
	BOOL retval=GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
	if (retval)
	{ 
		FILETIME ftime; 
		FileTimeToLocalFileTime(&lpLastWriteTime, &ftime); // 转换成本地时间 
		FileTimeToSystemTime(&ftime, &stime); // 转换成系统时间格式
	} 
	CloseHandle(hDir); // 关闭打开过的文件夹
	return retval;
}


//对
BOOL SortCArray(CArray<CString,CString> &A)
{
	CArray<CString,CString> B;
	CArray<CString,CString> C;
	CArray<int,int> D;

	for(int i=0;i<A.GetCount();i++)
	{
		CString temp=A[i];
		int n=_ttoi(temp);
		int m=_ttoi(A[i]);

		if(A[i].GetAt(0)<'0'||A[i].GetAt(0)>'9')
		{
			B.Add(A[i]);
		}
		else
		{
			D.Add(_ttoi(A[i]));
			C.Add(A[i]);
		}
	}
	for(int i=0;i<D.GetCount();i++)
	{
		int minIndex=i;
		for(int j=i+1;j<D.GetCount();j++)
		{
			if(D[minIndex]>D[j])
			{
				minIndex=j;
			}
		}
		A.SetAt(i,C[minIndex]);
		

		if(minIndex!=i)
		{
			int tempD=D[i];
			D.SetAt(i,D[minIndex]);
			D.SetAt(minIndex,tempD);

			CString tempC=C[i];
			C.SetAt(i,C[minIndex]);
			C.SetAt(minIndex,tempC);
		}

	}
	for(int i=D.GetCount();i<A.GetCount();i++)
	{
		A[i]=B[i-D.GetCount()];
	}
	return 1;
}

BOOL SortCArrayByFirst(CArray<CString,CString> &A,CArray<CString,CString> &B)
{
	CArray<CString,CString> A1;
	CArray<CString,CString> A2;
	CArray<CString,CString> B1;
	CArray<CString,CString> B2;
	CArray<int,int> D;

	for(int i=0;i<A.GetCount();i++)
	{
		CString temp=A[i];
		int n=_ttoi(temp);
		int m=_ttoi(A[i]);

		if(A[i].GetAt(0)<'0'||A[i].GetAt(0)>'9')
		{
			A2.Add(A[i]);
			B2.Add(B[i]);
		}
		else
		{
			D.Add(_ttoi(A[i]));
			A1.Add(A[i]);
			B1.Add(B[i]);
		}
	}
	for(int i=0;i<D.GetCount();i++)
	{
		int minIndex=i;
		for(int j=i+1;j<D.GetCount();j++)
		{
			if(D[minIndex]>D[j])
			{
				minIndex=j;
			}
		}
		A[i]=A1[minIndex];
		B[i]=B1[minIndex];

		if(minIndex!=i)
		{
			int tempD=D[i];
			D.SetAt(i,D[minIndex]);
			D.SetAt(minIndex,tempD);

			CString tempA1=A1[i];
			A1.SetAt(i,A1[minIndex]);
			A1.SetAt(minIndex,tempA1);

			CString tempB1=B1[i];
			B1.SetAt(i,B1[minIndex]);
			B1.SetAt(minIndex,tempB1);
		}

	}
	for(int i=D.GetCount();i<A.GetCount();i++)
	{
		A[i]=A2[i-D.GetCount()];
		B[i]=B2[i-D.GetCount()];

	}
	return 1;
}


int CString2Matrix(CMatrix &A,const CString &cstr,const CString &Sep)
{
	int DataStrlenth=cstr.GetLength();
	wchar_t *str1=(wchar_t *)malloc(sizeof(wchar_t)*(DataStrlenth*2));
	wcscpy(str1,CT2CW(cstr));
	wchar_t *token; 
	token=wcstok(str1,Sep);
	int row=1;
	int column=0;
	while(token!=NULL&&row<=A.m_row)
	{
		double a=_wtof(token);
		if(column<A.m_column)
		{
			column++;
			A.SetItem(row,column,a);
			
		}
		else
		{
			column=1;
			row++;
			A.SetItem(row,column,a);	
		}
		token=wcstok(NULL,Sep);
	}
	free(str1);
	str1=NULL;
	if(column<A.m_column||row<A.m_row)
	{
		return -1;
	}
	if(token!=NULL)
	{
		return 0;
	}
	return 1;	
}


CString Char2CString(const char *buf)
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int bufLen = strlen(buf);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP,0,buf,bufLen,NULL,0);
	TCHAR *buftemp = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP,0,buf,bufLen,buftemp,len);
	buftemp[len] = '\0';  //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buftemp);
	//删除缓冲区
	delete []buftemp;
	return pWideChar;
}

char * CString2Char(const CString cString) 
{
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),NULL,0,NULL,NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pchar = new char[len+2];   //以字节为单位
	//char * pchar = (char*) malloc(sizeof(char)*(len+1));   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength(),pchar,len,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,cString,cString.GetLength() + 1 ,pchar,len + 1 ,NULL,NULL);
	pchar[len+1] = '/0';   //多字节字符以'/0'结束

	//delete[] pchar;
	return pchar;  //此函数申请了内存空间，需要在函数外delete
}

BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	//文件夹旧位置
	int iPathLen = _tcslen(lpszFromPath);
	TCHAR tczFromFolder[MAX_PATH+1];
	ZeroMemory(tczFromFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFromFolder, lpszFromPath);
	tczFromFolder[iPathLen] = _T('\0');
	tczFromFolder[iPathLen+1] = _T('\0');

	//文件夹新位置
	iPathLen = _tcslen(lpszToPath);
	TCHAR tczToFolder[MAX_PATH+1];
	ZeroMemory(tczToFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczToFolder, lpszToPath);
	tczToFolder[iPathLen] = _T('\0');
	tczToFolder[iPathLen+1] = _T('\0');

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = tczFromFolder;
	FileOp.pTo = tczToFolder;
	FileOp.wFunc = FO_MOVE;
	return SHFileOperation(&FileOp) == 0;
}

BOOL DeleteFolder(LPCTSTR lpszPath)
{
	int iPathLen = _tcslen(lpszPath);

	TCHAR tczFolder[MAX_PATH+1];
	ZeroMemory(tczFolder, (MAX_PATH+1)*sizeof(TCHAR));
	_tcscpy(tczFolder, lpszPath);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen+1] = _T('\0');

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = tczFolder;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}


BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszFromPath;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp) == 0;
}

void ClipChildren(HWND hwnd, HRGN parent)
{
	HWND child = ::GetWindow(hwnd, GW_CHILD);
	while(child)
	{
		RECT rect;
		GetWindowRect(child, &rect);
		ScreenToClient(hwnd, (LPPOINT)&rect.left);
		ScreenToClient(hwnd, (LPPOINT)&rect.right);
		HRGN rgn = CreateRectRgnIndirect(&rect);
		CombineRgn(parent, parent, rgn, RGN_DIFF);
		DeleteObject(rgn);
		child = GetWindow(child, GW_HWNDNEXT);
	}
}
