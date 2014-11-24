// DiffSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DiffSettingDlg.h"
#include "afxdialogex.h"


// CDiffSettingDlg 对话框

IMPLEMENT_DYNAMIC(CDiffSettingDlg, CDialogEx)

CDiffSettingDlg::CDiffSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiffSettingDlg::IDD, pParent)
{

}

CDiffSettingDlg::~CDiffSettingDlg()
{
}

void CDiffSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ORDER, m_ComboOrder);
}


BEGIN_MESSAGE_MAP(CDiffSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiffSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiffSettingDlg 消息处理程序


BOOL CDiffSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboOrder.AddString(L"一阶");
	m_ComboOrder.AddString(L"二阶");
	m_ComboOrder.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDiffSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(0 == m_ComboOrder.GetCurSel())
		m_Order = 1;
	else if(1 == m_ComboOrder.GetCurSel())
		m_Order = 2;
	else
	{
		MessageBox(L"请选择求导的阶数",L"导数设置",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}
