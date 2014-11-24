// AddSpecToAnaInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "AddSpecToAnaInfo.h"
#include "afxdialogex.h"
#include "FlavourTypeIndex.h"
#include "BrandTypeIndex.h"
#include "AlcoholTypeIndex.h"
#include "TrueWineLib.h"
#include "WineTypeIndex.h"
// CAddSpecToAnaInfo 对话框

IMPLEMENT_DYNAMIC(CAddSpecToAnaInfo, CDialogEx)

CAddSpecToAnaInfo::CAddSpecToAnaInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddSpecToAnaInfo::IDD, pParent)
	, m_Winename(_T(""))
	, m_Alcoholcontent(_T(""))
	, m_Flavour(_T(""))
	, m_Brand(_T(""))
	, m_WinenameExam(_T("白云边5年陈酿"))
	, m_AlcoholContentExam(_T("42"))
	, m_FlavourExam(_T("浓酱兼香型"))
	, m_BrandExam(_T("白云边"))
{

}

CAddSpecToAnaInfo::~CAddSpecToAnaInfo()
{
}

void CAddSpecToAnaInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WINENAME, m_ComboWineName);
	DDX_CBString(pDX, IDC_COMBO_WINENAME, m_Winename);
	DDX_Control(pDX, IDC_COMBO_ALCOHOLCENTENT, m_ComboAlcoholContent);
	DDX_CBString(pDX, IDC_COMBO_ALCOHOLCENTENT, m_Alcoholcontent);
	DDX_Control(pDX, IDC_COMBO_FLAVOUR, m_ComboFlavour);
	DDX_CBString(pDX, IDC_COMBO_FLAVOUR, m_Flavour);
	DDX_Control(pDX, IDC_COMBO_BRAND, m_ComboBrand);
	DDX_CBString(pDX, IDC_COMBO_BRAND, m_Brand);
	DDX_CBString(pDX, IDC_COMBO5, m_WinenameExam);
	DDX_CBString(pDX, IDC_COMBO6, m_AlcoholContentExam);
	DDX_CBString(pDX, IDC_COMBO7, m_FlavourExam);
	DDX_CBString(pDX, IDC_COMBO8, m_BrandExam);
}


BEGIN_MESSAGE_MAP(CAddSpecToAnaInfo, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_WINENAME, &CAddSpecToAnaInfo::OnCbnSelchangeComboWinename)
	ON_CBN_EDITCHANGE(IDC_COMBO_WINENAME, &CAddSpecToAnaInfo::OnCbnEditchangeComboWinename)
	ON_CBN_SELCHANGE(IDC_COMBO_ALCOHOLCENTENT, &CAddSpecToAnaInfo::OnCbnSelchangeComboAlcoholcentent)
	ON_CBN_SELCHANGE(IDC_COMBO_FLAVOUR, &CAddSpecToAnaInfo::OnCbnSelchangeComboFlavour)
	ON_CBN_SELCHANGE(IDC_COMBO_BRAND, &CAddSpecToAnaInfo::OnCbnSelchangeComboBrand)
END_MESSAGE_MAP()


// CAddSpecToAnaInfo 消息处理程序


BOOL CAddSpecToAnaInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString command=L"SELECT DISTINCT AlcoholContent FROM TrueWineLib";
	Alcohol.Open(AFX_DB_USE_DEFAULT_TYPE,command);
	command=L"SELECT DISTINCT Flavour FROM TrueWineLib";
	Flavour.Open(AFX_DB_USE_DEFAULT_TYPE,command);
	command=L"SELECT DISTINCT Brand FROM TrueWineLib";
	Brand.Open(AFX_DB_USE_DEFAULT_TYPE,command);
	command=L"SELECT DISTINCT WineName FROM TrueWineLib";
	WineName.Open(AFX_DB_USE_DEFAULT_TYPE,command);

	while(!Alcohol.IsEOF())
	{
		m_ComboAlcoholContent.AddString(Alcohol.m_WineName);
		Alcohol.MoveNext();
	}
	while(!Flavour.IsEOF())
	{
		m_ComboFlavour.AddString(Flavour.m_WineName);
		Flavour.MoveNext();
	}
	while(!Brand.IsEOF())
	{
		m_ComboBrand.AddString(Brand.m_WineName);
		Brand.MoveNext();
	}
	while(!WineName.IsEOF())
	{
		m_ComboWineName.AddString(WineName.m_WineName);
		WineName.MoveNext();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboWinename()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CAddSpecToAnaInfo::OnCbnSelchangeComboAlcoholcentent()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboFlavour()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboBrand()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CAddSpecToAnaInfo::OnCbnEditchangeComboWinename()
{
	// TODO: 在此添加控件通知处理程序代码

}
