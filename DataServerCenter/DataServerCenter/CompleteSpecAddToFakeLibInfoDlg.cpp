// CompleteSpecAddToFakeLibInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "CompleteSpecAddToFakeLibInfoDlg.h"
#include "afxdialogex.h"


// CCompleteSpecAddToFakeLibInfoDlg 对话框

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


// CCompleteSpecAddToFakeLibInfoDlg 消息处理程序

void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditWinenameFake()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"输入白酒名称！";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditComfromFake()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"用简短的文字说明白酒来源信息，主要由以下部分组成：采集人员或单位，采集时间，采集方式,该说明将作为溯源的信息";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditSpecidtitleFake()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"输入此批白酒光谱数据的总体编号，用于生成光谱编号，形式由光谱来源信息首字母+光谱采集日期组成，如：2012年5月1日在武汉采集的白云边3年陈酿光谱数据：BYB3NWH20120501";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnEnSetfocusEditManagerFake()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_WineNameInstruction=L"";
	//m_TraceInfoInstruction = L"";
	m_SpecIDTitleInstruction=L"";
	m_ComfromInstruction=L"";
	m_ManagerInstruction=L"请当前管理员如实填写自己的姓名";
	UpdateData(FALSE);
}


void CCompleteSpecAddToFakeLibInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_WineName.IsEmpty())
	{
		MessageBox(L"白酒名称不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_SpecIDTitle.IsEmpty())
	{
		MessageBox(L"白酒入库编号不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_Manager.IsEmpty())
	{
		MessageBox(L"操作人员不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}
	if(m_Comfrom.IsEmpty())
	{
		MessageBox(L"白酒来源信息不能为空！",L"光谱入库",MB_ICONWARNING);
		return ;
	}

	CDialogEx::OnOK();
}
