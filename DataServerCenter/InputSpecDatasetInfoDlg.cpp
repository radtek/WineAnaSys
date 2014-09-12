// InputSpecDatasetInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "InputSpecDatasetInfoDlg.h"
#include "afxdialogex.h"


// CInputSpecDatasetInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CInputSpecDatasetInfoDlg, CDialogEx)

CInputSpecDatasetInfoDlg::CInputSpecDatasetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputSpecDatasetInfoDlg::IDD, pParent)
	, m_WineName(_T(""))
	, m_AlcoholContent(_T(""))
	, m_Flavour(_T(""))
	, m_Brand(_T(""))
{

}

CInputSpecDatasetInfoDlg::~CInputSpecDatasetInfoDlg()
{
}

void CInputSpecDatasetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WINENAME, m_WineName);
	DDX_Text(pDX, IDC_EDIT_ALCOHOLCONTENT, m_AlcoholContent);
	DDX_Text(pDX, IDC_EDIT_FLAVOUR, m_Flavour);
	DDX_Text(pDX, IDC_EDIT_BRAND, m_Brand);
}


BEGIN_MESSAGE_MAP(CInputSpecDatasetInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputSpecDatasetInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputSpecDatasetInfoDlg ��Ϣ�������


void CInputSpecDatasetInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"�׾�������Ϣ����Ϊ��!",L"��Ϣ��������",MB_ICONWARNING);
		return;
	}
	if(m_AlcoholContent.IsEmpty())
	{
		MessageBox(L"�׾ƾƾ�����Ϣ����Ϊ�գ�",L"��Ϣ��������",MB_ICONWARNING);
		return;
	}
	if(m_Flavour.IsEmpty())
	{
		MessageBox(L"�׾�������Ϣ����Ϊ�գ�",L"��Ϣ��������",MB_ICONWARNING);
		return;
	}
	if(m_Brand.IsEmpty())
	{
		MessageBox(L"�׾�Ʒ����Ϣ����Ϊ�գ�",L"��Ϣ��������",MB_ICONWARNING);
		return;
	}
	
	CDialogEx::OnOK();
}
