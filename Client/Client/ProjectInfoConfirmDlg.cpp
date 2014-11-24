// ProjectInfoConfirmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ProjectInfoConfirmDlg.h"
#include "afxdialogex.h"


// CProjectInfoConfirmDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectInfoConfirmDlg, CDialogEx)

CProjectInfoConfirmDlg::CProjectInfoConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectInfoConfirmDlg::IDD, pParent)
{

}

CProjectInfoConfirmDlg::~CProjectInfoConfirmDlg()
{
}

void CProjectInfoConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECTINFO, m_ListProjectInfo);
}


BEGIN_MESSAGE_MAP(CProjectInfoConfirmDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProjectInfoConfirmDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProjectInfoConfirmDlg ��Ϣ�������


void CProjectInfoConfirmDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CDialogEx::OnOK();
}


BOOL CProjectInfoConfirmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListProjectInfo.InsertColumn(0,L"",LVCFMT_LEFT,200);
	m_ListProjectInfo.InsertColumn(1,L"",LVCFMT_LEFT,200);
	DWORD dwStyle = m_ListProjectInfo.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	//  dwStyle |= LVS_EX_CHECKBOXES; //itemǰ����checkbox�ؼ�
	m_ListProjectInfo.SetExtendedStyle(dwStyle); //������չ���


	m_ListProjectInfo.InsertItem(0,L"��Ŀ����");
	m_ListProjectInfo.SetItemText(0,1,m_ProjectName);
	m_ListProjectInfo.InsertItem(1,L"�׾�����");
	m_ListProjectInfo.SetItemText(1,1,m_WineName);
	m_ListProjectInfo.InsertItem(2,L"�׾ƾƾ�Ũ��");
	m_ListProjectInfo.SetItemText(2,1,m_AlcoholContent);
	m_ListProjectInfo.InsertItem(3,L"�׾�����");
	m_ListProjectInfo.SetItemText(3,1,m_Flavour);
	m_ListProjectInfo.InsertItem(4,L"�׾�Ʒ��");
	m_ListProjectInfo.SetItemText(4,1,m_Brand);
	m_ListProjectInfo.InsertItem(5,L"�û�����");
	m_ListProjectInfo.SetItemText(5,1,m_MailAddr);
	m_ListProjectInfo.InsertItem(6,L"����������");
	m_ListProjectInfo.SetItemText(6,1,m_SenderName);
	m_ListProjectInfo.InsertItem(7,L"��Դģʽ");
	m_ListProjectInfo.SetItemText(7,1,m_AnaType);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
