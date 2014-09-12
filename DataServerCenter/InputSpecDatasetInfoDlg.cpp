// InputSpecDatasetInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "InputSpecDatasetInfoDlg.h"
#include "afxdialogex.h"


// CInputSpecDatasetInfoDlg 对话框

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


// CInputSpecDatasetInfoDlg 消息处理程序


void CInputSpecDatasetInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"白酒名称信息不能为空!",L"信息输入有误",MB_ICONWARNING);
		return;
	}
	if(m_AlcoholContent.IsEmpty())
	{
		MessageBox(L"白酒酒精度信息不能为空！",L"信息输入有误",MB_ICONWARNING);
		return;
	}
	if(m_Flavour.IsEmpty())
	{
		MessageBox(L"白酒香型信息不能为空！",L"信息输入有误",MB_ICONWARNING);
		return;
	}
	if(m_Brand.IsEmpty())
	{
		MessageBox(L"白酒品牌信息不能为空！",L"信息输入有误",MB_ICONWARNING);
		return;
	}
	
	CDialogEx::OnOK();
}
