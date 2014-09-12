// AnalasisOfflineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "AnalasisOfflineDlg.h"
#include "afxdialogex.h"


// CAnalasisOfflineDlg �Ի���

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


// CAnalasisOfflineDlg ��Ϣ�������


void CAnalasisOfflineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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


BOOL CAnalasisOfflineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAnalasisOfflineDlg::OnBnClickedButtonBackfromoffline()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CClientView *pwnd=(CClientView*)this->GetParent();
//	pwnd->isDlgUISelect=true;
	pwnd->isDlgLogin=false;
//	pwnd->isDlgAnalasisOnline=false;
//	pwnd->isDlgAnalasisOffline=false;
	pwnd->isSpecCompareDlg=false;
	::PostMessage(this->GetParent()->m_hWnd,UIDISPLAY,NULL,NULL);
}
