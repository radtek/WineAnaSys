
// MSChartTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMSChartTestApp:
// �йش����ʵ�֣������ MSChartTest.cpp
//

class CMSChartTestApp : public CWinApp
{
public:
	CMSChartTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMSChartTestApp theApp;