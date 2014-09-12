// SmoothSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "SmoothSettingDlg.h"
#include "afxdialogex.h"

// CSmoothSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSmoothSettingDlg, CDialogEx)

CSmoothSettingDlg::CSmoothSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmoothSettingDlg::IDD, pParent)
{

}

CSmoothSettingDlg::~CSmoothSettingDlg()
{
}

void CSmoothSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SMOOTH, m_ComboSmooth);
}


BEGIN_MESSAGE_MAP(CSmoothSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSmoothSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSmoothSettingDlg 消息处理程序


BOOL CSmoothSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for(int i =0;i<9;++i)
	{
		CString szSmPts;
		szSmPts.Format(L"%d",i+2);
		m_ComboSmooth.AddString(szSmPts);
	}
	m_ComboSmooth.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSmoothSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	if(CB_ERR == m_ComboSmooth.GetCurSel())
	{
		MessageBox(L"请选择平滑的点数",L"平滑设置",MB_ICONERROR);
		return;
	}
	CString szSmPts;
	m_ComboSmooth.GetLBText(m_ComboSmooth.GetCurSel(),szSmPts);
	m_SmoothPts = _wtoi(szSmPts);		
}

