// DetailCompareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DetailCompareDlg.h"
#include "afxdialogex.h"


// CDetailCompareDlg �Ի���

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


// CDetailCompareDlg ��Ϣ�������


BOOL CDetailCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
