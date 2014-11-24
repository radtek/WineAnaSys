// ClusterSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "ClusterSettingDlg.h"
#include "afxdialogex.h"


// CClusterSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CClusterSettingDlg, CDialogEx)

CClusterSettingDlg::CClusterSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClusterSettingDlg::IDD, pParent)
	, m_RadioCluMethod(0)
	, m_szeps(_T("12"))
	, m_szMinpts(_T("5"))
	, m_szClusterNum(_T(""))
{

}

CClusterSettingDlg::~CClusterSettingDlg()
{
}

void CClusterSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DBSCAN, m_RadioCluMethod);
	DDX_Control(pDX, IDC_EDIT_EPS, m_EditEps);
	DDX_Control(pDX, IDC_COMBO_MINPTS, m_ComboMinPts);
	DDX_Control(pDX, IDC_COMBO_CLUSTERNUM, m_ComboClusterNum);
	DDX_Text(pDX, IDC_EDIT_EPS, m_szeps);
	DDX_CBString(pDX, IDC_COMBO_MINPTS, m_szMinpts);
	DDX_CBString(pDX, IDC_COMBO_CLUSTERNUM, m_szClusterNum);
}


BEGIN_MESSAGE_MAP(CClusterSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_DBSCAN, &CClusterSettingDlg::OnBnClickedRadioDbscan)
	ON_BN_CLICKED(IDC_RADIO_KMEANS, &CClusterSettingDlg::OnBnClickedRadioKmeans)
	ON_BN_CLICKED(IDOK, &CClusterSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CClusterSettingDlg ��Ϣ�������


BOOL CClusterSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i = 1;i<=20;++i)
	{
		CString tmp;
		tmp.Format(L"%d",i);
		m_ComboMinPts.AddString(tmp);
	}
	for(int i = 1;i<=20;++i)
	{
		CString tmp;
		tmp.Format(L"%d",i);
		m_ComboClusterNum.AddString(tmp);
	}
	OnBnClickedRadioDbscan();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CClusterSettingDlg::OnBnClickedRadioDbscan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioCluMethod = 0;
	m_ComboClusterNum.EnableWindow(0);
	m_EditEps.EnableWindow(1);
	m_ComboMinPts.EnableWindow(1);
}


void CClusterSettingDlg::OnBnClickedRadioKmeans()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioCluMethod = 1;
	m_EditEps.EnableWindow(0);
	m_ComboMinPts.EnableWindow(0);
	m_ComboClusterNum.EnableWindow(1);
}


void CClusterSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_RadioCluMethod == 0) 
	{
		//���ѡ��DBSCAN
		if(m_szeps.IsEmpty())   //ɨ��뾶
		{
			MessageBox(L"ɨ��뾶����Ϊ��",L"��������",MB_ICONERROR);
			return;
		}
		m_eps = _wtof(m_szeps);
		if(m_eps == 0)   //ɨ��뾶
		{
			MessageBox(L"������Ϸ���ɨ��뾶",L"��������",MB_ICONERROR);
			return;
		}

		if(m_szMinpts.IsEmpty())   //ɨ��뾶
		{
			MessageBox(L"��С��������Ϊ��",L"��������",MB_ICONERROR);
			return;
		}
		m_Minpts = _wtof(m_szMinpts);
		if(m_Minpts == 0)   //ɨ��뾶
		{
			MessageBox(L"������Ϸ�����С����",L"��������",MB_ICONERROR);
			return;
		}
	}
	else
	{		
		if(m_szClusterNum.IsEmpty())   //ɨ��뾶
		{
			MessageBox(L"�������������Ϊ��",L"��������",MB_ICONERROR);
			return;
		}
		m_ClusterNum = _wtof(m_szClusterNum);
		if(m_ClusterNum == 0)   //ɨ��뾶
		{
			MessageBox(L"������Ϸ��ľ��������",L"��������",MB_ICONERROR);
			return;
		}
	}
	CDialogEx::OnOK();
}
