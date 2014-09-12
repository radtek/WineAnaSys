// AddToLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "AddToLibDlg.h"
#include "afxdialogex.h"


// CAddToLibDlg 对话框

IMPLEMENT_DYNAMIC(CAddToLibDlg, CDialogEx)

CAddToLibDlg::CAddToLibDlg(int AddNumber,int CurrentProgress,CWnd* pParent/*=NULL*/)
	: CDialogEx(CAddToLibDlg::IDD, pParent)
	, m_StaticAddInfo(_T(""))
{

	m_AddNumber = AddNumber;
	m_CurrentProgress = CurrentProgress;
	
}

CAddToLibDlg::~CAddToLibDlg()
{
}

void CAddToLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_ADDPROGRESS, m_AddToLibProgress);
	DDX_Text(pDX, IDC_STATIC_ADDINFO, m_StaticAddInfo);
}


BEGIN_MESSAGE_MAP(CAddToLibDlg, CDialogEx)
	ON_MESSAGE(WM_DISPLAY,OnDisplay)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAddToLibDlg 消息处理程序


BOOL CAddToLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetClientRect(&m_rect);
	m_AddToLibProgress.SetRange(0,m_AddNumber-1);
	m_AddToLibProgress.SetPos(m_CurrentProgress-1);

	m_StaticAddInfo.Format(L"正在添加第%d条光谱，共%d条",m_CurrentProgress,m_AddNumber);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CAddToLibDlg::OnDisplay(WPARAM wParam,LPARAM lParam)
{
	m_AddToLibProgress.SetPos(m_CurrentProgress-1);
	m_StaticAddInfo.Format(L"正在添加第%d条光谱，共%d条",m_CurrentProgress,m_AddNumber);
	

	UpdateData(FALSE);
	//OnPaint();
	return 0;
}





void CAddToLibDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *pWnd[2]; 
	pWnd[0] = GetDlgItem(IDC_STATIC_ADDINFO);
	pWnd[1] = GetDlgItem(IDC_PROGRESS_ADDPROGRESS);

	for(int i=0;i<2;i++)
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
