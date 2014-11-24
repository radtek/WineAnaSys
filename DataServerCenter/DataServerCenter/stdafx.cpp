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

// stdafx.cpp : 只包括标准包含文件的源文件
// DataServerCenter.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
CArray<CString,CString> g_ArrayComponnetName;

char skin_path[MAX_PATH] = "skin\\bOzen.ssk";

int GetLandscapeMode()
{
	int tmp = -1;
	PRINTDLG pd;
	pd.lStructSize=(DWORD)sizeof(PRINTDLG);
	BOOL bRet=AfxGetApp()->GetPrinterDeviceDefaults(&pd);
	if(bRet)
	{
		// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(pd.hDevMode);
		// get orientation to landscape
		tmp = pDevMode->dmOrientation;
		::GlobalUnlock(pd.hDevMode);
	}
	return tmp;
}


void SetLandscapeMode(int PrintMode)
{
	PRINTDLG pd;
	pd.lStructSize=(DWORD)sizeof(PRINTDLG);
	BOOL bRet=AfxGetApp()->GetPrinterDeviceDefaults(&pd);
	if(bRet)
	{
		// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(pd.hDevMode);
		// set orientation to landscape
		if(PrintMode==1)//纵向打印
			pDevMode->dmOrientation=DMORIENT_PORTRAIT;
		else if(PrintMode==2)//横向打印
			pDevMode->dmOrientation=DMORIENT_LANDSCAPE;	  
		::GlobalUnlock(pd.hDevMode);
	}
}