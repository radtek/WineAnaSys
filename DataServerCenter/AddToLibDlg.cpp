// AddToLibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "AddToLibDlg.h"
#include "afxdialogex.h"


// CAddToLibDlg �Ի���

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


// CAddToLibDlg ��Ϣ�������


BOOL CAddToLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetClientRect(&m_rect);
	m_AddToLibProgress.SetRange(0,m_AddNumber-1);
	m_AddToLibProgress.SetPos(m_CurrentProgress-1);

	m_StaticAddInfo.Format(L"������ӵ�%d�����ף���%d��",m_CurrentProgress,m_AddNumber);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


LRESULT CAddToLibDlg::OnDisplay(WPARAM wParam,LPARAM lParam)
{
	m_AddToLibProgress.SetPos(m_CurrentProgress-1);
	m_StaticAddInfo.Format(L"������ӵ�%d�����ף���%d��",m_CurrentProgress,m_AddNumber);
	

	UpdateData(FALSE);
	//OnPaint();
	return 0;
}





void CAddToLibDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CWnd *pWnd[2]; 
	pWnd[0] = GetDlgItem(IDC_STATIC_ADDINFO);
	pWnd[1] = GetDlgItem(IDC_PROGRESS_ADDPROGRESS);

	for(int i=0;i<2;i++)
	{
		//��ȡ�ؼ���� 
		if(pWnd[i])//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ���� 
		{ 
			CRect rect; //��ȡ�ؼ��仯ǰ��С 
			pWnd[i]->GetWindowRect(&rect); 
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е��������� 
			//cx/m_rect.Width()Ϊ�Ի����ں���ı仯���� 
			rect.left=rect.left*cx/m_rect.Width();//////�����ؼ���С 
			rect.right=rect.right*cx/m_rect.Width(); 
			rect.top=rect.top*cy/m_rect.Height(); 
			rect.bottom=rect.bottom*cy/m_rect.Height(); 
			pWnd[i]->MoveWindow(rect);//���ÿؼ���С 
		}
	}

	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С 
}
