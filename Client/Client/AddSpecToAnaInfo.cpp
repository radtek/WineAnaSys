// AddSpecToAnaInfo.cpp : ʵ���ļ�
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
// CAddSpecToAnaInfo �Ի���

IMPLEMENT_DYNAMIC(CAddSpecToAnaInfo, CDialogEx)

CAddSpecToAnaInfo::CAddSpecToAnaInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddSpecToAnaInfo::IDD, pParent)
	, m_Winename(_T(""))
	, m_Alcoholcontent(_T(""))
	, m_Flavour(_T(""))
	, m_Brand(_T(""))
	, m_WinenameExam(_T("���Ʊ�5�����"))
	, m_AlcoholContentExam(_T("42"))
	, m_FlavourExam(_T("Ũ��������"))
	, m_BrandExam(_T("���Ʊ�"))
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


// CAddSpecToAnaInfo ��Ϣ�������


BOOL CAddSpecToAnaInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboWinename()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CAddSpecToAnaInfo::OnCbnSelchangeComboAlcoholcentent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboFlavour()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAddSpecToAnaInfo::OnCbnSelchangeComboBrand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CAddSpecToAnaInfo::OnCbnEditchangeComboWinename()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
