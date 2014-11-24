// ImportProjectDataProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ImportProjectDataProgressDlg.h"
#include "afxdialogex.h"


// CImportProjectDataProgressDlg �Ի���

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


// CImportProjectDataProgressDlg ��Ϣ�������


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcaimportdata(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"���������У������ĵȴ�";
	m_PCA_Progress.SetPos(wParam);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcapretreat(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"����Ԥ����";
	m_PCA_Progress.SetPos(71);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcacompute(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"PCA����";
	m_PCA_Progress.SetPos(81);
	UpdateData(FALSE);
	return 0;
}


afx_msg LRESULT CImportProjectDataProgressDlg::OnPcafinish(WPARAM wParam, LPARAM lParam)
{
	m_PCAProgressInfo = L"���";
	m_PCA_Progress.SetPos(100);
	UpdateData(FALSE);
	return 0;
}


BOOL CImportProjectDataProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_PCA_Progress.SetRange(1,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CImportProjectDataProgressDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
