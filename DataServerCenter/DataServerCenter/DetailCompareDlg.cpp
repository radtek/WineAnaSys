// DetailCompareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DetailCompareDlg.h"
#include "afxdialogex.h"


// CDetailCompareDlg 对话框

IMPLEMENT_DYNAMIC(CDetailCompareDlg, CDialogEx)

CDetailCompareDlg::CDetailCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetailCompareDlg::IDD, pParent)
{

}

CDetailCompareDlg::~CDetailCompareDlg()
{
}

void CDetailCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailCompareDlg, CDialogEx)
END_MESSAGE_MAP()


// CDetailCompareDlg 消息处理程序


BOOL CDetailCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
