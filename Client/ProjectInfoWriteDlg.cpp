// ProjectInfoWriteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ProjectInfoWriteDlg.h"
#include "afxdialogex.h"
#include <atlrx.h>

// CProjectInfoWriteDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectInfoWriteDlg, CDialogEx)

CProjectInfoWriteDlg::CProjectInfoWriteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectInfoWriteDlg::IDD, pParent)
	, m_ProjectName(_T(""))
	, m_WineName(_T(""))
	, m_AlcoholContent(_T(""))
	, m_Flavour(_T(""))
	, m_Brand(_T(""))
	, m_MailAddr(_T(""))
	, m_AnaType(_T(""))
	, m_SenderName(_T(""))
{

}

CProjectInfoWriteDlg::~CProjectInfoWriteDlg()
{
}

void CProjectInfoWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROJECTNAME, m_ProjectName);
	DDX_Text(pDX, IDC_EDIT_WINENAME, m_WineName);
	DDX_Text(pDX, IDC_EDIT_WINEALCOHOLCONTENT, m_AlcoholContent);
	DDX_Text(pDX, IDC_EDIT_WINEFLAVOUR, m_Flavour);
	DDX_Text(pDX, IDC_EDIT_WINEBRAND, m_Brand);
	DDX_Text(pDX, IDC_EDIT_USERMAILADDR, m_MailAddr);
	DDX_Control(pDX, IDC_COMBO_ANATYPE, m_ComboAnaType);
	DDX_CBString(pDX, IDC_COMBO_ANATYPE, m_AnaType);
	DDX_Text(pDX, IDC_EDIT_SENDERNAME, m_SenderName);
}


BEGIN_MESSAGE_MAP(CProjectInfoWriteDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SENDDATA, &CProjectInfoWriteDlg::OnBnClickedButtonSenddata)
END_MESSAGE_MAP()


// CProjectInfoWriteDlg ��Ϣ�������


void CProjectInfoWriteDlg::OnBnClickedButtonSenddata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���³�Ա����
	UpdateData(TRUE);
	//�����Ŀ����
	if(m_ProjectName.IsEmpty())
	{
		MessageBox(L"��Ŀ���Ʋ���Ϊ�գ�",L"��Ŀ����",MB_ICONWARNING);
		return;
	}
	//��������ַ
	if(m_MailAddr.IsEmpty())
	{
		MessageBox(L"����дһ�������ַ��",L"��Ŀ����",MB_ICONWARNING);
		return;
	}
	//��������ʽ
	CString strRegex=L"({[a-zA-Z0-9_]+@[a-zA-Z0-9]+[.][a-zA-Z0-9]+[.]?[a-zA-Z0-9]+})";
	CAtlRegExp<CAtlRECharTraitsW> reRule;
	wchar_t *wt = (wchar_t *)(LPCTSTR)strRegex;
	REParseError status = reRule.Parse((const ATL::CAtlRegExp<CAtlRECharTraitsW>::RECHAR *)wt);
	if (REPARSE_ERROR_OK != status)
	{
		AfxMessageBox(L"������ʼ���ַ���Ϸ���");
		return;
	}
	CAtlREMatchContext<CAtlRECharTraitsW> mcRule;
	wt = (wchar_t *)(LPCTSTR)m_MailAddr;
	if (!reRule.Match((const ATL::CAtlRegExp<CAtlRECharTraitsW>::RECHAR *)wt,&mcRule))
	{
		AfxMessageBox(L"������ʼ���ַ���Ϸ���");
		return;
	}
	else
	{
		for (UINT nGroupIndex = 0; nGroupIndex < mcRule.m_uNumGroups; ++nGroupIndex)
		{
			const CAtlREMatchContext<>::RECHAR* szStart = 0;
			const CAtlREMatchContext<>::RECHAR* szEnd = 0;
			mcRule.GetMatch(nGroupIndex, &szStart, &szEnd);
			ptrdiff_t nLength = szEnd - szStart;
			CString strEmailAddress(szStart,  static_cast<int>(nLength));
			if(strEmailAddress.Compare(m_MailAddr)!=0)
			{
				CString strPrompt;
				strPrompt.Format(L"������ʼ���ַ���Ϸ�����Ҫ����%s ��",strEmailAddress);
				AfxMessageBox(strPrompt);
				return;
			}
		}
	}
	//��鷢��������
	if(m_SenderName.IsEmpty())
	{
		MessageBox(L"����ʵ��д������������",L"��Ŀ����",MB_ICONWARNING);
		return;
	}
	if(m_AnaType.IsEmpty())
	{
		MessageBox(L"��ѡ��һ�����ģʽ��",L"��Ŀ����",MB_ICONWARNING);
		return;
	}
	//CString s=m_AnaType;
	CDialogEx::OnOK();
	//CString s=m_ProjectName;
}


BOOL CProjectInfoWriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_ComboAnaType.AddString(L"���߼��");
	m_ComboAnaType.AddString(L"רҵ���");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
