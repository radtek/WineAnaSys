// DBManageSheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageSheet.h"


// CDBManageSheet

IMPLEMENT_DYNAMIC(CDBManageSheet, CPropertySheet)

CDBManageSheet::CDBManageSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDBManageSheet::CDBManageSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CDBManageSheet::~CDBManageSheet()
{

}


BEGIN_MESSAGE_MAP(CDBManageSheet, CPropertySheet)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDBManageSheet ��Ϣ�������


void CDBManageSheet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertySheet::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
}


void CDBManageSheet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPropertySheet::OnPaint()
}
