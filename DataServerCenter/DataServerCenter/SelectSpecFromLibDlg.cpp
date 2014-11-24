// SelectSpecFromLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SelectSpecFromLibDlg.h"
#include "afxdialogex.h"
#include "SkinPPWTL.h"

// CSelectSpecFromLibDlg 对话框

IMPLEMENT_DYNAMIC(CSelectSpecFromLibDlg, CDialogEx)

CSelectSpecFromLibDlg::CSelectSpecFromLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectSpecFromLibDlg::IDD, pParent)
	,m_SelectSpecFromLibSheet(L"")
	,m_SelPage(0)
{
	//m_TrueWineLib.m_strSort=L"WineName";
}

CSelectSpecFromLibDlg::~CSelectSpecFromLibDlg()
{
}

void CSelectSpecFromLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_btnImportSpec);
	//DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSelectSpecFromLibDlg, CDialogEx)
	ON_WM_PAINT()
//	ON_COMMAND(IDOK, &CSelectSpecFromLibDlg::OnIdok)
ON_BN_CLICKED(IDOK, &CSelectSpecFromLibDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectSpecFromLibDlg 消息处理程序




BOOL CSelectSpecFromLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化

	m_SelectSpecFromTrueLibDlg.m_pParent = this;
	m_SelectSpecFromLibSheet.AddPage(&m_SelectSpecFromTrueLibDlg);
	m_SelectSpecFromLibSheet.AddPage(&m_SelectSpecFromFakeLibDlg);
	m_SelectSpecFromLibSheet.Create(this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0);
	m_SelectSpecFromLibSheet.ModifyStyleEx(0,WS_EX_CONTROLPARENT);  

	CRect rc;
	GetClientRect(&rc);
	//rc.left+=10;    //改变rect的大小，防止CPropertyPage盖住CPropertySheet  
	//rc.top+=20;  
	rc.bottom-=50;  
	//rc.right-=10; 
	m_SelectSpecFromLibSheet.MoveWindow(rc);
	m_SelectSpecFromLibSheet.GetTabControl()->MoveWindow(rc);  
	rc.left+=10;    //改变rect的大小，防止CPropertyPage盖住CPropertySheet  
	rc.top+=20;  
	rc.bottom-=10;  
	rc.right-=10;   

	m_SelectSpecFromLibSheet.GetActivePage()->MoveWindow(rc);  

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSelectSpecFromLibDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CSelectSpecFromLibDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	m_SelPage = m_SelectSpecFromLibSheet.GetActiveIndex();
	CDialogEx::OnOK();
}
