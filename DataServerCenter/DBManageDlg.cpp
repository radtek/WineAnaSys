// DBManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBManageDlg.h"
#include "afxdialogex.h"
#include "DataServerCenterView.h"
#include "SkinPPWTL.h"
#include "MainUIDlg.h"
// CDBManageDlg 对话框

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


// CDBManageDlg 消息处理程序


BOOL CDBManageDlg::OnInitDialog()
{
	skinppRemoveSkin(); //移除皮肤
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_CDBManageSheet.AddPage(&m_DBManagePageTrue);
	m_CDBManageSheet.AddPage(&m_DBManagePageFake);
	m_CDBManageSheet.Create(this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0);
    m_CDBManageSheet.ModifyStyleEx(0,WS_EX_CONTROLPARENT);  

	CRect rc;
    GetClientRect(&rc);
	m_CDBManageSheet.MoveWindow(rc);
	m_CDBManageSheet.GetTabControl()->MoveWindow(rc);  
	rc.left+=10;    //改变rect的大小，防止CPropertyPage盖住CPropertySheet  
	rc.top+=20;  
	rc.bottom-=10;  
	rc.right-=10;    
	m_CDBManageSheet.SetActivePage(SelectPage);
	m_CDBManageSheet.GetActivePage()->MoveWindow(rc);  

	skinppLoadSkin(skin_path); //加载皮肤
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDBManageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SendMessage(((CDataServerCenterView*)pwnd)->pDlg->m_hWnd,WM_CONTINUEPREDICTPROJECT,NULL,NULL);
	CDialogEx::OnClose();
}
