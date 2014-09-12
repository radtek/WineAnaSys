// DiffSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DiffSettingDlg.h"
#include "afxdialogex.h"


// CDiffSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CDiffSettingDlg, CDialogEx)

CDiffSettingDlg::CDiffSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiffSettingDlg::IDD, pParent)
{

}

CDiffSettingDlg::~CDiffSettingDlg()
{
}

void CDiffSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ORDER, m_ComboOrder);
}


BEGIN_MESSAGE_MAP(CDiffSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiffSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiffSettingDlg ��Ϣ�������


BOOL CDiffSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboOrder.AddString(L"һ��");
	m_ComboOrder.AddString(L"����");
	m_ComboOrder.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDiffSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(0 == m_ComboOrder.GetCurSel())
		m_Order = 1;
	else if(1 == m_ComboOrder.GetCurSel())
		m_Order = 2;
	else
	{
		MessageBox(L"��ѡ���󵼵Ľ���",L"��������",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}
