// ShowInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ShowInfoDlg.h"
#include "afxdialogex.h"


// CShowInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CShowInfoDlg, CDialogEx)

	CShowInfoDlg::CShowInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowInfoDlg::IDD, pParent)
	, m_StaticInfoShow(_T(""))
{

}

CShowInfoDlg::~CShowInfoDlg()
{
}

void CShowInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SHOWINFO, m_StaticInfoShow);
}


BEGIN_MESSAGE_MAP(CShowInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CShowInfoDlg ��Ϣ�������
