// SelectSpecFromLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SelectSpecFromLibDlg.h"
#include "afxdialogex.h"


// CSelectSpecFromLibDlg 对话框

IMPLEMENT_DYNAMIC(CSelectSpecFromLibDlg, CDialogEx)

CSelectSpecFromLibDlg::CSelectSpecFromLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectSpecFromLibDlg::IDD, pParent)
	, m_Key1(_T(""))
	, m_Key2(_T(""))
	, m_Key3(_T(""))
{
	//m_pTrueWineLib=new CTrueWineLib();

	m_TrueWineLib.m_strSort=L"WineName";
}

CSelectSpecFromLibDlg::~CSelectSpecFromLibDlg()
{
}

void CSelectSpecFromLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_ComboField1);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_ComboField2);
	DDX_Control(pDX, IDC_COMBO_FIELD3, m_ComboField3);
	DDX_Control(pDX, IDC_COMBO_ANDOR1, m_ComboAndOr1);
	DDX_Control(pDX, IDC_COMBO_ANDOR2, m_ComboAndOr2);
	DDX_Text(pDX, IDC_EDIT_KEY1, m_Key1);
	DDX_Text(pDX, IDC_EDIT_KEY2, m_Key2);
	DDX_Control(pDX, IDC_EDIT_KEY3, m_key3);
	DDX_Control(pDX, IDC_LIST_SPECLIB, m_ListSpec);
	DDX_Text(pDX, IDC_EDIT_KEY3, m_Key3);
}


BEGIN_MESSAGE_MAP(CSelectSpecFromLibDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWITHKEYWORDS, &CSelectSpecFromLibDlg::OnBnClickedButtonSearchwithkeywords)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL, &CSelectSpecFromLibDlg::OnBnClickedButtonShowall)
	ON_BN_CLICKED(IDOK, &CSelectSpecFromLibDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLIST, &CSelectSpecFromLibDlg::OnBnClickedButtonClearlist)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CSelectSpecFromLibDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CSelectSpecFromLibDlg 消息处理程序




BOOL CSelectSpecFromLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboField1.InsertString(0,L"所有项");
	m_ComboField1.InsertString(1,L"白酒名称");
	m_ComboField1.InsertString(2,L"白酒酒精度数");
	m_ComboField1.InsertString(3,L"白酒香型");
	m_ComboField1.InsertString(4,L"白酒品牌");

	m_ComboField2.InsertString(0,L"所有项");
	m_ComboField2.InsertString(1,L"白酒名称");
	m_ComboField2.InsertString(2,L"白酒酒精度数");
	m_ComboField2.InsertString(3,L"白酒香型");
	m_ComboField2.InsertString(4,L"白酒品牌");

	m_ComboField3.InsertString(0,L"所有项");
	m_ComboField3.InsertString(1,L"白酒名称");
	m_ComboField3.InsertString(2,L"白酒酒精度数");
	m_ComboField3.InsertString(3,L"白酒香型");
	m_ComboField3.InsertString(4,L"白酒品牌");

	m_ComboField1.SetCurSel(0);
	m_ComboField2.SetCurSel(0);
	m_ComboField3.SetCurSel(0);

	m_ComboAndOr1.InsertString(0,L"AND");
	m_ComboAndOr1.InsertString(1,L"OR");
	m_ComboAndOr2.InsertString(0,L"AND");
	m_ComboAndOr2.InsertString(1,L"OR");

	m_ComboAndOr1.SetCurSel(0);
	m_ComboAndOr2.SetCurSel(0);
	///////////////////
	DWORD dwStyle;

	m_ListSpec.InsertColumn( 0, L"入库日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpec.InsertColumn( 1, L"白酒名称", LVCFMT_LEFT, 100 );//插入列 
	m_ListSpec.InsertColumn( 2, L"白酒信息", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpec.InsertColumn( 2, L"生产日期", LVCFMT_LEFT, 100 );//插入列
	m_ListSpec.InsertColumn( 3, L"生产批号", LVCFMT_LEFT, 100 );//插入列
	m_ListSpec.InsertColumn( 4, L"来源说明", LVCFMT_LEFT, 100 );//插入列
	m_ListSpec.InsertColumn( 5, L"入库日期", LVCFMT_LEFT, 100 );//插入列

	dwStyle = m_ListSpec.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//  dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListSpec.SetExtendedStyle(dwStyle); 

    ////


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelectSpecFromLibDlg::OnBnClickedButtonSearchwithkeywords()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(!m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Open();
	}

	if(m_Key1.IsEmpty()&&m_Key2.IsEmpty()&&m_Key3.IsEmpty())
	{
		MessageBox(L"请填写搜索关键词！",L"搜索出错",MB_ICONWARNING);
		m_TrueWineLib.Close();
		return ;
	}

	CString strFilter1,strFilter2,strFilter3;
	CString AndOr1,AndOr2;

	m_ComboAndOr1.GetLBText(m_ComboField1.GetCurSel(),AndOr1);
	m_ComboAndOr2.GetLBText(m_ComboField2.GetCurSel(),AndOr2);

	m_TrueWineLib.m_strFilter=L"";

	if(!m_Key1.IsEmpty())
	{
		CString ComboField1;

		m_ComboField1.GetLBText(m_ComboField1.GetCurSel(),ComboField1);
		if(ComboField1 == L"所有项")
		{
			strFilter1.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key1,m_Key1,m_Key1,m_Key1);
		}
		else if(ComboField1 == L"白酒名称")
		{
			strFilter1.Format(L" (WineName = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒酒精度数")
		{
			strFilter1.Format(L" (AlcoholContent = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒香型")
		{
			strFilter1.Format(L" (Flavour = '%s') ",m_Key1);
		}
		else if(ComboField1 == L"白酒品牌")
		{
			strFilter1.Format(L" (Brand = '%s') ",m_Key1);
		}
		m_TrueWineLib.m_strFilter+=strFilter1;
	}

	if(!m_Key2.IsEmpty())
	{
		CString ComboField2;
		m_ComboField2.GetLBText(m_ComboField2.GetCurSel(),ComboField2);

		if(ComboField2 == L"所有项")
		{

			strFilter2.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key2,m_Key2,m_Key2,m_Key2);
		}
		else if(ComboField2 == L"白酒名称")
		{
			strFilter2.Format(L" (WineName = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"白酒酒精度数")
		{
			strFilter2.Format(L" (AlcoholContent = '%s') ",m_Key2);
		}
		else if(ComboField2 == L"白酒香型")
		{
			strFilter2.Format(L" (Flavour = '%s')",m_Key2);
		}
		else if(ComboField2 == L"白酒品牌")
		{
			strFilter2.Format(L" (Brand = '%s') ",m_Key2);
		}
		if(!m_TrueWineLib.m_strFilter.IsEmpty())
			m_TrueWineLib.m_strFilter+=AndOr1;
		m_TrueWineLib.m_strFilter+=strFilter2;
	}
	///////
	if(!m_Key3.IsEmpty())
	{

		CString ComboField3;
		m_ComboField3.GetLBText(m_ComboField3.GetCurSel(),ComboField3);	
		if(ComboField3 == L"所有项")
		{

			strFilter3.Format(L" (WineName = '%s' OR AlcoholContent = '%s' OR Flavour = '%s' OR Brand = '%s') ",
				m_Key3,m_Key3,m_Key3,m_Key3);
		}
		else if(ComboField3 == L"白酒名称")
		{
			strFilter3.Format(L" (WineName = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒酒精度数")
		{
			strFilter3.Format(L" (AlcoholContent = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒香型")
		{
			strFilter3.Format(L" (Flavour = '%s') ",m_Key3);
		}
		else if(ComboField3 == L"白酒品牌")
		{
			strFilter3.Format(L" (Brand = '%s') ",m_Key3);
		}
		if(!m_TrueWineLib.m_strFilter.IsEmpty())
			m_TrueWineLib.m_strFilter+=AndOr2;
		m_TrueWineLib.m_strFilter+=strFilter3;
	}

	m_TrueWineLib.Requery();
	ShowSpec();
}


void CSelectSpecFromLibDlg::OnBnClickedButtonShowall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_TrueWineLib.IsOpen())
	{
		m_TrueWineLib.Open();
	}
	else
	{
		m_TrueWineLib.m_strFilter=L"";
		m_TrueWineLib.Requery();
	}
	ShowSpec();
}

int CSelectSpecFromLibDlg::ShowSpec(void)
{
	m_ListSpec.DeleteAllItems();
	if(m_TrueWineLib.IsEOF())
	{
		MessageBox(L"没有相关记录!",L"无记录",MB_ICONWARNING);
		return -1;
	}
	m_TrueWineLib.MoveFirst();

	int count=0;
	while(!m_TrueWineLib.IsEOF())
	{
		CString temp;
		temp.Format(L"%s,%s,%s",m_TrueWineLib.m_AlcoholContent,m_TrueWineLib.m_Flavour,m_TrueWineLib.m_Brand);
		m_ListSpec.InsertItem(count,m_TrueWineLib.m_WineName);
		m_ListSpec.SetItemText(count,1,temp);
		m_ListSpec.SetItemText(count,2,m_TrueWineLib.m_ProductionDate);
		m_ListSpec.SetItemText(count,3,m_TrueWineLib.m_ProductionBatchNo);
		m_ListSpec.SetItemText(count,4,m_TrueWineLib.m_ComFrom);
		m_ListSpec.SetItemText(count,5,m_TrueWineLib.m_ImportDate);


		m_TrueWineLib.MoveNext();
		count++;
	}
	m_TrueWineLib.MoveFirst();
	return 0;
}


void CSelectSpecFromLibDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CSelectSpecFromLibDlg::OnBnClickedButtonClearlist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListSpec.DeleteAllItems();
}


void CSelectSpecFromLibDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListSpec.GetFirstSelectedItemPosition();
	int nItem;
	while (pos)
	{
		nItem = m_ListSpec.GetNextSelectedItem(pos);
		m_ListSpec.DeleteItem(nItem);				
		pos = m_ListSpec.GetFirstSelectedItemPosition();
		// you could do your own processing on nItem here
	}

}
