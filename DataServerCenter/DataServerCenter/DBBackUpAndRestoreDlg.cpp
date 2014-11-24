// DBBackUpAndRestoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "DBBackUpAndRestoreDlg.h"
#include "afxdialogex.h"


// CDBBackUpAndRestoreDlg �Ի���

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


// CDBBackUpAndRestoreDlg ��Ϣ�������


BOOL CDBBackUpAndRestoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_EditBrowseBackup.EnableFolderBrowseButton();
	m_EditBrowseRestore.EnableFileBrowseButton();

	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_DBBACKUPANDRESTORE);
	SetIcon(m_hIcon,TRUE); //����Ϊ��ͼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDBBackUpAndRestoreDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"·������",L"���ݿⱸ����ά��",MB_ICONERROR);
		return;
	}
	CDialogEx::OnOK();
}


void CDBBackUpAndRestoreDlg::OnBnClickedRadioBackup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radioBackupOrRestore = 0;
}


void CDBBackUpAndRestoreDlg::OnBnClickedRadioRestore()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radioBackupOrRestore = 1;
}
