// ProjectInfoConfirmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ProjectInfoConfirmDlg.h"
#include "afxdialogex.h"


// CProjectInfoConfirmDlg 对话框

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


// CProjectInfoConfirmDlg 消息处理程序


void CProjectInfoConfirmDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码


	CDialogEx::OnOK();
}


BOOL CProjectInfoConfirmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListProjectInfo.InsertColumn(0,L"",LVCFMT_LEFT,200);
	m_ListProjectInfo.InsertColumn(1,L"",LVCFMT_LEFT,200);
	DWORD dwStyle = m_ListProjectInfo.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//  dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListProjectInfo.SetExtendedStyle(dwStyle); //设置扩展风格


	m_ListProjectInfo.InsertItem(0,L"项目名称");
	m_ListProjectInfo.SetItemText(0,1,m_ProjectName);
	m_ListProjectInfo.InsertItem(1,L"白酒名称");
	m_ListProjectInfo.SetItemText(1,1,m_WineName);
	m_ListProjectInfo.InsertItem(2,L"白酒酒精浓度");
	m_ListProjectInfo.SetItemText(2,1,m_AlcoholContent);
	m_ListProjectInfo.InsertItem(3,L"白酒香型");
	m_ListProjectInfo.SetItemText(3,1,m_Flavour);
	m_ListProjectInfo.InsertItem(4,L"白酒品牌");
	m_ListProjectInfo.SetItemText(4,1,m_Brand);
	m_ListProjectInfo.InsertItem(5,L"用户邮箱");
	m_ListProjectInfo.SetItemText(5,1,m_MailAddr);
	m_ListProjectInfo.InsertItem(6,L"发送人姓名");
	m_ListProjectInfo.SetItemText(6,1,m_SenderName);
	m_ListProjectInfo.InsertItem(7,L"检测模式");
	m_ListProjectInfo.SetItemText(7,1,m_AnaType);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
