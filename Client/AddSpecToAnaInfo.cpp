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
/*	m_ComboWineName.GetLBText(m_ComboWineName.GetCurSel(),m_Winename);


	CString strCondition=L"";
	if(!m_Winename.IsEmpty())
	{
		CString temp;
		temp.Format(L"WineName = '%s'",m_Winename);
		strCondition+=temp;
	}
	if(!m_Brand.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Brand = '%s'",m_Brand);
		strCondition+=temp;
	}
	if(!m_Flavour.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Flavour = '%s'",m_Flavour);
		strCondition+=temp;
	}
	if(!m_Alcoholcontent.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND AlcoholContent = '%s'",m_Alcoholcontent);
		strCondition+=temp;
	}

	Brand.m_strFilter=strCondition;
	Flavour.m_strFilter=strCondition;
	Alcohol.m_strFilter=strCondition;

	Brand.Requery();
	Flavour.Requery();
	Alcohol.Requery();

	for (int i= m_ComboBrand.GetCount()-1;i>= 0; i--)
	{
		m_ComboBrand.DeleteString(i);
	}
	for (int i= m_ComboFlavour.GetCount()-1;i>= 0; i--)
	{
		m_ComboFlavour.DeleteString(i);
	}
	for (int i= m_ComboAlcoholContent.GetCount()-1;i>= 0; i--)
	{
		m_ComboAlcoholContent.DeleteString(i);
	}

	while(!Brand.IsEOF())
	{
		m_ComboBrand.AddString(Brand.m_WineName);
		Brand.MoveNext();
	}
	while(!Flavour.IsEOF())
	{
		m_ComboFlavour.AddString(Flavour.m_WineName);
		Flavour.MoveNext();
	}
	while(!Alcohol.IsEOF())
	{
		m_ComboAlcoholContent.AddString(Alcohol.m_WineName);
		Alcohol.MoveNext();
	}*/
}

void CAddSpecToAnaInfo::OnCbnSelchangeComboAlcoholcentent()
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_ComboAlcoholContent.GetLBText(m_ComboAlcoholContent.GetCurSel(),m_Alcoholcontent);


	CString strCondition=L"";
	if(!m_Alcoholcontent.IsEmpty())
	{
		CString temp;
		temp.Format(L"AlcoholContent = '%s'",m_Alcoholcontent);
		strCondition+=temp;
	}
	if(!m_Winename.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND WineName = '%s'",m_Winename);
		strCondition+=temp;
	}
	if(!m_Flavour.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Flavour = '%s'",m_Flavour);
		strCondition+=temp;
	}
	if(!m_Brand.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Brand = '%s'",m_Brand);
		strCondition+=temp;
	}
	WineName.m_strFilter=strCondition;
	Flavour.m_strFilter=strCondition;
	Brand.m_strFilter=strCondition;

	WineName.Requery();
	Flavour.Requery();
	Brand.Requery();

	for (int i= m_ComboWineName.GetCount()-1;i>= 0; i--)
	{
		m_ComboWineName.DeleteString(i);
	}
	for (int i= m_ComboFlavour.GetCount()-1;i>= 0; i--)
	{
		m_ComboFlavour.DeleteString(i);
	}
	for (int i= m_ComboBrand.GetCount()-1;i>= 0; i--)
	{
		m_ComboBrand.DeleteString(i);
	}

	while(!WineName.IsEOF())
	{
		m_ComboWineName.AddString(WineName.m_WineName);
		WineName.MoveNext();
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
	}*/
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboFlavour()
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_ComboFlavour.GetLBText(m_ComboFlavour.GetCurSel(),m_Flavour);


	CString strCondition=L"";
	if(!m_Flavour.IsEmpty())
	{
		CString temp;
		temp.Format(L" Flavour= '%s'",m_Flavour);
		strCondition+=temp;
	}
	if(!m_Winename.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND WineName = '%s'",m_Winename);
		strCondition+=temp;
	}
	if(!m_Alcoholcontent.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND AlcoholContent = '%s'",m_Alcoholcontent);
		strCondition+=temp;
	}
	if(!m_Brand.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Brand = '%s'",m_Brand);
		strCondition+=temp;
	}
	WineName.m_strFilter=strCondition;
	Alcohol.m_strFilter=strCondition;
	Brand.m_strFilter=strCondition;

	WineName.Requery();
	Alcohol.Requery();
	Brand.Requery();

	for (int i= m_ComboWineName.GetCount()-1;i>= 0; i--)
	{
		m_ComboWineName.DeleteString(i);
	}
	for (int i= m_ComboAlcoholContent.GetCount()-1;i>= 0; i--)
	{
		m_ComboAlcoholContent.DeleteString(i);
	}
	for (int i= m_ComboBrand.GetCount()-1;i>= 0; i--)
	{
		m_ComboBrand.DeleteString(i);
	}

	while(!WineName.IsEOF())
	{
		m_ComboWineName.AddString(WineName.m_WineName);
		WineName.MoveNext();
	}
	while(!Alcohol.IsEOF())
	{
		m_ComboAlcoholContent.AddString(Alcohol.m_WineName);
		Alcohol.MoveNext();
	}
	while(!Brand.IsEOF())
	{
		m_ComboBrand.AddString(Brand.m_WineName);
		Brand.MoveNext();
	}*/
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboBrand()
{
	// TODO: 在此添加控件通知处理程序代码
/*	m_ComboBrand.GetLBText(m_ComboBrand.GetCurSel(),m_Brand);


	CString strCondition=L"";
	if(!m_Brand.IsEmpty())
	{
		CString temp;
		temp.Format(L"Brand = '%s'",m_Brand);
		strCondition+=temp;
	}
	if(!m_Winename.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND WineName = '%s'",m_Winename);
		strCondition+=temp;
	}
	if(!m_Flavour.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND Flavour = '%s'",m_Flavour);
		strCondition+=temp;
	}
	if(!m_Alcoholcontent.IsEmpty())
	{
		CString temp;
		temp.Format(L"AND AlcoholContent = '%s'",m_Alcoholcontent);
		strCondition+=temp;
	}

	WineName.m_strFilter=strCondition;
	Flavour.m_strFilter=strCondition;
	Alcohol.m_strFilter=strCondition;

	WineName.Requery();
	Flavour.Requery();
	Alcohol.Requery();

	for (int i= m_ComboWineName.GetCount()-1;i>= 0; i--)
	{
		m_ComboWineName.DeleteString(i);
	}
	for (int i= m_ComboFlavour.GetCount()-1;i>= 0; i--)
	{
		m_ComboFlavour.DeleteString(i);
	}
	for (int i= m_ComboAlcoholContent.GetCount()-1;i>= 0; i--)
	{
		m_ComboAlcoholContent.DeleteString(i);
	}

	while(!WineName.IsEOF())
	{
		m_ComboWineName.AddString(WineName.m_WineName);
		WineName.MoveNext();
	}
	while(!Flavour.IsEOF())
	{
		m_ComboFlavour.AddString(Flavour.m_WineName);
		Flavour.MoveNext();
	}
	while(!Alcohol.IsEOF())
	{
		m_ComboAlcoholContent.AddString(Alcohol.m_WineName);
		Alcohol.MoveNext();
	}*/
}

void CAddSpecToAnaInfo::OnCbnEditchangeComboWinename()
{
	// TODO: 在此添加控件通知处理程序代码

}