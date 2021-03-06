// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DataServerCenter.h : DataServerCenter 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "Matrix.h"
#include <Windows.h>
// CDataServerCenterApp:
// 有关此类的实现，请参阅 DataServerCenter.cpp
//
#define SPEC_LENTH 869
class CDataServerCenterApp : public CWinAppEx
{
public:
	CDataServerCenterApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDataServerCenterApp theApp;

BOOL DisplayPic(CString strFileName, HWND hWnd, int nScrWidth, int nScrHeight);
BOOL GetDirTime(CString DirName, SYSTEMTIME &stime);

BOOL SortCArray(CArray<CString,CString> &A);
BOOL SortCArrayByFirst(CArray<CString,CString> &A,CArray<CString,CString> &B);

int CString2Matrix(CMatrix &A,const CString &cstr,const CString &Sep);

double PredictProject(const CString &projectpath,const CString &specName);

int PredictProject(const CString &SpecPath,CMatrix &ComponentConcentration);

CString Char2CString(const char *buf);

char * CString2Char(const CString cString);

CString GetDate();


//文件夹操作 移动 删除 复制
BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
BOOL DeleteFolder(LPCTSTR lpszPath);
BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);

void ClipChildren(HWND hwnd, HRGN parent);

