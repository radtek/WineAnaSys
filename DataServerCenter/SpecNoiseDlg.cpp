// SpecNoiseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SpecNoiseDlg.h"
#include "afxdialogex.h"


// CSpecNoiseDlg �Ի���

IMPLEMENT_DYNAMIC(CSpecNoiseDlg, CDialogEx)

CSpecNoiseDlg::CSpecNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpecNoiseDlg::IDD, pParent)
	, m_NoiseInfo(_T(""))
{

}

CSpecNoiseDlg::~CSpecNoiseDlg()
{
}

void CSpecNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_INFO, m_NoiseInfo);
}


BEGIN_MESSAGE_MAP(CSpecNoiseDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpecNoiseDlg ��Ϣ�������
