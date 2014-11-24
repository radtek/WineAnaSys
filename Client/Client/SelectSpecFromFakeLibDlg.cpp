// SelectSpecFromFakeLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "SelectSpecFromFakeLibDlg.h"
#include "afxdialogex.h"


// CSelectSpecFromFakeLibDlg 对话框

IMPLEMENT_DYNAMIC(CSelectSpecFromFakeLibDlg, CPropertyPage)

	CSelectSpecFromFakeLibDlg::CSelectSpecFromFakeLibDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CSelectSpecFromFakeLibDlg::IDD)
{

}
CSelectSpecFromFakeLibDlg::~CSelectSpecFromFakeLibDlg()
{
	if(m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Close();
	}
}

void CSelectSpecFromFakeLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_Key);
	DDX_Control(pDX, IDC_LIST_SPECLIB, m_ListSpec);
}


BEGIN_MESSAGE_MAP(CSelectSpecFromFakeLibDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHWITHKEYWORDS, &CSelectSpecFromFakeLibDlg::OnBnClickedButtonSearchwithkeywords)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL, &CSelectSpecFromFakeLibDlg::OnBnClickedButtonShowall)
	ON_BN_CLICKED(IDOK, &CSelectSpecFromFakeLibDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLIST, &CSelectSpecFromFakeLibDlg::OnBnClickedButtonClearlist)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CSelectSpecFromFakeLibDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CSelectSpecFromLibDlg 消息处理程序

BOOL CSelectSpecFromFakeLibDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	///////////////////
	DWORD dwStyle;

	m_ListSpec.InsertColumn( 0, L"", LVCFMT_LEFT, 30 );             //插入列 
	m_ListSpec.InsertColumn( 1, L"光谱编号", LVCFMT_LEFT, 100 );    //插入列
	m_ListSpec.InsertColumn( 2, L"仿冒白酒名称", LVCFMT_LEFT, 150 );//插入列 
	m_ListSpec.InsertColumn( 3, L"来源说明", LVCFMT_LEFT, 100 );    //插入列
	m_ListSpec.InsertColumn( 4, L"入库日期", LVCFMT_LEFT, 100 );    //插入列
	m_ListSpec.InsertColumn( 5, L"操作员", LVCFMT_LEFT, 100 );      //插入列

	dwStyle = m_ListSpec.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT;         //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES;             //网格线（report风格时）
	m_ListSpec.SetExtendedStyle(dwStyle); 

	m_ListSpec.SetHeaderBackColor(168,193,221,2);           //设置扩展风格
	m_ListSpec.SetRowHeigt(28);                             //设置扩展风格
	m_ListSpec.SetHeaderHeight(1);                          //设置扩展风格
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelectSpecFromFakeLibDlg::OnBnClickedButtonSearchwithkeywords()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(m_Key.IsEmpty())//&&m_Key2.IsEmpty()&&m_Key3.IsEmpty())
	{
		MessageBox(L"请填写搜索关键词！",L"搜索出错",MB_ICONWARNING);
		return ;
	}
	if(!m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Open();
	}
	m_FakeWineLib.m_strFilter.Format(L"WineNameFaked LIKE '%%%s%%' OR Comfrom LIKE '%%%s%%'",m_Key,m_Key);
	m_FakeWineLib.Requery();
	ShowSpec();
}


void CSelectSpecFromFakeLibDlg::OnBnClickedButtonShowall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_FakeWineLib.m_strFilter=L"";
	if(!m_FakeWineLib.IsOpen())
	{
		m_FakeWineLib.Open();
	}
	else
	{
		m_FakeWineLib.Requery();
	}
	ShowSpec();
}

int CSelectSpecFromFakeLibDlg::ShowSpec(void)
{
	m_ListSpec.DeleteAllItems();
	if(m_FakeWineLib.IsEOF())
	{
		MessageBox(L"没有相关记录!",L"无记录",MB_ICONWARNING);
		return -1;
	}
	m_FakeWineLib.MoveFirst();

	int count=0;
	while(!m_FakeWineLib.IsEOF())
	{
		CString num;
		num.Format(L"%d",count+1);
		m_ListSpec.InsertItem(count,num);
		m_ListSpec.SetItemText(count,1,m_FakeWineLib.m_SpectrumID);
		m_ListSpec.SetItemText(count,2,m_FakeWineLib.m_WineNameFaked);
		m_ListSpec.SetItemText(count,3,m_FakeWineLib.m_Comfrom);
		m_ListSpec.SetItemText(count,4,m_FakeWineLib.m_ImportDate);
		m_ListSpec.SetItemText(count,5,m_FakeWineLib.m_Manager);
		m_FakeWineLib.MoveNext();
		count++;
	}
	m_FakeWineLib.MoveFirst();
	return 0;
}


void CSelectSpecFromFakeLibDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CPropertyPage::OnOK();
}


void CSelectSpecFromFakeLibDlg::OnBnClickedButtonClearlist()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListSpec.DeleteAllItems();
}


void CSelectSpecFromFakeLibDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListSpec.GetFirstSelectedItemPosition();
	int nItem;
	while (pos)
	{
		nItem = m_ListSpec.GetNextSelectedItem(pos);
		m_ListSpec.DeleteItem(nItem);				
		pos = m_ListSpec.GetFirstSelectedItemPosition();
	}
}
