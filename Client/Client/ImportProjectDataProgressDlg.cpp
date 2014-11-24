// ImportProjectDataProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ImportProjectDataProgressDlg.h"
#include "afxdialogex.h"


// CImportProjectDataProgressDlg 对话框

IMPLEMENT_DYNAMIC(CImportProjectDataProgressDlg, CDialogEx)

	CImportProjectDataProgressDlg::CImportProjectDataProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportProjectDataProgressDlg::IDD, pParent)
	, m_PCAProgressInfo(_T(""))
{

}

CImportProjectDataProgressDlg::~CImportProjectDataProgressDlg()
{
}

void CImportProjectDataProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PROGRESSINFO, m_PCAProgressInfo);
	DDX_Control(pDX, IDC_PROGRESS_PCA, m_PCA_Progress);
}


BEGIN_MESSAGE_MAP(CImportProjectDataProgressDlg, CDialogEx)
	ON_MESSAGE(WM_PCAIMPORTDATA, &CImportProjectDataProgressDlg::OnPcaimportdata)
	ON_MESSAGE(WM_PCAPRETREAT, &CImportProjectDataProgressDlg::OnPcapretreat)
	ON_MESSAGE(WM_PCACOMPUTE, &CImportProjectDataProgressDlg::OnPcacompute)
	ON_MESSAGE(WM_PCAFINISH, &CImportProjectDataProgressDlg::OnPcafinish)
	ON_BN_CLICKED(IDCANCEL, &CImportProjectDataProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CImportProjectDataProgressDlg 消息处理程序


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcaimportdata(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"导入数据中，请耐心等待";
	m_PCA_Progress.SetPos(wParam);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcapretreat(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"数据预处理";
	m_PCA_Progress.SetPos(71);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcacompute(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"PCA计算";
	m_PCA_Progress.SetPos(81);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcafinish(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"完成";
	m_PCA_Progress.SetPos(100);
	UpdateData(FALSE);
	return 0;
}


BOOL CImportProjectDataProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_PCA_Progress.SetRange(1,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CImportProjectDataProgressDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
