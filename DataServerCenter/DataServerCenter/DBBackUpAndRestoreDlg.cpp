// DBBackUpAndRestoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBBackUpAndRestoreDlg.h"
#include "afxdialogex.h"


// CDBBackUpAndRestoreDlg 对话框

IMPLEMENT_DYNAMIC(CDBBackUpAndRestoreDlg, CDialogEx)

CDBBackUpAndRestoreDlg::CDBBackUpAndRestoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBBackUpAndRestoreDlg::IDD, pParent)
	, m_radioBackupOrRestore(0)
{

}

CDBBackUpAndRestoreDlg::~CDBBackUpAndRestoreDlg()
{
}

void CDBBackUpAndRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BACKUP, m_radioBackupOrRestore);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_BACKUP, m_EditBrowseBackup);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_RESTORE, m_EditBrowseRestore);
}


BEGIN_MESSAGE_MAP(CDBBackUpAndRestoreDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDBBackUpAndRestoreDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_BACKUP, &CDBBackUpAndRestoreDlg::OnBnClickedRadioBackup)
	ON_BN_CLICKED(IDC_RADIO_RESTORE, &CDBBackUpAndRestoreDlg::OnBnClickedRadioRestore)
END_MESSAGE_MAP()


// CDBBackUpAndRestoreDlg 消息处理程序


BOOL CDBBackUpAndRestoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditBrowseBackup.EnableFolderBrowseButton();
	m_EditBrowseRestore.EnableFileBrowseButton();

	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_DBBACKUPANDRESTORE);
	SetIcon(m_hIcon,TRUE); //设置为大图标
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDBBackUpAndRestoreDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_radioBackupOrRestore == 0)
	{
		m_EditBrowseBackup.GetWindowTextW(m_BackupOrRestorePath);
	}
	else
	{
		m_EditBrowseRestore.GetWindowTextW(m_BackupOrRestorePath);
	}
	if(!PathFileExists(m_BackupOrRestorePath))
	{
		MessageBox(L"路径有误！",L"数据库备份与维护",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}


void CDBBackUpAndRestoreDlg::OnBnClickedRadioBackup()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioBackupOrRestore = 0;
}


void CDBBackUpAndRestoreDlg::OnBnClickedRadioRestore()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioBackupOrRestore = 1;
}
