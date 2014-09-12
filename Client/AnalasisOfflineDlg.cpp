// AnalasisOfflineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "AnalasisOfflineDlg.h"
#include "afxdialogex.h"


// CAnalasisOfflineDlg 对话框

IMPLEMENT_DYNAMIC(CAnalasisOfflineDlg, CDialogEx)

CAnalasisOfflineDlg::CAnalasisOfflineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnalasisOfflineDlg::IDD, pParent)
{

}

CAnalasisOfflineDlg::~CAnalasisOfflineDlg()
{
}

void CAnalasisOfflineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnalasisOfflineDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BACKFROMOFFLINE, &CAnalasisOfflineDlg::OnBnClickedButtonBackfromoffline)
END_MESSAGE_MAP()


// CAnalasisOfflineDlg 消息处理程序


void CAnalasisOfflineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *pWnd[9]; 
	pWnd[0] = GetDlgItem(IDC_BUTTON_ADDDATA);
	pWnd[1] = GetDlgItem(IDC_EDIT_DATACURRENT);
	pWnd[2] = GetDlgItem(IDC_STATIC_WINENAME);
	pWnd[3] = GetDlgItem(IDC_COMBO_INPUTWINENAME);
	pWnd[4] = GetDlgItem(IDC_STATIC_USER_MAIL);
	pWnd[5] = GetDlgItem(IDC_EDIT_USERMAILADDR);
	pWnd[6] = GetDlgItem(IDC_BUTTON_SENDDATA);
	pWnd[7] = GetDlgItem(IDC_BUTTON_BACKFROMOFFLINE);

	//pWnd[7]=GetDlgItem(IDC_STATIC_LOGIN_TITLE);

	for(int i=0;i<8;i++)
	{
		//获取控件句柄 
		if(pWnd[i])//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建 
		{ 
			CRect rect; //获取控件变化前大小 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标 
			//cx/m_rect.Width()为对话框在横向的变化比例 
			rect.left=rect.left*cx/m_rect.Width();//////调整控件大小 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//设置控件大小 
		}
	}
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小 
}


BOOL CAnalasisOfflineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAnalasisOfflineDlg::OnBnClickedButtonBackfromoffline()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientView *pwnd=(CClientView*)this->GetParent();
//	pwnd->isDlgUISelect=true;
	pwnd->isDlgLogin=false;
//	pwnd->isDlgAnalasisOnline=false;
//	pwnd->isDlgAnalasisOffline=false;
	pwnd->isSpecCompareDlg=false;
	::PostMessage(this->GetParent()->m_hWnd,UIDISPLAY,NULL,NULL);
}
