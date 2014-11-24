// SelectSpecFromFakeLibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SelectSpecFromFakeLibDlg.h"
#include "afxdialogex.h"


// CSelectSpecFromFakeLibDlg �Ի���

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


// CSelectSpecFromLibDlg ��Ϣ�������




BOOL CSelectSpecFromFakeLibDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	///////////////////
	DWORD dwStyle;

	m_ListSpec.InsertColumn( 0, L"", LVCFMT_LEFT, 30 );//������ 
	m_ListSpec.InsertColumn( 1, L"���ױ��", LVCFMT_LEFT, 100 );//������
	m_ListSpec.InsertColumn( 2, L"��ð�׾�����", LVCFMT_LEFT, 150 );//������ 
	m_ListSpec.InsertColumn( 3, L"��Դ˵��", LVCFMT_LEFT, 100 );//������
	m_ListSpec.InsertColumn( 4, L"�������", LVCFMT_LEFT, 100 );//������
	m_ListSpec.InsertColumn( 5, L"����Ա", LVCFMT_LEFT, 100 );//������

	dwStyle = m_ListSpec.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	m_ListSpec.SetExtendedStyle(dwStyle); 

	m_ListSpec.SetHeaderBackColor(168,193,221,2);           //������չ���
	m_ListSpec.SetRowHeigt(28);              //������չ���
	m_ListSpec.SetHeaderHeight(1);           //������չ���

	m_FakeWineLib.m_strSort.Format(L"WineNameFaked,Comfrom");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSelectSpecFromFakeLibDlg::OnBnClickedButtonSearchwithkeywords()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(m_Key.IsEmpty())//&&m_Key2.IsEmpty()&&m_Key3.IsEmpty())
	{
		MessageBox(L"����д�����ؼ��ʣ�",L"��������",MB_ICONWARNING);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"û����ؼ�¼!",L"�޼�¼",MB_ICONWARNING);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPropertyPage::OnOK();
}


void CSelectSpecFromFakeLibDlg::OnBnClickedButtonClearlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ListSpec.DeleteAllItems();
}


void CSelectSpecFromFakeLibDlg::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_ListSpec.GetFirstSelectedItemPosition();
	int nItem;
	while (pos)
	{
		nItem = m_ListSpec.GetNextSelectedItem(pos);
		m_ListSpec.DeleteItem(nItem);				
		pos = m_ListSpec.GetFirstSelectedItemPosition();
	}
}
