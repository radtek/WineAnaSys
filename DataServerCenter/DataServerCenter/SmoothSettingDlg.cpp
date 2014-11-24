// SmoothSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataServerCenter.h"
#include "SmoothSettingDlg.h"
#include "afxdialogex.h"


// CSmoothSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSmoothSettingDlg, CDialogEx)

CSmoothSettingDlg::CSmoothSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmoothSettingDlg::IDD, pParent)
{

}

CSmoothSettingDlg::~CSmoothSettingDlg()
{
}

void CSmoothSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SMOOTH, m_ComboSmooth);
}


BEGIN_MESSAGE_MAP(CSmoothSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSmoothSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSmoothSettingDlg ��Ϣ�������


BOOL CSmoothSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i =0;i<9;++i)
	{
		CString szSmPts;
		szSmPts.Format(L"%d",i+2);
		m_ComboSmooth.AddString(szSmPts);
	}
	m_ComboSmooth.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSmoothSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();

	if(CB_ERR == m_ComboSmooth.GetCurSel())
	{
		MessageBox(L"��ѡ��ƽ���ĵ���",L"ƽ������",MB_ICONERROR);
		return;
	}
	CString szSmPts;
	m_ComboSmooth.GetLBText(m_ComboSmooth.GetCurSel(),szSmPts);
	m_SmoothPts = _wtoi(szSmPts);		
}
