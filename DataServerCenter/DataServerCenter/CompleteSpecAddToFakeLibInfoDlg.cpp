// CompleteSpecAddToFakeLibInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "CompleteSpecAddToFakeLibInfoDlg.h"
#include "afxdialogex.h"


// CCompleteSpecAddToFakeLibInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CCompleteSpecAddToFakeLibInfoDlg, CDialogEx)

CCompleteSpecAddToFakeLibInfoDlg::CCompleteSpecAddToFakeLibInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompleteSpecAddToFakeLibInfoDlg::IDD, pParent)
	, m_WineName(_T(""))
	, m_Comfrom(_T(""))
	, m_SpecIDTitle(_T(""))
	, m_Manager(_T(""))
	, m_WineNameInstruction(_T(""))
	, m_ComfromInstruction(_T(""))
	, m_SpecIDTitleInstruction(_T(""))
	, m_ManagerInstruction(_T(""))
{

}

CCompleteSpecAddToFakeLibInfoDlg::~CCompleteSpecAddToFakeLibInfoDlg()
{
}

void CCompleteSpecAddToFakeLibInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WINENAME_FAKE, m_WineName);
	//DDX_Text(pDX, IDC_EDIT_TRACEINFO_FAKE, m_TraceInfo);
	DDX_Text(pDX, IDC_EDIT_COMFROM_FAKE, m_Comfrom);
	DDX_Text(pDX, IDC_EDIT_SPECIDTITLE_FAKE, m_SpecIDTitle);
	DDX_Text(pDX, IDC_EDIT_MANAGER_FAKE, m_Manager);
	DDX_Text(pDX, IDC_EDIT_WINENAMEINSTRUCTION_FAKE, m_WineNameInstruction);
	//DDX_Text(pDX, IDC_EDIT_TRACEINFOINSTRUCTION_FAKE, m_TraceInfoInstruction);
	DDX_Text(pDX, IDC_EDIT_COMFROMINSTRUCTION_FAKE, m_ComfromInstruction);
	DDX_Text(pDX, IDC_EDIT_SPECIDTITLEINSTRUCTION_FAKE, m_SpecIDTitleInstruction);
	DDX_Text(pDX, IDC_EDIT_MANAGERINSTRUCTION_FAKE, m_ManagerInstruction);
}


BEGIN_MESSAGE_MAP(CCompleteSpecAddToFakeLibInfoDlg, CDialogEx)
	//ON_EN_CHANGE(IDC_EDIT_WINENAME_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnChangeEditWinenameFake)
	//ON_EN_CHANGE(IDC_EDIT_TRACEINFO_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnChangeEditTraceinfoFake)
	//ON_EN_CHANGE(IDC_EDIT_COMFROM_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnChangeEditComfromFake)
	//ON_EN_CHANGE(IDC_EDIT_SPECIDTITLE_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnChangeEditSpecidtitleFake)
	//ON_EN_CHANGE(IDC_EDIT_MANAGER_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnChangeEditManagerFake)
	ON_EN_SETFOCUS(IDC_EDIT_WINENAME_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditWinenameFake)
	ON_EN_SETFOCUS(IDC_EDIT_COMFROM_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditComfromFake)
	ON_EN_SETFOCUS(IDC_EDIT_SPECIDTITLE_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditSpecidtitleFake)
	ON_EN_SETFOCUS(IDC_EDIT_MANAGER_FAKE, &CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditManagerFake)
	ON_BN_CLICKED(IDOK, &CCompleteSpecAddToFakeLibInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCompleteSpecAddToFakeLibInfoDlg ��Ϣ�������

void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditWinenameFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"����׾����ƣ�";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditComfromFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"�ü�̵�����˵���׾���Դ��Ϣ����Ҫ�����²�����ɣ��ɼ���Ա��λ���ɼ�ʱ�䣬�ɼ���ʽ,��˵������Ϊ��Դ����Ϣ";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditSpecidtitleFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"��������׾ƹ������ݵ������ţ��������ɹ��ױ�ţ���ʽ�ɹ�����Դ��Ϣ����ĸ+���ײɼ�������ɣ��磺2012��5��1�����人�ɼ��İ��Ʊ�3�����������ݣ�BYB3NWH20120501";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditManagerFake()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"�뵱ǰ����Ա��ʵ��д�Լ�������";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"�׾����Ʋ���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_SpecIDTitle.IsEmpty())
	{
		MessageBox(L"�׾�����Ų���Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_Manager.IsEmpty())
	{
		MessageBox(L"������Ա����Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}
	if(m_Comfrom.IsEmpty())
	{
		MessageBox(L"�׾���Դ��Ϣ����Ϊ�գ�",L"�������",MB_ICONWARNING);
		return ;
	}

	CDialogEx::OnOK();
}
