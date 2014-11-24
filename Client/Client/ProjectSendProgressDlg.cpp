// ProjectSendProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ProjectSendProgressDlg.h"
#include "afxdialogex.h"


// CProjectSendProgressDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectSendProgressDlg, CDialogEx)

CProjectSendProgressDlg::CProjectSendProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectSendProgressDlg::IDD, pParent)
	, m_SendState(_T(""))
	, m_SendPercent(_T(""))
	, m_ProjectSize(_T(""))
	, m_SendedSize(_T(""))
{
}

CProjectSendProgressDlg::~CProjectSendProgressDlg()
{
}

void CProjectSendProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SENDSTATE, m_SendState);
	DDX_Text(pDX, IDC_STATIC_SENDPERCENT, m_SendPercent);
	DDX_Text(pDX, IDC_STATIC_PROJECTSIZE, m_ProjectSize);
	DDX_Text(pDX, IDC_STATIC_SENDEDSIZE, m_SendedSize);
	DDX_Control(pDX, IDC_PROGRESS_PROJECTSEND, m_ProgressProjectSend);
}


BEGIN_MESSAGE_MAP(CProjectSendProgressDlg, CDialogEx)
	ON_MESSAGE(WM_SHOWSENDPROGRESS, &CProjectSendProgressDlg::OnShowsendprogress)
	ON_BN_CLICKED(IDCANCEL, &CProjectSendProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CProjectSendProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// CProjectSendProgressDlg ��Ϣ�������
afx_msg LRESULT CProjectSendProgressDlg::OnShowsendprogress(WPARAM wParam, LPARAM lParam)
{
	if(m_MaxRange!=0)
	{
		m_ProgressProjectSend.SetRange(0,m_MaxRange-1);
		m_ProgressProjectSend.SetPos(m_CurrentProgress-1);
	}
	m_ProgressProjectSend.SetPos(m_CurrentProgress-1);
	UpdateData(FALSE);
	return 0;
}

void CProjectSendProgressDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitThreadPackAndSendProject = true;
	while(!ThreadPackAndSendProjectExited)
	{
		Sleep(10);
	}
	CDialogEx::OnCancel();
}
