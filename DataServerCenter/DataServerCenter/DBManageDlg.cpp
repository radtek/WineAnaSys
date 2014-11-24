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
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDBManageDlg 消息处理程序


BOOL CDBManageDlg::OnInitDialog()
{
	//skinppRemoveSkin(); //移除皮肤
	CDialogEx::OnInitDialog();

	//设置对话框图标
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE); //设置为大图标

	//设置大小
	double ratioX = 0.65;
	double ratioY = 0.85;
	int xSize=::GetSystemMetrics(SM_CXSCREEN);
	int ySize=::GetSystemMetrics(SM_CYSCREEN);
	int cx=xSize*ratioX;
	int cy=ySize*ratioY;
	this->MoveWindow((xSize-cx)/2,(ySize-cy)/2,xSize*ratioX,ySize*ratioY);

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

	//skinppLoadSkin(skin_path); //加载皮肤
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDBManageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SendMessage(((CDataServerCenterView*)pwnd)->pDlg->m_hWnd,WM_CONTINUEPREDICTPROJECT,NULL,NULL);
	CDialogEx::OnClose();
}


void CDBManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rc;
	GetClientRect(&rc);
	m_CDBManageSheet.MoveWindow(rc);
	m_CDBManageSheet.GetTabControl()->MoveWindow(rc);

	//rc.left+=10; //改变rect的大小，防止CPropertyPage盖住CPropertySheet  
	//rc.top+=20;  
	//rc.bottom-=10;  
	//rc.right-=10;  
	//m_CDBManageSheet.GetActivePage()->MoveWindow(rc);   
}
