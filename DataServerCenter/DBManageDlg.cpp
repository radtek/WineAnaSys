// DBManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageDlg.h"
#include "afxdialogex.h"
#include "DataServerCenterView.h"
#include "SkinPPWTL.h"
#include "MainUIDlg.h"
// CDBManageDlg �Ի���

IMPLEMENT_DYNAMIC(CDBManageDlg, CDialogEx)

CDBManageDlg::CDBManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBManageDlg::IDD, pParent)
	,m_CDBManageSheet(L"")
{
}

CDBManageDlg::~CDBManageDlg()
{
}

void CDBManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBManageDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDBManageDlg ��Ϣ�������


BOOL CDBManageDlg::OnInitDialog()
{
	skinppRemoveSkin(); //�Ƴ�Ƥ��
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_CDBManageSheet.AddPage(&m_DBManagePageTrue);
	m_CDBManageSheet.AddPage(&m_DBManagePageFake);
	m_CDBManageSheet.Create(this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0);
    m_CDBManageSheet.ModifyStyleEx(0,WS_EX_CONTROLPARENT);  

	CRect rc;
    GetClientRect(&rc);
	m_CDBManageSheet.MoveWindow(rc);
	m_CDBManageSheet.GetTabControl()->MoveWindow(rc);  
	rc.left+=10;    //�ı�rect�Ĵ�С����ֹCPropertyPage��סCPropertySheet  
	rc.top+=20;  
	rc.bottom-=10;  
	rc.right-=10;    
	m_CDBManageSheet.SetActivePage(SelectPage);
	m_CDBManageSheet.GetActivePage()->MoveWindow(rc);  

	skinppLoadSkin(skin_path); //����Ƥ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDBManageDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SendMessage(((CDataServerCenterView*)pwnd)->pDlg->m_hWnd,WM_CONTINUEPREDICTPROJECT,NULL,NULL);
	CDialogEx::OnClose();
}
