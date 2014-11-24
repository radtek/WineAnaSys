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

// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// DataServerCenter.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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
		if(PrintMode==1)//�����ӡ
			pDevMode->dmOrientation=DMORIENT_PORTRAIT;
		else if(PrintMode==2)//�����ӡ
			pDevMode->dmOrientation=DMORIENT_LANDSCAPE;	  
		::GlobalUnlock(pd.hDevMode);
	}
}