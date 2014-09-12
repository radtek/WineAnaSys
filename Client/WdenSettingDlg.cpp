// WdenSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "WdenSettingDlg.h"
#include "afxdialogex.h"

// CWdenSettingDlg 对话框

IMPLEMENT_DYNAMIC(CWdenSettingDlg, CDialogEx)

CWdenSettingDlg::CWdenSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWdenSettingDlg::IDD, pParent)
{

}

CWdenSettingDlg::~CWdenSettingDlg()
{
}

void CWdenSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WNAME1SET, m_ComboWname1);
	DDX_Control(pDX, IDC_COMBO_WNAME2SET, m_ComboWname2);
	DDX_Control(pDX, IDC_COMBO_DECLEVELSET, m_ComboDecLevel);
	DDX_Control(pDX, IDC_COMBO_TTYPESET, m_ComboTType);
	DDX_Control(pDX, IDC_COMBO_SORHSET, m_ComboSorH);
	DDX_Control(pDX, IDC_COMBO_TSCALSET, m_ComboTScal);
}


BEGIN_MESSAGE_MAP(CWdenSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWdenSettingDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_WNAME1SET, &CWdenSettingDlg::OnCbnSelchangeComboWname1set)
END_MESSAGE_MAP()


// CWdenSettingDlg 消息处理程序


BOOL CWdenSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboWname1.AddString(L"haar");
	m_ComboWname1.AddString(L"db");
	m_ComboWname1.AddString(L"sym");
	
	//分解层数
	for(int i =0;i<9;++i)
	{
		CString szDeclevel;
		szDeclevel.Format(L"%d",i+1);
		m_ComboDecLevel.AddString(szDeclevel);
	}

	m_ComboTType.AddString(L"rigrsure");
	m_ComboTType.AddString(L"heursure");
	m_ComboTType.AddString(L"sqtwolog");
	m_ComboTType.AddString(L"minimaxi");

	m_ComboTScal.AddString(L"one");
	m_ComboTScal.AddString(L"sln");
	m_ComboTScal.AddString(L"mln");

	m_ComboSorH.AddString(L"硬阈值");
	m_ComboSorH.AddString(L"软阈值");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CWdenSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ComboWname1.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择正确的小波！",L"预处理设置",MB_ICONERROR);
		return ;
	}

	if(m_ComboWname1.GetCurSel() !=0&&m_ComboWname2.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择正确的小波！",L"预处理设置",MB_ICONERROR);
		return ;
	}

	if(m_ComboDecLevel.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择小波分解的层数！",L"预处理设置",MB_ICONERROR);
		return ;
	}
	
	CString wName1,wName2;
	m_ComboWname1.GetLBText(m_ComboWname1.GetCurSel(),wName1);

	if(m_ComboWname2.GetCurSel()!=CB_ERR)
		m_ComboWname2.GetLBText(m_ComboWname2.GetCurSel(),wName2);
	m_wName = wName1+wName2;

	CString szDecLevel;
	m_ComboDecLevel.GetLBText(m_ComboDecLevel.GetCurSel(),szDecLevel);
	m_DecLevel = _wtoi(szDecLevel); 


	if(m_ComboTType.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择阈值选择方法！",L"预处理设置",MB_ICONERROR);
		return ;
	}
	if(m_ComboTScal.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择阈值处理方法！",L"预处理设置",MB_ICONERROR);
		return ;
	}
	if(m_ComboSorH.GetCurSel() == CB_ERR)
	{
		MessageBox(L"请选择阈值使用方式！",L"预处理设置",MB_ICONERROR);
		return ;
	}

	m_ComboTType.GetLBText(m_ComboTType.GetCurSel(),m_TTppe);
	m_ComboTScal.GetLBText(m_ComboTScal.GetCurSel(),m_TScal);

	CString szsorh;
	m_ComboSorH.GetLBText(m_ComboSorH.GetCurSel(),szsorh);

	if(szsorh == L"硬阈值")
		m_TSORH = L"h";
	else
		m_TSORH = L"s";


	CDialogEx::OnOK();
}


void CWdenSettingDlg::OnCbnSelchangeComboWname1set()
{
	// TODO: 在此添加控件通知处理程序代码
	int m_Wname1Index = m_ComboWname1.GetCurSel();
	if(m_Wname1Index == 1) //如果选择db小波
	{
		m_ComboWname2.EnableWindow(1);
		for (int i = m_ComboWname2.GetCount() - 1; i >= 0; i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		for(int i =0;i<5;++i)
		{
			CString szNum;
			szNum.Format(L"%d",i+1);
			m_ComboWname2.AddString(szNum);
		}
	}
	else if(m_Wname1Index == 2) //如果选择sym小波
	{
		m_ComboWname2.EnableWindow(1);
		for (int i = m_ComboWname2.GetCount()-1;i>=0;i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		for(int i =1;i<4;++i)
		{
			CString szNum;
			szNum.Format(L"%d",i+1);
			m_ComboWname2.AddString(szNum);
		}
	}
	else
	{
		for (int i = m_ComboWname2.GetCount()-1;i>=0;i--)
		{
			m_ComboWname2.DeleteString(i);
		}
		m_ComboWname2.EnableWindow(0);
	}
}

