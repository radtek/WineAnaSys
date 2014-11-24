// ProjectInfoWriteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ProjectInfoWriteDlg.h"
#include "afxdialogex.h"
#include <atlrx.h>

// CProjectInfoWriteDlg 对话框

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


// CProjectInfoWriteDlg 消息处理程序


void CProjectInfoWriteDlg::OnBnClickedButtonSenddata()
{
	// TODO: 在此添加控件通知处理程序代码
	//更新成员内容
	UpdateData(TRUE);
	//检查项目名称
	if(m_ProjectName.IsEmpty())
	{
		MessageBox(L"项目名称不能为空！",L"项目发送",MB_ICONWARNING);
		return;
	}
	//检查邮箱地址
	if(m_MailAddr.IsEmpty())
	{
		MessageBox(L"请填写一个邮箱地址！",L"项目发送",MB_ICONWARNING);
		return;
	}
	//检查邮箱格式
	CString strRegex=L"({[a-zA-Z0-9_]+@[a-zA-Z0-9]+[.][a-zA-Z0-9]+[.]?[a-zA-Z0-9]+})";
	CAtlRegExp<CAtlRECharTraitsW> reRule;
	wchar_t *wt = (wchar_t *)(LPCTSTR)strRegex;
	REParseError status = reRule.Parse((const ATL::CAtlRegExp<CAtlRECharTraitsW>::RECHAR *)wt);
	if (REPARSE_ERROR_OK != status)
	{
		AfxMessageBox(L"输入的邮件地址不合法！");
		return;
	}
	CAtlREMatchContext<CAtlRECharTraitsW> mcRule;
	wt = (wchar_t *)(LPCTSTR)m_MailAddr;
	if (!reRule.Match((const ATL::CAtlRegExp<CAtlRECharTraitsW>::RECHAR *)wt,&mcRule))
	{
		AfxMessageBox(L"输入的邮件地址不合法！");
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
				strPrompt.Format(L"输入的邮件地址不合法，您要输入%s 吗！",strEmailAddress);
				AfxMessageBox(strPrompt);
				return;
			}
		}
	}
	//检查发送人姓名
	if(m_SenderName.IsEmpty())
	{
		MessageBox(L"请如实填写发送人姓名！",L"项目发送",MB_ICONWARNING);
		return;
	}
	if(m_AnaType.IsEmpty())
	{
		MessageBox(L"请选择一个溯源模式！",L"项目发送",MB_ICONWARNING);
		return;
	}
	CDialogEx::OnOK();
}


BOOL CProjectInfoWriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ComboAnaType.AddString(L"在线溯源");
	m_ComboAnaType.AddString(L"专业溯源");
	m_ComboAnaType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
