#ifndef IMPLEMENTFUNC_H
#define IMPLEMENTFUNC_H
#include "stdafx.h"
#include "Client.h"
#include "Matrix.h"
BOOL DisplayPic(CString strFileName, HWND hWnd, int nScrWidth, int nScrHeight);
BOOL GetDirTime(CString DirName, SYSTEMTIME &stime);

BOOL SortCArray(CArray<CString,CString> &A);
BOOL SortCArrayByFirst(CArray<CString,CString> &A,CArray<CString,CString> &B);

int CString2Matrix(CMatrix &A,const CString &cstr,const CString &Sep);

CString Char2CString(const char *buf);

char * CString2Char(const CString cString);

//文件夹操作 移动 删除 复制
BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
BOOL DeleteFolder(LPCTSTR lpszPath);
BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);

void ClipChildren(HWND hwnd, HRGN parent);


#endif