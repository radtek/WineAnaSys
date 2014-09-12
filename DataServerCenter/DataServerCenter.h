// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// DataServerCenter.h : DataServerCenter Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "Matrix.h"
#include <Windows.h>
// CDataServerCenterApp:
// �йش����ʵ�֣������ DataServerCenter.cpp
//
#define SPEC_LENTH 869
class CDataServerCenterApp : public CWinAppEx
{
public:
	CDataServerCenterApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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

CString Char2CString(const char *buf);

char * CString2Char(const CString cString);

CString GetDate();


//�ļ��в��� �ƶ� ɾ�� ����
BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
BOOL DeleteFolder(LPCTSTR lpszPath);
BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);

void ClipChildren(HWND hwnd, HRGN parent);

